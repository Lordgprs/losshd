#include "losshd.h"

Ipv4::Ipv4() {
  std::fill (data_, data_ + sizeof(data_), 0);
}

char8_t Ipv4::version() const {
  return (data_[0] >> 4) & 0xF;
}

uint16_t Ipv4::header_length() const {
  return (data_[0] & 0xF) << 2;
}

char8_t Ipv4::tos() const {
  return data_[1];
}

uint32_t Ipv4::time_to_live() const {
  return data_[8];
}

boost::asio::ip::address_v4 Ipv4::source_address() const {
  boost::asio::ip::address_v4::bytes_type bytes = {
    {data_[12], data_[13], data_[14], data_[15]}
  };
  return boost::asio::ip::address_v4(bytes);
}

uint32_t Ipv4::source_address_uint32() const {
  return (data_[12] << 24) | (data_[13] << 16) | (data_[14] << 8) | data_[15];
}

Icmp::Icmp() {
  std::fill(data_, data_ + sizeof(data_), 0);
}

char8_t Icmp::type() const {
  return data_[0];
}

char8_t Icmp::code() const {
  return data_[1];
}

uint16_t Icmp::identifier() const {
  return decode(4, 5);
}

uint16_t Icmp::sequence_number() const {
  return decode(6, 7);
}

void Icmp::type(char8_t n) {
  data_[0] = n;
}

void Icmp::code(char8_t n) {
  data_[1] = n;
}

void Icmp::checksum(uint16_t n) {
  encode(2, 3, n);
}

void Icmp::identifier(uint16_t n) {
  encode(4, 5, n);
}

void Icmp::sequence_number(uint16_t n) {
  encode(6, 7, n);
}

std::istream& operator>>(std::istream& inputStream, Icmp &header) {
  return inputStream.read(reinterpret_cast<char *>(header.data_), 8);
}

std::ostream& operator<<(std::ostream& outputStream, const Icmp &header) {
  return outputStream.write(reinterpret_cast<const char *>(header.data_), 8);
}

std::istream &operator>>(std::istream &is, Ipv4 &header) {
  is.read(reinterpret_cast<char *>(header.data_), 20);
  if (header.version() != 4)
    is.setstate(std::ios::failbit);
  std::streamsize optionsLength = header.header_length() - 20;
  if (optionsLength < 0 || optionsLength > 40)
    is.setstate(std::ios::failbit);
  else
    is.read(reinterpret_cast<char *>(header.data_) + 20, optionsLength);
  return is;
}

IcmpSender::IcmpSender(boost::asio::io_context &io_context, const char *destination, uint16_t count, uint16_t interval, std::mutex &mtx, std::condition_variable &condition):
  sequenceNumber_(0),
  numReplies_(0),
  repliesCount_(0),
  requestsCount_(1),
  interval_(interval),
  size_(1400),
  count_(count),
  mtx_(mtx),
  condition_(condition),
  socket_(io_context, icmp::v4()) {

  destination_ =  icmp::endpoint(boost::asio::ip::address::from_string(destination), 0);
  start_send();
}

uint16_t IcmpSender::received() const {
  return repliesCount_;
}

uint16_t IcmpSender::sent() const {
  return requestsCount_;
}

void IcmpSender::start_send() {
  std::string body(size_ - sizeof(Icmp), '\0');
  // Create an ICMP header
  Icmp icmp;
  icmp.type(Icmp::ECHO_REQUEST);
  icmp.code(0);
  icmp.identifier(static_cast<uint16_t>(getpid()));
  for (size_t i = 0; i < count_; i++) {
    icmp.sequence_number(++sequenceNumber_);
    calculate_checksum(icmp, body.begin(), body.end());
    // Encode the request packet
    boost::asio::streambuf requestBuffer;
    std::ostream outputStream(&requestBuffer);
    outputStream << icmp << body;
  
    // Send the request
    timeSent_ = steady_timer::clock_type::now();
    socket_.send_to(requestBuffer.data(), destination_);
    std::this_thread::sleep_for(std::chrono::milliseconds(interval_));
  } 
}

IcmpReceiver::IcmpReceiver(boost::asio::io_context &io_context, int &senders, std::unordered_map<uint32_t, uint32_t> &results, std::mutex &mtx, std::condition_variable &condition):
  mtx_(mtx),
  condition_(condition),
  dt_(io_context, RECEIVE_TIMER_FREQUENCY),
  sendersCount_(senders),
  pingResults_(results),
  socket_(io_context, icmp::v4()) {
    /*std::unique_lock<std::mutex> ul(mtx_);
    sendersCount_ = senders;
    pingResults_ = results;
    mtx_.unlock();*/
    start_receive();
  }
  
void IcmpReceiver::start_receive() {
  // Discard any data already in the buffer
  replyBuffer_.consume(replyBuffer_.size());
  if ((!sendersUnlocked_) || (sendersUnlocked_ && sendersCount_ > 0)) {
    socket_.async_receive(replyBuffer_.prepare(65536),
      boost::bind(&IcmpReceiver::handle_receive, this, boost::placeholders::_2));
    dt_.async_wait(boost::bind(&IcmpReceiver::check_if_senders_exist, this));
  }
  if (!sendersUnlocked_) {
    std::unique_lock<std::mutex> ul(mtx_);
    // Notifying senders: we are ready to catch echo replies
    condition_.notify_all();
    mtx_.unlock();
    sendersUnlocked_ = true;
  }
}

void IcmpReceiver::check_if_senders_exist() {
  std::unique_lock<std::mutex> ul(mtx_);
  auto count = sendersCount_;
  mtx_.unlock();
  // Stopping catching replies if all senders have been finished
  if (count == 0) {
    dt_.cancel();
    socket_.cancel();
  }
}

void IcmpReceiver::handle_receive(std::size_t length) {
  std::unique_lock<std::mutex> ul(mtx_);
  mtx_.unlock();
  replyBuffer_.commit(length);
  
  // Decode the reply packet
  std::istream inputStream(&replyBuffer_);
  Ipv4 ipv4;
  Icmp icmp;
  inputStream >> ipv4 >> icmp;
  if (inputStream && icmp.type() == Icmp::ECHO_REPLY
  && icmp.identifier() == getpid()) {
    dt_.cancel();
    uint32_t src = ipv4.source_address_uint32();
    mtx_.lock();
    pingResults_[src]++;
    mtx_.unlock();
  }
  mtx_.lock();
  if(sendersCount_ > 0) {
    start_receive();
  }
  mtx_.unlock();
}

OptionsLosshd::OptionsLosshd (int argc, char *argv[]):
Options::Options(argc, argv) {
  desc_.add_options()
    ("help", "this help")
    ("dbname,n", po::value<std::string>(), "database name")
    ("dbhost,h", po::value<std::string>(), "database server address")
    ("dbuser,U", po::value<std::string>(), "database user name")
    ("dbpass,P", po::value<std::string>(), "database password")
    ("interval,i", po::value<uint16_t>(), "ping interval, ms")
    ("size,s", po::value<uint16_t>(), "packet size, bytes")
    ("count,c", po::value<uint16_t>(), "count of icmp echo requests")
    ("daemon,D", "start daemonized");
  po::store(po::parse_command_line(argc, argv, desc_), vm_);
  po::notify(vm_);
  
  check_options();
}
void OptionsLosshd::check_options() {
  if (vm_.count("help") != 0) {
    std::cout << desc_ << std::endl;
    std::exit(EXIT_SUCCESS);
  }
  if (
    vm_.count("dbname") == 0 ||
    vm_.count("dbuser") == 0 ||
    vm_.count("interval") == 0 ||
    vm_.count("size") == 0 ||
    vm_.count("count") == 0
   ) {
    std::cerr << "Error via processing arguments!" << std::endl << std::endl;
    std::cerr << desc_ << std::endl;
    std::exit(EXIT_FAILURE);
  }
}

std::string OptionsLosshd::getDbname() const {
  return vm_.at("dbname").as<std::string>();
}

std::string OptionsLosshd::getDbhost() const {
  return vm_.at("dbhost").as<std::string>();
}

std::string OptionsLosshd::getDbuser() const {
  return vm_.at("dbuser").as<std::string>();
}

std::string OptionsLosshd::getDbpass() const {
  return vm_.at("dbpass").as<std::string>();
}

uint16_t OptionsLosshd::getCount() const {
  return vm_.at("count").as<uint16_t>();
}

uint16_t OptionsLosshd::getInterval() const {
  return vm_.at("interval").as<uint16_t>();
}

uint16_t OptionsLosshd::getSize() const {
  return vm_.at("size").as<uint16_t>();
}

bool OptionsLosshd::isDaemon() const {
  if(vm_.count("daemon") > 0)
    return true;
  return false;
}

Scheduler::Scheduler(OptionsLosshd &options):
options_(options),
conn_("dbname=" + options.getDbname() + " user=" + options.getDbuser() + " password=" + options.getDbpass() + " hostaddr=" + options.getDbhost()), txn_(conn_),
addressList_(get_addresses_for_ping()) {
  for (auto address: addressList_)
    pingResults_.insert({get_ip_from_string(address), 0});
}

Scheduler::~Scheduler() {}

auto Scheduler::createReceiver() {
  return [](int &senders, std::unordered_map<uint32_t, uint32_t> &results, std::mutex &mtx, std::condition_variable &condition) {
    boost::asio::io_context io_context;
    IcmpReceiver pinger(io_context, senders, results, mtx, condition);
    io_context.run();
  };
}

auto Scheduler::createSender() {
  return [](std::string address, int &senders, std::mutex &mtx, std::condition_variable &condition, OptionsLosshd &options){
    std::unique_lock<std::mutex> ul(mtx);
    // Waiting for receiver starts
    condition.wait(ul);
    mtx.unlock();
    boost::asio::io_context io_context;
    IcmpSender pinger(io_context, address.c_str(), options.getCount(), options.getInterval(), mtx, condition);
    mtx.lock();
    senders--;
    mtx.unlock();
  };
}

void Scheduler::run() {
  std::vector<std::thread> threads;
  int sendersCount = addressList_.size();
  // Starting ICMP receiver
  std::thread t(createReceiver(), std::ref(sendersCount), std::ref(pingResults_), std::ref(mtx_), std::ref(condition_));
  // Starting ICMP senders
  for (size_t i = 0; i < addressList_.size(); i++) {
    std::thread t(createSender(), addressList_[i], std::ref(sendersCount), std::ref(mtx_), std::ref(condition_), std::ref(options_));
    threads.push_back(std::move(t));
  }
  // Joining senders
  for (size_t i = 0; i < threads.size(); i++)
     threads.at(i).join();
  // Joining receiver
  t.join();
  std::cout << "End of collecting results:" << std::endl;
  for (auto i: pingResults_) {
    std::cout << "from IP " <<  get_ip_from_uint32(i.first) << " received: " << i.second << std::endl;
    txn_.exec("UPDATE ext_packetlosshd_dbg SET \
        loss = " + std::to_string(static_cast<double>(options_.getCount() - i.second) / options_.getCount() * 100) + ", " + " \
        last_update = NOW() \
        WHERE \
        ip = '" + get_ip_from_uint32(i.first) + "'");
  }
  std::cout << std::endl;
}

void Scheduler::clean() {
  pingResults_.clear();
  addressList_ = get_addresses_for_ping();
  for (auto address: addressList_)
    pingResults_.insert({get_ip_from_string(address), 0});
}

std::vector<std::string> Scheduler::get_addresses_for_ping() const {
  std::string req = "";
  std::vector<std::string> addresses;
  txn_.exec("DELETE FROM ext_packetlosshd_dbg WHERE last_read + interval '7 days' < NOW()");
  for (auto row: txn_.exec("SELECT ip FROM ext_packetlosshd_dbg LIMIT 100"))
    addresses.push_back(row[0].c_str());
  return addresses;
}

std::string Scheduler::get_ip_from_uint32 (const uint32_t ip) const {
  uint32_t ipaddr = ip;
  std::array<uint8_t, 4> octets;
  int8_t octetNum = 3;
  std::string result = "";
  for (auto i = 0; i < 4; i++) {
    octets[3 - i] = ipaddr % 256;
    ipaddr >>= 8;
    if (i > 0)
      result = '.' + result;
    result = std::to_string (octets[3 - i]) + result;
  }
  return result;
}

int main(int argc, char *argv[]) {
  const auto PAUSE_BETWEEN_ITERATIONS = std::chrono::seconds(5);
  OptionsLosshd options(argc, argv);
  if (options.isDaemon()) {
    std::cout << "Running as a daemon..." << std::endl;
    auto pid = fork();
    if (pid > 0)
      return EXIT_SUCCESS;
    if (pid < 0) {
      std::cerr << "Error while doing fork()! Exiting...";
      return EXIT_FAILURE;
    }
    umask(0);
    setsid();
    if (chdir("/") < 0) {
      std::cerr << "Error while attempting chdir()!" << std::endl;
      return EXIT_FAILURE;
    };
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
  }
  Scheduler scheduler(options);
  while (true) {
    std::cout << "New iteration started. Pinging hosts..." << std::endl;
    scheduler.run();
    scheduler.clean();
    std::this_thread::sleep_for(PAUSE_BETWEEN_ITERATIONS);
  }
  return EXIT_SUCCESS;
}
