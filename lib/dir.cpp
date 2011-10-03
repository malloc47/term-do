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
  string name() {return "dir";}
  void update(list_t new_tokens) {tokens = new_tokens;}
  void update(string new_wd) {PWD = new_wd + "/";}
  bool match() {
    if(tokens.empty()) return false;
    return is_dir(tokens.back());
  }
  list_t list() {
    if(tokens.empty()) {
      return base_list();
    }
    else { // this case may not be obvious, so here goes...
      // look at the last token to see if we're completing a directory
      // (if not, just show pwd)
      if(!is_dir(tokens[tokens.size()-1])) return base_list();

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
	if(!is_dir(tokens[start-1]))
	  break;

      if(!tokens[start].compare("~/"))
	listing = HOME;
      else if(tokens[start][0] == '/')
	listing = "";
      else
	listing = PWD;

      // now walk from the starting directory token (e.g. second/) to
      // the latest token, concatenating the directories
      for(unsigned int i=start;i<tokens.size();i++)
	listing = listing + (tokens[i].compare("~/") ? tokens[i] : "");

      // get both a directory and file listing
      list_t dir = ls_d(listing);
      // it's not remotely legitimate to blanketly assume that ./ is
      // going to be used solely for exec-able scripts...
      list_t files = (!tokens[start].compare("./")) && start == tokens.size()-1 ? ls_fe(listing) : ls_f(listing);
      // concatenate the two listings
      CONCAT2_l(dir,files,output);
      return output;
    }
    return list_t();
  }

  string cmd() {return "";}

  list_t list(string in) {
    if(in.empty()) filters.clear();
    else filters = splitString(in,',');
    return list();
  }

private: 
  list_t tokens;
  string PWD;
  string HOME;
  list_t filters;
  bool valid_dir(string dir){
    struct stat entry;
    return !(stat(dir.c_str(), &entry) < 0) && S_ISDIR(entry.st_mode);
  }

  bool valid_file(string file){
    struct stat entry;
    return !(stat(file.c_str(), &entry) < 0) && S_ISREG(entry.st_mode);
  }

  // bool isdir(string token) {return token[token.size()-1]=='/';}

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
      if(S_ISDIR(st.st_mode)) {
	output.push_back(string(file->d_name)+"/");
      }
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
      if(S_ISREG(st.st_mode)) {
	if(filters.empty())
	  output.push_back(string(file->d_name));
	else
	  FOR_l(i,filters)
	    if(string(file->d_name).find(filters[i]) !=string::npos) {
	      output.push_back(string(file->d_name));
	      break;
	    }
      }
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
      if(S_ISREG(st.st_mode) && access((path+"/"+name).c_str(),X_OK) == 0) {
	if(filters.empty())
	  output.push_back(string(file->d_name));
	else
	  FOR_l(i,filters)
	    if(string(file->d_name).find(filters[i]) !=string::npos) {
	      output.push_back(string(file->d_name));
	      break;
	    }
	output.push_back(string(file->d_name));
      }
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

  list_t splitString(const string s, char delim) {
    list_t elems;
    stringstream ss(s);
    string item;
    while(getline(ss,item,delim))
      elems.push_back(item);
    return elems;
  }
};

extern "C" Plugin* create_plugin() {return new Dir();}
extern "C" void destroy_plugin( Plugin* plugin ) {delete plugin;}
