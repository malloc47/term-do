#include "client.h"
#ifdef DAEMON

Client::Client() {
  try {
    server_send = new message_queue(open_only,"term_do_send");
    server_receive = new message_queue(open_only,"term_do_receive");
  }
  catch(interprocess_exception &ex){
    throw;
  }
  // string send = "%s:"+string(getenv("PWD"));
  // server_send->send(send.data(),send.size(),0);
  sendToServer("%s:"+string(getenv("PWD")));

  view = new View("/-/");
  // view->refreshLine(server->getQuery(),server->getMatches(),server->getTokens());
}

Client::~Client() {
  sendToServer("%r");
  sendToServer("%die");
  // string send = "%r";
  // server_send->send(send.data(),send.size(),0);
  view->clearLine(); 
  delete server_send;
  delete server_receive;
  delete view;
}

int Client::handleChar(char c) {
  bool done = false;
  string send = "";

  // standard character range
  if( (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c=='/' || ( c > ' ' && c <= '~') ) {
    // really need to figure out why send="%c:"+c fails
    send="%c:c";
    send[3]=c;
  }
  // C-s
  else if(c==19)
    send="%f";
  // C-r
  else if(c==18)
    send="%b";
  // backspace
  else if(c==127)
    // if query out of characters to backspace
    send="%bksp";
  // C-f
  else if(c==6)
    send="%full";
  // tab
  else if(c==9)
    send="%cvt";
  // space
  else if(c==' ') // (c==32)
    send="%ct";
  // enter
  else if(c==13) 
    send="%cft";

  // printf("\r\n%s\n",send.c_str());

  if(!send.empty())
    sendToServer(send);
    // server_send->send(send.data(),send.size(),0);

  if(c==13) {
    if(!getFromServer().compare("1")) done = true;
  }
  
  // C-c , C-d , C-g, or time to quit
  return (c==3 || c==4 || c==7 || done);
}

void Client::sendToServer(string to_send) {
  stringstream out;
  out << getpid(); 
  to_send = out.str()+":"+to_send;
  // server_send->send(pre_send.data(),pre_send.size(),0);
  server_send->send(to_send.data(),to_send.size(),0);
}

string Client::getFromServer(string to_send, string to_send_after) {
  if(to_send.empty()) return "";
  if(to_send_after.empty()) return getFromServer(to_send);
  size_t const max_size = 1024; // terminals longer than this are scary
  string toreceive;
  toreceive.resize(max_size);
  size_t msg_size;
  unsigned msg_priority;
  sendToServer(to_send);
  sendToServer(to_send_after);
  // server_send->send(to_send.data(),to_send.size(),0);
  // server_send->send(to_send_after.data(),to_send_after.size(),0);
  server_receive->receive(&toreceive[0], toreceive.size(), msg_size, msg_priority);
  toreceive.resize(msg_size);
  return toreceive;
}

string Client::getFromServer(string to_send) {
  if(to_send.empty()) return "";
  size_t const max_size = 1024; // terminals longer than this are scary
  string toreceive;
  toreceive.resize(max_size);
  size_t msg_size;
  unsigned msg_priority;
  sendToServer(to_send);
  // server_send->send(to_send.data(),to_send.size(),0);
  server_receive->receive(&toreceive[0], toreceive.size(), msg_size, msg_priority);
  toreceive.resize(msg_size);
  return toreceive;
}

string Client::getFromServer() {
  size_t const max_size = 1024; // terminals longer than this are scary
  string toreceive;
  toreceive.resize(max_size);
  size_t msg_size;
  unsigned msg_priority;
  server_receive->receive(&toreceive[0], toreceive.size(), msg_size, msg_priority);
  toreceive.resize(msg_size);
  return toreceive;
}

string Client::loopDo() {
  int done=0;
  do {
    stringstream out;
    out << view->getWidth(); 
    string query = "%p:" + out.str();
    string line1 = getFromServer(query);
    // split on newline
    int split = line1.find_first_of('\n');
    string line2 = line1.substr(split+1);
    line1 = line1.substr(0,split);
    view->clearLine();
    view->printLine(line1.c_str());
    view->pushCursor();
    view->printLine(line2.c_str());
    view->popCursor();
    done = handleChar(view->getChar());
  } while(!done);
  return getFromServer("%cmd");
}

void Client::run(string cmd) {
  view->clearLine();
  delete view;
  if(!cmd.empty()) system(cmd.c_str());
  view = new View("/-/");
}

void Client::reset() {
  sendToServer("%r");
  // string send = "%r";
  // server_send->send(send.data(),send.size(),0);
}
#endif
