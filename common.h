#ifndef COMMON_H_
#define COMMON_H_

#include <vector>
#include <string>

using namespace std;

typedef vector<string> list_t;
typedef vector<string> *list_p;

// // typedef list_t (*list_f) ();
// // typedef void (*update_f) (list_t);
// // typedef string (*cmd_f) ();
// // typedef void (*init_f) ();

// typedef Plugin* (*create_f)();
// typedef void (*destroy_f)(Plugin*);

// typedef struct {
//   // list_f list;
//   // update_f update;
//   // cmd_f cmd;
//   // init_f init;
//   // void *handle;
//   create_f create;
//   Plugin* obj;
//   destroy_f destroy;
//   void *handle;
// } plugin_t;

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
