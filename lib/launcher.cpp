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
#include "plugin.h"
#include "../tst.h"
#include "../searcher.h"
// #include <boost/algorithm/string.hpp>
#include <functional>
#include "../completion.hpp"

using namespace std;
// using namespace boost;

typedef struct {
  string completion;
  map<string,string> subcompletions;
} completion_t;

class Launcher : public Plugin {
public:
  Launcher() {
    dictionary = new TST();
    getExec(dictionary);
    readConfig("launcher");
    readConfig2("launcher2");
    // Completion test("git");
    // list_t test_list,test_list2,test_list3,test_list4,test_search;
    // test_list4.push_back("commit");
    // test_list.push_back("add");
    // test_search=test_list;
    // test_list2=test_list;
    // test_list.push_back("-f");
    // test_list2.push_back("-a");
    // test_list3=test_list2;
    // test_list3.push_back("hi");
    // printf("\r\nInserting 1\n");
    // test.insert(test_list);
    // printf("\r\nInserting 2\n");
    // test.insert(test_list2);
    // test.insert(test_list3);
    // test.insert(test_list4);
    // printf("\r\nSearching\n");
    // list_t output = test.search(test_search);
    // printf("\r\n-------------------------\n");
    // FOR_l(i,output) {
    //   printf("\r\n%s\n",output[i].c_str());
    // }

    // list_t test_search2;
    // list_t output2 = test.search(test_list2);
    // printf("\r\n-------------------------\n");
    // FOR_l(i,output2) {
    //   printf("\r\n%s\n",output2[i].c_str());
    // }

    // list_t test_search3;
    // test_search3.push_back("none");
    // test_search3.push_back("-f");
    // list_t output3 = test.search(test_search3);
    // printf("\r\n-------------------------\n");
    // FOR_l(i,output3) {
    //   printf("\r\n%s\n",output3[i].c_str());
    // }
  }

  ~Launcher() {delete dictionary;}

  string name() { return "launcher";}

  void update(list_t new_tokens) {tokens = new_tokens;}

  bool match() {return false;}

  list_t list() {
    // return +x files on $PATH if no command has been typed yet, or if
    // its a command that takes other commands (e.g. watch)

    // if(tokens.empty() || (!tokens[0].compare("time") || 
    // 			  !tokens[0].compare("watch")))

    if(tokens.empty())
      return dictionary->sort();

    string command;
    int cmd_pos = 0;
    FORB_l(i,tokens) {
      if(completions_new.count(tokens[i])) {
	command = tokens[i];
	cmd_pos=i;
	break;
      }
    }

    if(command.empty() || !completions_new.count(command))
      return dictionary->sort();

    list_t input;
    input.push_back(tokens.back());
    // Pull top-level completion, if it works
    list_t output = completions_new[command]->search(input);
    // printf("\r\nNEW--------------------\n");
    // FOR_l(i,input) printf("\r%s\n",input[i].c_str());
    // printf("\rNEW--------------------\n");
    // FOR_l(i,output) printf("\r%s\n",output[i].c_str());
    // printf("\r\nNEW--------------------\n");
    // Walk backwards through the tokens, adding each and re-searching
    for(int pos=tokens.size()-2; pos>cmd_pos; pos--) {
      if(output.empty()) {
	input.insert(input.begin(),tokens[pos]);
	output = completions_new[command]->search(input);
	// printf("\r\nNEW--------------------\n");
	// FOR_l(i,input) printf("\r%s\n",input[i].c_str());
	// printf("\rNEW--------------------\n");
	// FOR_l(i,output) printf("\r%s\n",output[i].c_str());
	// printf("\r\nNEW--------------------\n");
      }
    }

    // If /still/ empty
    if(output.empty())
      output = completions_new[command]->all();

    FOR_l(i,output)
      if(functions.count(output[i])) {
	  output = execCmd(functions[output[i]]);
	  break;
	}

    return output;

    // completion_t comp = completions[command];

    // list_t output, comp_l = splitString(comp.completion,' ');
    
    // FOR_l(i,comp_l) {
    //   if(!comp_l[i].empty() && comp_l[i][0] == '_') {
    // 	list_t new_output = execCmd(functions[comp_l[i]]);
    //     FOR_l(j,new_output) output.push_back(new_output[j]);
    //   }
    //   else
    // 	output.push_back(comp_l[i]);
    // }

    // if(!tokens.back().compare(command) || !comp.subcompletions.count(tokens.back()))
    //   return output;

    // string subcomp = comp.subcompletions[tokens.back()];
    // list_t new_output = splitString(subcomp,' ');
    // // TODO: call functions here too
    // FOR_l(i,new_output) output.push_back(new_output[i]);

    // return output;

    // else
    //   return list_t();
  }

  string complete() {return tokens.empty() ? "" : "dir";}

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
  TST *dictionary;
  list_t tokens;
  map<string, Completion*> completions_new;
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
	  dict->insert(name);
	}
      }
      closedir(dirp);
    }
  }
  list_t splitString(const string s, char delim) {
    list_t elems;
    stringstream ss(s);
    string item;
    while(getline(ss,item,delim)) {
      elems.push_back(item);
    }
    return elems;
  }

  string trim(string s) {
    s.erase(s.begin(), find_if(s.begin(), s.end(), not1(ptr_fun<int, int>(isspace))));
    s.erase(find_if(s.rbegin(), s.rend(), not1(ptr_fun<int, int>(isspace))).base(), s.end());
    return s;
  }

  bool is_cmd(string input) {
    return dictionary->search(input);
    // return find(dictionary.begin(), dictionary.end(), input) != dictionary.end();
  }
  // string trim(string s) {
  //   return s.erase(remove_if(s.begin(), s.end(), isspace), s.end());
  // }

  map<string,completion_t> completions;
  map<string,string> functions;

  void readConfig(string filename) {
    ifstream file(filename.c_str());
    if(!file.is_open()) return;
    while(!file.eof()) {
      string head, tail;
      getline(file,head,':');
      // http://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
      head=trim(head);
      getline(file,tail);
      tail=trim(tail);
      
      // function entry
      if(!head.empty() && head[0] == '_') {
	// cout << "\r\n" << "function -> " << head << " : " << tail << "\n";
	functions.insert(pair<string,string>(head,tail));
      }
      // first completion entry
      else if(!completions.count(head)) {
	// cout << "\r\n" << "completions -> " << head << " : " << tail << "\n";
	completion_t completion;
	completion.completion=tail;
	completions.insert(pair<string,completion_t>(head,completion));
      }
      // "switch" completion entry specifying the parameters of the switch
      else {
	list_t line = splitString(tail,' ');
	string first = line.front();
	string rest = assembleLine(line,1);
	first=trim(first);
	rest=trim(rest);
	// cout << "\r\n" << "switches -> " << first << " : " << rest << "\n";
	completions[head].subcompletions.insert(pair<string,string>(first,rest));
      }
    }
    file.close();
  }
  void readConfig2(string filename) {
    ifstream file(filename.c_str());
    if(!file.is_open()) return;
    string cmd;
    list_t history;
    int prev_size = 0;
    // printf("\r\nStart\n");
    while(!file.eof()) {
      string head,tail;
      getline(file,head,':');
      getline(file,tail);
      if(head.find('\n')!=string::npos || tail.find('\n')!=string::npos) {
	printf("\r\nLauncher config parse error!\n");
	return;
      }

      // New command
      // printf("\r\nLine\n");
      if(head.empty()) continue;
      if(head[0] == '_')
	functions.insert(pair<string,string>(head,tail));
      else if(head[0] != '|') {
	cmd=trim(head);
	printf("\r\nSetting cmd to %s\n",cmd.c_str());
	completions_new.insert(pair< string,Completion* >(cmd,new Completion(cmd)));
	list_t line = splitString(tail,' ');
	FOR_l(i,line) {
	  if(line.empty()) continue;
	  printf("\r\n%s\n",line[i].c_str());
	  completions_new[cmd]->insert(trim(line[i]));
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
	  // printf("i==%d c==%c\n",i,head[i]);
	  if(head[i] == '|') new_size++;
	  else break;
	}
	// Remove stack chars from head
	new_head = trim(head.substr(new_size));
	printf("\r\nInserting %s\n",new_head.c_str());
	printf("\r\nPrev:%d New: %d\n",prev_size,new_size);
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
	  printf("\r\nOLD--------------------\n");
	  FOR_l(i,history) printf("\r\n%s\n",history[i].c_str());
	  printf("\r\nOLD--------------------\n");
	  completions_new[cmd]->insert(history);
	}
	// If there are any trailing commands, add them
	if(!tail.empty()) {
	  list_t line = splitString(tail,' ');
	  FOR_l(i,line) {
	    if(line.empty()) continue;
	    list_t new_entry = history;
	    new_entry.push_back(trim(line[i]));
	    printf("\r\nOLD--------------------\n");
	    FOR_l(i,new_entry) printf("\r\n%s\n",new_entry[i].c_str());
	    printf("\r\nOLD--------------------\n");
	    completions_new[cmd]->insert(new_entry);
	  }
	}
	prev_size = new_size;
      }
    }
    file.close();
    // list_t search;
    // search.push_back("--help");
    // // search.push_back("-f");
    // printf("\r\nB--------------------\n");
    // list_t output = completions_new["mplayer"]->search(search);
    // FOR_l(i,output) printf("\r\n%s\n",output[i].c_str());
    // printf("\r\nA--------------------\n");
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
      FOR_l(i,split_line) output.push_back(split_line[i]);
    }
    pclose(fpipe);
    return output;
  }
};

extern "C" Plugin* create_plugin() {return new Launcher;}
extern "C" void destroy_plugin( Plugin* plugin ) {delete plugin;}
