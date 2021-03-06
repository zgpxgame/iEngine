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
#include "BoxEmitter.h"
#include "Particle.h"
#include "Exception.h"
#include "StringConverter.h"



namespace renderer {


//-----------------------------------------------------------------------
BoxEmitter::BoxEmitter() {
  initDefaults("Box");
}
//-----------------------------------------------------------------------
void BoxEmitter::_initParticle(Particle* pParticle) {
  Vector3 xOff, yOff, zOff;

  xOff = Math::SymmetricRandom() * mXRange;
  yOff = Math::SymmetricRandom() * mYRange;
  zOff = Math::SymmetricRandom() * mZRange;

  pParticle->mPosition = mPosition + xOff + yOff + zOff;


  // Generate complex data by reference
  genEmissionColour(pParticle->mColour);
  genEmissionDirection(pParticle->mDirection);
  genEmissionVelocity(pParticle->mDirection);

  // Generate simpler data
  pParticle->mTimeToLive = genEmissionTTL();

}


}


