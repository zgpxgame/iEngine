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
#include "ColourFaderAffector2.h"
#include "ParticleSystem.h"
#include "StringConverter.h"
#include "Particle.h"


namespace renderer {

// init statics
// Phase 1
ColourFaderAffector2::CmdRedAdjust1 ColourFaderAffector2::msRedCmd1;
ColourFaderAffector2::CmdGreenAdjust1 ColourFaderAffector2::msGreenCmd1;
ColourFaderAffector2::CmdBlueAdjust1 ColourFaderAffector2::msBlueCmd1;
ColourFaderAffector2::CmdAlphaAdjust1 ColourFaderAffector2::msAlphaCmd1;

// Phase 2
ColourFaderAffector2::CmdRedAdjust2 ColourFaderAffector2::msRedCmd2;
ColourFaderAffector2::CmdGreenAdjust2 ColourFaderAffector2::msGreenCmd2;
ColourFaderAffector2::CmdBlueAdjust2 ColourFaderAffector2::msBlueCmd2;
ColourFaderAffector2::CmdAlphaAdjust2 ColourFaderAffector2::msAlphaCmd2;

ColourFaderAffector2::CmdStateChange ColourFaderAffector2::msStateCmd;


//-----------------------------------------------------------------------
ColourFaderAffector2::ColourFaderAffector2() {
  mRedAdj1 = mGreenAdj1 = mBlueAdj1 = mAlphaAdj1 = 0;
  mRedAdj2 = mGreenAdj2 = mBlueAdj2 = mAlphaAdj2 = 0;
  mType = "ColourFader2";
  StateChangeVal = 1;	// Switch when there is 1 second left on the TTL

  // Init parameters
  if (createParamDictionary("ColourFaderAffector2")) {
    ParamDictionary* dict = getParamDictionary();

    // Phase 1
    dict->addParameter(ParameterDef("red1",
                                    "The amount by which to adjust the red component of particles per second.",
                                    PT_REAL), &msRedCmd1);
    dict->addParameter(ParameterDef("green1",
                                    "The amount by which to adjust the green component of particles per second.",
                                    PT_REAL), &msGreenCmd1);
    dict->addParameter(ParameterDef("blue1",
                                    "The amount by which to adjust the blue component of particles per second.",
                                    PT_REAL), &msBlueCmd1);
    dict->addParameter(ParameterDef("alpha1",
                                    "The amount by which to adjust the alpha component of particles per second.",
                                    PT_REAL), &msAlphaCmd1);

    // Phase 2
    dict->addParameter(ParameterDef("red2",
                                    "The amount by which to adjust the red component of particles per second.",
                                    PT_REAL), &msRedCmd2);
    dict->addParameter(ParameterDef("green2",
                                    "The amount by which to adjust the green component of particles per second.",
                                    PT_REAL), &msGreenCmd2);
    dict->addParameter(ParameterDef("blue2",
                                    "The amount by which to adjust the blue component of particles per second.",
                                    PT_REAL), &msBlueCmd2);
    dict->addParameter(ParameterDef("alpha2",
                                    "The amount by which to adjust the alpha component of particles per second.",
                                    PT_REAL), &msAlphaCmd2);

    // State Change Value
    dict->addParameter(ParameterDef("state_change",
                                    "When the particle has this much time to live left, it will switch to state 2.",
                                    PT_REAL), &msStateCmd);

  }
}
//-----------------------------------------------------------------------
void ColourFaderAffector2::_affectParticles(ParticleSystem* pSystem, Real timeElapsed) {
  ParticleIterator pi = pSystem->_getIterator();
  Particle *p;
  Real dr1, dg1, db1, da1;
  Real dr2, dg2, db2, da2;

  // Scale adjustments by time
  dr1 = mRedAdj1   * timeElapsed;
  dg1 = mGreenAdj1 * timeElapsed;
  db1 = mBlueAdj1  * timeElapsed;
  da1 = mAlphaAdj1 * timeElapsed;

  // Scale adjustments by time
  dr2 = mRedAdj2   * timeElapsed;
  dg2 = mGreenAdj2 * timeElapsed;
  db2 = mBlueAdj2  * timeElapsed;
  da2 = mAlphaAdj2 * timeElapsed;

  while (!pi.end()) {
    p = pi.getNext();

    if( p->mTimeToLive > StateChangeVal ) {
      applyAdjustWithClamp(&p->mColour.r, dr1);
      applyAdjustWithClamp(&p->mColour.g, dg1);
      applyAdjustWithClamp(&p->mColour.b, db1);
      applyAdjustWithClamp(&p->mColour.a, da1);
    } else {
      applyAdjustWithClamp(&p->mColour.r, dr2);
      applyAdjustWithClamp(&p->mColour.g, dg2);
      applyAdjustWithClamp(&p->mColour.b, db2);
      applyAdjustWithClamp(&p->mColour.a, da2);
    }
  }

}
//-----------------------------------------------------------------------
void ColourFaderAffector2::setAdjust1(Real red, Real green, Real blue, Real alpha) {
  mRedAdj1 = red;
  mGreenAdj1 = green;
  mBlueAdj1 = blue;
  mAlphaAdj1 = alpha;
}
//-----------------------------------------------------------------------
void ColourFaderAffector2::setAdjust2(Real red, Real green, Real blue, Real alpha) {
  mRedAdj2 = red;
  mGreenAdj2 = green;
  mBlueAdj2 = blue;
  mAlphaAdj2 = alpha;
}

//-----------------------------------------------------------------------
void ColourFaderAffector2::setRedAdjust1(Real red) {
  mRedAdj1 = red;
}
//-----------------------------------------------------------------------
void ColourFaderAffector2::setRedAdjust2(Real red) {
  mRedAdj2 = red;
}
//-----------------------------------------------------------------------
Real ColourFaderAffector2::getRedAdjust1(void) {
  return mRedAdj1;
}
//-----------------------------------------------------------------------
Real ColourFaderAffector2::getRedAdjust2(void) {
  return mRedAdj2;
}
//-----------------------------------------------------------------------
void ColourFaderAffector2::setGreenAdjust1(Real green) {
  mGreenAdj1 = green;
}
//-----------------------------------------------------------------------
void ColourFaderAffector2::setGreenAdjust2(Real green) {
  mGreenAdj2 = green;
}
//-----------------------------------------------------------------------
Real ColourFaderAffector2::getGreenAdjust1(void) {
  return mGreenAdj1;
}
//-----------------------------------------------------------------------
Real ColourFaderAffector2::getGreenAdjust2(void) {
  return mGreenAdj2;
}
//-----------------------------------------------------------------------
void ColourFaderAffector2::setBlueAdjust1(Real blue) {
  mBlueAdj1 = blue;
}
//-----------------------------------------------------------------------
void ColourFaderAffector2::setBlueAdjust2(Real blue) {
  mBlueAdj2 = blue;
}
//-----------------------------------------------------------------------
Real ColourFaderAffector2::getBlueAdjust1(void) {
  return mBlueAdj1;
}
//-----------------------------------------------------------------------
Real ColourFaderAffector2::getBlueAdjust2(void) {
  return mBlueAdj2;
}
//-----------------------------------------------------------------------
void ColourFaderAffector2::setAlphaAdjust1(Real alpha) {
  mAlphaAdj1 = alpha;
}
//-----------------------------------------------------------------------
void ColourFaderAffector2::setAlphaAdjust2(Real alpha) {
  mAlphaAdj2 = alpha;
}
//-----------------------------------------------------------------------
Real ColourFaderAffector2::getAlphaAdjust1(void) {
  return mAlphaAdj1;
}
//-----------------------------------------------------------------------
Real ColourFaderAffector2::getAlphaAdjust2(void) {
  return mAlphaAdj1;
}
//-----------------------------------------------------------------------
void ColourFaderAffector2::setStateChange(Real NewValue) {
  StateChangeVal = NewValue;
}
//-----------------------------------------------------------------------
Real ColourFaderAffector2::getStateChange(void) {
  return StateChangeVal;
}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
// Command objects
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
String ColourFaderAffector2::CmdRedAdjust1::doGet(void* target) {
  return StringConverter::toString(
           static_cast<ColourFaderAffector2*>(target)->getRedAdjust1() );
}
void ColourFaderAffector2::CmdRedAdjust1::doSet(void* target, const String& val) {
  static_cast<ColourFaderAffector2*>(target)->setRedAdjust1(
    StringConverter::parseReal(val));
}
String ColourFaderAffector2::CmdRedAdjust2::doGet(void* target) {
  return StringConverter::toString(
           static_cast<ColourFaderAffector2*>(target)->getRedAdjust2() );
}
void ColourFaderAffector2::CmdRedAdjust2::doSet(void* target, const String& val) {
  static_cast<ColourFaderAffector2*>(target)->setRedAdjust2(
    StringConverter::parseReal(val));
}
//-----------------------------------------------------------------------
String ColourFaderAffector2::CmdGreenAdjust1::doGet(void* target) {
  return StringConverter::toString(
           static_cast<ColourFaderAffector2*>(target)->getGreenAdjust1() );
}
void ColourFaderAffector2::CmdGreenAdjust1::doSet(void* target, const String& val) {
  static_cast<ColourFaderAffector2*>(target)->setGreenAdjust1(
    StringConverter::parseReal(val));
}
String ColourFaderAffector2::CmdGreenAdjust2::doGet(void* target) {
  return StringConverter::toString(
           static_cast<ColourFaderAffector2*>(target)->getGreenAdjust2() );
}
void ColourFaderAffector2::CmdGreenAdjust2::doSet(void* target, const String& val) {
  static_cast<ColourFaderAffector2*>(target)->setGreenAdjust2(
    StringConverter::parseReal(val));
}
//-----------------------------------------------------------------------
String ColourFaderAffector2::CmdBlueAdjust1::doGet(void* target) {
  return StringConverter::toString(
           static_cast<ColourFaderAffector2*>(target)->getBlueAdjust1() );
}
void ColourFaderAffector2::CmdBlueAdjust1::doSet(void* target, const String& val) {
  static_cast<ColourFaderAffector2*>(target)->setBlueAdjust1(
    StringConverter::parseReal(val));
}
String ColourFaderAffector2::CmdBlueAdjust2::doGet(void* target) {
  return StringConverter::toString(
           static_cast<ColourFaderAffector2*>(target)->getBlueAdjust2() );
}
void ColourFaderAffector2::CmdBlueAdjust2::doSet(void* target, const String& val) {
  static_cast<ColourFaderAffector2*>(target)->setBlueAdjust2(
    StringConverter::parseReal(val));
}
//-----------------------------------------------------------------------
String ColourFaderAffector2::CmdAlphaAdjust1::doGet(void* target) {
  return StringConverter::toString(
           static_cast<ColourFaderAffector2*>(target)->getAlphaAdjust1() );
}
void ColourFaderAffector2::CmdAlphaAdjust1::doSet(void* target, const String& val) {
  static_cast<ColourFaderAffector2*>(target)->setAlphaAdjust1(
    StringConverter::parseReal(val));
}
String ColourFaderAffector2::CmdAlphaAdjust2::doGet(void* target) {
  return StringConverter::toString(
           static_cast<ColourFaderAffector2*>(target)->getAlphaAdjust2() );
}
void ColourFaderAffector2::CmdAlphaAdjust2::doSet(void* target, const String& val) {
  static_cast<ColourFaderAffector2*>(target)->setAlphaAdjust2(
    StringConverter::parseReal(val));
}
//-----------------------------------------------------------------------
String ColourFaderAffector2::CmdStateChange::doGet(void* target) {
  return StringConverter::toString(
           static_cast<ColourFaderAffector2*>(target)->getStateChange() );
}
void ColourFaderAffector2::CmdStateChange::doSet(void* target, const String& val) {
  static_cast<ColourFaderAffector2*>(target)->setStateChange(
    StringConverter::parseReal(val));
}
}



