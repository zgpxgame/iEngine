/*
-----------------------------------------------------------------------------
This source file is part of OGRE
    (Object-oriented Graphics Rendering Engine)
For the latest info, see http://ogre.sourceforge.net/

Copyright 2000-2002 The OGRE Team
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
/*
-----------------------------------------------------------------------------
Filename:    ExampleApplication.h
Description: Base class for all the OGRE examples
-----------------------------------------------------------------------------
*/

#ifndef __ExampleApplication_H__
#define __ExampleApplication_H__

#include "RenderEngine.h"
#include "ConfigFile.h"
#include "ExampleFrameListener.h"

using namespace renderer;

/** Base class which manages the standard startup of an Ogre application.
    Designed to be subclassed for specific examples if required.
*/
class ExampleApplication {
public:
  /// Standard constructor
  ExampleApplication() {
    mFrameListener = 0;
    mRoot = 0;
  }
  /// Standard destructor
  virtual ~ExampleApplication() {
    if (mFrameListener)
      delete mFrameListener;
    if (mRoot)
      delete mRoot;
  }

  /// Start the example
  virtual void go(void) {
    if (!setup())
      return;

    mRoot->startRendering();
  }

protected:
  Root *mRoot;
  Camera* mCamera;
  SceneManager* mSceneMgr;
  FrameListener* mFrameListener;
  RenderWindow* mWindow;

  // These internal methods package up the stages in the startup process
  /** Sets up the application - returns false if the user chooses to abandon configuration. */
  virtual bool setup(void) {
    mRoot = new Root();

    setupResources();

    bool carryOn = configure();
    if (!carryOn) return false;

    chooseSceneManager();
    createCamera();
    createViewports();

    // Set default mipmap level (NB some APIs ignore this)
    TextureManager::getSingleton().setDefaultNumMipMaps(5);

    // Create the scene
    createScene();

    createFrameListener();

    return true;

  }




};


#endif
