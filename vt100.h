#ifndef VT100_H_
#define VT100_H_

#include <unistd.h>
#include <termios.h>
#include <stdio.h>
#include <stdlib.h>

class VT100 {
public:
  VT100();
  ~VT100();
  void clearLine(void);

private:
  struct termios old_termios;
};

#endif
