#include "vt100.h"

VT100::VT100() {
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
  tcsetattr(0, TCSANOW, &old_termios);
}

void VT100::clearLine() {
    printf("\33[2K\r");
}
