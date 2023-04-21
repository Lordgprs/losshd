#include <iostream>
#include <pqxx/pqxx>
#include <boost/program_options.hpp>
#include "getloss.h"

OptionsGetloss::OptionsGetloss(int argc, char *argv[]) :
Options(argc, argv) {
  desc_.add_options()
    ("help", "this help")
    ("dbname,n", po::value<std::string>(), "database name")
    ("dbhost,h", po::value<std::string>(), "database server address")
    ("dbuser,U", po::value<std::string>(), "database user name")
    ("dbpass,P", po::value<std::string>(), "database password")
    ("address,A", po::value<std::string>(), "address to ping");
  po::store(po::parse_command_line(argc, argv, desc_), vm_);
  po::notify(vm_);
  
  check_options();
}

void OptionsGetloss::check_options() {
  if (vm_.count("help") != 0) {
    std::cout << desc_ << std::endl;
    std::exit(EXIT_SUCCESS);
  }
  if (
    vm_.count("dbname") == 0 ||
    vm_.count("dbhost") == 0 ||
    vm_.count("dbuser") == 0 ||
    vm_.count("address") == 0
   ) {
    std::cerr << "Error via processing arguments!" << std::endl << std::endl;
    std::cerr << desc_ << std::endl;
    std::exit(EXIT_FAILURE);
  }
}

std::string OptionsGetloss::getDbname() const {
  return vm_.at("dbname").as<std::string>();
}

std::string OptionsGetloss::getDbhost() const {
  return vm_.at("dbhost").as<std::string>();
}

std::string OptionsGetloss::getDbuser() const {
  return vm_.at("dbuser").as<std::string>();
}

std::string OptionsGetloss::getDbpass() const {
  return vm_.at("dbpass").as<std::string>();
}

std::string OptionsGetloss::getAddress() const {
  return vm_.at("address").as<std::string>();
}

Database::Database(const OptionsGetloss *options):
conn_("dbname=" + options->getDbname() + " user=" + options->getDbuser() + " password=" + options->getDbpass() + " hostaddr=" + options->getDbhost()), txn_(conn_) {
  
}
Database::~Database() {
  txn_.commit();
}
double Database::getLoss(std::string ip) const {
  double loss = 0;
  std::string req = "SELECT loss FROM ext_packetlosshd_dbg WHERE ip = '" + ip + "'";
  auto result = txn_.exec(req);
  if (result.size() > 0) {
    loss = result[0][0].as<double>();
    txn_.exec("UPDATE ext_packetlosshd_dbg SET last_read = NOW() WHERE ip = '" + ip + "'");
  }
  else 
    txn_.exec("INSERT INTO ext_packetlosshd_dbg (ip) VALUES ('" + ip + "')");
  txn_.exec("DELETE FROM ext_packetlosshd_dbg WHERE last_read + interval '7 days' < NOW()");
  return loss;
}

int main(int argc, char **argv) {
  OptionsGetloss options(argc, argv);
  Database db(&options);
  std::cout << db.getLoss(options.getAddress()) << std::endl;
  return EXIT_SUCCESS;
}
