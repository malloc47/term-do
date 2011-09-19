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
#include <boost/algorithm/string.hpp>
#include "../completion.hpp"

using namespace std;
using namespace boost;

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
    Completion test("git");
    list_t test_list,test_list2,test_list3,test_list4,test_search;
    test_list4.push_back("commit");
    test_list.push_back("add");
    test_search=test_list;
    test_list2=test_list;
    test_list.push_back("-f");
    test_list2.push_back("-a");
    test_list3=test_list2;
    test_list3.push_back("hi");
    printf("\r\nInserting 1\n");
    test.insert(test_list);
    printf("\r\nInserting 2\n");
    test.insert(test_list2);
    test.insert(test_list3);
    test.insert(test_list4);
    printf("\r\nSearching\n");
    list_t output = test.search(test_search);
    printf("\r\n-------------------------\n");
    FOR_l(i,output) {
      printf("\r\n%s\n",output[i].c_str());
    }

    list_t test_search2;
    list_t output2 = test.search(test_list2);
    printf("\r\n-------------------------\n");
    FOR_l(i,output2) {
      printf("\r\n%s\n",output2[i].c_str());
    }

    list_t test_search3;
    test_search3.push_back("none");
    test_search3.push_back("-f");
    list_t output3 = test.search(test_search3);
    printf("\r\n-------------------------\n");
    FOR_l(i,output3) {
      printf("\r\n%s\n",output3[i].c_str());
    }
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
    FORB_l(i,tokens) {
      if(completions.count(tokens[i])) {
	command = tokens[i];
	break;
      }
    }

    if(command.empty())
      return dictionary->sort();

    completion_t comp = completions[command];

    list_t output, comp_l = splitString(comp.completion,' ');
    
    FOR_l(i,comp_l) {
      if(!comp_l[i].empty() && comp_l[i][0] == '_') {
	list_t new_output = execCmd(functions[comp_l[i]]);
        FOR_l(j,new_output) output.push_back(new_output[j]);
      }
      else
	output.push_back(comp_l[i]);
    }

    if(!tokens.back().compare(command) || !comp.subcompletions.count(tokens.back()))
      return output;

    string subcomp = comp.subcompletions[tokens.back()];
    list_t new_output = splitString(subcomp,' ');
    // TODO: call functions here too
    FOR_l(i,new_output) output.push_back(new_output[i]);

    return output;
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
	if(S_ISREG(st.st_mode) && access((paths[i]+"/"+name).c_str(),X_OK) == 0)
	  dict->insert(name);
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
      trim(head);
      getline(file,tail);
      trim(tail);
      
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
	trim(first);
	trim(rest);
	// cout << "\r\n" << "switches -> " << first << " : " << rest << "\n";
	completions[head].subcompletions.insert(pair<string,string>(first,rest));
      }
    }
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
