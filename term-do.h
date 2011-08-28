#ifndef TERMDO_H_
#define TERMDO_H_

#include "vt100.h"
#include <stdio.h>
#include <stdlib.h>
#include <string>

using namespace std;

class TermDo {
public:
  TermDo(string);
  void loop_do(VT100);
  void setPrompt(string);
  int handle_char(int);

private:
  string prompt;
};

#endif
