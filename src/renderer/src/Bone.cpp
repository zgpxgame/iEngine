/*
-----------------------------------------------------------------------------
This source file is part of OGRE
    (Object-oriented Graphics Rendering Engine)
For the latest info, see http://ogre.sourceforge.net/

Copyright (c)2000-2002 The OGRE Team
Also see acknowledgements in Readme.html

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.
-----------------------------------------------------------------------------
*/

#include "Bone.h"
#include "Skeleton.h"
#include "TagPoint.h"

namespace renderer {

//---------------------------------------------------------------------
Bone::Bone(unsigned short handle, Skeleton* creator)
  : Node(), mManuallyControlled(false), mHandle(handle), mCreator(creator) {
}
//---------------------------------------------------------------------
Bone::Bone(const String& name, unsigned short handle, Skeleton* creator)
  : Node(name), mManuallyControlled(false), mHandle(handle), mCreator(creator) {
}
//---------------------------------------------------------------------
Bone::~Bone() {
}
//---------------------------------------------------------------------
Bone* Bone::createChild(unsigned short handle, const Vector3& translate,
                        const Quaternion& rotate) {
  Bone* retBone = mCreator->createBone(handle);
  retBone->translate(translate);
  retBone->rotate(rotate);
  this->addChild(retBone);
  return retBone;
}
//---------------------------------------------------------------------
Node* Bone::createChildImpl(void) {
  return mCreator->createBone();
}
//---------------------------------------------------------------------
Node* Bone::createChildImpl(const String& name) {
  return mCreator->createBone(name);
}
//---------------------------------------------------------------------
void Bone::setBindingPose(void) {
  setInitialState();

  // Save inverse derived, used for mesh transform later (assumes _update() has been called by Skeleton)
  makeInverseTransform(_getDerivedPosition(), Vector3::UNIT_SCALE ,
                       _getDerivedOrientation(), mBindDerivedInverseTransform);
}
//---------------------------------------------------------------------
void Bone::reset(void) {
  resetToInitialState();
}
//---------------------------------------------------------------------
void Bone::setManuallyControlled(bool manuallyControlled) {
  this->mManuallyControlled = manuallyControlled;
}
//---------------------------------------------------------------------
bool Bone::isManuallyControlled() {
  return mManuallyControlled;
}
//---------------------------------------------------------------------
Matrix4 Bone::_getBindingPoseInverseTransform(void) {
  return mBindDerivedInverseTransform;
}
//---------------------------------------------------------------------
unsigned short Bone::getHandle(void) const {
  return mHandle;
}

TagPoint *Bone::createChildTagPoint(const Quaternion &offsetOrientation,const Vector3 &offsetPosition) {
  TagPoint *tp = mCreator->createTagPoint(offsetOrientation, offsetPosition);
  addChild(tp);

  return tp;
}





}

