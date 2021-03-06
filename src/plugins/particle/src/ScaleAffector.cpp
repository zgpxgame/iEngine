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
#include "ScaleAffector.h"
#include "ParticleSystem.h"
#include "StringConverter.h"
#include "Particle.h"


namespace renderer {

// init statics
ScaleAffector::CmdScaleAdjust ScaleAffector::msScaleCmd;

//-----------------------------------------------------------------------
ScaleAffector::ScaleAffector() {
  mScaleAdj = 0;
  mType = "Scaler";

  // Init parameters
  if (createParamDictionary("ScaleAffector")) {
    ParamDictionary* dict = getParamDictionary();

    dict->addParameter(ParameterDef("rate",
                                    "The amount by which to adjust the x and y scale components of particles per second.",
                                    PT_REAL), &msScaleCmd);
  }
}
//-----------------------------------------------------------------------
void ScaleAffector::_affectParticles(ParticleSystem* pSystem, Real timeElapsed) {
  ParticleIterator pi = pSystem->_getIterator();
  Particle *p;
  Real ds;

  // Scale adjustments by time
  ds = mScaleAdj * timeElapsed;

  Real NewWide, NewHigh;

  while (!pi.end()) {
    p = pi.getNext();

    if( p->hasOwnDimensions() == false ) {
      p->setDimensions( pSystem->getDefaultWidth() , pSystem->getDefaultHeight() );
    } else {
      NewWide = p->getOwnWidth()  + ds;
      NewHigh = p->getOwnHeight() + ds;

      p->setDimensions( NewWide, NewHigh );
    }
  }

}
//-----------------------------------------------------------------------
void ScaleAffector::setAdjust( Real rate ) {
  mScaleAdj = rate;
}
//-----------------------------------------------------------------------
Real ScaleAffector::getAdjust(void) {
  return mScaleAdj;
}
//-----------------------------------------------------------------------

//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
// Command objects
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
String ScaleAffector::CmdScaleAdjust::doGet(void* target) {
  return StringConverter::toString(
           static_cast<ScaleAffector*>(target)->getAdjust() );
}
void ScaleAffector::CmdScaleAdjust::doSet(void* target, const String& val) {
  static_cast<ScaleAffector*>(target)->setAdjust(
    StringConverter::parseReal(val));
}

}



