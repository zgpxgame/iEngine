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

#include "Root.h"
#include "ParticleSystemManager.h"
#include "ParticleAffectorFactory.h"
#include "ParticleEmitterFactory.h"

#include "PointEmitterFactory.h"
#include "BoxEmitterFactory.h"
#include "EllipsoidEmitterFactory.h"
#include "HollowEllipsoidEmitterFactory.h"
#include "RingEmitterFactory.h"
#include "CylinderEmitterFactory.h"
#include "LinearForceAffectorFactory.h"
#include "ColourFaderAffectorFactory.h"
#include "ColourFaderAffectorFactory2.h"
#include "ScaleAffectorFactory.h"

#define EXPORT __declspec(dllexport)

namespace renderer {

std::vector<ParticleEmitterFactory*> emitterFactories;
std::vector<ParticleAffectorFactory*> affectorFactories;

//-----------------------------------------------------------------------
void registerParticleFactories(void) {
  // -- Create all new particle emitter factories --
  ParticleEmitterFactory* pEmitFact;

  // PointEmitter
  pEmitFact = new PointEmitterFactory();
  ParticleSystemManager::getSingleton().addEmitterFactory(pEmitFact);
  emitterFactories.push_back(pEmitFact);

  // BoxEmitter
  pEmitFact = new BoxEmitterFactory();
  ParticleSystemManager::getSingleton().addEmitterFactory(pEmitFact);
  emitterFactories.push_back(pEmitFact);

  // EllipsoidEmitter
  pEmitFact = new EllipsoidEmitterFactory();
  ParticleSystemManager::getSingleton().addEmitterFactory(pEmitFact);
  emitterFactories.push_back(pEmitFact);

  // CylinderEmitter
  pEmitFact = new CylinderEmitterFactory();
  ParticleSystemManager::getSingleton().addEmitterFactory(pEmitFact);
  emitterFactories.push_back(pEmitFact);

  // RingEmitter
  pEmitFact = new RingEmitterFactory();
  ParticleSystemManager::getSingleton().addEmitterFactory(pEmitFact);
  emitterFactories.push_back(pEmitFact);

  // HollowEllipsoidEmitter
  pEmitFact = new HollowEllipsoidEmitterFactory();
  ParticleSystemManager::getSingleton().addEmitterFactory(pEmitFact);
  emitterFactories.push_back(pEmitFact);

  // -- Create all new particle affector factories --
  ParticleAffectorFactory* pAffFact;

  // LinearForceAffector
  pAffFact = new LinearForceAffectorFactory();
  ParticleSystemManager::getSingleton().addAffectorFactory(pAffFact);
  affectorFactories.push_back(pAffFact);

  // ColourFaderAffector
  pAffFact = new ColourFaderAffectorFactory();
  ParticleSystemManager::getSingleton().addAffectorFactory(pAffFact);
  affectorFactories.push_back(pAffFact);

  // ColourFaderAffector
  pAffFact = new ColourFaderAffectorFactory2();
  ParticleSystemManager::getSingleton().addAffectorFactory(pAffFact);
  affectorFactories.push_back(pAffFact);

  // ScaleAffector
  pAffFact = new ScaleAffectorFactory();
  ParticleSystemManager::getSingleton().addAffectorFactory(pAffFact);
  affectorFactories.push_back(pAffFact);

}
//-----------------------------------------------------------------------
void destroyParticleFactories(void) {
  std::vector<ParticleEmitterFactory*>::iterator ei;
  std::vector<ParticleAffectorFactory*>::iterator ai;

  for (ei = emitterFactories.begin(); ei != emitterFactories.end(); ++ei) {
    delete (*ei);
  }

  for (ai = affectorFactories.begin(); ai != affectorFactories.end(); ++ai) {
    delete (*ai);
  }


}
//-----------------------------------------------------------------------
extern "C" EXPORT void dllStartPlugin(void) throw() {
  // Particle SFX
  registerParticleFactories();
}

//-----------------------------------------------------------------------
extern "C" EXPORT void dllStopPlugin(void) {
  // Particle SFX
  destroyParticleFactories();

}


}

