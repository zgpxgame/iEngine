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
#include "Matrix4.h"

#include "Vector3.h"
#include "Matrix3.h"

namespace renderer {

const Matrix4 Matrix4::ZERO(
  0, 0, 0, 0,
  0, 0, 0, 0,
  0, 0, 0, 0,
  0, 0, 0, 0 );

const Matrix4 Matrix4::IDENTITY(
  1, 0, 0, 0,
  0, 1, 0, 0,
  0, 0, 1, 0,
  0, 0, 0, 1 );


inline Real
MINOR(const Matrix4& m, const int r0, const int r1, const int r2, const int c0, const int c1, const int c2) {
  return m[r0][c0] * (m[r1][c1] * m[r2][c2] - m[r2][c1] * m[r1][c2]) -
         m[r0][c1] * (m[r1][c0] * m[r2][c2] - m[r2][c0] * m[r1][c2]) +
         m[r0][c2] * (m[r1][c0] * m[r2][c1] - m[r2][c0] * m[r1][c1]);
}


Matrix4 Matrix4::adjoint() const {
  return Matrix4( MINOR(*this, 1, 2, 3, 1, 2, 3),
                  -MINOR(*this, 0, 2, 3, 1, 2, 3),
                  MINOR(*this, 0, 1, 3, 1, 2, 3),
                  -MINOR(*this, 0, 1, 2, 1, 2, 3),

                  -MINOR(*this, 1, 2, 3, 0, 2, 3),
                  MINOR(*this, 0, 2, 3, 0, 2, 3),
                  -MINOR(*this, 0, 1, 3, 0, 2, 3),
                  MINOR(*this, 0, 1, 2, 0, 2, 3),

                  MINOR(*this, 1, 2, 3, 0, 1, 3),
                  -MINOR(*this, 0, 2, 3, 0, 1, 3),
                  MINOR(*this, 0, 1, 3, 0, 1, 3),
                  -MINOR(*this, 0, 1, 2, 0, 1, 3),

                  -MINOR(*this, 1, 2, 3, 0, 1, 2),
                  MINOR(*this, 0, 2, 3, 0, 1, 2),
                  -MINOR(*this, 0, 1, 3, 0, 1, 2),
                  MINOR(*this, 0, 1, 2, 0, 1, 2));
}


Real Matrix4::determinant() const {
  return m[0][0] * MINOR(*this, 1, 2, 3, 1, 2, 3) -
         m[0][1] * MINOR(*this, 1, 2, 3, 0, 2, 3) +
         m[0][2] * MINOR(*this, 1, 2, 3, 0, 1, 3) -
         m[0][3] * MINOR(*this, 1, 2, 3, 0, 1, 2);
}

Matrix4 Matrix4::inverse() const {
  return adjoint() * (1.0f / determinant());
}

}
