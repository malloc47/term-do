#ifndef TERMDO_H_
#define TERMDO_H_

#include "common.h"
#include "vt100.h"
#include "query.h"
#include "plugins.h"
#include "view.h"
#include "server.h"
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

class TermDo {
public:
  TermDo();
  ~TermDo();
  string loopDo();

private:
  void init();
  void cleanup();
  int handleChar(char);
  bool commitValidToken();
  bool commitToken();
  class View view;
  class Server *server;
};

#endif
