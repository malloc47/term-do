#include "view.h"

View::View() {prompt=""; init();}
View::~View() {}
void View::init() {}
void View::setPrompt(string prompt) {this->prompt = prompt;}

string View::formatList(vector<string> list, const string d1, const string sep, const string d2) {
  string output = "";
  if(list.empty()) return output;
  for(unsigned int i=0;i<list.size();i++)
    output = output + (i==0 ? "" : sep) + list.at(i);;
  return d1 + output + d2;
}

string View::formatList(vector<string> list, const string d1, const string sep, const string d2,const unsigned int length) { 
  string output = "";
  if(list.empty()) return output;
  for(unsigned int i=0;i<list.size();i++)
    if(output.length() + list.at(i).length() + 1 > length)
      break;
    else
      output = output + (i==0 ? "" : sep) + list.at(i);;
  return d1 + output + d2;
}

vector<string> View::chopList(vector<string> list, unsigned int length) {
  for(unsigned int i=0;i<list.size();i++)
    list.at(i) = list.at(i).substr(length);
  return list;
}

void View::refreshLine(string query, vector<string> matches, vector<string> verbs) {
  clearLine();
  string output = prompt + formatList(verbs,"["," ","]") + " " + query;
  *this << output.c_str();
  pushCursor();
  string match_str = formatList(chopList(matches,query.length()),"{"," | ","}",
				getWidth() - output.length());
  *this << match_str.substr(0,getWidth() - output.length()).c_str();
  popCursor();
}
