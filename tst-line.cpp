#include "tst-line.h"

void TSTLine::insert(string s) {
  if(s.find(" ")!=string::npos) {
    lines.push_back(s);
    list_t split_line = splitLine(s);
    FOR_l(i,split_line)
      TST::insert(split_line[i]);
  }
  else
    TST::insert(s);
}
// bool TSTLine::search(string s) {}

list_t TSTLine::searchp(string s) {
  list_t candidates = TST::searchp(s);
  list_t output = candidates;
  //FIXME: really ugly brute-force hack
  FOR_l(i,lines)
    FOR_l(j,candidates)
    if(lines[i].find(candidates[j]) != string::npos) {
	output.push_back(lines[i]);
	break;
    }
  return output;
}

// bool TSTLine::containsp(string s) {}

list_t TSTLine::splitLine(const string s) {
  list_t output;
  stringstream ss(s);
  string item;
  while(getline(ss,item,' '))
    if(!item.empty() && item.compare(" "))
      output.push_back(item);
  return output;
}

// list_t TSTLine::splitPath(const string s) {
//   list_t output;
//   stringstream ss(s);
//   string item;
//   while(getline(ss,item,"/"))
//     if(!item.empty() && item.compare("/"))
//       output.push_back(item+"/");
//   return output;
// }
