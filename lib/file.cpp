#include <vector>
#include <string>

using namespace std;

#ifdef __cplusplus
extern "C" {
#endif

vector<string> list(vector<string> input) {
  vector<string> dictionary;
  if(input.empty()) {
    dictionary.push_back("view");
    dictionary.push_back("open");
    dictionary.push_back("vim");
    dictionary.push_back("vimdiff");
    dictionary.push_back("vimpager");
    dictionary.push_back("vga-on");
    dictionary.push_back("vgimport");
    dictionary.push_back("vi");
    dictionary.push_back("vncpasswd");
    dictionary.push_back("vgcfgbackup");
    dictionary.push_back("vgimportclone");
    dictionary.push_back("video-dimensions");
    dictionary.push_back("vimtutor");
    dictionary.push_back("vncserver");
    dictionary.push_back("vgcfgrestore");
    dictionary.push_back("vgmerge");
    dictionary.push_back("video-length");
    dictionary.push_back("vipw");
    dictionary.push_back("vncviewer");
    dictionary.push_back("vgchange");
    dictionary.push_back("vgmknodes");
    dictionary.push_back("video-objects");
    dictionary.push_back("visualinfo");
    dictionary.push_back("vpe");
    dictionary.push_back("vainfo");
    dictionary.push_back("vgck");
    dictionary.push_back("vgreduce");
    dictionary.push_back("video-report");
    dictionary.push_back("visudo");
    dictionary.push_back("vpl2ovp");
    dictionary.push_back("vdir");
    dictionary.push_back("vgconvert");
    dictionary.push_back("vgremove");
    dictionary.push_back("video-server");
    dictionary.push_back("vlc");
    dictionary.push_back("vpl2vpl");
    dictionary.push_back("vedit");
    dictionary.push_back("vgcreate");
    dictionary.push_back("vgrename");
    dictionary.push_back("vlc-wrapper");
    dictionary.push_back("vptovf");
    dictionary.push_back("vercmp");
    dictionary.push_back("vgdisplay");
    dictionary.push_back("vgs");
    dictionary.push_back("vigr");
    dictionary.push_back("vlna");
    dictionary.push_back("vpxdec");
    dictionary.push_back("vftovp");
    dictionary.push_back("vgexport");
    dictionary.push_back("vgscan");
    dictionary.push_back("vmstat");
    dictionary.push_back("vpxenc");
    dictionary.push_back("vga-off");
    dictionary.push_back("vgextend");
    dictionary.push_back("vgsplit");
    dictionary.push_back("vncconnect");
  }
  else if(!input.front().compare("vim")) {
    dictionary.push_back("test1");
    dictionary.push_back("test2");
    dictionary.push_back("test3");
  }
  else
    dictionary.push_back(input.front());

  return dictionary;
}

#ifdef __cplusplus
}
#endif
