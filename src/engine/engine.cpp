
#include "engine/engine.h"
#include "base/memory/singleton.h"
#include "renderer/include/Root.h"
#include "renderer/include/RenderSystem.h"
#include "renderer/include/ConfigFile.h"
#include "third_party/lua5.1/lua.hpp"

//#if PLATFORM == WIN32
#include "engine/window_win32.h"
//#else if PLATFORM == IOS
//#include "engine/window_ios.h"
//#else if PLATFORM == ANDROID
//#include "engine/window_android.h"
//#endif

namespace engine {

iEngine* iEngine::GetInstance() {
  return Singleton<iEngine, StaticMemorySingletonTraits<iEngine> >::get();
}

iEngine::iEngine() : renderer_root_(NULL) {

}

iEngine::~iEngine() {

}

bool iEngine::Init() {
  L = lua_open();

  renderer_root_ = new renderer::Root();
  renderer_root_->restoreConfig();

  // 根据配置文件创建渲染窗口
  render_window_ = new OSWindow();
  std::string name = "test";
  int width = 800, height = 600;
  int colourDepth = 32;
  bool fullScreen = false;
  int left = 0, top = 0;
  bool depthBuffer = true;

  render_window_->create(name, width, height, colourDepth, fullScreen, 
    left, top, depthBuffer);

  // ？
  SetupResource();

  renderer_root_->initialise();

  // Set default mipmap level (NB some APIs ignore this)
  renderer::TextureManager::getSingleton().setDefaultNumMipMaps(5);

  // 读取配置

  // 配置渲染器
  ConfigRenderer();

  // 渲染窗口挂接到渲染系统
  renderer_root_->getRenderSystem()->attachRenderTarget(*render_window_);
  return true;
}

bool iEngine::RunFrame(float delta_time) {
  if (!exit_) {
    //ClearEventTimes();

    renderer::RenderSystem* render_sys = renderer_root_->getRenderSystem();
    render_sys->ResetStatistics();

	// 运行窗口循环
    render_window_->RunMessageLoop();
    // 特效系统
    // 物理系统
    // AI
    // GUI
    renderer_root_->RunFrame(delta_time);
    
  }

  return exit_;
}

void iEngine::Exit() {
  renderer_root_->shutdown();
  delete renderer_root_;
  //delete render_window_;
  renderer_root_ = NULL;
  render_window_ = NULL;
  lua_close(L);
}

void iEngine::SetupResource() {
  // Load resource paths from config file
  renderer::ConfigFile cf;
  cf.load("resources.cfg");

  // Go through all settings in the file
  renderer::ConfigFile::SettingsIterator i = cf.getSettingsIterator();

  String typeName, archName;
  while (i.hasMoreElements()) {
    typeName = i.peekNextKey();
    archName = i.getNext();
    renderer::ResourceManager::addCommonArchiveEx( archName, typeName );
  }
}

void iEngine::RequestExit() {
  exit_ = true;
}

void iEngine::ConfigRenderer() {
  
}

}
