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
#include "RenderWindow.h"

#include "Root.h"
#include "RenderSystem.h"
#include "Viewport.h"
#include "SceneManager.h"

namespace renderer {

RenderWindow::RenderWindow()
  : RenderTarget() {
}

//-----------------------------------------------------------------------
void RenderWindow::getMetrics(int &width, int &height, int &colourDepth, int &left, int &top) {
  width = mWidth;
  height = mHeight;
  colourDepth = mColourDepth;
  left = mLeft;
  top = mTop;
}
//-----------------------------------------------------------------------
bool RenderWindow::isFullScreen(void) {
  return mIsFullScreen;
}
//-----------------------------------------------------------------------
void RenderWindow::update(void) {
  // call superclass
  RenderTarget::update();

  // Swap buffers
  swapBuffers(Root::getSingleton().getRenderSystem()->getWaitForVerticalBlank());
}

}
