
#include "TagPoint.h"
#include "Matrix4.h"
#include "Matrix3.h"
#include "Entity.h"
#include "SceneNode.h"
#include "Skeleton.h"
#include "Quaternion.h"


using namespace renderer;


TagPoint::TagPoint(unsigned short handle, Skeleton* creator): Bone(handle, creator) {
  mParentEntity = 0;
  mChildObject = 0;
}


TagPoint::~TagPoint() {
}

Entity *TagPoint::getParentEntity(void) {
  return mParentEntity;
}

void TagPoint::setParentEntity(Entity *pEntity) {
  mParentEntity = pEntity;
}


void TagPoint::setChildObject(MovableObject *pObject) {
  mChildObject = pObject;
}

Matrix4 TagPoint::_getFullTransform(void) {
  //return getParentEntityTransform() * Node::_getFullTransform();
  // Now that we're overriding updateFromParent(), this doesn't need to be different
  return Node::_getFullTransform();
}

Matrix4 TagPoint::_getNodeFullTransform(void) {
  return Node::_getFullTransform();
}


Matrix4 TagPoint::getParentEntityTransform() {

  return mParentEntity->_getParentNodeFullTransform();
}


void TagPoint::_update(bool updateChildren, bool parentHasChanged) {
  //It's the parent entity that is currently updating the skeleton and his bones
  if(mParentEntity == mCreator->getCurrentEntity()) {
    Node::_update(updateChildren, parentHasChanged);
  }
}

void TagPoint::needUpdate() {
  // We need to tell parent entities node
  if (mParentEntity) {
    Node* n = mParentEntity->getParentNode();
    if (n) {
      n->needUpdate();
    }

  }

}

void TagPoint::_updateFromParent(void) const {
  // Call superclass
  Bone::_updateFromParent();

  // Include Entity transform
  if (mParentEntity) {
    Node* entityParentNode = mParentEntity->getParentNode();
    if (entityParentNode) {
      Quaternion mParentQ = entityParentNode->_getDerivedOrientation();
      mDerivedOrientation = mParentQ * mDerivedOrientation;

      // Change position vector based on parent's orientation
      mDerivedPosition = mParentQ * mDerivedPosition;


      // Add altered position vector to parents
      mDerivedPosition += entityParentNode->_getDerivedPosition();
    }
  }

}



