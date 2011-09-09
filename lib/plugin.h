#ifndef PLUGIN_H_
#define PLUGIN_H_

#include "../common.h"

class Plugin {
public:
  virtual list_t list() = 0;
  virtual void update(list_t) = 0;
  virtual string cmd() = 0;
  virtual void init() = 0;
};

#endif
