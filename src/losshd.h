#ifndef LOSSHD_MAIN_H
#define LOSSHD_MAIN_H

#include <algorithm>
#include <istream>
#include <iostream>
#include <ostream>
#include <thread>
#include <unordered_map>

#include <pqxx/pqxx>
#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>
#include <boost/program_options.hpp>
#include <boost/asio/ip/address_v4.hpp>
#include "options.h"

using boost::asio::ip::icmp;
using boost::asio::steady_timer;
namespace chrono = boost::asio::chrono;

class Ipv4 {
public:
  Ipv4();
  char8_t version() const;
  uint16_t header_length() const;
  char8_t tos() const;
  uint32_t time_to_live() const;
  boost::asio::ip::address_v4 source_address() const;
  uint32_t source_address_uint32() const;
  friend std::istream &operator>>(std::istream &is, Ipv4 &header);

private:
  char8_t data_[60];
};

class Icmp {
public:
  enum {
    ECHO_REPLY = 0,
    DESTINATION_UNREACHABLE = 3,
    SOURCE_QUENCH = 4,
    REDIRECT = 5,
    ECHO_REQUEST = 8,
    TIME_EXCEEDED = 11,
    PARAMETER_PROBLEM = 12,
    TIMESTAMP_REQUEST = 13,
    TIMESTAMP_REPLY = 14,
    INFO_REQUEST = 15,
    INFO_REPLY = 16,
    ADDRESS_REQUEST = 17,
    ADDRESS_REPLY = 18
  };
  Icmp();
  char8_t type() const;
  char8_t code() const;
  uint16_t identifier() const;
  uint16_t sequence_number() const;
  void type(char8_t);
  void code(char8_t);
  void checksum(uint16_t);
  void identifier(uint16_t);
  void sequence_number(uint16_t);
  friend std::istream& operator>>(std::istream &inputStream, Icmp &header);
  friend std::ostream& operator<<(std::ostream &outputStream, const Icmp &header);

private:
  uint16_t decode(int32_t a, int32_t b) const {
    return (data_[a] << 8) + data_[b];
  }
  void encode(int32_t a, int32_t b, uint16_t n) {
    data_[a] = static_cast<char8_t>(n >> 8);
    data_[b] = static_cast<char8_t>(n & 0xFF);
  }
  
  char8_t data_[8];
};

template <typename T>
void calculate_checksum(Icmp &header, T bodyBegin, T bodyEnd) {
  uint32_t sum = (header.type() << 8) + header.code() + header.identifier() + header.sequence_number();
  T bodyIterator = bodyBegin;
  while (bodyIterator != bodyEnd) {
    sum += (static_cast<char8_t>(*bodyIterator++) << 8);
    if (bodyIterator != bodyEnd)
      sum += static_cast<char8_t>(*bodyIterator++);
  }
  
  sum = (sum >> 16) + (sum & 0xFFFF);
  sum += (sum >> 16);
  header.checksum(static_cast<uint16_t>(~sum));
}

uint32_t get_ip_from_string (const std::string strIp) {
  int32_t oldPos = 0;
  std::array<uint8_t, 4> octets;
  int8_t octetNum = 3;
  uint32_t result = 0;
  for (size_t i = 0; i < strIp.length(); i++)
    if (strIp.at(i) == '.') {
      octets.at(octetNum--) = stoi(strIp.substr(oldPos, i - oldPos));
      oldPos = i + 1;
    }
  octets.at(0) = stoi(strIp.substr(oldPos, strIp.length() - oldPos));
  for (size_t i = 0; i < 4; i++) {
    if (i > 0)
      result <<= 8;
    result += octets[3 - i];
  }
  return result;
}

class IcmpSender {
public:
  IcmpSender() = delete;
  IcmpSender(const IcmpSender &) = delete;
  IcmpSender(const IcmpSender &&) = delete;
  IcmpSender & operator=(const IcmpSender &) = delete;
  IcmpSender & operator=(const IcmpSender &&) = delete;
  IcmpSender(boost::asio::io_context &, const char *, uint16_t, uint16_t, std::mutex &, std::condition_variable &);
  uint16_t received() const;
  uint16_t sent() const;

private:
  void start_send();

  icmp::endpoint destination_;
  icmp::socket socket_;
  uint16_t sequenceNumber_;
  uint16_t count_;
  chrono::steady_clock::time_point timeSent_;
  boost::asio::streambuf replyBuffer_;
  std::size_t numReplies_;
  std::size_t repliesCount_;
  std::size_t requestsCount_;
  std::size_t interval_;
  std::size_t size_;
  std::mutex &mtx_;
  std::condition_variable &condition_;
};

class IcmpReceiver {
public:
  IcmpReceiver() = delete;
  IcmpReceiver(const IcmpReceiver &) = delete;
  IcmpReceiver(const IcmpReceiver &&) = delete;
  IcmpReceiver & operator=(const IcmpReceiver &) = delete;
  IcmpReceiver & operator=(const IcmpReceiver &&) = delete;
  IcmpReceiver(boost::asio::io_context &, int &, std::unordered_map<uint32_t, uint32_t> &, std::mutex &, std::condition_variable &);

private:
  void start_receive();
  void check_if_senders_exist();
  void handle_receive(std::size_t length);
  
  icmp::socket socket_;
  chrono::steady_clock::time_point timeSent_;
  boost::asio::streambuf replyBuffer_;
  int &sendersCount_;
  std::unordered_map<uint32_t, uint32_t> &pingResults_;
  std::mutex &mtx_;
  std::condition_variable &condition_;
  bool sendersUnlocked_ = false;
  boost::asio::deadline_timer dt_;
  const boost::posix_time::time_duration RECEIVE_TIMER_FREQUENCY = boost::posix_time::seconds(5);
};

class OptionsLosshd : public Options {
public:
  OptionsLosshd() = delete;
  OptionsLosshd(const OptionsLosshd &) = delete;
  OptionsLosshd(const OptionsLosshd &&) = delete;
  OptionsLosshd & operator=(const OptionsLosshd &) = delete;
  OptionsLosshd & operator=(const OptionsLosshd &&) = delete;
  OptionsLosshd (int, char **);
  std::string getDbname() const;
  std::string getDbhost() const;
  std::string getDbuser() const;
  std::string getDbpass() const;
  uint16_t getCount() const;
  uint16_t getInterval() const;
  uint16_t getSize() const;
  bool isDaemon() const;

private:
  void check_options();
};

class Scheduler {
public:
  Scheduler(OptionsLosshd &);
  ~Scheduler();
  void run();
  void clean();

private:
  std::string get_ip_from_uint32 (const uint32_t ip) const;
  std::vector<std::string> get_addresses_for_ping() const;
  auto createReceiver(); 
  auto createSender(); 
  
  OptionsLosshd &options_;
  pqxx::connection conn_;
  pqxx::nontransaction mutable txn_;
  std::mutex mtx_;
  std::vector<std::string> addressList_;
  std::unordered_map<uint32_t, uint32_t> pingResults_;
  std::condition_variable condition_;
};

#endif
