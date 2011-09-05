#ifndef COMMON_H_
#define COMMON_H_

#include <vector>
#include <string>

using namespace std;

typedef vector<string> list_t;
typedef vector<string> *list_p;

typedef list_t (*list_f) ();
typedef void (*update_f) (list_t);
typedef string (*cmd_f) ();
typedef void (*init_f) ();

typedef struct {
  list_f list;
  update_f update;
  cmd_f cmd;
  init_f init;
  void *handle;
} plugin_t;

#define TYPE_NOMATCH 0;
#define TYPE_FIXED 1;
#define TYPE_ANY 2;
#define TYPE_FILE 3;

#define FOR_l(i,v) for(unsigned int (i)=0;(i)<(v).size();(i)++)
#define FORB_l(i,v) for(unsigned int (i)=(v).size()-1;(i)>0;(i)++)
#define FLATTEN_l(l,s)							\
  for(unsigned int flatten_var=0; flatten_var<(l).size(); flatten_var++) \
    s=s + (flatten_var ? " " : "") + (l)[flatten_var];
#define CONTAINS_l(l,s)					\
  (std::find((l).begin(), (l).end(),(s)) != (l).end())

#define CONCAT2_l(p,q,v)			\
  list_t (v);					\
  (v).reserve((p).size()+(q).size());		\
  (v).insert((v).end(),(p).begin(),(p).end());	\
  (v).insert((v).end(),(q).begin(),(q).end());

inline bool is_dir(string input) {
  if(input.empty()) return false;
  return input[input.size()-1]=='/';
}  

#endif
