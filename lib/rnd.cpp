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

// http://stackoverflow.com/questions/2146792/how-do-you-generate-random-strings-in-
// only using a fixed version, obviously...
string randomStrGen(int length) {
    static string charset = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";
    string result;
    result.resize(length);
    for (int i = 0; i < length; i++)
        result[i] = charset[rand() % charset.length()];
    return result;
}


#ifdef __cplusplus
extern "C" {
#endif

void init() {}

void update(list_t new_tokens) {tokens = new_tokens; srand(time(NULL));}

list_t list() {
  list_t output;
  for(int i=0;i<rand()%100;i++)
    output.push_back(randomStrGen(rand()%20));
  return output;
}

string cmd() {
  return "";
}

#ifdef __cplusplus
}
#endif
