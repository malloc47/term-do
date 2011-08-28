#include "term-do.h"

TermDo::TermDo(string prompt) {
  this->prompt = prompt;
}

void TermDo::loop_do(VT100 term_control) {
  printf(prompt.c_str());
}

int TermDo::handle_char(int c) {
    if(c >= 'a' && c <= 'z') {
      printf("%c", c);
    }

    /* C-c or C-d */
    return (c==3 || c==4);

}

void TermDo::setPrompt(string prompt) {
  this->prompt = prompt;
}

int main(int argc, char *argv[]) {

  VT100 term_control;
  TermDo term_logic("/-/");

  int c,done;
  done=0;
  while(!done) {
    c=getchar();
    // printf(" got a  %03X ", c);
    // term_logic.loop_do(term_control);

    // if(c >= 'a' && c <= 'z') {

    //   printf("%c", c);
    //   if(c=='a') {
    // 	term_control.clearLine();
    // 	printf("You typed a ");
    //   }

    // }

    // /* C-c or C-d */
    // done = (c==3 || c==4);
    done = term_logic.handle_char(c);
  }

  term_control.clearLine();
}
