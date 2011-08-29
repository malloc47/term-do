#include "vt100.h"

VT100::VT100() {
  // place us in raw mode
  struct termios new_termios;
  setvbuf(stdout, NULL, _IONBF ,0);
  tcgetattr(0, &old_termios);

  new_termios=old_termios;
  new_termios.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
  new_termios.c_oflag &= ~(OPOST);
  new_termios.c_cflag |= (CS8);
  new_termios.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
  new_termios.c_cc[VMIN]=1;
  new_termios.c_cc[VTIME]=2;
  tcsetattr(0, TCSAFLUSH, &new_termios);
}

VT100::~VT100() {
  // return us to the previous mode
  tcsetattr(0, TCSANOW, &old_termios);
}

void VT100::operator<< (const char * param) {printLine(param);}

void VT100::clearLine() {printf("\33[2K\r");}

char VT100::getChar() {return getchar();}

void VT100::pushCursor() {printf("\33[s");}

void VT100::popCursor() {printf("\33[u");}

void VT100::printLine(const char * input) {printf(input);}

int VT100::getWidth() {
    struct winsize term_size;
    if (ioctl(1, TIOCGWINSZ, &term_size) != -1) 
      return term_size.ws_col;
    return 80;
}
