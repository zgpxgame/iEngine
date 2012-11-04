

#include <windows.h>
#include "base/at_exit.h"
#include "base/memory/scoped_ptr.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
  base::AtExitManager exit_manager;

  CommandLine::Init(GetCommandLine());

  scoped_ptr<GameDemoDelegate> game_delegate(new GameDemoDelegate());

  engine::iEngine::Init(game_delegate.get());
  engine::iEngine::Run();
  return 0;
}