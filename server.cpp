#include "server.h"
#ifdef DAEMON

Server::Server() {
  // the message queue names are defined in terms of the client, so
  // they must be swapped here
  message_queue::remove("term_do_receive");
  message_queue::remove("term_do_send");
  client_send = new message_queue(create_only,"term_do_receive",128,1024);
  client_receive = new message_queue(create_only,"term_do_send",128,128);
  // seed the pool
  for(unsigned int i=0;i<PRELOAD;i++)
    termdo_pool.push(new TermDo(load_plugins));
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
    query=getFromClient();

    // strip off PID and convert to int
    int head=query.find_first_of(':');
    stringstream(query.substr(0,head)) >> pid;
    // remainder is command
    query = query.substr(head+1);

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
	  termdo_pool.push(new TermDo(load_plugins));

      // grap the first available from the pool
      termdo=termdo_pool.top();
      termdo_map.insert(pair<int,TermDo*>(pid,termdo));
      termdo_pool.pop();
    }

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
    else if(!query.compare("%pwd"))
      sendToClient(termdo->getCWD());
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
      // erase client entry
      termdo_map.erase(pid);
      // add old allocated "slot" back into the pool
      termdo_pool.push(termdo);
      // garbage-collect if there are too many unused elements in the pool
      while(termdo_pool.size() > 3*termdo_map.size()+PRELOAD) {
	delete termdo_pool.top();
	termdo_pool.pop();
      }
    }
    // prompt
    else if(!query.substr(0,3).compare("%p:")) {
      int term_width;
      stringstream(query.substr(3)) >> term_width;
      string p = prompt(termdo,term_width);
      sendToClient(p);
    }
  }

  return "";
}

string Server::getCmd(TermDo* termdo) {
  string cmd = termdo->getCommand();
  if(cmd.empty()) {
    list_t list = termdo->getTokens();
    FOR_l(i,list)
      cmd = cmd + (i==0 || is_dir(list[i-1]) || is_opt(list[i-1]) ? "" : " ") + list.at(i);
  }
  // behavior here is slightly different from running standalone:
  // directory is changed in server, and no message is printed
  if(!cmd.empty() && !cmd.substr(0,3).compare("cd ")) {
    string new_path = cd(cmd.substr(3),termdo->getCWD());
    termdo->setCWD(new_path);
    termdo->resetHard();
  }
  return cmd;
}

// highly hackish
string Server::cd(string path,string old_path) {
  for(unsigned int i=0;i<path.length();i++)
    if(path[i]=='~') {
      string head = path.substr(0,i);
      string tail = path.substr(i+1);
      path=head+string(getenv("HOME"))+tail;
      break;
    }
  chdir(old_path.c_str());
  char temp_path [PATH_MAX];
  if(realpath(path.c_str(), temp_path)!=NULL)
    return string(temp_path);
  else
    return "";
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
  // client should split on newline
  output = "/-/" + output + query + "\n";
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
#endif
