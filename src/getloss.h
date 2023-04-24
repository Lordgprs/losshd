#ifndef LOSSHD_GETLOSS_H
#define LOSSHD_GETLOSS_H

#include "options.h"
#include <pqxx/pqxx>

class OptionsGetloss : public Options {
public:
  OptionsGetloss() = delete;
  OptionsGetloss(const OptionsGetloss &) = delete;
  OptionsGetloss(const OptionsGetloss &&) = delete;
  OptionsGetloss & operator=(const OptionsGetloss &) = delete;
  OptionsGetloss & operator=(const OptionsGetloss &&) = delete;
  OptionsGetloss(int argc, char **argv);
  void check_options();
  std::string getDbname() const;
  std::string getDbhost() const;
  std::string getDbuser() const;
  std::string getDbpass() const;
  std::string getAddress() const;
};

class Database {
public:
  Database() = delete;
  Database(const OptionsGetloss *);
  ~Database();
  double getLoss(std::string) const;
private:
  pqxx::connection conn_;
  pqxx::work mutable txn_;
};


#endif
