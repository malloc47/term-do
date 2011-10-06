#include "config.h"
#ifdef DAEMON
#ifndef SERVER_H_
#define SERVER_H_

#include <boost/interprocess/ipc/message_queue.hpp>
#include "common.h"
#include "term-do.h"
#include "frontend.h"
#include "view.h"
#include <stack>

#define PRELOAD 3

using namespace std;
using namespace boost::interprocess;

class Server : public Frontend {
public:
  Server();
  ~Server();
  string loopDo();
  void reset();
  void run(string);
private:
  string getFromClient();
  void sendToClient(string);
  string cd(string,string);
  string prompt(TermDo*,unsigned int);
  string prompt1(TermDo*,unsigned int);
  string prompt2(TermDo*,unsigned int,unsigned int);
  string getCmd(TermDo*);
  message_queue *client_receive;
  message_queue *client_send;
  map<int,TermDo*> termdo_map;
  stack<TermDo*> termdo_pool; // should float between 2X and 3X size of termdo_map
};

#endif
#endif
