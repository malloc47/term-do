#include <vector>
#include <string>
#include <stack>
#include <map>
#include <algorithm>
#include <sstream>
#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include "plugin.h"
#include "plug.h"

using namespace std;

class BashComp : public Plugin {
public:
  BashComp() {
    FILE *fpipe;
    char buffer[256];

    if ( !(fpipe = (FILE*)popen(string("./comp-func.sh").c_str(),"r")) ) {  
      printf("\rProblems with pipe\n");
      return;
    }
    while (fgets(buffer, sizeof buffer, fpipe)) {
      string line = string(buffer);
      unsigned int split;
      line.erase(remove(line.begin(), line.end(), '\n'), line.end());
      split = line.find(",");
      lookup.insert( pair<string,string>(line.substr(0,split),line.substr(split+1)) );  
    }
    pclose(fpipe);
  }

  string name() {return "bash-comp";};
  void update(list_t new_tokens) {tokens = new_tokens;}
  bool match() {return false;}

  list_t list() {
    if(tokens.empty()) return list_t();
    if(!lookup.count(tokens[0])) return list_t();

    list_t output;
    string command = "./completion.sh \"";
    FOR_l(i,tokens)
      command = command + (i?(tokens[i-1][tokens[i-1].size()-1]=='/' ? "" : " "):"") + tokens[i];
    command = command + "\" \"" + lookup.find(tokens[0])->second + "\"" ;

    FILE *fpipe;
    char buffer[256];

    if ( !(fpipe = (FILE*)popen(command.c_str(),"r")) ) {  
      printf("\rProblems with pipe\n");
      return list_t();
    }
    while (fgets(buffer, sizeof buffer, fpipe)) {
      string line = string(buffer);
      line.erase(remove(line.begin(), line.end(), '\n'), line.end());
      // printf("\n\n\r%s\n\n",buffer);
      output.push_back(line);
    }
    pclose(fpipe);

    // if ( !(fpipe = (FILE*)popen((command + "-").c_str(),"r")) ) {  
    //     perror("Problems with pipe");
    //     return output;
    // }
    // while (fgets(buffer, sizeof buffer, fpipe))
    //   output.push_back(string(buffer));
    // pclose(fpipe);

    return output;
  }

  string cmd() {
    return "";
  }

private:
  list_t tokens;
  map<string,string> lookup;
};

extern "C" Plugin* create_plugin() {return new BashComp;}
extern "C" void destroy_plugin( Plugin* plugin ) {delete plugin;}
