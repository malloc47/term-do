#include "term-do.h"

TermDo::TermDo() {
  prompt = "";
  init();
}

TermDo::TermDo(string prompt) {
  this->prompt = prompt;
  init();
}

TermDo::~TermDo() {
  term.clearLine();
}

void TermDo::init(){
  match_offset=0;
  refreshLine();
}

string TermDo::formatMatches(vector<string> matches, unsigned int length) {
  string output = "{";
  for(unsigned int i=0;i<matches.size();i++) {
    unsigned int j = ((i+match_offset) % matches.size());
    if(output.length() + matches.at(j).length() > length)
      break;
    else
      output = output + (i==0 ? "" : " | ") + matches.at(j);
  }
  output = output + "}";
  return output;
}

void TermDo::refreshLine() {
  term.clearLine();
  string output = prompt + matcher.getQuery();
  term << output.c_str();
  term.pushCursor();
  string matches = formatMatches(matcher.getMatches(),
				 term.getWidth() - output.length());
  term << matches.substr(0,term.getWidth() - output.length()).c_str();
  term.popCursor();
}

int TermDo::handleChar(char c) {
  // standard character range
  if(c >= 'a' && c <= 'z')
    matcher.addChar(c);
  // downcase "automatically"
  else if(c >= 'A' && c <= 'Z')
    matcher.addChar(c-('Z'-'z'));
  // allow other valid characters
  else if( c >= ' ' && c <= '~')
    matcher.addChar(c);
  // C-s
  else if(c==19)
    match_offset++;
  // C-r
  else if(c==18) {
    if(match_offset) 
      match_offset--;
    else
      match_offset=matcher.getMatches().size();
  }
  // enter
  else if(c==13)
    command=matcher.getMatches().at(match_offset % matcher.getMatches().size());
  // backspace
  else if(c==127)
    matcher.removeChar();

  // C-c , C-d , C-g, Enter
  return (c==3 || c==4 || c==7 || c==13);
}

void TermDo::setPrompt(string prompt) {
  this->prompt = prompt;
}

void TermDo::loopDo() {
  int done=0;
  while(!done) {
    done = handleChar(term.getChar());
    refreshLine();
  }
}

int main(int argc, char *argv[]) {
  // string command;

  // like this so TermDo (and subsequently VT100's) destructor is called
  {
    TermDo term_logic("/-/ ");
    term_logic.loopDo();
    // command = term_logic.command;
  }

  // system(command.c_str());

  // printf("\r%s\n",command.c_str());
}
