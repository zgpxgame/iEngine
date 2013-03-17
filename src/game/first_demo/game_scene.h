
#ifndef _GAME_SCENE_H_
#define _GAME_SCENE_H_

namespace renderer {
  class Camera;
  class SceneManager;
  class SceneNode;
  class AnimationState;
}

// ”Œœ∑≥°æ∞
class GameScene {
public:
  void Create();
  void UpdateFrame(float delta_time);

private:
  void ChooseSceneManager();
  void CreateCamera();
  void CreateViewports();
  void CreateScene();


  renderer::Camera* camera_;
  renderer::SceneManager* scene_mgr_;
  renderer::SceneNode* fountain_node_;
  renderer::AnimationState* anim_state_;
};
#endif