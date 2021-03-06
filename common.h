#ifndef COMMON_H_
#define COMMON_H_

#include "config.h"
#include <vector>
#include <string>

using namespace std;

typedef vector<string> list_t;
typedef vector< pair<int,string> > freq_list_t;
typedef vector<string> *list_p;

#define FOR_l(i,v) for(unsigned int (i)=0;(i)<(v).size();(i)++)
#define FORB_l(i,v) for(int (i)=(v).size()-1;(i)>=0;(i)--)
#define SEARCHER_t TST

#define MODE_STANDALONE 0
#define MODE_CLIENT 1
#define MODE_DAEMON 2

// globals (just easier to do this than thread them throuh the objects)
extern list_t load_plugins;
extern string library_path;
extern string config_folder;

inline bool is_dir(string input) {
  if(input.empty()) return false;
  return input[input.size()-1]=='/';
}

inline bool is_opt(string input) {
  if(input.empty()) return false;
  return input[input.size()-1]=='=';
}

inline void downcase(string &s) {
  FOR_l(i,s)
    if(s[i] >= 'A' && s[i] <= 'Z')
      s[i] -= ('Z'-'z');
}

inline string assembleLine(list_t &line, unsigned int offset = 0) {
  string output = "";
  for(unsigned int i=offset; i<line.size(); i++)
    output = output + (i==0 || is_dir(line[i-1]) || is_opt(line[i-1])  ? "" : " ") + line[i];
  return output;
}

extern string library_path; // should not be included by plugins

#endif
