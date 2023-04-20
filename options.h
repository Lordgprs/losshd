#ifndef LOSSHD_OPTIONS_H
#define LOSSHD_OPTIONS_H

#include <string>
#include <boost/program_options.hpp>

namespace po = boost::program_options;

class Options {
public:
  Options() = delete;
  Options(const Options &) = delete;
  Options(const Options &&) = delete;
  Options & operator=(const Options &) = delete;
  Options & operator=(const Options &&) = delete;
  Options (int, char **);
protected:
  po::options_description desc_;
  po::variables_map vm_;
};

#endif
