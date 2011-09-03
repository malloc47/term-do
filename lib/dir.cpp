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

list_t tokens;
const string pwd = string(getenv("PWD"))+"/";
const string home = string(getenv("HOME"))+"/";

bool valid(string dir){
  struct stat entry;
  return !(stat(dir.c_str(), &entry) < 0) && S_ISDIR(entry.st_mode);
}

bool isdir(string token) {return token[token.size()-1]=='/';}

list_t ls_d(string path) {
  list_t output;
  DIR* dirp = opendir(path.c_str());
  struct dirent *file;
  if(dirp==NULL)
    return list_t();
  while ((file = readdir(dirp)) != NULL) {
    string name(file->d_name);
    if(!name.compare(".") || !name.compare("..")) continue;
    struct stat st;
    if(stat((path+"/"+name).c_str(),&st)) continue;
    if(S_ISDIR(st.st_mode))
      output.push_back(string(file->d_name)+"/");
  }
  closedir(dirp);
  return output;
}

list_t ls_f(string path) {
  list_t output;
  DIR* dirp = opendir(path.c_str());
  struct dirent *file;
  if(dirp==NULL)
    return list_t();
  while ((file = readdir(dirp)) != NULL) {
    string name(file->d_name);
    struct stat st;
    if(stat((path+"/"+name).c_str(),&st)) continue;
    if(S_ISREG(st.st_mode))
      output.push_back(string(file->d_name));
  }
  closedir(dirp);
  return output;
}

list_t base_list() {
  list_t dir = ls_d(pwd);
  list_t files = ls_f(pwd);
  CONCAT2_l(dir,files,output);
  output.push_back("../");
  output.push_back("~/");
  return output;
}

#ifdef __cplusplus
extern "C" {
#endif

void update(list_t new_tokens) {tokens = new_tokens;}

list_t list() {
  if(tokens.empty()) {
    return base_list();
  }
  else {
    if(!isdir(tokens[tokens.size()-1])) return base_list();
    string listing="";
    
    // unsigned int start = tokens.size()-1;
    // for(;start>0;start++)
    //   if(tokens[start].at(tokens[start].size()-1)=='/')
    // 	break;

    // for(unsigned int i=start;i<tokens.size();i++)
    //   output = output + tokens[i];

    // find index of first directory listing
    unsigned int start = tokens.size()-1;
    for(;start>0;start--)
      if(!isdir(tokens[start-1])) {
    	break;
      }

    for(unsigned int i=start;i<tokens.size();i++)
      listing = listing + (tokens[i].compare("~/") ? tokens[i] : home);

    // FOR_l(i,tokens)
    //   if(tokens[i].at(tokens[i].size()-1)=='/')
    // 	pwd = pwd + tokens[i];

    list_t dir = ls_d(listing);
    list_t files = ls_f(listing);
    CONCAT2_l(dir,files,output);
    output.push_back("../");
    output.push_back("~/");
    return output;
  }
  return list_t();
}

string cmd() {
  return "";
}

#ifdef __cplusplus
}
#endif
