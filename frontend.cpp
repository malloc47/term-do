#include "frontend.h"
Frontend::~Frontend() {} // Must be defined for derived class destructors to be called
void Frontend::run(string cmd_output) {
  ofstream cmdfile((config_folder+"/cmd").c_str());
  if(cmdfile) {
    cmdfile << cmd_output;
    cmdfile.close();
  }
}
