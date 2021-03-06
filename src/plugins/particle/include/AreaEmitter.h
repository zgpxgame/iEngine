/*
-----------------------------------------------------------------------------
This source file is part of OGRE
    (Object-oriented Graphics Rendering Engine)
For the latest info, see http://ogre.sourceforge.net/

Copyright ) 2000-2002 The OGRE Team
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
#ifndef __AreaEmitter_H__
#define __AreaEmitter_H__

#include "ParticleFXPrerequisites.h"
#include "ParticleEmitter.h"

namespace renderer {

/** Particle emitter which emits particles randomly from points inside
    an area (box, sphere, ellipsoid whatever subclasses choose to be).
@remarks
    This is an empty superclass and needs to be subclassed. Basic particle
    emitter emits particles from/in an (unspecified) area. The
    initial direction of these particles can either be a single direction
    (i.e. a line), a random scattering inside a cone, or a random
    scattering in all directions, depending the 'angle' parameter, which
    is the angle across which to scatter the particles either side of the
    base direction of the emitter.
*/
class _ParticleFXExport AreaEmitter : public ParticleEmitter {
public:
  /** Command object for area emitter size (see ParamCommand).*/
  class CmdWidth : public ParamCommand {
  public:
    String doGet(void* target);
    void doSet(void* target, const String& val);
  };
  /** Command object for area emitter size (see ParamCommand).*/
  class CmdHeight : public ParamCommand {
  public:
    String doGet(void* target);
    void doSet(void* target, const String& val);
  };
  /** Command object for area emitter size (see ParamCommand).*/
  class CmdDepth : public ParamCommand {
  public:
    String doGet(void* target);
    void doSet(void* target, const String& val);
  };


  AreaEmitter() {}


  /** See ParticleEmitter. */
  unsigned short _getEmissionCount(Real timeElapsed);

  /** Overloaded to update the trans. matrix */
  void setDirection( const Vector3& direction );

  /** Sets the size of the area from which particles are emitted.
  @param
      size Vector describing the size of the area. The area extends
      around the center point by half the x, y and z components of
      this vector. The box is aligned such that it's local Z axis points
      along it's direction (see setDirection)
  */
  void setSize(const Vector3& size);

  /** Sets the size of the area from which particles are emitted.
  @param x,y,z
      Individual axis lengths describing the size of the area. The area
      extends around the center point by half the x, y and z components
      of this vector. The box is aligned such that it's local Z axis
      points along it's direction (see setDirection)
  */
  void setSize(Real x, Real y, Real z);
  /** Sets the size of the clear space inside the area from where NO particles are emitted.
  @param x,y,z
      Individual axis lengths describing the size of the clear space.
      The clear space is aligned like the outer area.
      (see setDirection and setSize)
  */
//        void setClearSpace(Real x, Real y, Real z);

  /** Sets the width (local x size) of the emitter. */
  void setWidth(Real width);
  /** Gets the width (local x size) of the emitter. */
  Real getWidth(void);
  /** Sets the height (local y size) of the emitter. */
  void setHeight(Real Height);
  /** Gets the height (local y size) of the emitter. */
  Real getHeight(void);
  /** Sets the depth (local y size) of the emitter. */
  void setDepth(Real Depth);
  /** Gets the depth (local y size) of the emitter. */
  Real getDepth(void);

protected:
  /// Size of the area
  Vector3 mSize;

  /// Local axes, not normalised, their magnitude reflects area size
  Vector3 mXRange, mYRange, mZRange;

  /// Internal method for generating the area axes
  void genAreaAxes(void);
  /// Internal for initializing some defaults and parameters
  void initDefaults(String mType);

  /// Command objects
  static CmdWidth msWidthCmd;
  static CmdHeight msHeightCmd;
  static CmdDepth msDepthCmd;



};

}

#endif

