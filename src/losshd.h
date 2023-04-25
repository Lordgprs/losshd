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
    kEchoReply = 0,
    kDestinationUnreachable = 3,
    kSourceQuench = 4,
    kRedirect = 5,
    kEchoRequest = 8,
    kTimeExceeded = 11,
    kParameterProblem = 12,
    kTimestampRequest = 13,
    kTimestampReply = 14,
    kInfoRequest = 15,
    kInfoReply = 16,
    kAddressRequest = 17,
    kAddressReply = 18
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
  void CalculateChecksum(auto body_begin, auto body_end); 
  friend std::istream& operator>>(std::istream &inputStream, Icmp &header);
  friend std::ostream& operator<<(std::ostream &outputStream, const Icmp &header);

private:
  uint16_t Decode(int32_t a, int32_t b) const {
    return (data_[a] << 8) + data_[b];
  }
  void Encode(int32_t a, int32_t b, uint16_t n) {
    data_[a] = static_cast<char8_t>(n >> 8);
    data_[b] = static_cast<char8_t>(n & 0xFF);
  }

  char8_t data_[8];
};


class IcmpSender {
public:
  IcmpSender() = delete;
  IcmpSender(const IcmpSender &) = delete;
  IcmpSender(const IcmpSender &&) = delete;
  IcmpSender & operator=(const IcmpSender &) = delete;
  IcmpSender & operator=(const IcmpSender &&) = delete;
  IcmpSender(boost::asio::io_context &, const char *, uint16_t, uint16_t, std::mutex &, std::condition_variable &);
  uint16_t sent() const;

private:
  void StartSend();

  icmp::endpoint destination_;
  icmp::socket socket_;
  uint16_t sequence_number_;
  uint16_t count_;
  chrono::steady_clock::time_point time_sent_;
  boost::asio::streambuf reply_buffer_;
  std::size_t requests_count_;
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
  void StartReceive();
  void CheckIfSendersExist();
  void HandleReceive(std::size_t length);
  
  icmp::socket socket_;
  chrono::steady_clock::time_point time_sent_;
  boost::asio::streambuf reply_buffer_;
  int &senders_count_;
  std::unordered_map<uint32_t, uint32_t> &ping_results_;
  std::mutex &mtx_;
  std::condition_variable &condition_;
  bool senders_unlocked_ = false;
  boost::asio::deadline_timer dt_;
  const boost::posix_time::time_duration kReceiveTimerFrequency = boost::posix_time::seconds(5);
};

class OptionsLosshd : public Options {
public:
  OptionsLosshd() = delete;
  OptionsLosshd(const OptionsLosshd &) = delete;
  OptionsLosshd(const OptionsLosshd &&) = delete;
  OptionsLosshd & operator=(const OptionsLosshd &) = delete;
  OptionsLosshd & operator=(const OptionsLosshd &&) = delete;
  OptionsLosshd (int, char **);
  std::string get_dbname() const;
  std::string get_dbhost() const;
  std::string get_dbuser() const;
  std::string get_dbpass() const;
  uint16_t get_count() const;
  uint16_t get_interval() const;
  uint16_t get_size() const;
  bool is_daemon() const;

private:
  void CheckOptions();
};

class Scheduler {
public:
  Scheduler(OptionsLosshd &);
  ~Scheduler();
  void Run();
  void Clean();

private:
  uint32_t GetIpFromString (const std::string &str_ip);
  std::string GetIpFromUint32 (const uint32_t ip) const;
  std::vector<std::string> GetAddressesForPing() const;
  auto CreateReceiver(); 
  auto CreateSender(); 
  
  OptionsLosshd &options_;
  pqxx::connection conn_;
  pqxx::nontransaction mutable txn_;
  std::mutex mtx_;
  std::vector<std::string> address_list_;
  std::unordered_map<uint32_t, uint32_t> ping_results_;
  std::condition_variable condition_;
};

#endif
