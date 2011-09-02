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
string command="";
stack <string> args;;
int match_type=TYPE_FIXED;

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

bool insert(string s) {
  if(command.empty()) {
    if(CONTAINS_l(dictionary,s)) {
      command = s;
      match_type=TYPE_ANY;
      return true;
    }
    else
      return false;
  }
  args.push(s);
  return true;
}

vector<string> list() {
  if(command.empty()) {
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
    return list_t();
  }
}

int type() {return match_type;}

string cmd() {
  string output = command;
  while(!args.empty()) {
    output = output + " " + args.top();
    args.pop();
  }
  return output;
}

#ifdef __cplusplus
}
#endif
