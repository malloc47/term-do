#ifndef VIEW_H_
#define VIEW_H_

#include "common.h"
#include <vector>
#include <string>
#include "vt100.h"
#include "query.h"
#include "plugins.h"

using namespace std;

class View : public VT100 {
public:
  View();
  View(string);
  ~View();
  void setPrompt(string);
  void refreshLine(string&,list_t&,list_t&);
  static string formatList(list_t&,const string,const string,const string);
  static string formatList(list_t&,const string,const string,const string,const unsigned int);
  static list_t chopList(list_t,const string);
private:
  string prompt;
};


#endif
