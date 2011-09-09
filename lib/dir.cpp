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

#define CONCAT2_l(p,q,v)			\
  list_t (v);					\
  (v).reserve((p).size()+(q).size());		\
  (v).insert((v).end(),(p).begin(),(p).end());	\
  (v).insert((v).end(),(q).begin(),(q).end());

using namespace std;

class Dir : public Plugin {
public:
  Dir() {
    PWD = string(getenv("PWD"))+"/";
    HOME = string(getenv("HOME"))+"/";
  }
  void init() {}
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
	if(!isdir(tokens[start-1]))
	  break;

      // now walk from the starting directory token (e.g. second/) to
      // the latest token, concatenating the directories
      for(unsigned int i=start;i<tokens.size();i++)
	listing = listing + (tokens[i].compare("~/") ? tokens[i] : HOME);

      // get both a directory and file listing
      list_t dir = ls_d(listing);
      // it's not remotely legitimate to blanketly assume that ./ is
      // going to be used solely for exec-able scripts...
      list_t files = (!tokens[start].compare("./")) ? ls_fe(listing) : ls_f(listing);
      // concatenate the two listings
      CONCAT2_l(dir,files,output);
      return output;
    }
    return list_t();
  }

  // helper plugins should generally not complete commands, but a useful
  // utility function is to change directories if there isn't any other
  // possible course of action (and its highly unlikey for this to
  // confict with any other plugins)
  string cmd() {
    // string output = "";
    // FOR_l(i,tokens)
    //   output = output + tokens[i];
    // printf("\r\n%s is %d\n",output.c_str(),valid_dir(output));
    // return valid_dir(output) ? "cd " + output : "";

    //stupid me: cd-ing in child process buys us nothing, so don't bother
    return "";
  }

private: 
  list_t tokens;
  string PWD;
  string HOME;
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
    output.push_back("./");
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

  list_t ls_fe(string path) {
    list_t output;
    DIR* dirp = opendir(path.c_str());
    struct dirent *file;
    if(dirp==NULL)
      return list_t();
    while ((file = readdir(dirp)) != NULL) {
      string name(file->d_name);
      struct stat st;
      if(stat((path+"/"+name).c_str(),&st)) continue;
      if(S_ISREG(st.st_mode) && access((path+"/"+name).c_str(),X_OK) == 0)
	output.push_back(string(file->d_name));
    }
    closedir(dirp);
    return output;
  }

  list_t base_list() {
    list_t dir = ls_d(PWD);
    list_t files = ls_f(PWD);
    CONCAT2_l(dir,files,output);
    output.push_back("/");
    output.push_back("~/");
    return output;
  }
};

extern "C" Plugin* create_plugin() {return new Dir;}
extern "C" void destroy_plugin( Plugin* plugin ) {delete plugin;}
