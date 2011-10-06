#ifndef STANDALONE_H_
#define STANDALONE_H_

#include "common.h"
#include "vt100.h"
#include "query.h"
#include "plugins.h"
#include "view.h"
#include "term-do.h"
#include "frontend.h"
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

class Standalone : public Frontend {
public:
  Standalone();
  ~Standalone();
  string loopDo();
  void run(string);
  string static cd(string);
  void reset();

private:
  void init();
  void cleanup();
  int handleChar(char);
  class View *view;
  class TermDo *server;
};

#endif
