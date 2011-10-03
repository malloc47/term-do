#ifndef PLUGIN_H_
#define PLUGIN_H_

#include "../common.h"

class Plugin {
public:
  // virtual ~Plugin() = 0;
  // Plugin(string);
  virtual list_t list() = 0;
  virtual list_t list(string in) {return this->list();}
  virtual void update(list_t) = 0;
  virtual void update(string) {}
  virtual string cmd() = 0;
  virtual bool match() = 0;
  virtual string name() = 0;
  virtual string complete() {return "";}
};

// Plugin::~Plugin() {}

#endif
