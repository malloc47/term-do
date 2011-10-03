#include "standalone.h"
#include "client.h"
#include "server.h"

#include <sys/file.h>

int main(int argc, char *argv[]) {
  int mode=0;
  int cmdargs;
  bool mode_specified=false;
  bool console=false;
  bool locked=false;
  static struct option long_options[] = {
    {"daemon", 0, 0, 'd'},
    {"client", 0, 0, 'c'},
    {"standalone", 0, 0, 's'},
    {"console", 0, 0, 'r'},
    {"lib", 1, 0, 'l'},
    {"help", 0, 0, 'h'},
    {"version", 0, 0, 'v'},
    {NULL, 0, NULL, 0}
  };
  int option_index = 0;

  library_path = "~/src/projects/term-do/lib";

  while ((cmdargs = getopt_long(argc, argv, "cdrl:hv",
				long_options, &option_index)) != -1) {
    switch (cmdargs) {
    case 'd':
      mode = MODE_DAEMON;
      mode_specified=true;
      break;
    case 'c':
      mode = MODE_CLIENT;
      mode_specified=true;
      break;
    case 's':
      mode = MODE_STANDALONE;
      mode_specified=true;
      break;
    case 'r':
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
  -r,--console                Use as console (don't exit after <Enter>) \n\
  -d,--daemon                 Launch daemon \n\
  -c,--client                 Connect to daemon \n\
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

  // open file and get lock (if successful, then we can daemonize)
  int file = open("term-do.pid", O_RDWR | O_CREAT, 0666);
  if (file < 0) locked=true;

  int lock = flock(file, LOCK_EX | LOCK_NB);
  if (lock) locked=true;

  if(lock && mode==MODE_DAEMON) {
    cout << "Daemon already running!" << endl;
    exit(1);
  }

  // Close lock right away if we're not a daemon
  if(!lock && mode!=MODE_DAEMON) {
    flock(lock,LOCK_UN);
    close(file);
  }

  // if there is a deamon switch us to client mode if not specifically
  // instructed otherwise
  if(mode==MODE_STANDALONE && !mode_specified && locked==true)
    mode=MODE_CLIENT;

  // if (optind < argc) {
  //   while (optind < argc)
  //     framenum = atoi(argv[optind++]);
  // }

  string command;

  Frontend *term_do;

  switch (mode) {
  case MODE_STANDALONE:
    term_do = new Standalone();
    break;
  case MODE_CLIENT:
    term_do = new Client();
    break;
  case MODE_DAEMON:
    cout << "Launching term-do daemon..." << endl;
    term_do = new Server();
    break;
  }

  if(console && mode != MODE_DAEMON) {
    do {
      term_do->reset();
      command = term_do->loopDo();
      term_do->run(command);
    } while(!command.empty());
  }
  else {
    command = term_do->loopDo();
    term_do->run(command);
  }
  
  delete term_do;

  flock(lock,LOCK_UN);
  close(file);
    // add command to bash history
    // system(("bash -c \"history -s " + command + "\"").c_str());
}
