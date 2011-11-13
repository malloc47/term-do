#include "common.h"
#include "standalone.h"

#ifdef DAEMON
#include "client.h"
#include "server.h"
#include <syslog.h>
#include <signal.h>
#endif

#include <unistd.h>
#include <sys/file.h>
#include <sys/types.h>
#include <sys/stat.h>

Frontend *term_do;

// externs in common.h
list_t load_plugins;
string library_path;
string config_folder;

#ifdef DAEMON
void handler(int sig) {
  delete term_do;
  openlog("term-do",LOG_PID,LOG_DAEMON);
  syslog(LOG_INFO, "term-do daemon stopped");
  closelog();
  exit(EXIT_SUCCESS);
}
#endif

int main(int argc, char *argv[]) {
  int cmdargs;
  bool console=false;
#ifdef DAEMON
  int mode=0;
  bool mode_specified=false;
  bool locked=false;
#endif

  config_folder = string(getenv("HOME"))+"/.term-do.d";

#ifdef DAEMON
  string lockfile;

  if(getuid()) {
    // mkdir((string(getenv("HOME"))+"/.term-do.d").c_str(),S_IRWXU);
    // lockfile=string(getenv("HOME"))+"/.term-do.d/term-do.pid";
    mkdir(config_folder.c_str(),S_IRWXU);
    lockfile=config_folder+"/term-do.pid";
  }
  else {
    lockfile="/var/run/term-do.pid";
  }
#endif

  static struct option long_options[] = {
#ifdef DAEMON
    {"daemon", 0, 0, 'd'},
    {"client", 0, 0, 'c'},
    {"standalone", 0, 0, 's'},
#endif
    {"console", 0, 0, 'r'},
    {"lib", 1, 0, 'l'},
    {"help", 0, 0, 'h'},
    {"version", 0, 0, 'v'},
    {NULL, 0, NULL, 0}
  };
  int option_index = 0;

  library_path = config_folder+"/plugins"+":"+string(LIBRARY_PATH);

  while ((cmdargs = getopt_long(argc, argv, 
#ifdef DAEMON
				"cdsrl:hv",
#else
				"rl:hv",
#endif
				long_options, &option_index)) != -1) {
    switch (cmdargs) {
#ifdef DAEMON
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
#endif
    case 'r':
      console=true;
      break;
    case 'l':
      {
	stringstream ss(optarg);
	string item;
	while(getline(ss,item,',')) {
	  // trim beginning
	  item.erase(item.begin(), find_if(item.begin(), item.end(), not1(ptr_fun<int, int>(isspace))));
	  // and end
	  item.erase(find_if(item.rbegin(), item.rend(), not1(ptr_fun<int, int>(isspace))).base(), item.end());
	  load_plugins.push_back(item);
	}
      }
      break;
    case 'h':
      cout << "Usage: " << argv[0] <<  " [options] \n\
Options: \n\
  -r,--console      Use as console (don't exit after <Enter>) \n" <<
#ifdef DAEMON
"  -d,--daemon       Launch in server mode and daemonize \n\
  -c,--client       Connect to daemon (default if daemon exists) \n\
  -s,--standalone   Launch as an independent application (default if no daemon) \n" <<
#endif
"  -l,--lib          Specify the plugins to load (comma-delimited) \n\
  -h,--help         Display this information \n\
  -v,--version      Display version information\n";
      exit(EXIT_SUCCESS);
      break;
    case 'v':
      cout << "Version information forthcoming\n";
      exit(EXIT_SUCCESS);
      break;
    case '?':
      exit(EXIT_FAILURE);
      break;
    }
  }

#ifdef DAEMON
  // open file and get lock (if successful, then we can daemonize)
  int file = open(lockfile.c_str(), O_RDWR | O_CREAT, 0666);
  if (file < 0) locked=true;

  int lock = flock(file, LOCK_EX | LOCK_NB);
  if (lock) locked=true;

  if(lock && mode==MODE_DAEMON) {
    cout << "Daemon already running!" << endl;
    cout << "Close other daemon or delete " << lockfile << 
      " to start in daemon mode"  << endl;
    exit(1);
  }

  // Close lock right away if we're not a daemon
  if(!lock && mode!=MODE_DAEMON) {
    flock(lock,LOCK_UN);
    close(file);
  }

  // if there is a deamon, switch us to client mode if not
  // specifically instructed otherwise
  if(mode==MODE_STANDALONE && !mode_specified && locked==true)
    mode=MODE_CLIENT;
#endif

  string command;

  ofstream pwdfile((config_folder+"/pwd").c_str());
  if(pwdfile) {
    pwdfile << string(getenv("PWD"));
    pwdfile.close();
  }

#ifdef DAEMON
  switch (mode) {
  case MODE_STANDALONE:
    term_do = new Standalone();
    break;
  case MODE_CLIENT:
    term_do = new Client();
    break;
  case MODE_DAEMON:
    // load once in daemon mode
    cout << "Launching term-do daemon" << endl;
    break;
  }
#else
  term_do = new Standalone();
#endif

#ifdef DAEMON
  if(mode != MODE_DAEMON) {
#endif
    if(console) {
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
#ifdef DAEMON
  }
  else {
    pid_t pid, sid;

    pid = fork();
    if (pid < 0)
      exit(EXIT_FAILURE);
    if (pid > 0)
      exit(EXIT_SUCCESS);

    umask(0);

    sid = setsid();
    if (sid < 0)
      exit(EXIT_FAILURE);
        
    if ((chdir("/")) < 0)
      exit(EXIT_FAILURE);
        
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    // if(daemon(0, 0) < 0)
    //   exit(EXIT_FAILURE);

    // handle simple TERM signal
    struct sigaction new_action;
    new_action.sa_handler = handler;
    new_action.sa_flags = 0;
    if(sigaction(SIGTERM,&new_action,NULL) == -1)
      exit(EXIT_FAILURE);

    openlog("term-do",LOG_PID,LOG_DAEMON);
    syslog(LOG_INFO, "term-do daemon started");
    closelog();
    
    // Not loading here means that any path-dependent errors won't
    // happen above, but may occur at other times (yes, discovered
    // this the hard way)
    term_do = new Server();
    term_do->loopDo();
        
    // exit(EXIT_SUCCESS);
  }
#endif
  delete term_do;

#ifdef DAEMON
  flock(lock,LOCK_UN);
  close(file);
  openlog("term-do",LOG_PID,LOG_DAEMON);
  syslog(LOG_INFO, "term-do daemon stopped");
  closelog();
#endif

  exit(EXIT_SUCCESS);

    // add command to bash history
    // system(("bash -c \"history -s " + command + "\"").c_str());
}
