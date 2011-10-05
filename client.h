#ifndef CLIENT_H_
#define CLIENT_H_

#include <boost/interprocess/ipc/message_queue.hpp>
#include "common.h"
#include "vt100.h"
#include "query.h"
#include "plugins.h"
#include "view.h"
#include "term-do.h"
#include "frontend.h"
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <sstream>
#include <string>
#include <vector>

using namespace std;
using namespace boost::interprocess;

class Client : public Frontend {
public:
  Client();
  ~Client();
  string loopDo();
  void run(string);
  void reset();

private:
  void init();
  void cleanup();
  int handleChar(char);
  void sendToServer(string);
  string getFromServer(string,string);
  string getFromServer(string);
  string getFromServer();
  class View *view;
  message_queue *server_send;
  message_queue *server_receive;
};

#endif
