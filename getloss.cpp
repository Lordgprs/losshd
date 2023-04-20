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
    ("dbpass,P", po::value<std::string>(), "database password");
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
    vm_.count("dbuser") == 0
   ) {
    std::cerr << "Error via processing arguments!" << std::endl << std::endl;
    std::cerr << desc_ << std::endl;
    std::exit(EXIT_FAILURE);
  }
}

class Database {
public:
  Database() = delete;
  Database(const std::string, const std::string, const std::string, const std::string) {
    
  }
  ~Database();
private:
  pqxxx::connection conn_;
  pqxx::work txn_;
};
Database::Database(const std::string dbname, const std::string dbhost, const std::string dbuser, const std::string dbpass) {
  
}

int main(int argc, char **argv) {
  OptionsGetloss options(argc, argv);
  return EXIT_SUCCESS;
}
