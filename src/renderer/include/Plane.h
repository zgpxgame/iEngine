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
// Original free version by:
// Magic Software, Inc.
// http://www.magic-software.com
// Copyright (c) 2000, All Rights Reserved

#ifndef __Plane_H__
#define __Plane_H__

#include "Prerequisites.h"

#include "Vector3.h"

namespace renderer {

/** Defines a plane in 3D space.
    @remarks
        A plane is defined in 3D space by the equation
        Ax + By + Cz + D = 0
    @par
        This equates to a vector (the normal of the plane, whose x, y
        and z components equate to the coefficients A, B and C
        respectively), and a constant (D) which is the distance along
        the normal you have to go to move the plane back to the origin.
 */
class _RendererExport Plane {
public:
  /** Default constructor - sets everything to 0.
  */
  Plane ();
  Plane (const Plane& rhs);
  Plane (const Vector3& rkNormal, Real fConstant);
  Plane (const Vector3& rkNormal, const Vector3& rkPoint);
  Plane (const Vector3& rkPoint0, const Vector3& rkPoint1,
         const Vector3& rkPoint2);

  /** The "positive side" of the plane is the half space to which the
      plane normal points. The "negative side" is the other half
      space. The flag "no side" indicates the plane itself.
  */
  enum Side {
    NO_SIDE,
    POSITIVE_SIDE,
    NEGATIVE_SIDE
  };

  Side getSide (const Vector3& rkPoint) const;

  /** This is a pseudodistance. The sign of the return value is
      positive if the point is on the positive side of the plane,
      negative if the point is on the negative side, and zero if the
      point is on the plane.
      @par
      The absolute value of the return value is the true distance only
      when the plane normal is a unit length vector.
  */
  Real getDistance (const Vector3& rkPoint) const;

  Vector3 normal;
  Real d;

  _RendererExport friend std::ostream& operator<< (std::ostream& o, Plane& p);
};


} // namespace Ogre

#endif
