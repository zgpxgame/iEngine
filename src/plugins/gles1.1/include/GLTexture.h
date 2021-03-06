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

#ifndef __GLTEXTURE_H__
#define __GLTEXTURE_H__

#include "GLPrerequisites.h"
#include "Platform.h"
#include "RenderTexture.h"
#include "Texture.h"

namespace renderer {

class GLTexture : public Texture {
public:
  // Constructor, called from SDLTextureManager
  GLTexture( String name, TextureType texType = TEX_TYPE_2D );
  GLTexture( String name, TextureType texType, uint width, uint height,
             uint num_mips, PixelFormat format, TextureUsage usage );

  virtual ~GLTexture();

  void load();
  void loadImage( const Image &img );
  void loadImages( const std::vector<Image> images );

  void unload();

  void createRenderTexture();

  void blitToTexture( const Image& src,
                      unsigned uStartX, unsigned uStartY );

  GLenum getGLTextureType(void) const;

  GLuint getGLID() const {
    return mTextureID;
  }

protected:
  void generateMipMaps( uchar *data, bool useSoftware,
                        unsigned int faceNumber );
  uchar* rescaleNPower2( const Image& src );
private:
  GLuint mTextureID;
};

class GLRenderTexture : public RenderTexture {
public:
  GLRenderTexture(const String& name, uint width, uint height)
    : RenderTexture(name, width, height) {
  }

  void _copyToTexture(void);

  bool requiresTextureFlipping() const {
    return false;
  }
  virtual void writeContentsToFile( const String & filename ) {}
  virtual void outputText(int x, int y, const String& text) {}
};
}

#endif // __GLTEXTURE_H__
