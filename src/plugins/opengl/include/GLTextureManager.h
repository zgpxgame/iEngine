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

#ifndef __SDLTextureManager_H__
#define __SDLTextureManager_H__

#include "GLPrerequisites.h"
#include "TextureManager.h"
#include "GLTexture.h"

namespace renderer {
/** SDL/GL-specific implementation of a TextureManager */
class GLTextureManager : public TextureManager {
public:
  GLTextureManager();
  virtual ~GLTextureManager();

  /** Creates a SDLTexture resource.
  */
  virtual Texture* create( const String& name, TextureType texType);
  /** Note that this function is not yet implemented.
      @todo
          Implement correct surface creation in the texture source file.
  */
  virtual Texture * createAsRenderTarget( const String& name ) {
    return NULL;
  }

  virtual Texture * createManual(const String& name, TextureType texType,
    uint width, uint height, uint num_mips, PixelFormat format, TextureUsage usage);

  /** Unloads & destroys textures. */
  void unloadAndDestroyAll();
};
}
#endif
