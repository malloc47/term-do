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
#include "../common.h"

using namespace std;

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

#ifdef __cplusplus
extern "C" {
#endif

void init() {}

void update(list_t new_tokens) {tokens = new_tokens;}

list_t list() {
  if(tokens.empty()) {
    // don't bother recomputing executable files
    if(!dictionary.empty()) return dictionary;

    list_t paths = splitString(string(getenv("PATH")),':');

    FOR_l(i,paths) {
      DIR* dirp = opendir(paths[i].c_str());
      struct dirent *entry;
      struct stat st;
      if(dirp==NULL) {
	// printf("\rInvalid directory on $PATH: %s\n",paths[i].c_str());
	continue;
      }
      while ((entry = readdir(dirp)) != NULL) {
	string name(entry->d_name);
	if(stat((paths[i]+"/"+name).c_str(),&st)) continue;
	if(S_ISREG(st.st_mode) && access((paths[i]+"/"+name).c_str(),X_OK) == 0)
	  dictionary.push_back(name);
      }
      closedir(dirp);
    }
    return dictionary;
  }
  else {
    if(!tokens.at(0).compare("vim") && tokens[tokens.size()-1][tokens[tokens.size()-1].size()-1] != '/') {
      list_t params;
      params.push_back("--");
      params.push_back("-v");
      params.push_back("-e");
      params.push_back("-s");
      params.push_back("-d");
      params.push_back("-y");
      params.push_back("-R");
      params.push_back("-Z");
      params.push_back("-m");
      params.push_back("-M");
      params.push_back("-b");
      params.push_back("-l");
      params.push_back("-C");
      params.push_back("-N");
      params.push_back("-V");
      params.push_back("-D");
      params.push_back("-n");
      params.push_back("-r");
      params.push_back("-r");
      params.push_back("-L");
      params.push_back("-A");
      params.push_back("-H");
      params.push_back("-F");
      params.push_back("-T");
      params.push_back("-u");
      params.push_back("--noplugin");
      params.push_back("-p");
      params.push_back("-o");
      params.push_back("-O");
      params.push_back("+");
      params.push_back("--cmd");
      params.push_back("-c");
      params.push_back("-S");
      params.push_back("-s");
      params.push_back("-w");
      params.push_back("-W");
      params.push_back("-x");
      params.push_back("--startuptime");
      params.push_back("-i");
      params.push_back("-h");
      params.push_back("--help");
      params.push_back("--version");
      return params;
    }
    return list_t();
  }
}

string cmd() {
  string output = "";
  FOR_l(i,tokens)
    output = output + (i? (tokens[i-1][tokens[i-1].size()-1]=='/' ? "" : " ") :"") + tokens[i];
  return output;
}

#ifdef __cplusplus
}
#endif
