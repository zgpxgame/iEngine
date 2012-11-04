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
#ifndef _Ogre_H__
#define _Ogre_H__
// This file includes all the other files which you will need to build a client application
#include "Prerequisites.h"

#include "Root.h"
#include "TextureManager.h"
#include "SceneManager.h"
#include "SceneManagerEnumerator.h"
#include "Camera.h"
#include "Viewport.h"
#include "RenderWindow.h"
#include "RenderSystem.h"
#include "Material.h"
#include "Light.h"
#include "Entity.h"
#include "SubEntity.h"
#include "SceneNode.h"
#include "Matrix3.h"
#include "Matrix4.h"
#include "MeshManager.h"
#include "TextureManager.h"
#include "Mesh.h"
#include "SubMesh.h"
#include "AxisAlignedBox.h"
#include "PatchSurface.h"
#include "MyMath.h"
#include "ControllerManager.h"
#include "LogManager.h"
#include "ArchiveManager.h"
#include "MaterialManager.h"
#include "SDDataChunk.h"
#include "BillboardSet.h"
#include "SimpleRenderable.h"
#include "ParticleSystemManager.h"
#include "ParticleEmitter.h"
#include "ParticleAffector.h"
#include "MeshSerializer.h"
#include "MaterialSerializer.h"
#include "Skeleton.h"
#include "Bone.h"
#include "SkeletonSerializer.h"
#include "SkeletonManager.h"
#include "Animation.h"
#include "AnimationTrack.h"
#include "KeyFrame.h"
#include "RenderTargetListener.h"
#include "RenderQueueListener.h"
#include "RenderTexture.h"
#include "UserDefinedObject.h"
// .... more to come

#endif
