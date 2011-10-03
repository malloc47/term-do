#ifndef FRONTEND_H_
#define FRONTEND_H_

#include "common.h"

using namespace std;

class Frontend {
public:
  virtual ~Frontend() = 0;
  virtual void reset() = 0;
  virtual string loopDo() = 0;
  virtual void run(string) = 0;
};

#endif
