#ifndef COMMON_H_
#define COMMON_H_

#include <vector>
#include <string>

using namespace std;

typedef vector<string> list_t;
typedef vector<string> *list_p;

#define FOR_l(i,v) for(unsigned int (i)=0;(i)<(v).size();(i)++)
#define FORB_l(i,v) for(int (i)=(v).size()-1;(i)>=0;(i)--)
#define SEARCHER_t TST

inline bool is_dir(string input) {
  if(input.empty()) return false;
  return input[input.size()-1]=='/';
}

inline bool is_opt(string input) {
  if(input.empty()) return false;
  return input[input.size()-1]=='=';
}

inline string assembleLine(list_t line, unsigned int offset = 0) {
  string output = "";
  for(unsigned int i=offset; i<line.size(); i++)
    output = output + (i==0 || is_dir(line[i-1]) || is_opt(line[i-1])  ? "" : " ") + line[i];
  return output;
}

extern string library_path; // should not be included by plugins

#endif
