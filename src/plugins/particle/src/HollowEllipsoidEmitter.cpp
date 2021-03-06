/*
-----------------------------------------------------------------------------
This source file is part of OGRE
    (Object-oriented Graphics Rendering Engine)
For the latest info, see http://ogre.sourceforge.net/

Copyright ) 2002 Tels <http://bloodgate.com> based on BoxEmitter
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
#include "HollowEllipsoidEmitter.h"
#include "Particle.h"
#include "Exception.h"
#include "StringConverter.h"
#include "Math.h"

/* Implements an Emitter whose emitting points all lie inside an ellipsoid.
   See <http://mathworld.wolfram.com/Ellipsoid.html> for mathematical details.

  If the lengths of two axes of an ellipsoid are the same, the figure is
  called a 'spheroid' (depending on whether c < a or c > a, an 'oblate
  spheroid' or 'prolate spheroid', respectively), and if all three are the
  same, it is a 'sphere' (ball).
*/

namespace renderer {

HollowEllipsoidEmitter::CmdInnerX HollowEllipsoidEmitter::msCmdInnerX;
HollowEllipsoidEmitter::CmdInnerY HollowEllipsoidEmitter::msCmdInnerY;
HollowEllipsoidEmitter::CmdInnerZ HollowEllipsoidEmitter::msCmdInnerZ;


//-----------------------------------------------------------------------
HollowEllipsoidEmitter::HollowEllipsoidEmitter() {
  initDefaults("HollowEllipsoid");
  // Add custom parameters
  ParamDictionary* pDict = getParamDictionary();

  pDict->addParameter(ParameterDef("inner_width", "Parametric value describing the proportion of the "
                                   "shape which is hollow.", PT_REAL), &msCmdInnerX);
  pDict->addParameter(ParameterDef("inner_height", "Parametric value describing the proportion of the "
                                   "shape which is hollow.", PT_REAL), &msCmdInnerY);
  pDict->addParameter(ParameterDef("inner_depth", "Parametric value describing the proportion of the "
                                   "shape which is hollow.", PT_REAL), &msCmdInnerZ);

  // default is half empty
  setInnerSize(0.5,0.5,0.5);
}
//-----------------------------------------------------------------------
void HollowEllipsoidEmitter::_initParticle(Particle* pParticle) {
  Real alpha, beta, a, b, c, x, y, z;

  // create two random angles alpha and beta
  // with these two angles, we are able to select any point on an
  // ellipsoid's surface
  alpha = Math::RangeRandom(0,Math::TWO_PI);
  beta  = Math::RangeRandom(0,Math::PI);

  // create three random radius values that are bigger than the inner
  // size, but smaller/equal than/to the outer size 1.0 (inner size is
  // between 0 and 1)
  a = Math::RangeRandom(mInnerSize.x,1.0);
  b = Math::RangeRandom(mInnerSize.y,1.0);
  c = Math::RangeRandom(mInnerSize.z,1.0);

  // with a,b,c we have defined a random ellipsoid between the inner
  // ellipsoid and the outer sphere (radius 1.0)
  // with alpha and beta we select on point on this random ellipsoid
  // and calculate the 3D coordinates of this point
  x = a * Math::Cos(alpha) * Math::Sin(beta);
  y = b * Math::Sin(alpha) * Math::Sin(beta);
  z = c * Math::Cos(beta);

  // scale the found point to the ellipsoid's size and move it
  // relatively to the center of the emitter point

  pParticle->mPosition = mPosition +
                         + x * mXRange + y * mYRange + z * mZRange;

  // Generate complex data by reference
  genEmissionColour(pParticle->mColour);
  genEmissionDirection(pParticle->mDirection);
  genEmissionVelocity(pParticle->mDirection);

  // Generate simpler data
  pParticle->mTimeToLive = genEmissionTTL();

}
//-----------------------------------------------------------------------
void HollowEllipsoidEmitter::setInnerSize(Real x, Real y, Real z) {
  assert((x > 0) && (x < 1.0) &&
         (y > 0) && (y < 1.0) &&
         (z > 0) && (z < 1.0));

  mInnerSize.x = x;
  mInnerSize.y = y;
  mInnerSize.z = z;
}
//-----------------------------------------------------------------------
void HollowEllipsoidEmitter::setInnerSizeX(Real x) {
  assert(x > 0 && x < 1.0);

  mInnerSize.x = x;
}
//-----------------------------------------------------------------------
void HollowEllipsoidEmitter::setInnerSizeY(Real y) {
  assert(y > 0 && y < 1.0);

  mInnerSize.y = y;
}
//-----------------------------------------------------------------------
void HollowEllipsoidEmitter::setInnerSizeZ(Real z) {
  assert(z > 0 && z < 1.0);

  mInnerSize.z = z;
}
//-----------------------------------------------------------------------
Real HollowEllipsoidEmitter::getInnerSizeX(void) {
  return mInnerSize.x;
}
//-----------------------------------------------------------------------
Real HollowEllipsoidEmitter::getInnerSizeY(void) {
  return mInnerSize.y;
}
//-----------------------------------------------------------------------
Real HollowEllipsoidEmitter::getInnerSizeZ(void) {
  return mInnerSize.z;
}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
// Command objects
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
String HollowEllipsoidEmitter::CmdInnerX::doGet(void* target) {
  return StringConverter::toString(
           static_cast<HollowEllipsoidEmitter*>(target)->getInnerSizeX() );
}
void HollowEllipsoidEmitter::CmdInnerX::doSet(void* target, const String& val) {
  static_cast<HollowEllipsoidEmitter*>(target)->setInnerSizeX(StringConverter::parseReal(val));
}
//-----------------------------------------------------------------------
String HollowEllipsoidEmitter::CmdInnerY::doGet(void* target) {
  return StringConverter::toString(
           static_cast<HollowEllipsoidEmitter*>(target)->getInnerSizeY() );
}
void HollowEllipsoidEmitter::CmdInnerY::doSet(void* target, const String& val) {
  static_cast<HollowEllipsoidEmitter*>(target)->setInnerSizeY(StringConverter::parseReal(val));
}
//-----------------------------------------------------------------------
String HollowEllipsoidEmitter::CmdInnerZ::doGet(void* target) {
  return StringConverter::toString(
           static_cast<HollowEllipsoidEmitter*>(target)->getInnerSizeZ() );
}
void HollowEllipsoidEmitter::CmdInnerZ::doSet(void* target, const String& val) {
  static_cast<HollowEllipsoidEmitter*>(target)->setInnerSizeZ(StringConverter::parseReal(val));
}


}


