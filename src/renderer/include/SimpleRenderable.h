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
#ifndef __SimpleRenderable_H__
#define __SimpleRenderable_H__

#include "Prerequisites.h"

#include "MovableObject.h"
#include "Renderable.h"
#include "AxisAlignedBox.h"
#include "Material.h"

namespace renderer {

class _RendererExport SimpleRenderable : public MovableObject, public Renderable {
protected:
  RenderOperation mRendOp;
  Matrix4 m_matWorldTransform;
  AxisAlignedBox mBox;

  String m_strMatName;
  Material *m_pMaterial;

  Real *m_pVertexCache;
  ushort *m_pIndexCache;
  Real *m_pNormalCache;
  RGBA *m_pDiffuseCache;
  RGBA *m_pSpecularCache;
  Real *m_pTexCache[OGRE_MAX_TEXTURE_COORD_SETS];

  /// The scene manager for the current frame.
  SceneManager *m_pParentSceneManager;

  /// The camera for the current frame.
  Camera *m_pCamera;

  /// The name of the object.
  String m_strName;

  /// Static member used to automatically generate names for SimpleRendaerable objects.
  static uint ms_uGenNameCount;

public:
  SimpleRenderable();

  Real **getVertexCache();
  ushort **getIndexCache();
  Real **getNormalCache();
  RGBA **getDiffuseCache();
  RGBA **getSpecularCache();
  Real **getTexCoordCache( unsigned short cn );

  void setMaterial( const String& matName );
  virtual Material* getMaterial(void) const;

  virtual void setRenderOperation( const RenderOperation& rend );
  virtual void getRenderOperation( RenderOperation& rend );
  RenderOperation& getRenderOperation();

  void setWorldTransform( const Matrix4& xform );
  virtual void getWorldTransforms( Matrix4* xform );


  virtual void _notifyCurrentCamera(Camera* cam);

  void setBoundingBox( const AxisAlignedBox& box );
  virtual const AxisAlignedBox& getBoundingBox(void) const;

  virtual void _updateRenderQueue(RenderQueue* queue);

  virtual ~SimpleRenderable();

  /** Overridden from MovableObject */
  virtual const String& getName(void) const;

  /** Overridden from MovableObject */
  virtual const String getMovableType(void) const;


};
}

#endif

