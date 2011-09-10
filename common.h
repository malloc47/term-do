#ifndef COMMON_H_
#define COMMON_H_

#include <vector>
#include <string>

using namespace std;

typedef vector<string> list_t;
typedef vector<string> *list_p;

#define FOR_l(i,v) for(unsigned int (i)=0;(i)<(v).size();(i)++)
#define FORB_l(i,v) for(unsigned int (i)=(v).size()-1;(i)>0;(i)++)

inline bool is_dir(string input) {
  if(input.empty()) return false;
  return input[input.size()-1]=='/';
}

inline string assembleLine(list_t line) {
  string output = "";
  FOR_l(i,line)
    output = output + (i==0 || is_dir(line[i-1]) ? "" : " ") + line[i];
  return output;
}

#endif
