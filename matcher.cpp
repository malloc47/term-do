#include "matcher.h"

Matcher::Matcher() {
  init();
}

void Matcher::init(){
  word="";
  command="";
  actions.push_back("view");
  actions.push_back("open");
  actions.push_back("vim");
  actions.push_back("vimdiff");
  actions.push_back("vimpager");
  actions.push_back("vga-on");
  actions.push_back("vgimport");
  actions.push_back("vi");
  actions.push_back("vncpasswd");
  actions.push_back("vgcfgbackup");
  actions.push_back("vgimportclone");
  actions.push_back("video-dimensions");
  actions.push_back("vimtutor");
  actions.push_back("vncserver");
  actions.push_back("vgcfgrestore");
  actions.push_back("vgmerge");
  actions.push_back("video-length");
  actions.push_back("vipw");
  actions.push_back("vncviewer");
  actions.push_back("vgchange");
  actions.push_back("vgmknodes");
  actions.push_back("video-objects");
  actions.push_back("visualinfo");
  actions.push_back("vpe");
  actions.push_back("vainfo");
  actions.push_back("vgck");
  actions.push_back("vgreduce");
  actions.push_back("video-report");
  actions.push_back("visudo");
  actions.push_back("vpl2ovp");
  actions.push_back("vdir");
  actions.push_back("vgconvert");
  actions.push_back("vgremove");
  actions.push_back("video-server");
  actions.push_back("vlc");
  actions.push_back("vpl2vpl");
  actions.push_back("vedit");
  actions.push_back("vgcreate");
  actions.push_back("vgrename");
  actions.push_back("vlc-wrapper");
  actions.push_back("vptovf");
  actions.push_back("vercmp");
  actions.push_back("vgdisplay");
  actions.push_back("vgs");
  actions.push_back("vigr");
  actions.push_back("vlna");
  actions.push_back("vpxdec");
  actions.push_back("vftovp");
  actions.push_back("vgexport");
  actions.push_back("vgscan");
  actions.push_back("vmstat");
  actions.push_back("vpxenc");
  actions.push_back("vga-off");
  actions.push_back("vgextend");
  actions.push_back("vgsplit");
  actions.push_back("vncconnect");        
}

vector<string> Matcher::wordMatches() {
  vector<string> output;
  for(unsigned int i=0;i<actions.size();i++)
    if(actions.at(i).substr(0,word.length()).compare(word) == 0)
      output.push_back(actions.at(i));

  // FIXME: easier to do this without shifting elements around
  // if(match_offset) {
  //   unsigned int offset = match_offset % output.size();
  //   printf("\r\n%d <> %u",match_offset,output.size());
  //   vector<string>::iterator it;
  //   for(unsigned int i=0;i<offset; i++) {
  //     output.push_back(output.front());
  //     output.erase(output.begin());
  //   }
  // }
  
  return output;
}

void Matcher::wordAdd(char c) {
  bool valid_char=false;
  unsigned int len=word.length()+1;
  // loop through all possible actions
  for(unsigned int i=0;i<actions.size();i++)
    // if candidate is long enough and
    if(actions.at(i).length() >= len && 
       // is a valid substring of a possible action
       actions.at(i).substr(0,len).compare(word+c) == 0)
      valid_char=true;

  // add char to word
  if(valid_char)
    word += c;
}

void Matcher::wordRemove() {
  if(word.length()>0)
    word=word.substr(0,word.length()-1);
}

string Matcher::wordCurrent() {return word;}
