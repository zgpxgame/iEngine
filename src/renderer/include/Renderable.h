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
#ifndef __Renderable_H__
#define __Renderable_H__

#include "Prerequisites.h"
#include "Common.h"

#include "RenderOperation.h"
#include "Matrix4.h"


namespace renderer {

/** Abstract class defining the interface all renderable objects must implement.
    @remarks
        This interface abstracts renderable discrete objects which will be queued in the render pipeline,
        grouped by material. Classes implementing this interface must be based on a single material, a single
        world matrix (or a collection of world matrices which are blended by weights), and must be
        renderable via a single render operation.
    @par
        Note that deciding whether to put these objects in the rendering pipeline is done from the more specific
        classes e.g. entities. Only once it is decided that the specific class is to be rendered is the abstract version
        created (could be more than one per visible object) and pushed onto the rendering queue.
*/
class _RendererExport Renderable {
public:
  /** Retrieves a pointer to the material this renderable object uses.
  */
  virtual Material* getMaterial(void) const = 0;
  /** Gets the render operation required to send this object to the frame buffer.
      @param
          rend RenderOperation structure to update.
  */
  virtual void getRenderOperation(RenderOperation& rend) = 0;
  /** Gets the world transform matrix / matrices for this renderable object.
      @remarks
          If the object has any derived transforms, these are expected to be up to date as long as
          all the SceneNode structures have been updated before this is called.
      @par
          This method will populate xform with 1 matrix if it does not use vertex blending. If it
          does use vertex blending it will fill the passed in pointer with an array of matrices,
          the length being the value returned from getNumWorldTransforms.
  */
  virtual void getWorldTransforms(Matrix4* xform) = 0;

  /** Returns the number of world transform matrices this renderable requires.
  @remarks
      When a renderable uses vertex blending, it uses multiple world matrices instead of a single
      one. Each vertex sent to the pipeline can reference one or more matrices in this list
      with given weights.
      If a renderable does not use vertex blending this method returns 1, which is the default for
      simplicity.
  */
  virtual unsigned short getNumWorldTransforms(void) {
    return 1;
  }

  /** Returns whether or not to use an 'identity' projection.
  @remarks
      Usually Renderable objects will use a projection matrix as determined
      by the active camera. However, if they want they can cancel this out
      and use an identity projection, which effectively projects in 2D using
      a {-1, 1} view space. Useful for overlay rendering. Normal renderables need
      not override this.
  */
  virtual bool useIdentityProjection(void) {
    return false;
  }

  /** Returns whether or not to use an 'identity' projection.
  @remarks
      Usually Renderable objects will use a view matrix as determined
      by the active camera. However, if they want they can cancel this out
      and use an identity matrix, which means all geometry is assumed
      to be relative to camera space already. Useful for overlay rendering.
      Normal renderables need not override this.
  */
  virtual bool useIdentityView(void) {
    return false;
  }

  /** Returns the camera-relative squared depth of this renderable.
  @remarks
  	Used to sort transparent objects. Squared depth is used rather than
  	actual depth to avoid having to perform a square root on the result.
  */
  virtual Real getSquaredViewDepth(const Camera* cam) const = 0;

  /** Returns the preferred rasterisation mode of this renderable.
  */
  virtual SceneDetailLevel getRenderDetail() {
    return SDL_SOLID;
  }

};



}

#endif //__Renderable_H__
