#ifndef LOSSHD_GETLOSS_H
#define LOSSHD_GETLOSS_H

#include "options.h"

class OptionsGetloss : public Options {
public:
  OptionsGetloss() = delete;
  OptionsGetloss(const OptionsGetloss &) = delete;
  OptionsGetloss(const OptionsGetloss &&) = delete;
  OptionsGetloss & operator=(const OptionsGetloss &) = delete;
  OptionsGetloss & operator=(const OptionsGetloss &&) = delete;
  OptionsGetloss(int argc, char **argv);
  void check_options();
};

#endif
