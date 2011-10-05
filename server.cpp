#include "server.h"

Server::Server() {
  // the message queue names are defined in terms of the client, so
  // they must be swapped here
  message_queue::remove("term_do_receive");
  message_queue::remove("term_do_send");
  client_send = new message_queue(create_only,"term_do_receive",128,1024);
  client_receive = new message_queue(create_only,"term_do_send",128,128);
  TermDo* termdo = new TermDo();
  termdo_l.push_back(termdo);
}

Server::~Server() {
  delete client_send;
  delete client_receive;
  message_queue::remove("term_do_receive");
  message_queue::remove("term_do_send");
  // delete termdo;
  FOR_l(i,termdo_l) {
    delete termdo_l[i];
  }
}

string Server::loopDo() {
  TermDo *termdo;
  string pid_s, query, response;
  int pid;
  while(true) {
    pid_s=getFromClient();
    stringstream(pid_s.substr(5)) >> pid;

    // if the pid has already been assigned to a "slot"
    if(pid_lookup.count(pid)) {
      termdo = termdo_l[pid_lookup[pid]];
    }
    else {
      int new_slot=-1;
      // check for the first slot available
      for(unsigned int i=0;i<termdo_l.size();i++)
	if(!slot_lookup.count(i)) {
	  cout << "New slot found" << endl;
	  new_slot = i;
	  break;
	}
      // if all slots are used
      if(new_slot < 0) {
	cout << "No slot found--new slot made" << endl;
	termdo_l.push_back(new TermDo());
	new_slot = termdo_l.size()-1;
      }
      // assign a slot
      pid_lookup.insert(pair<int,int>(pid,new_slot));
      slot_lookup.insert(pair<int,int>(new_slot,pid));
      termdo=termdo_l[new_slot];
    }

    cout << pid_lookup.size() << endl;

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
      int empty_slot = pid_lookup[pid];
      pid_lookup.erase(pid);
      slot_lookup.erase(empty_slot);
      // TODO: garbage collect empty slots
      // need better system than just a mapped index into a vector
      // if we want to safely remove items from the vector without reindexing it
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
