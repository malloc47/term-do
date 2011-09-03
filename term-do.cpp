#include "term-do.h"

TermDo::TermDo() {
  view.setPrompt("/-/");
  init();
}

TermDo::~TermDo() {view.clearLine(); cleanup();}

void TermDo::init() {
  matcher = new Matcher();
  plugins.getDictionary(matcher);
  view.refreshLine(matcher->getQuery(),matcher->getMatches(),plugins.getTokens());
}

void TermDo::cleanup() {
  delete matcher;
}

int TermDo::handleChar(char c) {
  bool done = false;

  // standard character range
  if(c >= 'a' && c <= 'z')
    matcher->addChar(c);
  // downcase "automatically"
  else if(c >= 'A' && c <= 'Z')
    matcher->addChar(c-('Z'-'z'));
  // allow other valid characters
  else if( c > ' ' && c <= '~')
    matcher->addChar(c);
  // C-s
  else if(c==19)
    matcher->rotateForward();
  // C-r
  else if(c==18) {
    matcher->rotateBackward();
  }
  // tab or space
  else if(c==9 || c==32) {
    if(matcher->getMatches().size()<=1 || matcher->exactMatch())
      commitToken();
  }
  // enter
  else if(c==13)
    done=commitToken();
  // backspace
  else if(c==127)
    // if matcher out of characters to backspace
    if(!matcher->removeChar()) {
      // remove token
      plugins.pop();
      cleanup();
      init();
    }
  
  // C-c , C-d , C-g, or time to quit
  return (c==3 || c==4 || c==7 || done);
}

bool TermDo::commitToken() {
  string match = matcher->getMatch();

  if(match.empty())
    return false;

  plugins.push(match);

  // get a new matcher
  cleanup();
  init();
  return true;
}

string TermDo::loopDo() {
  int done=0;
  while(!done) {
    done = handleChar(view.getChar());
    view.refreshLine(matcher->getQuery(),matcher->getMatches(),plugins.getTokens());
  }
  return plugins.getCommand();
}

int main(int argc, char *argv[]) {
  string command;

  // like this so TermDo (and subsequently VT100's) destructor is called
  {
    TermDo term_logic;
    command = term_logic.loopDo();
  }
  
  if(!command.empty())
    system(command.c_str());
}
