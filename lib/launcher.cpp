#include <vector>
#include <string>
#include <stack>
#include <algorithm>
#include <sstream>
#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include "plugin.h"

using namespace std;

class Launcher : public Plugin {
public:
  void init() {
    list_t paths = splitString(string(getenv("PATH")),':');
    FOR_l(i,paths) {
      DIR* dirp = opendir(paths[i].c_str());
      struct dirent *entry;
      struct stat st;
      if(dirp==NULL) continue;
      while ((entry = readdir(dirp)) != NULL) {
	string name(entry->d_name);
	if(stat((paths[i]+"/"+name).c_str(),&st)) continue;
	if(S_ISREG(st.st_mode) && access((paths[i]+"/"+name).c_str(),X_OK) == 0)
	  dictionary.push_back(name);
      }
      closedir(dirp);
    }
  }

  void update(list_t new_tokens) {tokens = new_tokens;}
  list_t list() {
    // return +x files on $PATH if no command has been typed yet, or if
    // its a command that takes other commands (e.g. watch)
    if(tokens.empty() || (!tokens[0].compare("time") || 
			  !tokens[0].compare("watch")))
      return dictionary;
    else
      return list_t();
  }

  string cmd() {
    // recognize only exec-able commands 
    if((!tokens.empty() && is_cmd(tokens[0])) ) {
      string output = "";
      // glob together tokens
      FOR_l(i,tokens)
	output = output + (i? (is_dir(tokens[i-1]) ? "" : " ") :"") + tokens[i];
      return output;
    }
    return "";
  }

private:
  list_t dictionary;
  list_t tokens;
  list_t splitString(const string s, char delim) {
    list_t elems;
    stringstream ss(s);
    string item;
    while(getline(ss,item,delim)) {
      elems.push_back(item);
    }
    return elems;
  }
  bool is_cmd(string input) {
    return find(dictionary.begin(), dictionary.end(), input) != dictionary.end();
  }
};

extern "C" Plugin* create_plugin() {return new Launcher;}
extern "C" void destroy_plugin( Plugin* plugin ) {delete plugin;}
