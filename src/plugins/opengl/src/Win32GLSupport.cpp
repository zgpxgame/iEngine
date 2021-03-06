#include "Exception.h"
#include "LogManager.h"

#include <algorithm>

#include "Win32GLSupport.h"

#include "Win32Window.h"

using namespace renderer;

namespace renderer {
template<class C> void remove_duplicates(C& c) {
  std::sort(c.begin(), c.end());
  typename C::iterator p = std::unique(c.begin(), c.end());
  c.erase(p, c.end());
}

// 添加渲染器选项
void Win32GLSupport::addConfig() {
  //TODO: EnumDisplayDevices http://msdn.microsoft.com/library/en-us/gdi/devcons_2303.asp
  /*vector<string> DisplayDevices;
  DISPLAY_DEVICE DisplayDevice;
  DisplayDevice.cb = sizeof(DISPLAY_DEVICE);
  DWORD i=0;
  while (EnumDisplayDevices(NULL, i++, &DisplayDevice, 0) {
  	DisplayDevices.push_back(DisplayDevice.DeviceName);
  }*/

  ConfigOption optFullScreen;
  ConfigOption optVideoMode;
  ConfigOption optColourDepth;
  ConfigOption optDisplayFrequency;
  ConfigOption optVSync;

  // FS setting possiblities
  optFullScreen.name = "Full Screen";
  optFullScreen.possibleValues.push_back("Yes");
  optFullScreen.possibleValues.push_back("No");
  optFullScreen.currentValue = "Yes";
  optFullScreen.immutable = false;

  // Video mode possiblities
  DEVMODE DevMode;
  DevMode.dmSize = sizeof(DEVMODE);
  optVideoMode.name = "Video Mode";
  optVideoMode.immutable = false;
  for (DWORD i = 0; EnumDisplaySettings(NULL, i, &DevMode); ++i) {
    if (DevMode.dmBitsPerPel < 16 || DevMode.dmPelsHeight < 480)
      continue;
    mDevModes.push_back(DevMode);
    char szBuf[16];
    _snprintf(szBuf, 16, "%d x %d", DevMode.dmPelsWidth, DevMode.dmPelsHeight);
    optVideoMode.possibleValues.push_back(szBuf);
  }
  remove_duplicates(optVideoMode.possibleValues);
  optVideoMode.currentValue = optVideoMode.possibleValues.front();

  optColourDepth.name = "Colour Depth";
  optColourDepth.immutable = false;
  optColourDepth.currentValue = "";

  optDisplayFrequency.name = "Display Frequency";
  optDisplayFrequency.immutable = false;
  optDisplayFrequency.currentValue = "";

  optVSync.name = "VSync";
  optVSync.immutable = false;
  optVSync.possibleValues.push_back("Yes");
  optVSync.possibleValues.push_back("No");
  optVSync.currentValue = "No";

  mOptions[optFullScreen.name] = optFullScreen;
  mOptions[optVideoMode.name] = optVideoMode;
  mOptions[optColourDepth.name] = optColourDepth;
  mOptions[optDisplayFrequency.name] = optDisplayFrequency;
  mOptions[optVSync.name] = optVSync;

  refreshConfig();
}

void Win32GLSupport::refreshConfig() {
  ConfigOptionMap::iterator optVideoMode = mOptions.find("Video Mode");
  ConfigOptionMap::iterator moptColourDepth = mOptions.find("Colour Depth");
  ConfigOptionMap::iterator moptDisplayFrequency = mOptions.find("Display Frequency");
  if(optVideoMode == mOptions.end() || moptColourDepth == mOptions.end() || moptDisplayFrequency == mOptions.end())
    Except(999, "Can't find mOptions!", "Win32GLSupport::refreshConfig");
  ConfigOption* optColourDepth = &moptColourDepth->second;
  ConfigOption* optDisplayFrequency = &moptDisplayFrequency->second;

  String val = optVideoMode->second.currentValue;
  String::size_type pos = val.find('x');
  if (pos == String::npos)
    Except(999, "Invalid Video Mode provided", "Win32GLSupport::refreshConfig");
  int width = atoi(val.substr(0, pos).c_str());

  // 根据当前的显示模式，刷新“位深”和“刷新频率”选项
  for(std::vector<DEVMODE>::const_iterator i = mDevModes.begin(); i != mDevModes.end(); ++i) {
    if (i->dmPelsWidth != width)
      continue;
    char buf[128];
    sprintf(buf, "%d", i->dmBitsPerPel);
    optColourDepth->possibleValues.push_back(buf);
    sprintf(buf, "%d", i->dmDisplayFrequency);
    optDisplayFrequency->possibleValues.push_back(buf);
  }

  // 删除重复的位深和刷新频率选项
  remove_duplicates(optColourDepth->possibleValues);
  remove_duplicates(optDisplayFrequency->possibleValues);

  // 第一个值作为当前选项
  optColourDepth->currentValue = optColourDepth->possibleValues.front();
  optDisplayFrequency->currentValue = optDisplayFrequency->possibleValues.front();
}

void Win32GLSupport::setConfigOption(const String &name, const String &value) {
  ConfigOptionMap::iterator it = mOptions.find(name);

  // Update
  if(it != mOptions.end())
    it->second.currentValue = value;
  else {
    char msg[128];
    sprintf( msg, "Option named '%s' does not exist.", name.c_str() );
    Except( Exception::ERR_INVALIDPARAMS, msg, "Win32GLSupport::setConfigOption" );
  }

  if( name == "Video Mode" )
    refreshConfig();

  if( name == "Full Screen" ) {
    it = mOptions.find( "Display Frequency" );
    if( value == "No" ) {
      it->second.currentValue = "N/A";
      it->second.immutable = true;
    } else {
      it->second.currentValue = it->second.possibleValues.front();
      it->second.immutable = false;
    }
  }
}

String Win32GLSupport::validateConfig() {
  // TODO, DX9
  return String("");
}

RenderWindow* Win32GLSupport::createWindow(bool autoCreateWindow, GLRenderSystem* renderSystem) {
  if (autoCreateWindow) {
    ConfigOptionMap::iterator opt = mOptions.find("Full Screen");
    if (opt == mOptions.end())
      Except(999, "Can't find full screen options!", "Win32GLSupport::createWindow");
    bool fullscreen = (opt->second.currentValue == "Yes");

    opt = mOptions.find("Video Mode");
    if (opt == mOptions.end())
      Except(999, "Can't find video mode options!", "Win32GLSupport::createWindow");
    String val = opt->second.currentValue;
    String::size_type pos = val.find('x');
    if (pos == String::npos)
      Except(999, "Invalid Video Mode provided", "Win32GLSupport::createWindow");

    int w = atoi(val.substr(0, pos).c_str());
    int h = atoi(val.substr(pos + 1).c_str());

    opt = mOptions.find("Colour Depth");
    if (opt == mOptions.end())
      Except(999, "Can't find Colour Depth options!", "Win32GLSupport::createWindow");
    int colourDepth = atoi(opt->second.currentValue.c_str());

    opt = mOptions.find("VSync");
    if (opt == mOptions.end())
      Except(999, "Can't find VSync options!", "Win32GLSupport::createWindow");
    bool vsync = (opt->second.currentValue == "Yes");
    renderSystem->setWaitForVerticalBlank(vsync);

    return renderSystem->createRenderWindow("OGRE Render Window", w, h, colourDepth, fullscreen);
  } else {
    // XXX What is the else?
    return NULL;
  }
}

RenderWindow* Win32GLSupport::newWindow(const String& name, int width, int height, int colourDepth,
                                        bool fullScreen, int left, int top, bool depthBuffer, RenderWindow* parentWindowHandle,
                                        bool vsync) {
  ConfigOptionMap::iterator opt = mOptions.find("Display Frequency");
  if (opt == mOptions.end())
    Except(999, "Can't find Colour Depth options!", "Win32GLSupport::newWindow");
  int displayFrequency = atoi(opt->second.currentValue.c_str());

  Win32Window* window = new Win32Window();
  window->create(name, width, height, colourDepth, fullScreen, left, top, depthBuffer,
                 parentWindowHandle, vsync, displayFrequency);
  return window;
}

void Win32GLSupport::start() {
  LogManager::getSingleton().logMessage("*** Starting Win32GL Subsystem ***");
}

void Win32GLSupport::stop() {
  LogManager::getSingleton().logMessage("*** Stopping Win32GL Subsystem ***");
}

void* Win32GLSupport::getProcAddress(const String& procname) {
  return (void*)wglGetProcAddress( procname.c_str() );
}

}
