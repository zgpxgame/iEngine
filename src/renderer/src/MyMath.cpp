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
#include "MyMath.h"
#include "asm_math.h"
#include "Vector3.h"
#include "Ray.h"
#include "Sphere.h"
#include "AxisAlignedBox.h"
#include "Plane.h"


namespace renderer {
template<> Math* Singleton<Math>::ms_Singleton = 0;

const Real Math::POS_INFINITY = std::numeric_limits<Real>::infinity();
const Real Math::NEG_INFINITY = -std::numeric_limits<Real>::infinity();
const Real Math::PI = Real( 4.0 * atan( 1.0 ) );
const Real Math::TWO_PI = Real( 2.0 * PI );
const Real Math::HALF_PI = Real( 0.5 * PI );

int Math::mTrigTableSize;
Math::AngleUnit Math::msAngleUnit;

Real  Math::mTrigTableFactor;
Real *Math::mSinTable = NULL;
Real *Math::mTanTable = NULL;

//-----------------------------------------------------------------------
Math::Math( unsigned int trigTableSize ) {
  msAngleUnit = AU_DEGREE;

  mTrigTableSize = trigTableSize;
  mTrigTableFactor = mTrigTableSize / Math::TWO_PI;

  mSinTable = new Real[mTrigTableSize];
  mTanTable = new Real[mTrigTableSize];

  buildTrigTables();

  // Init random number generator
  srand( (unsigned)time(0) );
}

//-----------------------------------------------------------------------
Math::~Math() {
  delete [] mSinTable;
  delete [] mTanTable;
}

//-----------------------------------------------------------------------
void Math::buildTrigTables(void) {
  // Build trig lookup tables
  // Could get away with building only PI sized Sin table but simpler this
  // way. Who cares, it'll ony use an extra 8k of memory anyway and I like
  // simplicity.
  Real angle;
  for (int i = 0; i < mTrigTableSize; ++i) {
    angle = Math::TWO_PI * i / mTrigTableSize;
    mSinTable[i] = sin(angle);
    mTanTable[i] = tan(angle);
  }
}
//-----------------------------------------------------------------------
int Math::IAbs (int iValue) {
  return ( iValue >= 0 ? iValue : -iValue );
}
//-----------------------------------------------------------------------
int Math::ICeil (float fValue) {
  return int(ceil(fValue));
}
//-----------------------------------------------------------------------
int Math::IFloor (float fValue) {
  return int(floor(fValue));
}
//-----------------------------------------------------------------------
int Math::ISign (int iValue) {
  return ( iValue > 0 ? +1 : ( iValue < 0 ? -1 : 0 ) );
}
//-----------------------------------------------------------------------
Real Math::Abs (Real fValue) {
  return Real(fabs(fValue));
}
//-----------------------------------------------------------------------
Real Math::ACos (Real fValue) {
  if ( -1.0 < fValue ) {
    if ( fValue < 1.0 )
      return Real(asm_arccos(fValue));
    else
      return 0.0;
  } else {
    return PI;
  }
}
//-----------------------------------------------------------------------
Real Math::ASin (Real fValue) {
  if ( -1.0 < fValue ) {
    if ( fValue < 1.0 )
      return Real(asm_arcsin(fValue));
    else
      return -HALF_PI;
  } else {
    return HALF_PI;
  }
}
//-----------------------------------------------------------------------
Real Math::ATan (Real fValue) {
  return Real(asm_arctan(fValue));
}
//-----------------------------------------------------------------------
Real Math::ATan2 (Real fY, Real fX) {
  return Real(atan2(fY,fX));
}
//-----------------------------------------------------------------------
Real Math::Ceil (Real fValue) {
  return Real(ceil(fValue));
}
//-----------------------------------------------------------------------
Real Math::Cos (Real fValue, bool useTables) {
  if (useTables) {
    // Convert to sin equivalent
    fValue += Math::HALF_PI;

    return Sin(fValue, useTables);

  } else {
    return Real(asm_cos(fValue));
  }
}
//-----------------------------------------------------------------------
Real Math::Exp (Real fValue) {
  return Real(exp(fValue));
}
//-----------------------------------------------------------------------
Real Math::Floor (Real fValue) {
  return Real(floor(fValue));
}
//-----------------------------------------------------------------------
Real Math::Log (Real fValue) {
  return Real(asm_ln(fValue));
}
//-----------------------------------------------------------------------
Real Math::Pow (Real fBase, Real fExponent) {
  return Real(pow(fBase,fExponent));
}
//-----------------------------------------------------------------------
Real Math::Sign (Real fValue) {
  if ( fValue > 0.0 )
    return 1.0;

  if ( fValue < 0.0 )
    return -1.0;

  return 0.0;
}
//-----------------------------------------------------------------------
Real Math::Sin (Real fValue, bool useTables) {
  if (useTables) {
    // Convert range to index values, wrap if required
    int idx;
    if (fValue >= 0) {
      idx = int(fValue * mTrigTableFactor) % mTrigTableSize;
    } else {
      idx = mTrigTableSize - (int(-fValue * mTrigTableFactor) % mTrigTableSize) - 1;
    }

    return mSinTable[idx];
  } else {
    return Real(asm_sin(fValue));
  }
}
//-----------------------------------------------------------------------
Real Math::Sqr (Real fValue) {
  return fValue*fValue;
}
//-----------------------------------------------------------------------
Real Math::Sqrt (Real fValue) {
  return Real(asm_sqrt(fValue));
}
//-----------------------------------------------------------------------
Real Math::InvSqrt (Real fValue) {
  return Real(asm_rsq(fValue));
}
//-----------------------------------------------------------------------
Real Math::UnitRandom () {
  return asm_rand() / asm_rand_max();
}

//-----------------------------------------------------------------------
Real Math::RangeRandom (Real fLow, Real fHigh) {
  return (fHigh-fLow)*UnitRandom() + fLow;
}

//-----------------------------------------------------------------------
Real Math::SymmetricRandom () {
  return 2.0f * UnitRandom() - 1.0f;
}

//-----------------------------------------------------------------------
Real Math::DegreesToRadians(Real degrees) {
  static Real fDeg2Rad = PI / Real(180.0);

  return degrees * fDeg2Rad;
}

//-----------------------------------------------------------------------
Real Math::RadiansToDegrees(Real radians) {
  static Real fRad2Deg = Real(180.0) / PI;

  return radians * fRad2Deg;
}

//-----------------------------------------------------------------------
void Math::setAngleUnit(Math::AngleUnit unit) {
  msAngleUnit = unit;
}
//-----------------------------------------------------------------------
Math::AngleUnit Math::getAngleUnit(void) {
  return msAngleUnit;
}
//-----------------------------------------------------------------------
Real Math::AngleUnitsToRadians(Real angleunits) {
  static Real fDeg2Rad = PI / Real(180.0);

  if (msAngleUnit == AU_DEGREE)
    return angleunits * fDeg2Rad;
  else
    return angleunits;
}

//-----------------------------------------------------------------------
Real Math::RadiansToAngleUnits(Real radians) {
  static Real fRad2Deg = Real(180.0) / PI;

  if (msAngleUnit == AU_DEGREE)
    return radians * fRad2Deg;
  else
    return radians;
}

//-----------------------------------------------------------------------
bool Math::pointInTri2D( Real px, Real py, Real ax, Real ay, Real bx, Real by, Real cx, Real cy ) {
  Real v1x, v2x, v1y, v2y;
  bool bClockwise;

  v1x = bx - ax;
  v1y = by - ay;

  v2x = px - bx;
  v2y = py - by;

  // For the sake of readability
#define Clockwise ( v1x * v2y - v1y * v2x >= 0.0 )

  bClockwise = Clockwise;

  v1x = cx - bx;
  v1y = cy - by;

  v2x = px - cx;
  v2y = py - cy;

  if( Clockwise != bClockwise )
    return false;

  v1x = ax - cx;
  v1y = ay - cy;

  v2x = px - ax;
  v2y = py - ay;

  if( Clockwise != bClockwise )
    return false;

  return true;

  // Clean up the #defines
#undef Clockwise
}

//-----------------------------------------------------------------------
Real Math::Tan(Real radians, bool useTables) {
  if (useTables) {
    // Convert range to index values, wrap if required
    int idx = int(radians *= mTrigTableFactor) % mTrigTableSize;
    return mTanTable[idx];
  } else {
    return Real(asm_tan(radians));
  }
}

//-----------------------------------------------------------------------
bool Math::RealEqual( Real a, Real b, Real tolerance ) {
  if ((b < (a + tolerance)) && (b > (a - tolerance)))
    return true;
  else
    return false;
}

//-----------------------------------------------------------------------
Math& Math::getSingleton(void) {
  return Singleton<Math>::getSingleton();
}
//-----------------------------------------------------------------------
std::pair<bool, Real> Math::intersects(const Ray& ray, const Plane& plane) {

  Real denom = plane.normal.dotProduct(ray.getDirection());
  if (denom < std::numeric_limits<Real>::epsilon()) {
    // Parallel
    return std::pair<bool, Real>(false, 0);
  } else {
    Real nom = plane.normal.dotProduct(ray.getOrigin()) + plane.d;
    Real t = -(nom/denom);
    return std::pair<bool, Real>(t >= 0, t);
  }

}
//-----------------------------------------------------------------------
std::pair<bool, Real> Math::intersects(const Ray& ray, const Sphere& sphere) {
  const Vector3& raydir = ray.getDirection();
  // Adjust ray origin relative to sphere center
  const Vector3& rayorig = ray.getOrigin() - sphere.getCenter();
  Real radius = sphere.getRadius();

  // Check origin inside first
  if (rayorig.squaredLength() <= radius*radius) {
    return std::pair<bool, Real>(true, 0);
  }

  // Mmm, quadratics
  // Build coeffs which can be used with std quadratic solver
  // ie t = (-b +/- sqrt(b*b + 4ac)) / 2a
  Real a = raydir.dotProduct(raydir);
  Real b = 2 * rayorig.dotProduct(raydir);
  Real c = rayorig.dotProduct(rayorig) - radius*radius;

  // Calc determinant
  Real d = (b*b) - (4 * a * c);
  if (d < 0) {
    // No intersection
    return std::pair<bool, Real>(false, 0);
  } else {
    // BTW, if d=0 there is one intersection, if d > 0 there are 2
    // But we only want the closest one, so that's ok, just use the
    // '-' version of the solver
    Real t = ( -b - Math::Sqrt(d) ) / (2 * a);
    return std::pair<bool, Real>(true, t);
  }


}
//-----------------------------------------------------------------------
std::pair<bool, Real> Math::intersects(const Ray& ray, const AxisAlignedBox& box) {
  if (box.isNull()) return std::pair<bool, Real>(false, 0);

  Real lowt = 0.0f;
  Real t;
  bool hit = false;
  Vector3 hitpoint;
  const Vector3& min = box.getMinimum();
  const Vector3& max = box.getMaximum();
  const Vector3& rayorig = ray.getOrigin();
  const Vector3& raydir = ray.getDirection();

  // Check origin inside first
  if ( !(rayorig < min || rayorig > max) ) {
    return std::pair<bool, Real>(true, 0);
  }

  // Check each face in turn, only check closest 3
  // Min x
  if (rayorig.x < min.x && raydir.x > 0) {
    t = (min.x - rayorig.x) / raydir.x;
    if (t > 0) {
      // Substitute t back into ray and check bounds and dist
      hitpoint = rayorig + raydir * t;
      if (hitpoint.y >= min.y && hitpoint.y <= max.y &&
          hitpoint.z >= min.z && hitpoint.z <= max.z &&
          t < lowt) {
        hit = true;
        lowt = t;
      }
    }
  }
  // Max x
  if (rayorig.x > max.x && raydir.x < 0) {
    t = (max.x - rayorig.x) / raydir.x;
    if (t > 0) {
      // Substitute t back into ray and check bounds and dist
      hitpoint = rayorig + raydir * t;
      if (hitpoint.y >= min.y && hitpoint.y <= max.y &&
          hitpoint.z >= min.z && hitpoint.z <= max.z &&
          t < lowt) {
        hit = true;
        lowt = t;
      }
    }
  }
  // Min y
  if (rayorig.y < min.y && raydir.y > 0) {
    t = (min.y - rayorig.y) / raydir.y;
    if (t > 0) {
      // Substitute t back into ray and check bounds and dist
      hitpoint = rayorig + raydir * t;
      if (hitpoint.x >= min.x && hitpoint.x <= max.x &&
          hitpoint.z >= min.z && hitpoint.z <= max.z &&
          t < lowt) {
        hit = true;
        lowt = t;
      }
    }
  }
  // Max y
  if (rayorig.y > max.y && raydir.y < 0) {
    t = (max.y - rayorig.y) / raydir.y;
    if (t > 0) {
      // Substitute t back into ray and check bounds and dist
      hitpoint = rayorig + raydir * t;
      if (hitpoint.x >= min.x && hitpoint.x <= max.x &&
          hitpoint.z >= min.z && hitpoint.z <= max.z &&
          t < lowt) {
        hit = true;
        lowt = t;
      }
    }
  }
  // Min z
  if (rayorig.z < min.z && raydir.z > 0) {
    t = (min.z - rayorig.z) / raydir.z;
    if (t > 0) {
      // Substitute t back into ray and check bounds and dist
      hitpoint = rayorig + raydir * t;
      if (hitpoint.x >= min.x && hitpoint.x <= max.x &&
          hitpoint.y >= min.y && hitpoint.y <= max.y &&
          t < lowt) {
        hit = true;
        lowt = t;
      }
    }
  }
  // Max z
  if (rayorig.z > max.z && raydir.z < 0) {
    t = (max.z - rayorig.z) / raydir.z;
    if (t > 0) {
      // Substitute t back into ray and check bounds and dist
      hitpoint = rayorig + raydir * t;
      if (hitpoint.x >= min.x && hitpoint.x <= max.x &&
          hitpoint.y >= min.y && hitpoint.y <= max.y &&
          t < lowt) {
        hit = true;
        lowt = t;
      }
    }
  }

  return std::pair<bool, Real>(hit, lowt);

}
//-----------------------------------------------------------------------
bool Math::intersects(const Sphere& sphere, const AxisAlignedBox& box) {
  if (box.isNull()) return false;

  // Use splitting planes
  const Vector3& center = sphere.getCenter();
  Real radius = sphere.getRadius();
  const Vector3& min = box.getMinimum();
  const Vector3& max = box.getMaximum();

  // just test facing planes, early fail if sphere is totally outside
  if (center.x < min.x &&
      min.x - center.x > radius) {
    return false;
  }
  if (center.x > max.x &&
      center.x  - max.x > radius) {
    return false;
  }

  if (center.y < min.y &&
      min.y - center.y > radius) {
    return false;
  }
  if (center.y > max.y &&
      center.y  - max.y > radius) {
    return false;
  }

  if (center.z < min.z &&
      min.z - center.z > radius) {
    return false;
  }
  if (center.z > max.z &&
      center.z  - max.z > radius) {
    return false;
  }

  // Must intersect
  return true;

}
//-----------------------------------------------------------------------
bool Math::intersects(const Plane& plane, const AxisAlignedBox& box) {
  if (box.isNull()) return false;

  // Get corners of the box
  const Vector3* pCorners = box.getAllCorners();


  // Test which side of the plane the corners are
  // Intersection occurs when at least one corner is on the
  // opposite side to another
  Plane::Side lastSide = plane.getSide(pCorners[0]);
  for (int corner = 1; corner < 8; ++corner) {
    if (plane.getSide(pCorners[corner]) != lastSide) {
      return true;
    }
  }

  return false;
}
//-----------------------------------------------------------------------
bool Math::intersects(const Sphere& sphere, const Plane& plane) {
  return (
           Math::Abs(plane.normal.dotProduct(sphere.getCenter()))
           <= sphere.getRadius() );
}
//-----------------------------------------------------------------------

}
