#ifndef SERVER_H_
#define SERVER_H_

#include <boost/interprocess/ipc/message_queue.hpp>
#include "common.h"
#include "term-do.h"
#include "frontend.h"
#include "view.h"

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
  string prompt(TermDo*,unsigned int);
  string prompt1(TermDo*,unsigned int);
  string prompt2(TermDo*,unsigned int,unsigned int);
  string getCmd(TermDo*);
  message_queue *client_receive;
  message_queue *client_send;
  map<int,int> pid_lookup; // should combine into bidirectional map
  map<int,int> slot_lookup;
  vector<TermDo*> termdo_l;
};

#endif
