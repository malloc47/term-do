#ifndef COMMON_H_
#define COMMON_H_

#include <vector>
#include <string>

using namespace std;

typedef vector<string> list_t;
typedef vector<string> *list_p;

typedef list_t (*list_f) ();
typedef bool (*insert_f) (string );
typedef int (*type_f) ();
typedef string (*cmd_f) ();

#define TYPE_NOMATCH 0;
#define TYPE_FIXED 1;
#define TYPE_ANY 2;
#define TYPE_FILE 3;

#define FOR_l(i,v) for(unsigned int (i)=0;(i)<(v).size();(i)++)
#define FLATTEN_l(l,s) \
  for(unsigned int flatten_var=0; flatten_var<(l).size(); flatten_var++) \
    s=s + (flatten_var ? " " : "") + (l)[flatten_var];

#define CONTAINS_l(l,s)					\
  (std::find((l).begin(), (l).end(),(s)) != (l).end())

#endif
