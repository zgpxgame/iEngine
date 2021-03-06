#ifndef __OgreWin32GLSupport_H__
#define __OgreWin32GLSupport_H__

#include "Win32Prerequisites.h"
#include "GLSupport.h"
#include "GLRenderSystem.h"

using std::vector;

namespace renderer {

class Win32GLSupport : public GLSupport {
public:
  /**
  * Add any special config values to the system.
  * Must have a "Full Screen" value that is a bool and a "Video Mode" value
  * that is a string in the form of wxhxb
  */
  void addConfig();

  void setConfigOption(const String &name, const String &value);

  /**
  * Make sure all the extra options are valid
  */
  String validateConfig();

  virtual RenderWindow* createWindow(bool autoCreateWindow, GLRenderSystem* renderSystem);

  /**
  * Create a new specific render window
  */
  virtual RenderWindow* newWindow(const String& name, int width, int height, int colourDepth,
                                  bool fullScreen, int left, int top, bool depthBuffer, RenderWindow* parentWindowHandle,
                                  bool vsync);

  /**
  * Start anything special
  */
  void start();
  /**
  * Stop anything special
  */
  void stop();

  /**
  * Get the address of a function
  */
  void* getProcAddress(const String& procname);
private:
  // Allowed video modes
  vector<DEVMODE> mDevModes;

  void refreshConfig();
};

}

#endif
