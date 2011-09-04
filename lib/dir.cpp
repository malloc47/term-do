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

bool valid_dir(string dir){
  struct stat entry;
  return !(stat(dir.c_str(), &entry) < 0) && S_ISDIR(entry.st_mode);
}

bool valid_file(string file){
  struct stat entry;
  return !(stat(file.c_str(), &entry) < 0) && S_ISREG(entry.st_mode);
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
  if(path.compare("/"))
     output.push_back("../");
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
  output.push_back("/");
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
  else { // this case may not be obvious, so here goes...
    // look at the last token to see if we're completing a directory
    // (if not, just show pwd)
    if(!isdir(tokens[tokens.size()-1])) return base_list();
    string listing="";
    
    // * walk *backwards* through the list to find the "first"
    //   directory entry
    // * this is to handle cases such as
    //     {cmd} first/directory/path/ --switch second/directory/path/
    //   where we only want to find second/ as the starting directory
    //   token so we don't end up trying to get the directory listing
    //   for /first/directory/path/second/directory/path
    // * this is done here by finding the first non-directory token
    //   and setting "start" to be its index
    unsigned int start = tokens.size()-1;
    for(;start>0;start--)
      if(!isdir(tokens[start-1])) {
    	break;
      }

    // now walk from the starting directory token (e.g. second/) to
    // the latest token, concatenating the directories
    for(unsigned int i=start;i<tokens.size();i++)
      listing = listing + (tokens[i].compare("~/") ? tokens[i] : home);

    // get both a directory and file listing
    list_t dir = ls_d(listing);
    list_t files = ls_f(listing);
    // concatenate the two listings
    CONCAT2_l(dir,files,output);
    return output;
  }
  return list_t();
}

// helper plugin--no need to return a complete command
string cmd() {
  return "";
}

#ifdef __cplusplus
}
#endif
