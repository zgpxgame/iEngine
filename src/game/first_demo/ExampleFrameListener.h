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
Filename:    ExampleFrameListener.h
Description: Defines an example frame listener which responds to frame events.
             This frame listener just moves a specified camera around based on
             keyboard and mouse movements.
             Mouse:    Freelook
             W or Up:  Forward
             S or Down:Backward
             A:           Step left
             D:        Step right
             PgUp:     Move upwards
             PgDown:   Move downwards
             O/P:       Yaw the root scene node (and it's children)
             I/K:       Pitch the root scene node (and it's children)
             F:           Toggle frame rate stats on/off
-----------------------------------------------------------------------------
*/

#ifndef __ExampleFrameListener_H__
#define __ExampleFrameListener_H__

#include "RenderEngine.h"

using namespace renderer;

class ExampleFrameListener: public FrameListener {
public:
  // Constructor takes a RenderWindow because it uses that to determine input context
  ExampleFrameListener(RenderWindow* win, Camera* cam) {
    mCamera = cam;
    mWindow = win;
    mNumScreenShots = 0;
    mTimeUntilNextToggle = 0;
  }
  virtual ~ExampleFrameListener() {
  }

  virtual bool frameStarted(const FrameEvent& evt) {
    return true;
  }

  virtual bool frameEnded(const FrameEvent& evt) {
    return true;
  }


  void moveCamera() {

    // Make all the changes to the camera
    // Note that YAW direction is around a fixed axis (freelook style) rather than a natural YAW (e.g. airplane)
    mCamera->yaw(mRotX);
    mCamera->pitch(mRotY);
    mCamera->moveRelative(mTranslateVector);

  }

protected:
  Camera* mCamera;
  Vector3 mTranslateVector;
  RenderWindow* mWindow;
  unsigned int mNumScreenShots;
  float mMoveScale;
  float mRotScale;
  // just to stop toggles flipping too fast
  Real mTimeUntilNextToggle ;
  float mRotX, mRotY;

};

#endif
