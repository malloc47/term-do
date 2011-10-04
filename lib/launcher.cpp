#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <sstream>
#include <iostream>
#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <functional>
#include "plugin.h"
#include "../tst.h"
#include "../searcher.h"
#include "../completion.hpp"

using namespace std;

class Launcher : public Plugin {
public:
  Launcher() {
    PWD = string(getenv("PWD"))+"/";
    dictionary = new TST();
    getExec(dictionary);
    readConfig("launcher");
  }

  ~Launcher() {delete dictionary;}

  string name() { return "launcher";}

  void update(list_t new_tokens) {tokens = new_tokens; dir_complete = "";}
  void update(string new_wd) {PWD = new_wd + "/";}

  bool match() {
    FORB_l(i,tokens)
      if(completions.count(tokens[i]))
	return true;
    return false;
  }

  list_t list() {
    if(tokens.empty())
      return dictionary->sort();

    string command;
    int cmd_pos = 0;
    FORB_l(i,tokens) {
      if(completions.count(tokens[i])) {
	command = tokens[i];
	cmd_pos=i;
	break;
      }
    }

    if(command.empty() || !completions.count(command))
      return dictionary->sort();

    list_t input;
    input.push_back(tokens.back());
    // Pull top-level completion, if it works
    list_t output = completions[command]->search(input);

    // Walk backwards through the tokens, adding each and re-searching
    // TODO: make this non-greedy
    for(int pos=tokens.size()-2; pos>cmd_pos; pos--) {
      if(output.empty()) {
	input.insert(input.begin(),tokens[pos]);
	output = completions[command]->search(input);
      }
    }

    // If /still/ empty
    if(output.empty()) {
      output = completions[command]->all();
      // show files too, if there is more than one token (first token
      // is command, so only accept whatever that command takes, by
      // default)
      if(tokens.size() > 1) output.push_back("%dir");
    }

    bool restart;
    do {
      restart = false;
      FOR_l(i,output) {
	// Add in function completions
	if(functions.count(output[i])) {
	  string to_exec = replace(replace(functions[output[i]],"$prev",tokens.back()),"$cmd",command);
	  list_t new_output = execCmd(to_exec);
	  output.erase(output.begin()+i);
	  FOR_l(j,new_output) output.push_back(new_output[j]);
	  restart=true;
	  break;
	}
	// and alias completions
	else if(aliases.count(output[i])) {
	  list_t new_output = splitString(aliases[output[i]],' ');
	  output.erase(output.begin()+i);
	  FOR_l(j,new_output) output.push_back(new_output[j]);
	  restart = true;
	  break;
	}
	else if(!output[i].substr(0,4).compare("%dir")) {
	  dir_complete=output[i].substr(1);
	  output.erase(output.begin()+i);
	}
      }
    } while(restart);

    return output;
  }

  string complete() {return dir_complete.empty() ? "" : dir_complete;}

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
  string PWD;
  TST *dictionary;
  list_t tokens;
  map<string, Completion*> completions;
  map<string,string> functions;
  map<string,string> aliases;
  string dir_complete;

  void getExec(Searcher *dict) {
    list_t paths = splitString(string(getenv("PATH")),':');
    FOR_l(i,paths) {
      DIR* dirp = opendir(paths[i].c_str());
      struct dirent *entry;
      struct stat st;
      if(dirp==NULL) continue;
      while ((entry = readdir(dirp)) != NULL) {
	string name(entry->d_name);
	if(stat((paths[i]+"/"+name).c_str(),&st)) continue;
	if(S_ISREG(st.st_mode) && access((paths[i]+"/"+name).c_str(),X_OK) == 0) {
	  dict->insert(trim(name));
	}
      }
      closedir(dirp);
    }
  }
  list_t splitString(const string s, char delim) {
    list_t elems;
    stringstream ss(s);
    string item;
    while(getline(ss,item,delim))
      elems.push_back(item);
    return elems;
  }

  string trim(string s) {
    s.erase(s.begin(), find_if(s.begin(), s.end(), not1(ptr_fun<int, int>(isspace))));
    s.erase(find_if(s.rbegin(), s.rend(), not1(ptr_fun<int, int>(isspace))).base(), s.end());
    s.erase(remove(s.begin(), s.end(), '\n'), s.end());
    return s;
  }

  string replace(string s, string prev, string updated) {
    int pos = 0;
    while((pos = s.find(prev, pos)) != string::npos) {
      s.replace(pos, prev.length(), updated);
      pos += updated.length();
    }
    return s;
  }

  bool is_cmd(string input) {
    return dictionary->search(input);
  }

  void readConfig(string filename) {
    ifstream file(filename.c_str());
    if(!file.is_open()) return;
    string cmd;
    list_t history;
    int prev_size = 0;
    while(!file.eof()) {
      string head,tail;
      getline(file,head,':');
      getline(file,tail);
      if(head.find('\n')!=string::npos || tail.find('\n')!=string::npos) {
	printf("\r\nLauncher config parse error!\n");
	return;
      }

      // New command
      if(head.empty()) continue;
      if(head[0] == '_')
	functions.insert(pair<string,string>(trim(head),trim(tail)));
      else if(head[0] == '/')
	// comment
	continue;
      else if(head[0] == '%')
	aliases.insert(pair<string,string>(trim(head),trim(tail)));
      else if(head[0] != '|') {
	list_t heads = splitString(head,' ');
	cmd=trim(head);
	completions.insert(pair< string,Completion* >(cmd,new Completion(cmd)));
	list_t line = splitString(tail,' ');
	FOR_l(i,line) {
	  if(line.empty()) continue;
	  completions[cmd]->insert(trim(line[i]));
	}
	prev_size=0;	
	history.clear();
      }
      // Push command on stack
      else {
	int new_size = 0;
	string new_head;
	// Determine new stack level
	for(unsigned int i=0; i<head.size(); i++) {
	  if(head[i] == '|') new_size++;
	  else break;
	}
	// Remove stack chars from head
	new_head = trim(head.substr(new_size));
	// Stack should only grow by one
	if(new_size > prev_size && new_size-prev_size == 1) {
	  history.push_back(new_head);
	}
	// Can shrink arbitrarily
	else if(new_size < prev_size) {
	  history.pop_back();
	  for(int i=0; i<prev_size-new_size; i++)
	    history.pop_back();
	  history.push_back(new_head);
	}
	else if(new_size == prev_size) {
	  history.pop_back();
	  history.push_back(new_head);
	}
	else {
	  printf("\r\nLauncher config parse error!\n");
	  return;
	}
	// History now in order, so push command
	if(!new_head.empty()) {
	  completions[cmd]->insert(history);
	}
	// If there are any trailing commands, add them
	if(!tail.empty()) {
	  list_t line = splitString(tail,' ');
	  FOR_l(i,line) {
	    if(line.empty()) continue;
	    list_t new_entry = history;
	    new_entry.push_back(trim(line[i]));
	    completions[cmd]->insert(new_entry);
	  }
	}
	prev_size = new_size;
      }
    }
    file.close();
  }
  list_t execCmd(string cmd) {
    FILE *fpipe;
    char buffer[4096];
    list_t output;

    if ( !(fpipe = (FILE*)popen(string(cmd).c_str(),"r")) ) {  
      printf("\rProblems with pipe\n");
      return list_t();
    }
    while (fgets(buffer, sizeof buffer, fpipe)) {
      string line = string(buffer);
      list_t split_line = splitString(line,' ');
      FOR_l(i,split_line) output.push_back(trim(split_line[i]));
    }
    pclose(fpipe);
    return output;
  }
};

extern "C" Plugin* create_plugin() {return new Launcher;}
extern "C" void destroy_plugin( Plugin* plugin ) {delete plugin;}
