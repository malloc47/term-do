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
  else if( c >= ' ' && c <= '~')
    matcher->addChar(c);
  // C-s
  else if(c==19)
    matcher->rotateForward();
  // C-r
  else if(c==18) {
    matcher->rotateBackward();
  }
  // tab
  else if(c==9) {
    if(matcher->getMatches().size()==1)
      done=commitToken();
    // else if(matcher->getMatches().size() > 1) {
    //   bool same=true;
    //   for(int i=0;i<matcher->getMatches().size();i++)
    // 	if(matcher->getMatches().at(i).substr(matcher->getQuery().size()).empty() ||
    // 	   ((i>0) && 
    // 	    matcher->getMatches().at(i).at(matcher->getQuery().size()) != matcher->getMatches().at(i-1).at(matcher->getQuery().size())))
    // 	  same=false;
    //   if(same) matcher->addChar(c);
    // }
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

  // C-c , C-d , C-g, Enter
  return (c==3 || c==4 || c==7 || done);
}

bool TermDo::commitToken() {
  if(!matcher->getMatches().front().compare(""))
    return false;

  plugins.push(matcher->getMatches().front());

  if(plugins.unambiguousCommand())
    return true;

  cleanup();
  init();
  return false;
}

string TermDo::loopDo() {
  int done=0;
  while(!done) {
    done = handleChar(view.getChar());
    view.refreshLine(matcher->getQuery(),matcher->getMatches(),plugins.getTokens());
  }
  // return matcher->getQuery().length() > 0 ? plugins.getDictionary().front() : "";
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
