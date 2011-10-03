#include "standalone.h"

list_t load_plugins;
string library_path;

Standalone::Standalone() {
  view = new View("/-/");
  server = new TermDo(load_plugins);
  view->refreshLine(server->getQuery(),server->getMatches(),server->getTokens());
}

Standalone::~Standalone() {
  view->clearLine(); 
  delete server;
  delete view;
}

int Standalone::handleChar(char c) {
  bool done = false;

  // standard character range
  if(c >= 'a' && c <= 'z')
    server->addChar(c);
  // downcase "automatically"
  else if(c >= 'A' && c <= 'Z')
    server->addChar(c);
    // query->addChar(c-('Z'-'z'));
  // make slash act like a tab, for ido-like behavior
  else if(c == '/')
    server->addChar(c);
  // allow other valid characters
  else if( c > ' ' && c <= '~')
    server->addChar(c);
  // C-s
  else if(c==19)
    server->rotateForward();
  // C-r
  else if(c==18) {
    server->rotateBackward();
  }
  // tab
  else if(c==9)
      server->commitValidToken();
  // space
  else if(c==' ') // (c==32)
    server->commitToken();
  // enter
  else if(c==13) 
    done = server->commitFinalToken();
  // backspace
  else if(c==127)
    // if query out of characters to backspace
    server->removeChar();
  // C-f
  else if(c==6)
    server->fullList();
  
  // C-c , C-d , C-g, or time to quit
  return (c==3 || c==4 || c==7 || done);
}

string Standalone::loopDo() {
  int done=0;
  while(!done) {
    done = handleChar(view->getChar());
    view->refreshLine(server->getQuery(),server->getMatches(),server->getTokens());
  }

  // assume a terminal-executable command if no match is found--useful
  // for when something nontrivial using the dir.so plugin is entered
  string cmd = server->getCommand();
  if(cmd.empty()) {
    list_t list = server->getTokens();
    string output ="";
    FOR_l(i,list)
	output = output + (i==0 || is_dir(list[i-1]) || is_opt(list[i-1]) ? "" : " ") + list.at(i);
    return output;
  }
  else
    return cmd;
}

void Standalone::run(string cmd) {
  view->clearLine();
  delete view;
  if(!cmd.empty()) system(cmd.c_str());
  view = new View("/-/");
}

void Standalone::reset() {
  server->reset();
  view->refreshLine(server->getQuery(),server->getMatches(),server->getTokens());
}
