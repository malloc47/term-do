#include "view.h"

View::View() {prompt=""; init();}
View::~View() {}
void View::init() {}
void View::setPrompt(string prompt) {this->prompt = prompt;}

string View::formatList(list_t list, const string d1, const string sep, const string d2) {
  string output = "";
  if(list.empty()) return output;
  FOR_l(i,list)
    output = output + (i==0 ? "" : sep) + list.at(i);;
  return d1 + output + d2;
}

string View::formatList(list_t list, const string d1, const string sep, const string d2,const unsigned int length) { 
  string output = "";
  if(list.empty()) return output;
  if(list.size()==1 && list.front().size()==0) return output;
  FOR_l(i,list)
    if(output.length() + list.at(i).length() + 1 > length)
      break;
    else
      output = output + (i==0 ? "" : sep) + list.at(i);;
  return d1 + output + d2;
}

list_t View::chopList(list_t list, unsigned int length) {
  FOR_l(i,list)
    list.at(i) = list.at(i).substr(length);
  return list;
}

void View::refreshLine(string query, list_t matches, list_t tokens) {
  clearLine();
  string output = prompt + formatList(tokens,"["," ","]") + " " + query;
  *this << output.c_str();
  pushCursor();
  string match_str = formatList(chopList(matches,
					 query.length()),
				"{"," | ","}",
				getWidth() - output.length());  
  *this << match_str.substr(0,getWidth() - output.length()).c_str();
  popCursor();
}
