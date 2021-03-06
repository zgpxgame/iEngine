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
#include "EllipsoidEmitter.h"
#include "Particle.h"
#include "Exception.h"
#include "StringConverter.h"


/* Implements an Emitter whose emitting points all lie inside an ellipsoid.
   See <http://mathworld.wolfram.com/Ellipsoid.html> for mathematical details.

  If the lengths of two axes of an ellipsoid are the same, the figure is
  called a 'spheroid' (depending on whether c < a or c > a, an 'oblate
  spheroid' or 'prolate spheroid', respectively), and if all three are the
  same, it is a 'sphere' (ball).
*/

namespace renderer {


//-----------------------------------------------------------------------
EllipsoidEmitter::EllipsoidEmitter() {
  initDefaults("Ellipsoid");
}
//-----------------------------------------------------------------------
void EllipsoidEmitter::_initParticle(Particle* pParticle) {
  Real x, y, z;

  // First we create a random point inside a bounding sphere with a
  // radius of 1 (this is easy to do). The distance of the point from
  // 0,0,0 must be <= 1 (== 1 means on the surface and we count this as
  // inside, too).

  while (true) {
    // three random values for one random point in 3D space

    x = Math::SymmetricRandom();
    y = Math::SymmetricRandom();
    z = Math::SymmetricRandom();

    // the distance of x,y,z from 0,0,0 is sqrt(x*x+y*y+z*z), but
    // as usual we can omit the sqrt(), since sqrt(1) == 1 and we
    // use the 1 as boundary:
    if ( x*x + y*y + z*z <= 1) {
      break;          // found one valid point inside
    }
  }

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

}


