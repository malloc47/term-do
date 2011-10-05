#include "server.h"

Server::Server() {
  // the message queue names are defined in terms of the client, so
  // they must be swapped here
  message_queue::remove("term_do_receive");
  message_queue::remove("term_do_send");
  client_send = new message_queue(create_only,"term_do_receive",128,1024);
  client_receive = new message_queue(create_only,"term_do_send",128,128);
  // seed the pool
  for(unsigned int i=0;i<2;i++)
    termdo_pool.push(new TermDo());
}

Server::~Server() {
  delete client_send;
  delete client_receive;
  message_queue::remove("term_do_receive");
  message_queue::remove("term_do_send");
  // delete termdo;
  while(!termdo_pool.empty()) {
    delete termdo_pool.top();
    termdo_pool.pop();
  }

  for (map<int,TermDo*>::const_iterator it = termdo_map.begin(); 
       it != termdo_map.end(); 
       it++) 
    delete it->second;
}

string Server::loopDo() {
  TermDo *termdo;
  string pid_s, query, response;
  int pid;
  while(true) {
    pid_s=getFromClient();
    stringstream(pid_s.substr(5)) >> pid;

    // if the pid has already been assigned to a "slot"
    if(termdo_map.count(pid)) {
      termdo = termdo_map[pid];
    }
    else {
      // if no more available in the pool
      if(termdo_pool.empty())
	// double total number of available
	// use size()+2 because we know we're going to be adding one below
	for(unsigned int i=0;i<termdo_map.size()+2;i++)
	  termdo_pool.push(new TermDo());

      // grap the first available form the pool
      termdo=termdo_pool.top();
      termdo_map.insert(pair<int,TermDo*>(pid,termdo));
      termdo_pool.pop();
    }

    query=getFromClient();
    if(!query.substr(0,3).compare("%c:"))
      termdo->addChar(query[3]);
    else if(!query.compare("%f"))
      termdo->rotateForward();
    else if(!query.compare("%b"))
      termdo->rotateBackward();
    else if(!query.compare("%r"))
      termdo->reset();
    else if(!query.compare("%bksp"))
      termdo->removeChar();
    else if(!query.compare("%full"))
      termdo->fullList();
    else if(!query.compare("%cmd"))
      sendToClient(getCmd(termdo));
    else if(!query.compare("%cvt")) {
      // sendToClient(termdo->commitValidToken() ? "1" : "0");
      termdo->commitValidToken();
    }
    else if(!query.compare("%ct")) {
      // sendToClient(termdo->commitToken() ? "1" : "0");
      termdo->commitToken();
    }
    else if(!query.compare("%cft")) {
      sendToClient(termdo->commitFinalToken() ? "1" : "0");
      // termdo->commitFinalToken();
    }
    // start
    else if(!query.substr(0,3).compare("%s:")) {
      termdo->reset();
      string tail = query.substr(3);
      termdo->setCWD(tail);
    }
    // terminate
    else if(!query.compare("%die")) {
      cout << "Removing slotted client" << endl;
      // erase client entry
      termdo_map.erase(pid);
      // add old allocated "slot" back into the pool
      termdo_pool.push(termdo);
      // garbage-collect if there are too many unused elements in the pool
      // (always keep 2)
      while(termdo_pool.size() > 3*termdo_map.size()+2) {
	delete termdo_pool.top();
	termdo_pool.pop();
      }
    }
    // prompt
    else if(!query.substr(0,3).compare("%p:")) {
      int term_width;
      stringstream(query.substr(3)) >> term_width;
      string p1 = prompt1(termdo,term_width);
      string p2 = prompt2(termdo,term_width,p1.length());
      // Send in 2 parts so the client can place the cursor properly
      sendToClient(p1);
      sendToClient(p2);
    }
  cout << termdo_map.size() << ":" << termdo_pool.size() << endl;
  }

  return "";
}

string Server::getCmd(TermDo* termdo) {
  string cmd = termdo->getCommand();
  if(cmd.empty()) {
    list_t list = termdo->getTokens();
    string output ="";
    FOR_l(i,list)
      output = output + (i==0 || is_dir(list[i-1]) || is_opt(list[i-1]) ? "" : " ") + list.at(i);
    return output;
  }
  else
    return cmd;
}

string Server::prompt1(TermDo* termdo,unsigned int width) {
  string query = termdo->getQuery();
  string output = View::formatList(termdo->getTokens(),"["," ","]") + " ";
  unsigned int len = width*2/3;
  if(output.length() > len)
    output = "[ ... " + output.substr(output.length()-len,len-1);
  output = "/-/" + output + query;
  return output;
}

string Server::prompt2(TermDo* termdo,unsigned int width, unsigned int width2) {
  list_t chopped = View::chopList(termdo->getMatches(), termdo->getQuery());
  return View::formatList(chopped, "{"," | ","}", width - width2);
}

string Server::prompt(TermDo* termdo,unsigned int width) {
  string query = termdo->getQuery();
  string output = View::formatList(termdo->getTokens(),"["," ","]") + " ";
  unsigned int len = width*2/3;
  if(output.length() > len)
    output = "[ ... " + output.substr(output.length()-len,len-1);
  output = "/-/" + output + query;
  list_t chopped = View::chopList(termdo->getMatches(), query);
  string match_str = View::formatList(chopped,
				"{"," | ","}",
				width - output.length());
  // output = output + match_str.substr(0,width-output.length());
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
  to_receive.resize(msg_size);
  // return string(buffer);
  return to_receive;
}

void Server::reset() {}
void Server::run(string cmd) {}

// int main(int argc, char *argv[]) {
//   library_path = "~/src/projects/term-do/lib";
//   Server logic;
//   logic.pollClients();
// }
