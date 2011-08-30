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
  matcher.setDictionary(verbs.getVerbs());
  refreshLine();
}

string TermDo::formatMatches(vector<string> matches, unsigned int length) {
  string output = "{";
  for(unsigned int i=0;i<matches.size();i++) {
    unsigned int j = ((i+match_offset) % matches.size());
    if(output.length() + matches.at(j).length() + 1 > length)
      break;
    else
      output = output + (i==0 ? "" : " | ") + 
	matches.at(j).substr(matcher.getQuery().length());
  }
  output = output + "}";
  return output;
}

string TermDo::formatTokens(vector<string> tokens) {
  string output = "";
  if(tokens.empty()) return output;
  for(unsigned int i=0;i<tokens.size();i++)
    output = output + (i==0 ? "" : " ") + tokens.at(i);;
  return "[" + output + "] ";
}

void TermDo::refreshLine() {
  term.clearLine();
  string output = prompt + formatTokens(verbs.getTokens()) + matcher.getQuery();
  term << output.c_str();
  term.pushCursor();
  string matches = formatMatches(matcher.getMatches(),
				 term.getWidth() - output.length());
  term << matches.substr(0,term.getWidth() - output.length()).c_str();
  term.popCursor();
}

int TermDo::handleChar(char c) {
  bool done = false;

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
    matcher.rotateForward();
  // C-r
  else if(c==18) {
    matcher.rotateBackward();
  }
  // tab
  else if(c==9) {
    if(matcher.getMatches().size()==1)
      done=commitToken();
    // else if(matcher.getMatches().size() > 1) {
    //   bool same=true;
    //   for(int i=0;i<matcher.getMatches().size();i++)
    // 	if(matcher.getMatches().at(i).substr(matcher.getQuery().size()).empty() ||
    // 	   ((i>0) && 
    // 	    matcher.getMatches().at(i).at(matcher.getQuery().size()) != matcher.getMatches().at(i-1).at(matcher.getQuery().size())))
    // 	  same=false;
    //   if(same) matcher.addChar(c);
    // }
  }
  // enter
  else if(c==13)
    done=commitToken();
  // backspace
  else if(c==127)
    // if matcher out of characters to backspace
    if(!matcher.removeChar()) {
      // remove token
      verbs.pop();
      matcher.setDictionary(verbs.getVerbs());
    }

  // C-c , C-d , C-g, Enter
  return (c==3 || c==4 || c==7 || done);
}

bool TermDo::commitToken() {
  if(!matcher.getMatches().front().compare(""))
    return false;

  verbs.push(matcher.getMatches().front());

  vector<string> verb = verbs.getVerbs();
  if(verb.size() < 2)
    return true;

  matcher.setDictionary(verb);
  return false;
}

void TermDo::setPrompt(string prompt) {
  this->prompt = prompt;
}

string TermDo::loopDo() {
  int done=0;
  while(!done) {
    done = handleChar(term.getChar());
    refreshLine();
  }
  return verbs.getVerbs().front();
}

int main(int argc, char *argv[]) {
  string command;

  // like this so TermDo (and subsequently VT100's) destructor is called
  {
    TermDo term_logic("/-/ ");
    command = term_logic.loopDo();
  }

  system(command.c_str());
}
