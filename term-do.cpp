#include "term-do.h"

list_t load_plugins;

TermDo::TermDo() {
  view.setPrompt("/-/");
  if(load_plugins.empty())
    plugins = new Plugins();
  else
    plugins = new Plugins(load_plugins);
  init();
}

TermDo::~TermDo() {
  view.clearLine(); 
  delete plugins;
  cleanup();
}

void TermDo::init() {
  matcher = new Matcher();
  plugins->getDictionary(matcher);
  view.refreshLine(matcher->getQuery(),matcher->getMatches(),plugins->getTokens());
}

void TermDo::cleanup() {
  delete matcher;
}

int TermDo::handleChar(char c) {
  bool done = false;

  // standard character range
  if(c >= 'a' && c <= 'z')
    matcher->addChar(c);
  // downcase "automatically"
  else if(c >= 'A' && c <= 'Z')
    matcher->addChar(c);
    // matcher->addChar(c-('Z'-'z'));
  // make slash act like a tab, for ido-like behavior
  else if(c == '/') {
    matcher->addChar(c);
    if(matcher->exactMatch()) commitToken();
  }
  // allow other valid characters
  else if( c > ' ' && c <= '~')
    matcher->addChar(c);
  // C-s
  else if(c==19)
    matcher->rotateForward();
  // C-r
  else if(c==18) {
    matcher->rotateBackward();
  }
  // tab
  else if(c==9) {
    //if(matcher->getMatches().size()<=1 || matcher->exactMatch())
      commitValidToken();
  }
  // space
  else if(c==' ') // (c==32)
    commitToken();
  // enter
  else if(c==13) {
    if(matcher->getMatches().size()<=1 || matcher->exactMatch())
      done=commitValidToken();
    done = (!matcher->getQuery().empty() || plugins->getTokens().size() > 0);
  }
  // backspace
  else if(c==127)
    // if matcher out of characters to backspace
    if(!matcher->removeChar()) {
      // remove token
      plugins->pop();
      cleanup();
      init();
    }
  
  // C-c , C-d , C-g, or time to quit
  return (c==3 || c==4 || c==7 || done);
}

bool TermDo::commitValidToken() {
  string match = matcher->getMatch();

  if(match.empty())
    return false;

  plugins->push(match);

  // get a new matcher
  cleanup();
  init();
  return true;
}

bool TermDo::commitToken() {
  if(matcher->getQuery().empty())
    return false;

  plugins->push(matcher->getQuery());

  // get a new matcher
  cleanup();
  init();
  return true;
}

string TermDo::loopDo() {
  int done=0;
  while(!done) {
    done = handleChar(view.getChar());
    view.refreshLine(matcher->getQuery(),matcher->getMatches(),plugins->getTokens());
  }

  // assume a terminal-executable command if no match is found--useful
  // for when something nontrivial using the dir.so plugin is entered
  string cmd = plugins->getCommand();
  if(cmd.empty()) {
    list_t list = plugins->getTokens();
    string output ="";
      FOR_l(i,list)
	output = output + (i==0 || is_dir(list[i-1]) ? "" : " ") + list.at(i);
    return output;
  }
  else
    return cmd;
}

int main(int argc, char *argv[]) {

  int cmdargs;
  static struct option long_options[] = {
    {"lib", 1, 0, 'l'},
    {"help", 0, 0, 'h'},
    {"version", 0, 0, 'v'},
    {NULL, 0, NULL, 0}
  };
  int option_index = 0;

  while ((cmdargs = getopt_long(argc, argv, "l:hv",
				long_options, &option_index)) != -1) {
    switch (cmdargs) {
    case 'l':
      {
	stringstream ss(optarg);
	string item;
	while(getline(ss,item,','))
	  load_plugins.push_back(item);
      }
      break;
    case 'h':
      printf("Usage: %s [options] \n\
Options: \n\
  -l,--lib                    Specify the plugins to load (comma-delimited) \n\
  -h,--help                   Display this information \n\
  -v,--version                Display version information\n",argv[0]);
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

  // like this so TermDo's destructor is called
  {
    TermDo term_logic;
    command = term_logic.loopDo();
  }
  
  if(!command.empty())
    system(command.c_str());
}
