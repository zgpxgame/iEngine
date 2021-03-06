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
#include "MeshManager.h"

#include "Mesh.h"
#include "SubMesh.h"
#include "Matrix4.h"
#include "Matrix3.h"
#include "Vector3.h"
#include "Plane.h"

namespace renderer {
#define PI 3.1415926535897932384626433832795

//-----------------------------------------------------------------------
template<> MeshManager* Singleton<MeshManager>::ms_Singleton = 0;
//-----------------------------------------------------------------------
MeshManager::MeshManager() {


}
//-----------------------------------------------------------------------
void MeshManager::_initialise(void) {
  // Create prefab objects
  createPrefabPlane();


}
//-----------------------------------------------------------------------
Resource* MeshManager::create( const String& name) {
  return new Mesh(name);
}
//-----------------------------------------------------------------------
Mesh* MeshManager::load( const String& filename, int priority) {
  Mesh* pMesh = (Mesh*)(getByName(filename));
  if (!pMesh) {
    pMesh = (Mesh*)create(filename);
    ResourceManager::load(pMesh, priority);
    //pMesh->_registerMaterials();
  }
  return pMesh;

}
//-----------------------------------------------------------------------
Mesh* MeshManager::createManual( const String& name) {
  Mesh* pMesh = (Mesh*)(getByName(name));
  if (!pMesh) {
    pMesh = (Mesh*)create(name);
    pMesh->setManuallyDefined(true);
    ResourceManager::load(pMesh,0);
  }

  return pMesh;
}
//-----------------------------------------------------------------------
Mesh* MeshManager::createPlane( const String& name, const Plane& plane, Real width, Real height, int xsegments, int ysegments,
                                bool normals, int numTexCoordSets, Real xTile, Real yTile, const Vector3& upVector) {
  int i;
  Mesh* pMesh = createManual(name);
  SubMesh *pSub = pMesh->createSubMesh();

  // Set options
  pMesh->sharedGeometry.hasColours = false;
  pMesh->sharedGeometry.hasNormals = normals;
  pMesh->sharedGeometry.normalStride = 0;
  pMesh->sharedGeometry.numTexCoords = numTexCoordSets;
  for (i = 0; i < numTexCoordSets; ++i) {
    pMesh->sharedGeometry.numTexCoordDimensions[i] = 2;
    pMesh->sharedGeometry.texCoordStride[i] = 0;
  }
  pMesh->sharedGeometry.numVertices = (xsegments + 1) * (ysegments + 1);
  pMesh->sharedGeometry.vertexStride = 0;

  // Allocate memory
  pMesh->sharedGeometry.pVertices = new Real[pMesh->sharedGeometry.numVertices * 3];
  if (normals)
    pMesh->sharedGeometry.pNormals = new Real[pMesh->sharedGeometry.numVertices * 3];
  for (i = 0; i < numTexCoordSets; ++i)
    pMesh->sharedGeometry.pTexCoords[i] = new Real[pMesh->sharedGeometry.numVertices * 2];

  // Work out the transform required
  // Default orientation of plane is normal along +z, distance 0
  Matrix4 xlate, xform, rot;
  Matrix3 rot3;
  xlate = rot = Matrix4::IDENTITY;
  // Determine axes
  Vector3 zAxis, yAxis, xAxis;
  zAxis = plane.normal;
  zAxis.normalise();
  yAxis = upVector;
  yAxis.normalise();
  xAxis = yAxis.crossProduct(zAxis);
  if (xAxis.length() == 0) {
    //upVector must be wrong
    Except(Exception::ERR_INVALIDPARAMS, "The upVector you supplied is parallel to the plane normal, so is not valid.",
           "MeshManager::createPlane");
  }

  rot3.FromAxes(xAxis, yAxis, zAxis);
  rot = rot3;

  // Set up standard xform from origin
  xlate.setTrans(plane.normal * -plane.d);

  // concatenate
  xform = xlate * rot;

  // Generate vertex data
  Real* pReal;
  Real xSpace = width / xsegments;
  Real ySpace = height / ysegments;
  Real halfWidth = width / 2;
  Real halfHeight = height / 2;
  Real xTex = (1.0f * xTile) / xsegments;
  Real yTex = (1.0f * yTile) / ysegments;
  Vector3 vec;

  for (int y = 0; y < ysegments + 1; ++y) {
    for (int x = 0; x < xsegments + 1; ++x) {
      pReal = pMesh->sharedGeometry.pVertices + (((y * (xsegments+1)) + x) * 3);
      // Work out centered on origin
      vec.x = (x * xSpace) - halfWidth;
      vec.y = (y * ySpace) - halfHeight;
      vec.z = 0.0f;
      // Transform by orientation and distance
      vec = xform * vec;
      // Assign to geometry
      pReal[0] = vec.x;
      pReal[1] = vec.y;
      pReal[2] = vec.z;

      if (normals) {
        pReal = pMesh->sharedGeometry.pNormals + (((y * (xsegments+1)) + x) * 3);
        // Default normal is along unit Z
        vec = Vector3::UNIT_Z;
        // Rotate
        vec = rot * vec;

        pReal[0] = vec.x;
        pReal[1] = vec.y;
        pReal[2] = vec.z;
      }

      for (i = 0; i < numTexCoordSets; ++i) {
        pReal = pMesh->sharedGeometry.pTexCoords[i] + (((y * (xsegments+1)) + x) * 2);
        pReal[0] = x * xTex;
        pReal[1] = y * yTex;
      }

    } // x
  } // y

  // Generate face list
  tesselate2DMesh(pSub, xsegments + 1, ysegments + 1, false);

  pMesh->_updateBounds();

  return pMesh;
}

//-----------------------------------------------------------------------
Mesh* MeshManager::createCurvedPlane( const String& name, const Plane& plane, Real width, Real height, Real bow, int xsegments, int ysegments,
                                      bool normals, int numTexCoordSets, Real xTile, Real yTile, const Vector3& upVector) {
  int i;
  Mesh* pMesh = createManual(name);
  SubMesh *pSub = pMesh->createSubMesh();

  // Set options
  pMesh->sharedGeometry.hasColours = false;
  pMesh->sharedGeometry.hasNormals = normals;
  pMesh->sharedGeometry.normalStride = 0;
  pMesh->sharedGeometry.numTexCoords = numTexCoordSets;
  for (i = 0; i < numTexCoordSets; ++i) {
    pMesh->sharedGeometry.numTexCoordDimensions[i] = 2;
    pMesh->sharedGeometry.texCoordStride[i] = 0;
  }
  pMesh->sharedGeometry.numVertices = (xsegments + 1) * (ysegments + 1);
  pMesh->sharedGeometry.vertexStride = 0;

  // Allocate memory
  pMesh->sharedGeometry.pVertices = new Real[pMesh->sharedGeometry.numVertices * 3];
  if (normals)
    pMesh->sharedGeometry.pNormals = new Real[pMesh->sharedGeometry.numVertices * 3];
  for (i = 0; i < numTexCoordSets; ++i)
    pMesh->sharedGeometry.pTexCoords[i] = new Real[pMesh->sharedGeometry.numVertices * 2];

  // Work out the transform required
  // Default orientation of plane is normal along +z, distance 0
  Matrix4 xlate, xform, rot;
  Matrix3 rot3;
  xlate = rot = Matrix4::IDENTITY;
  // Determine axes
  Vector3 zAxis, yAxis, xAxis;
  zAxis = plane.normal;
  zAxis.normalise();
  yAxis = upVector;
  yAxis.normalise();
  xAxis = yAxis.crossProduct(zAxis);
  if (xAxis.length() == 0) {
    //upVector must be wrong
    Except(Exception::ERR_INVALIDPARAMS, "The upVector you supplied is parallel to the plane normal, so is not valid.",
           "MeshManager::createPlane");
  }

  rot3.FromAxes(xAxis, yAxis, zAxis);
  rot = rot3;

  // Set up standard xform from origin
  xlate.setTrans(plane.normal * -plane.d);

  // concatenate
  xform = xlate * rot;

  // Generate vertex data
  Real* pReal;
  Real xSpace = width / xsegments;
  Real ySpace = height / ysegments;
  Real halfWidth = width / 2;
  Real halfHeight = height / 2;
  Real xTex = (1.0f * xTile) / xsegments;
  Real yTex = (1.0f * yTile) / ysegments;
  Vector3 vec;

  Real diff_x, diff_y, dist;

  for (int y = 0; y < ysegments + 1; ++y) {
    for (int x = 0; x < xsegments + 1; ++x) {
      pReal = pMesh->sharedGeometry.pVertices + (((y * (xsegments+1)) + x) * 3);
      // Work out centered on origin
      vec.x = (x * xSpace) - halfWidth;
      vec.y = (y * ySpace) - halfHeight;

      // Here's where curved plane is different from standard plane.  Amazing, I know.
      diff_x = (x - ((xsegments) / 2)) / static_cast<Real>((xsegments));
      diff_y = (y - ((ysegments) / 2)) / static_cast<Real>((ysegments));
      dist = sqrt(diff_x*diff_x + diff_y * diff_y );
      vec.z = (-sin((1-dist) * (PI/2)) * bow) + bow;

      // Transform by orientation and distance
      vec = xform * vec;
      // Assign to geometry
      pReal[0] = vec.x;
      pReal[1] = vec.y;
      pReal[2] = vec.z;

      if (normals) {
        /* This part is kinda 'wrong' for curved planes... but curved planes are
           very valuable outside sky planes, which don't typically need normals
           so I'm not going to mess with it for now. */

        pReal = pMesh->sharedGeometry.pNormals + (((y * (xsegments+1)) + x) * 3);
        // Default normal is along unit Z
        vec = Vector3::UNIT_Z;
        // Rotate
        vec = rot * vec;

        pReal[0] = vec.x;
        pReal[1] = vec.y;
        pReal[2] = vec.z;
      }

      for (i = 0; i < numTexCoordSets; ++i) {
        pReal = pMesh->sharedGeometry.pTexCoords[i] + (((y * (xsegments+1)) + x) * 2);
        pReal[0] = x * xTex;
        pReal[1] = y * yTex;
      }

    } // x
  } // y

  // Generate face list
  tesselate2DMesh(pSub, xsegments + 1, ysegments + 1, false);

  pMesh->_updateBounds();

  return pMesh;
}

//-----------------------------------------------------------------------
void MeshManager::tesselate2DMesh(SubMesh* sm, int meshWidth, int meshHeight, bool doubleSided) {
  // The mesh is built, just make a list of indexes to spit out the triangles
  int vInc, uInc, v, u, iterations;
  int vCount, uCount;

  if (doubleSided) {
    iterations = 2;
    vInc = 1;
    v = 0; // Start with front
  } else {
    iterations = 1;
    vInc = 1;
    v = 0;
  }

  // Allocate memory for faces
  // Num faces, width*height*2 (2 tris per square)
  sm->numFaces = (meshWidth-1) * (meshHeight-1) * 2 * iterations;
  sm->faceVertexIndices = new unsigned short[sm->numFaces * 3];

  int v1, v2, v3;
  //bool firstTri = true;
  unsigned short* pIndexes = sm->faceVertexIndices;

  while (iterations--) {
    // Make tris in a zigzag pattern (compatible with strips)
    u = 0;
    uInc = 1; // Start with moving +u

    vCount = meshHeight - 1;
    while (vCount--) {
      uCount = meshWidth - 1;
      while (uCount--) {
        // First Tri in cell
        // -----------------
        v1 = ((v + vInc) * meshWidth) + u;
        v2 = (v * meshWidth) + u;
        v3 = ((v + vInc) * meshWidth) + (u + uInc);
        // Output indexes
        *pIndexes++ = v1;
        *pIndexes++ = v2;
        *pIndexes++ = v3;
        // Second Tri in cell
        // ------------------
        v1 = ((v + vInc) * meshWidth) + (u + uInc);
        v2 = (v * meshWidth) + u;
        v3 = (v * meshWidth) + (u + uInc);
        // Output indexes
        *pIndexes++ = v1;
        *pIndexes++ = v2;
        *pIndexes++ = v3;

        // Next column
        u += uInc;
      }
      // Next row
      v += vInc;
      u = 0;


    }

    // Reverse vInc for double sided
    v = meshHeight - 1;
    vInc = -vInc;

  }

}

//-----------------------------------------------------------------------
void MeshManager::createPrefabPlane(void) {
  Mesh* msh = (Mesh*)create("Prefab_Plane");
  SubMesh* sub = msh->createSubMesh();
  Real vertices[12] = {-100, -100, 0,
                       100, -100, 0,
                       100,  100, 0,
                       -100,  100, 0
                      };
  Real normals[12] = {0,0,1,
                      0,0,1,
                      0,0,1,
                      0,0,1,
                     };
  Real texCoords[8] = {0,0,
                       1,0,
                       1,1,
                       0,1
                      };
  msh->sharedGeometry.hasColours = false;
  msh->sharedGeometry.hasNormals = true;
  msh->sharedGeometry.numTexCoords = 1;
  msh->sharedGeometry.numTexCoordDimensions[0] = 2;
  msh->sharedGeometry.numVertices = 4;
  msh->sharedGeometry.pVertices = new Real[12];
  memcpy(msh->sharedGeometry.pVertices, vertices, sizeof(Real)*12);
  msh->sharedGeometry.pNormals = new Real[12];
  memcpy(msh->sharedGeometry.pNormals, normals, sizeof(Real)*12);
  msh->sharedGeometry.pTexCoords[0] = new Real[8];
  memcpy(msh->sharedGeometry.pTexCoords[0], texCoords, sizeof(Real)*8);

  sub->useSharedVertices = true;
  sub->faceVertexIndices = new unsigned short[6];
  unsigned short faces[6] = {0,1,2,
                             0,2,3
                            };
  memcpy(sub->faceVertexIndices, faces, sizeof(unsigned short)*6);
  sub->numFaces = 2;

  mResources[msh->getName()] = msh;
}
//-----------------------------------------------------------------------
MeshManager& MeshManager::getSingleton(void) {
  return Singleton<MeshManager>::getSingleton();
}
//-----------------------------------------------------------------------



}
