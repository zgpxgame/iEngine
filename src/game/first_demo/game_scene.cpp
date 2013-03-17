
#include "game/first_demo/game_scene.h"
#include "renderer/include/RenderEngine.h"
#include "engine/engine.h"

void GameScene::Create() {
  // 选定场景管理器
  ChooseSceneManager();

  CreateCamera();
  CreateViewports();

  // Create the scene
  CreateScene();
}

void GameScene::ChooseSceneManager(void) {
  // Get the SceneManager, in this case a generic one
  scene_mgr_ = renderer::Root::getSingletonPtr()->getSceneManager(renderer::ST_GENERIC);
}

void GameScene::CreateCamera(void) {
  // Create the camera
  camera_ = scene_mgr_->createCamera("PlayerCam");

  // Position it at 500 in Z direction
  camera_->setPosition(renderer::Vector3(0, 0, 500));
  // Look back along -Z
  camera_->lookAt(renderer::Vector3(0, 0, -300));
  camera_->setNearClipDistance(5);
}

void GameScene::CreateScene() {
  // Set ambient light
  scene_mgr_->setAmbientLight(renderer::ColourValue(0.2, 0.2, 0.2));

  // Create a skydome
  scene_mgr_->setSkyDome(true, "Examples/CloudySky", 5, 8);

  // Create a light
  renderer::Light* l = scene_mgr_->createLight("MainLight");
  // Accept default settings: point light, white diffuse, just set position
  // NB I could attach the light to a SceneNode if I wanted it to move automatically with
  //  other objects, but I don't
  l->setPosition(20,80,50);

  renderer::Entity *ent;

  // Define a floor plane mesh
  renderer::Plane p;
  p.normal = renderer::Vector3::UNIT_Y;
  p.d = 200;

  renderer::MeshManager::getSingleton().createPlane("FloorPlane", 
    p, 200000, 200000, 20, 20, true, 1, 50, 50, renderer::Vector3::UNIT_Z);

  // Create an entity (the floor)
  ent = scene_mgr_->createEntity("floor", "FloorPlane");
  ent->setMaterialName("Examples/RustySteel");

  renderer::SceneNode* root_node = scene_mgr_->getRootSceneNode();

  // Attach to child of root node, better for culling 
  // (otherwise bounds are the combination of the 2)
  static_cast<renderer::SceneNode*>(root_node->createChild())->attachObject(ent);

  // Add a head, give it it's own node
  renderer::SceneNode* head_node;
  head_node = static_cast<renderer::SceneNode*>(root_node->createChild());
  ent = scene_mgr_->createEntity("head", "ogrehead.mesh");
  head_node->attachObject(ent);

  // Make sure the camera track this node
  camera_->setAutoTracking(true, head_node);

  // Create the camera node & attach camera
  renderer::SceneNode* camera_node = static_cast<renderer::SceneNode*>(root_node->createChild());
  camera_node->attachObject(camera_);

  // set up spline animation of node
  renderer::Animation* anim = scene_mgr_->createAnimation("CameraTrack", 10);
  // Spline it for nice curves
  anim->setInterpolationMode(renderer::Animation::IM_SPLINE);
  // Create a track to animate the camera's node
  renderer::AnimationTrack* track = anim->createTrack(0, camera_node);
  // Setup keyframes
  renderer::KeyFrame* key = track->createKeyFrame(0); // startposition
  key = track->createKeyFrame(2.5);
  key->setTranslate(renderer::Vector3(500,500,-1000));
  key = track->createKeyFrame(5);
  key->setTranslate(renderer::Vector3(-1500,1000,-600));
  key = track->createKeyFrame(7.5);
  key->setTranslate(renderer::Vector3(0,-100,0));
  key = track->createKeyFrame(10);
  key->setTranslate(renderer::Vector3(0,0,0));
  // Create a new animation state to track this
  anim_state_ = scene_mgr_->createAnimationState("CameraTrack");
  anim_state_->setEnabled(true);

  // Put in a bit of fog for the hell of it
  scene_mgr_->setFog(renderer::FOG_EXP, renderer::ColourValue::White, 0.0002);
}

void GameScene::CreateViewports(void) {
  // Create one viewport, entire window
  renderer::RenderWindow* window = engine::iEngine::GetInstance()->render_window();
  renderer::Viewport* vp = window->addViewport(camera_);
  vp->setBackgroundColour(renderer::ColourValue(0, 0, 0));
}

void GameScene::UpdateFrame(float delta_time) {
  anim_state_->addTime(delta_time);
}

