
#ifndef _IENGINE_H_
#define _IENGINE_H_

#include "engine_delegate.h"

namespace engine {

class iEngine : public base::Singleton<iEngine> {
public:
  static bool Init(iEngineDelegate* delegate);
  static bool Run();
};
}

#endif