#include "client.h"

list_t load_plugins;
string library_path;

Client::Client() {
  try {
    server_send = new message_queue(open_only,"term_do_send");
    server_get = new message_queue(open_only,"term_do_get");
  }
  catch(interprocess_exception &ex){
    cout << ex.what() << endl;
    throw;
  }
  view = new View("/-/");
  // view->refreshLine(server->getQuery(),server->getMatches(),server->getTokens());
}

Client::~Client() {
  view->clearLine(); 
  delete server_send;
  delete server_get;
  delete view;
}

int Client::handleChar(char c) {
  bool done = false;
  string to_send;

  // standard character range
  if( (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c=='/' || ( c > ' ' && c <= '~') )
    to_send="%c:"+c;
  // C-s
  else if(c==19)
    to_send="%f";
  // C-r
  else if(c==18)
    to_send="%b";
  // tab
  else if(c==9)
    to_send="%cvt";
  // space
  else if(c==' ') // (c==32)
    to_send="%ct";
  // enter
  else if(c==13) 
    to_send="%cft";
  // backspace
  else if(c==127)
    // if query out of characters to backspace
    to_send="%bksp";
  // C-f
  else if(c==6)
    to_send="%full";

  if(!to_send.empty())
    server_send->send(to_send.data(),to_send.size(),0);
  
  // C-c , C-d , C-g, or time to quit
  return (c==3 || c==4 || c==7 || done);
}

string Client::getFromServer(string to_send) {
  size_t const max_size = 4096; // terminals longer than this are scary
  string toreceive;
  toreceive.resize(max_size);
  size_t msg_size;
  unsigned msg_priority;
  server_send->send(to_send.data(),to_send.size(),0);
  server_get->receive(&toreceive[0], toreceive.size(), msg_size, msg_priority);
  toreceive.resize(msg_size);
  return toreceive;
}

string Client::loopDo() {
  int done=0;
  while(!done) {
    done = handleChar(view->getChar());
    view->printLine(getFromServer("%p").c_str());
  }

  return getFromServer("%cmd");
}

void Client::run(string cmd) {
  view->clearLine();
  delete view;
  if(!cmd.empty()) system(cmd.c_str());
  view = new View("/-/");
}

void Client::reset() {
  view->clearLine();
  view->printLine(getFromServer("%r").c_str());
}

#include "history.h"

int main(int argc, char *argv[]) {

  int cmdargs;
  bool console=false;
  static struct option long_options[] = {
    {"console", 1, 0, 'c'},
    {"lib", 1, 0, 'l'},
    {"help", 0, 0, 'h'},
    {"version", 0, 0, 'v'},
    {NULL, 0, NULL, 0}
  };
  int option_index = 0;

  library_path = "~/src/projects/term-do/lib";

  while ((cmdargs = getopt_long(argc, argv, "cl:hv",
				long_options, &option_index)) != -1) {
    switch (cmdargs) {
    case 'c':
      console=true;
      break;
    case 'l':
      {
	stringstream ss(optarg);
	string item;
	while(getline(ss,item,','))
	  load_plugins.push_back(item);
      }
      break;
    case 'h':
      cout << "Usage: " << argv[0] <<  " [options] \n\
Options: \n\
  -c,--console                Use as console (don't exit after <Enter>) \n\
  -l,--lib                    Specify the plugins to load (comma-delimited) \n\
  -h,--help                   Display this information \n\
  -v,--version                Display version information\n";
      exit(0);
      break;
    case '?':
      exit(0);
      break;
    }
  }

  // if (optind < argc) {
  //   while (optind < argc)
  //     framenum = atoi(argv[optind++]);
  // }

  string command;

  if(console) {
    try {
      Client term_logic;
      do {
	term_logic.reset();
	command = term_logic.loopDo();
	term_logic.run(command);
      } while(!command.empty());
    }
    catch(exception& e) {
      cout << "could not connect to daemon" << endl;
    }
  }
  else {
    try {
      Client term_logic;
      command = term_logic.loopDo();
      term_logic.run(command);
    }
    catch(exception& e) {
      cout << "could not connect to daemon" << endl;
    }
  }
    // add command to bash history
    // system(("bash -c \"history -s " + command + "\"").c_str());
}
