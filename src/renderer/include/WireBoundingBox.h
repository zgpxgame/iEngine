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
#ifndef _WireBoundingBox_H__
#define _WireBoundingBox_H__

#include "Prerequisites.h"

#include "SimpleRenderable.h"

namespace renderer {

/** Allows the rendering of a wireframe bounding box.
    @remarks
        This class builds a wireframe renderable from a given aabb. A pointer to this class can be
	added to a render queue to display the bounding box of an object.
*/
class _RendererExport WireBoundingBox : public SimpleRenderable {

protected:
  /** Pointer to the wireframe vertex data.
  */
  float mVertexData[12*6];

  /** Pointer to the wireframe color data.
  */
  long mDiffuses[24];

  /** Override this method to prevent parent transforms (rotation,translation,scale)
  */
  void getWorldTransforms( Matrix4* xform );

  /** Pointer to a RenderOperation that contains the OT_LINE_LIST description of the wireframe.
  */
//		RenderOperation mRenderOp;

  /** Builds the wireframe line list.
  */
  void setupBoundingBoxVertices(AxisAlignedBox &aab);

  Real mRadius;

public:

  WireBoundingBox();
  ~WireBoundingBox();

  /** Builds the wireframe line list.
      @param
          aabb bounding box to build a wireframe from.
  */
  void setupBoundingBox(AxisAlignedBox aabb);

  Real getSquaredViewDepth(const Camera* cam) const;

  Real getBoundingRadius(void) const {
    return mRadius;
  }

};

}// namespace

#endif


