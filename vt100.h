#ifndef VT100_H_
#define VT100_H_

#include <unistd.h>
#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>

#define TEXT_NORMAL "\33[0m"
#define TEXT_BOLD "\33[1m"

class VT100 {
public:
  VT100();
  ~VT100();
  void operator << (const char *);
  void clearLine();
  char getChar();
  void pushCursor();
  void popCursor();
  void printLine(const char *);
  int getWidth();

private:
  struct termios old_termios;
};

#endif
