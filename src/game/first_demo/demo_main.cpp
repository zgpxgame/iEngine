
#if OGRE_PLATFORM == PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif

#include "base/at_exit.h"
#include "base/command_line.h"
#include "base/logging.h"
#include "engine/engine.h"
#include "game/first_demo/game_scene.h"

namespace renderer {
	class Camera;
	class SceneManager;
	class RenderWindow;
}

class GameApp {
public:
	bool Init();
	bool Launch();
	void Exit();

private:
	void CreateGameScene();

	GameScene* game_scene_;
	renderer::RenderWindow* render_window_;
};


void GameApp::CreateGameScene() {
	game_scene_ = new GameScene();
	game_scene_->Create();
}

bool GameApp::Init() {
	engine::iEngine::GetInstance()->Init();
	// ...
	CreateGameScene();
  return true;
}

bool GameApp::Launch() {
  int last_tick = GetTickCount();
	while (1) {
    int now_tick = GetTickCount();
    float delta_time = (now_tick - last_tick) / 1000.0f;
		game_scene_->UpdateFrame(delta_time);
		if (engine::iEngine::GetInstance()->RunFrame(delta_time))
			break;

    last_tick = now_tick;
    //Sleep(20);
	}
  return true;
}

void GameApp::Exit() {
	delete game_scene_;
	game_scene_ = NULL;
	engine::iEngine::GetInstance()->Exit();
}

int main(int argc, char **argv) {
  base::AtExitManager at_exit;

  CommandLine::Init(0, 0);

  logging::InitLogging(L"demo.log", 
    logging::LOG_TO_BOTH_FILE_AND_SYSTEM_DEBUG_LOG, 
    logging::DONT_LOCK_LOG_FILE, logging::DELETE_OLD_LOG_FILE,
    logging::ENABLE_DCHECK_FOR_NON_OFFICIAL_RELEASE_BUILDS);

  GameApp app;
  app.Init();
  app.Launch();
  app.Exit();

  return 0;
}
