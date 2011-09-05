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

#ifdef __cplusplus
extern "C" {
#endif

void init() {system("./completion.sh &");}

void update(list_t new_tokens) {tokens = new_tokens;}

list_t list() {

  if(tokens.empty())
    return list_t();

  list_t output;

  // string command = "./completion.sh \"";
  string command = "echo \"";

  FOR_l(i,tokens)
    command = command + (i?(tokens[i-1][tokens[i-1].size()-1]=='/' ? "" : " "):"") + tokens[i];
  command = command + "\\ \" > comp-in &";

  FILE *fpipe;
  char buffer[256];

  system(command.c_str());
  if ( !(fpipe = (FILE*)popen("./completion-reader.sh","r")) ) {  
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

#ifdef __cplusplus
}
#endif
