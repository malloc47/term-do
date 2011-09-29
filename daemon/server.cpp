#include "server.h"

string library_path;

Server::Server() {
  // the message queue names are defined in terms of the client, so
  // they must be swapped here
  cout << "Removing old message queues" << endl;
  message_queue::remove("term_do_receive");
  message_queue::remove("term_do_send");
  cout << "Adding new message queues" << endl;
  client_send = new message_queue(create_only,"term_do_receive",128,1024);
  client_receive = new message_queue(create_only,"term_do_send",128,128);
  cout << "Creating new logic instance" << endl;
  termdo = new TermDo();
  cout << "Done with constructor" << endl;
}

Server::~Server() {
  delete client_send;
  delete client_receive;
  message_queue::remove("term_do_receive");
  message_queue::remove("term_do_send");
  delete termdo;
}

void Server::pollClients() {
  string query, response;
  while(true) {
    query=getFromClient();
    if(!query.substr(0,3).compare("%c:"))
      termdo->addChar(query[3]);
    else if(!query.compare("%f"))
      termdo->rotateForward();
    else if(!query.compare("%b"))
      termdo->rotateBackward();
    else if(!query.compare("%cvt"))
      termdo->commitValidToken();
    else if(!query.compare("%ct"))
      termdo->commitToken();
    else if(!query.compare("%cft"))
      termdo->commitFinalToken();
    else if(!query.compare("%bksp"))
      termdo->removeChar();
    else if(!query.compare("%full"))
      termdo->fullList();
    else if(!query.compare("%p"))
      sendToClient(prompt());
    else if(!query.compare("%cmd"))
      sendToClient("ls");

    cout << "Received: " << query << endl;
  }
}

string Server::prompt() {
  string query = termdo->getQuery();
  string output = View::formatList(termdo->getTokens(),"["," ","]") + " ";
  unsigned int len = 100;
  if(output.length() > len)
    output = "[ ... " + output.substr(output.length()-len,len-1);
  output = "/-/" + output + query;
  list_t chopped = View::chopList(termdo->getMatches(), query);
  string match_str = View::formatList(chopped,
				"{"," | ","}",
				100 - output.length());
  output = output + match_str;
  return output;
}

void Server::sendToClient(string to_send) {
  client_send->send(to_send.data(),
		    to_send.size() > 1024 ? 1024 : to_send.size() ,
		    0);
}

string Server::getFromClient() {
  size_t const max_size = 128;
  string to_receive;
  // char buffer[max_size];
  to_receive.resize(max_size);
  size_t msg_size;
  unsigned int msg_priority;
  client_receive->receive(&to_receive[0], max_size, msg_size, msg_priority);
  cout << "Done with query" << endl;
  to_receive.resize(msg_size);
  // return string(buffer);
  return to_receive;
}

int main(int argc, char *argv[]) {
  library_path = "~/src/projects/term-do/lib";
  Server logic;
  logic.pollClients();
}

