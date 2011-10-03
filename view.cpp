#include "view.h"

View::View() {prompt="";}
View::View(string pmt) {prompt=pmt;}
View::~View() {}
void View::setPrompt(string prompt) {this->prompt = prompt;}

string View::formatList(list_t &list, const string d1, const string sep, const string d2) {
  string output = "";
  if(list.empty()) return output;
  FOR_l(i,list)
    output = output + (i==0 || is_dir(list[i-1]) || is_opt(list[i-1]) ? "" : sep) + list.at(i);
  return d1 + output + d2;
}

string View::formatList(list_t &list, const string d1, const string sep, const string d2,const unsigned int length) { 
  string output = "";
  if(list.empty()) return output;
  if(list.size()==1 && list.front().size()==0) return output;
  FOR_l(i,list)
    if(output.length() + list.at(i).length() + sep.length()*2 +
       d2.length()*2 + string("... ").length() > length) {
      output = output + sep + "... ";
      break;
    }
    else
      output = output + (i==0 ? "" : sep) + list.at(i);;
  return d1 + output + d2;
}

list_t View::chopList(list_t list, const string prefix) {
  if(list.empty()) return list;
  FOR_l(i,list) {
    string item = list[i];
    // normal chop
    if(!item.compare(0,prefix.length(),prefix))
      list.at(i) = item.substr(prefix.length());
    else
      list.at(i) = "["+item+"]";
  }
  return list;
}

void View::refreshLine(string &query, list_t &matches, list_t &tokens) {
  clearLine();
  string output = formatList(tokens,"["," ","]") + " ";
  unsigned int len = getWidth()*2/3;
  if(output.length() > len)
    output = "[ ... " + output.substr(output.length()-len,len-1);
  output = prompt + output + query;
  *this << output.c_str();
  pushCursor();
  list_t chopped = chopList(matches, query);
  string match_str = formatList(chopped,
				"{"," | ","}",
				getWidth() - output.length());  
  *this << match_str.substr(0,getWidth() - output.length()).c_str();
  popCursor();
}
