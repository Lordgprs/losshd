#ifndef LOSSHD_GETLOSS_H
#define LOSSHD_GETLOSS_H

#include <iostream>
#include <boost/program_options.hpp>
#include <pqxx/pqxx>
#include "options.h"

class OptionsGetloss : public Options {
public:
  OptionsGetloss() = delete;
  OptionsGetloss(const OptionsGetloss &) = delete;
  OptionsGetloss(const OptionsGetloss &&) = delete;
  OptionsGetloss & operator=(const OptionsGetloss &) = delete;
  OptionsGetloss & operator=(const OptionsGetloss &&) = delete;
  OptionsGetloss(int argc, char **argv);
  void CheckOptions();
  std::string get_dbname() const;
  std::string get_dbhost() const;
  std::string get_dbuser() const;
  std::string get_dbpass() const;
  std::string get_address() const;
};

class Database {
public:
  Database() = delete;
  Database(const OptionsGetloss &);
  ~Database();
  double get_loss(const std::string &) const;
private:
  pqxx::connection conn_;
  pqxx::work mutable txn_;
};


#endif
