#include "client.h"

Client::Client() {
  try {
    server_send = new message_queue(open_only,"term_do_send");
    server_receive = new message_queue(open_only,"term_do_receive");
  }
  catch(interprocess_exception &ex){
    throw;
  }
  string send = "%s:"+string(getenv("PWD"));
  server_send->send(send.data(),send.size(),0);

  view = new View("/-/");
  // view->refreshLine(server->getQuery(),server->getMatches(),server->getTokens());
}

Client::~Client() {
  string send = "%r";
  server_send->send(send.data(),send.size(),0);
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
    server_send->send(send.data(),send.size(),0);

  if(c==13) {
    if(!getFromServer().compare("1")) done = true;
  }
  
  // C-c , C-d , C-g, or time to quit
  return (c==3 || c==4 || c==7 || done);
}

string Client::getFromServer(string to_send, string to_send_after) {
  if(to_send.empty()) return "";
  if(to_send_after.empty()) return getFromServer(to_send);
  size_t const max_size = 1024; // terminals longer than this are scary
  string toreceive;
  toreceive.resize(max_size);
  size_t msg_size;
  unsigned msg_priority;
  server_send->send(to_send.data(),to_send.size(),0);
  server_send->send(to_send_after.data(),to_send_after.size(),0);
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
  server_send->send(to_send.data(),to_send.size(),0);
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
    string line2 = getFromServer();
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
  string send = "%r";
  server_send->send(send.data(),send.size(),0);
}

// int main(int argc, char *argv[]) {

//   int cmdargs;
//   bool console=false;
//   static struct option long_options[] = {
//     {"console", 1, 0, 'c'},
//     {"lib", 1, 0, 'l'},
//     {"help", 0, 0, 'h'},
//     {"version", 0, 0, 'v'},
//     {NULL, 0, NULL, 0}
//   };
//   int option_index = 0;

//   library_path = "~/src/projects/term-do/lib";

//   while ((cmdargs = getopt_long(argc, argv, "cl:hv",
// 				long_options, &option_index)) != -1) {
//     switch (cmdargs) {
//     case 'c':
//       console=true;
//       break;
//     case 'l':
//       {
// 	stringstream ss(optarg);
// 	string item;
// 	while(getline(ss,item,','))
// 	  load_plugins.push_back(item);
//       }
//       break;
//     case 'h':
//       cout << "Usage: " << argv[0] <<  " [options] \n\
// Options: \n\
//   -c,--console                Use as console (don't exit after <Enter>) \n\
//   -l,--lib                    Specify the plugins to load (comma-delimited) \n\
//   -h,--help                   Display this information \n\
//   -v,--version                Display version information\n";
//       exit(0);
//       break;
//     case '?':
//       exit(0);
//       break;
//     }
//   }

//   // if (optind < argc) {
//   //   while (optind < argc)
//   //     framenum = atoi(argv[optind++]);
//   // }

//   string command;

//   if(console) {
//     try {
//       Client term_logic;
//       do {
// 	term_logic.reset();
// 	command = term_logic.loopDo();
// 	term_logic.run(command);
//       } while(!command.empty());
//     }
//     catch(exception& e) {
//       cout << "could not connect to daemon" << endl;
//     }
//   }
//   else {
//     try {
//       Client term_logic;
//       command = term_logic.loopDo();
//       term_logic.run(command);
//     }
//     catch(exception& e) {
//       cout << "could not connect to daemon" << endl;
//     }
//   }
//     // add command to bash history
//     // system(("bash -c \"history -s " + command + "\"").c_str());
// }
