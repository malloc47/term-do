#ifndef TERMDO_H_
#define TERMDO_H_

#include "common.h"
#include "vt100.h"
#include "matcher.h"
#include "verbs.h"
#include "view.h"
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>

using namespace std;

class TermDo {
public:
  TermDo();
  ~TermDo();
  bool commitToken();
  string loopDo();

private:
  int handleChar(char);
  class Matcher matcher;
  class Verbs verbs;
  class View view;
  string command;
};

#endif
