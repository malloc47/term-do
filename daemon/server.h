#ifndef SERVER_H_
#define SERVER_H_

#include <boost/interprocess/ipc/message_queue.hpp>
#include "common.h"
#include "term-do.h"
#include "view.h"

using namespace std;
using namespace boost::interprocess;

class Server {
public:
  Server();
  ~Server();
  void pollClients();
private:
  string getFromClient();
  void sendToClient(string);
  string prompt(unsigned int);
  string prompt1(unsigned int);
  string prompt2(unsigned int,unsigned int);
  string getCmd();
  message_queue *client_receive;
  message_queue *client_send;
  TermDo *termdo;
};

#endif
