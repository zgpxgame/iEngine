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
#include "Mesh.h"

#include "base/string_split.h"
#include "base/string_util.h"
#include "SubMesh.h"
#include "MaterialManager.h"
#include "LogManager.h"
#include "DataChunk.h"
#include "MeshSerializer.h"
#include "SkeletonManager.h"
#include "Skeleton.h"
#include <algorithm>


namespace renderer {

//-----------------------------------------------------------------------
Mesh::Mesh(String name) {
  mName = name;
  sharedGeometry.hasColours = false;
  sharedGeometry.hasNormals = false;
  sharedGeometry.vertexStride = 0;
  sharedGeometry.colourStride = 0;
  sharedGeometry.normalStride = 0;
  sharedGeometry.numTexCoords = 1;
  sharedGeometry.numTexCoordDimensions[0] = 2;
  sharedGeometry.numVertices = 0;
  sharedGeometry.pColours = 0;
  sharedGeometry.pNormals = 0;
  sharedGeometry.pBlendingWeights = 0;
  sharedGeometry.numBlendWeightsPerVertex = 0;

  for (int i = 0; i < OGRE_MAX_TEXTURE_COORD_SETS; ++i) {
    sharedGeometry.pTexCoords[i] = 0;
    sharedGeometry.texCoordStride[i] = 0;
  }
  sharedGeometry.pVertices = 0;
  // Default to load from file
  mManuallyDefined = false;
  mUpdateBounds = true;
  setSkeletonName("");
  mBoneAssignmentsOutOfDate = false;
  mNumLods = 1;
  // Init first (manual) lod
  MeshLodUsage lod;
  lod.fromDepthSquared = 0.0f;
  mMeshLodUsageList.push_back(lod);
  mIsLodManual = false;


}

//-----------------------------------------------------------------------
Mesh::~Mesh() {
  if (mIsLoaded) {
    unload();
  }
}

//-----------------------------------------------------------------------
SubMesh* Mesh::createSubMesh() {
  SubMesh* sub = new SubMesh();
  sub->parent = this;

  mSubMeshList.push_back(sub);

  return sub;
}
//-----------------------------------------------------------------------
SubMesh* Mesh::createSubMesh(const String& name) {
  SubMesh *sub = createSubMesh();
  nameSubMesh(name, (ushort)mSubMeshList.size()-1);
  return sub ;
}
//-----------------------------------------------------------------------
unsigned short Mesh::getNumSubMeshes() const {
  return static_cast< unsigned short >( mSubMeshList.size() );
}

//---------------------------------------------------------------------
void Mesh::nameSubMesh(const String& name, ushort index) {
  mSubMeshNameMap[name] = index ;
}

//-----------------------------------------------------------------------
SubMesh* Mesh::getSubMesh(const String& name) const {
  ushort index = _getSubMeshIndex(name);
  return getSubMesh(index);
}
//-----------------------------------------------------------------------
SubMesh* Mesh::getSubMesh(unsigned short index) const {
  SubMeshList::const_iterator i = mSubMeshList.begin();
  return const_cast<SubMesh*>(i[index]);
}
//-----------------------------------------------------------------------
void Mesh::load() {
  // Load from specified 'name'
  if (mIsLoaded) {
    unload();
    mIsLoaded = false;
  }

  if (!mManuallyDefined) {
    MeshSerializer serializer;
    char msg[100];
    sprintf(msg, "Mesh: Loading %s .", mName.c_str());
    LogManager::getSingleton().logMessage(msg);

    DataChunk chunk;
    MeshManager::getSingleton()._findResourceData(mName, chunk);

    // Determine file type
    std::vector<String> extVec;
    base::SplitString(mName, '.', &extVec);

    String& ext = extVec[extVec.size() - 1];
    ext = StringToLowerASCII(ext);

    if (ext == "oof") {
      serializer.importLegacyOof(chunk, this);
    } else if (ext == "mesh") {
      serializer.importMesh(chunk, this);
    } else {
      // Unsupported format
      chunk.clear();
      Except(999, "Unsupported object file format.",
             "Mesh::load");
    }

    chunk.clear();
  }

  _updateBounds();

}

//-----------------------------------------------------------------------
void Mesh::unload() {
  // Teardown submeshes
  for (SubMeshList::iterator i = mSubMeshList.begin();
       i != mSubMeshList.end(); ++i) {
    delete *i;
  }
  if (sharedGeometry.pVertices) {
    delete[] sharedGeometry.pVertices;
    sharedGeometry.pVertices = 0;
  }
  // Deallocate individual components if they have their own buffers
  // NB Assuming that if some components use the same buffer, all do and vice versa
  if (sharedGeometry.vertexStride == 0) {

    // Destroy shared buffers
    if (sharedGeometry.pColours) {
      delete[] sharedGeometry.pColours;
      sharedGeometry.pColours = 0;
    }
    if (sharedGeometry.pNormals) {
      delete[] sharedGeometry.pNormals;
      sharedGeometry.pNormals = 0;
    }
    for (int j = 0; j < OGRE_MAX_TEXTURE_COORD_SETS; ++j) {
      if (sharedGeometry.pTexCoords[j]) {
        delete[] sharedGeometry.pTexCoords[j];
        sharedGeometry.pTexCoords[j] = 0;
      }
    }
  }
  // Clear SubMesh names
  mSubMeshNameMap.clear();
}

//-----------------------------------------------------------------------
void Mesh::_dumpContents(String filename) {
  std::ofstream of;

  of.open(filename.c_str());

  of << "-= Debug output of model " << mName << " =-" << std::endl << std::endl;
  if (sharedGeometry.numVertices > 0) {
    of << "-= Shared geometry =- " << std::endl;
    _dumpGeometry(sharedGeometry, of);
  }

  for (SubMeshList::iterator i = mSubMeshList.begin(); i != mSubMeshList.end(); ++i) {
    of << "-= SubMesh Entry =-" << std::endl;
    of << "Material Name = " << (*i)->getMaterialName() << std::endl;
    of << "numFaces = " << (*i)->numFaces << std::endl;
    of << "useSharedVertices = " << (*i)->useSharedVertices << std::endl;
    if (!(*i)->useSharedVertices) {
      of << "-= Dedicated Geometry =-" << std::endl;
      _dumpGeometry((*i)->geometry, of);
    }

    of << "-= Face List =-" << std::endl;
    for (int idx = 0; idx < (*i)->numFaces; ++idx) {
      of << (*i)->faceVertexIndices[idx*3] << ", " <<
         (*i)->faceVertexIndices[(idx*3) + 1] << ", " <<
         (*i)->faceVertexIndices[(idx*3) + 2] << std::endl;
    }
  }

  of.close();


}

void Mesh::_dumpGeometry(GeometryData &g, std::ofstream& of) {
  int i;

  of << "hasColours = " << g.hasColours << std::endl;
  of << "hasNormals = " << g.hasNormals << std::endl;
  of << "numTexCoords = " << g.numTexCoords << std::endl;
  for (i = 0; i < g.numTexCoords; ++i)
    of << "numTexCoordDimensions[" << i << "] = " << g.numTexCoordDimensions[i] << std::endl;
  of << "numVertices = " << g.numVertices << std::endl;
  // Write out vertices
  of << "Vertex Position Data:" << std::endl;
  Real* pReal = g.pVertices;
  unsigned long* pULong;
  for (i = 0; i < g.numVertices; ++i) {
    of << *pReal++ << ", " <<
       *pReal++ << ", " <<
       *pReal++ << std::endl;
    pReal = (Real*)((char*)pReal + g.vertexStride);
  }
  if (g.hasColours) {
    // Write out colours
    of << "Vertex Colour Data:" << std::endl;
    pULong = g.pColours;
    for (int i = 0; i < g.numVertices; ++i) {
      of << *pULong++ << std::endl;
      pULong = (unsigned long*)((char*)pULong + g.colourStride);
    }
  }
  if (g.hasNormals) {
    // Write out Normals
    of << "Vertex Normal Data:" << std::endl;
    pReal = g.pNormals;
    for (int i = 0; i < g.numVertices; ++i) {
      of << *pReal++ << ", " <<
         *pReal++ << ", " <<
         *pReal++ << std::endl;
      pReal = (Real*)((char*)pReal + g.normalStride);
    }
  }
  // Write out textures
  of << "Vertex Texture Coordinate Data:" << std::endl;
  int index;
  index = 0;
  for (int set = 0; set < g.numTexCoords; ++set) {
    of << "Set " << set << ":" << std::endl;
    pReal = g.pTexCoords[set];
    for ( i = 0; i < g.numVertices; ++i) {
      for (int d = 0; d < g.numTexCoordDimensions[set]; ++ d) {
        of << *pReal++ << " ";
      }
      of << std::endl;
      pReal = (Real*)((char*)pReal + g.texCoordStride[set]);
    }
  }


}

//-----------------------------------------------------------------------
void Mesh::setManuallyDefined(bool manual) {
  mManuallyDefined = manual;
}

//-----------------------------------------------------------------------
Mesh* Mesh::clone(String newName) {
  // This is a bit like a copy constructor, but with the additional aspect of registering the clone with
  //  the MeshManager

  // New Mesh is assumed to be manually defined rather than loaded since you're cloning it for a reason
  Mesh* newMesh = MeshManager::getSingleton().createManual(newName);

  bool isSharedGeometry;
  isSharedGeometry = false;

  // Copy submeshes first
  std::vector<SubMesh*>::iterator subi;
  SubMesh* newSub;
  for (subi = mSubMeshList.begin(); subi != mSubMeshList.end(); ++subi) {
    newSub = newMesh->createSubMesh();
    newSub->mMaterialName = (*subi)->mMaterialName;
    newSub->mMatInitialised = (*subi)->mMatInitialised;
    newSub->numFaces = (*subi)->numFaces;
    newSub->parent = newMesh;
    newSub->useSharedVertices = (*subi)->useSharedVertices;
    newSub->useTriStrips = (*subi)->useTriStrips;

    if ((*subi)->useSharedVertices) {
      // Make a note to copy shared geometry later
      isSharedGeometry = true;
    } else {
      // Copy unique geometry
      cloneGeometry((*subi)->geometry, newSub->geometry);
    }

    // Copy indexes
    int numIndexes;

    if ((*subi)->useTriStrips) {
      numIndexes = (*subi)->numFaces + 2;
    } else {
      numIndexes = (*subi)->numFaces * 3;
    }
    newSub->faceVertexIndices = new unsigned short[numIndexes];
    memcpy(newSub->faceVertexIndices, (*subi)->faceVertexIndices, sizeof(unsigned short) * numIndexes);

  }

  // Copy shared geometry, if any
  if (isSharedGeometry) {
    cloneGeometry(sharedGeometry, newMesh->sharedGeometry);
  }

  // Copy submesh names
  newMesh->mSubMeshNameMap = mSubMeshNameMap ;

  return newMesh;

}
//-----------------------------------------------------------------------
void Mesh::cloneGeometry(GeometryData& source, GeometryData& dest) {
  int tex;

  dest.colourStride = source.colourStride;
  dest.hasColours = source.hasColours;
  dest.hasNormals = source.hasNormals;
  dest.normalStride = source.normalStride;
  dest.numTexCoords = source.numTexCoords;
  for (tex = 0; tex < source.numTexCoords; ++tex) {
    dest.numTexCoordDimensions[tex] = source.numTexCoordDimensions[tex];
    dest.texCoordStride[tex] = source.texCoordStride[tex];
  }
  dest.numVertices = source.numVertices;
  dest.vertexStride = source.vertexStride;

  // Create geometry
  dest.pVertices = new Real[source.numVertices * 3];
  memcpy(dest.pVertices, source.pVertices, sizeof(Real) * source.numVertices * 3);

  if (source.hasColours) {
    dest.pColours = new unsigned long[source.numVertices];
    memcpy(dest.pColours, source.pColours, sizeof(int) * source.numVertices);
  }
  if (source.hasNormals) {
    dest.pNormals = new Real[source.numVertices * 3];
    memcpy(dest.pNormals, source.pNormals, sizeof(Real) * source.numVertices * 3);
  }
  for (tex = 0; tex < source.numTexCoords; ++tex) {
    dest.pTexCoords[tex] = new Real[source.numVertices * source.numTexCoordDimensions[tex]];
    memcpy(dest.pTexCoords[tex], source.pTexCoords[tex], sizeof(Real) * source.numVertices *
           source.numTexCoordDimensions[tex]);
  }
}
//-----------------------------------------------------------------------
void Mesh::_updateBounds(void) {
  Vector3 min, max;
  bool first = true;
  bool useShared = false;
  int vert;

  Real maxSquaredLength = -1.0f;

  // Loop through SubMeshes, find extents
  SubMeshList::iterator i;
  for (i = mSubMeshList.begin(); i != mSubMeshList.end(); ++i) {
    if ((*i)->useSharedVertices) {
      useShared = true;
    } else {
      for (vert = 0; vert < (*i)->geometry.numVertices * 3; vert+=(3+ (*i)->geometry.vertexStride)) {
        if (first || (*i)->geometry.pVertices[vert] < min.x) {
          min.x = (*i)->geometry.pVertices[vert];
        }
        if (first || (*i)->geometry.pVertices[vert+1] < min.y) {
          min.y = (*i)->geometry.pVertices[vert+1];
        }
        if (first || (*i)->geometry.pVertices[vert+2] < min.z) {
          min.z = (*i)->geometry.pVertices[vert+2];
        }
        if (first || (*i)->geometry.pVertices[vert] > max.x) {
          max.x = (*i)->geometry.pVertices[vert];
        }
        if (first || (*i)->geometry.pVertices[vert+1] > max.y) {
          max.y = (*i)->geometry.pVertices[vert+1];
        }
        if (first || (*i)->geometry.pVertices[vert+2] > max.z) {
          max.z = (*i)->geometry.pVertices[vert+2];
        }
        first = false;

        Real newSqlLen =
          (*i)->geometry.pVertices[vert]  * (*i)->geometry.pVertices[vert] +
          (*i)->geometry.pVertices[vert+1] * (*i)->geometry.pVertices[vert+1] +
          (*i)->geometry.pVertices[vert+2] * (*i)->geometry.pVertices[vert+2];
        maxSquaredLength = std::max(newSqlLen, maxSquaredLength);
      }
    }
  }

  // Check shared
  if (useShared) {
    for (vert = 0; vert < sharedGeometry.numVertices * 3; vert+=(3 + sharedGeometry.vertexStride)) {
      if (first || sharedGeometry.pVertices[vert] < min.x) {
        min.x = sharedGeometry.pVertices[vert];
      }
      if (first || sharedGeometry.pVertices[vert+1] < min.y) {
        min.y = sharedGeometry.pVertices[vert+1];
      }
      if (first || sharedGeometry.pVertices[vert+2] < min.z) {
        min.z = sharedGeometry.pVertices[vert+2];
      }
      if (first || sharedGeometry.pVertices[vert] > max.x) {
        max.x = sharedGeometry.pVertices[vert];
      }
      if (first || sharedGeometry.pVertices[vert+1] > max.y) {
        max.y = sharedGeometry.pVertices[vert+1];
      }
      if (first || sharedGeometry.pVertices[vert+2] > max.z) {
        max.z = sharedGeometry.pVertices[vert+2];
      }
      first = false;
      Real newSqlLen = sharedGeometry.pVertices[vert]* sharedGeometry.pVertices[vert] +
                       sharedGeometry.pVertices[vert+1] * sharedGeometry.pVertices[vert+1] +
                       sharedGeometry.pVertices[vert+2] * sharedGeometry.pVertices[vert+2];
      maxSquaredLength = std::max(newSqlLen, maxSquaredLength);
    }
  }

  // Pad out the AABB a little, helps with most bounds tests
  min -= Vector3::UNIT_SCALE;
  max += Vector3::UNIT_SCALE;
  mAABB.setExtents(min, max);
  // Pad out the sphere a little too
  mBoundRadius = Math::Sqrt(maxSquaredLength) * 1.25;
  mUpdateBounds = false;

}
//-----------------------------------------------------------------------
const AxisAlignedBox& Mesh::getBounds(void) {
  if (mUpdateBounds)
    _updateBounds();
  return mAABB;
}
//-----------------------------------------------------------------------
void Mesh::_setBounds(const AxisAlignedBox& bounds) {
  mAABB = bounds;
  // Set sphere bouds; not the tightest by since we're using
  // manual AABB it is the only way
  Real sqLen1 = mAABB.getMinimum().squaredLength();
  Real sqLen2 = mAABB.getMaximum().squaredLength();
  mBoundRadius = Math::Sqrt(std::max(sqLen1, sqLen2));

  mUpdateBounds = false;
}
//-----------------------------------------------------------------------
void Mesh::setSkeletonName(const String& skelName) {
  mSkeletonName = skelName;

  if (skelName == "") {
    // No skeleton
    mSkeleton = 0;
  } else {
    // Load skeleton
    try {
      mSkeleton = SkeletonManager::getSingleton().load(skelName);
    } catch (...) {
      mSkeleton = 0;
      // Log this error
      std::stringstream msg("Unable to load skeleton ");
      msg << skelName << " for Mesh " << mName
          << " This Mesh will not be animated. ";
      LogManager::getSingleton().logMessage(msg.str());

    }


  }
}
//-----------------------------------------------------------------------
bool Mesh::hasSkeleton(void) const {
  return !(mSkeletonName.empty());
}
//-----------------------------------------------------------------------
Skeleton* Mesh::getSkeleton(void) const {
  return mSkeleton;
}
//-----------------------------------------------------------------------
void Mesh::addBoneAssignment(const VertexBoneAssignment& vertBoneAssign) {
  mBoneAssignments.insert(
    VertexBoneAssignmentList::value_type(vertBoneAssign.vertexIndex, vertBoneAssign));
  mBoneAssignmentsOutOfDate = true;
}
//-----------------------------------------------------------------------
void Mesh::clearBoneAssignments(void) {
  mBoneAssignments.clear();
  mBoneAssignmentsOutOfDate = true;
}
//-----------------------------------------------------------------------
void Mesh::_initAnimationState(AnimationStateSet* animSet) {
  // Delegate to Skeleton
  assert(mSkeleton && "Skeleton not present");
  mSkeleton->_initAnimationState(animSet);

  // Take the opportunity to update the compiled bone assignments
  if (mBoneAssignmentsOutOfDate)
    compileBoneAssignments();

  SubMeshList::iterator i;
  for (i = mSubMeshList.begin(); i != mSubMeshList.end(); ++i) {
    if ((*i)->mBoneAssignmentsOutOfDate) {
      (*i)->compileBoneAssignments();
    }
  }
}
//-----------------------------------------------------------------------
unsigned short Mesh::_getNumBoneMatrices(void) {
  // Delegate to Skeleton
  assert(mSkeleton && "Skeleton not present");

  return mSkeleton->getNumBones();
}
//-----------------------------------------------------------------------
void Mesh::_getBoneMatrices(const AnimationStateSet& animSet, Matrix4* pMatrices) {
  // Delegate to Skeleton
  assert(mSkeleton && "Skeleton not present");

  mSkeleton->setAnimationState(animSet);
  mSkeleton->_getBoneMatrices(pMatrices);

}
//-----------------------------------------------------------------------
void Mesh::compileBoneAssignments(void) {
  // Deallocate
  if (sharedGeometry.pBlendingWeights) {
    delete [] sharedGeometry.pBlendingWeights;
    sharedGeometry.pBlendingWeights = 0;
  }

  // Iterate through, finding the largest # bones per vertex
  unsigned short maxBones = 0;
  unsigned short currBones, lastVertIdx = std::numeric_limits< ushort >::max();
  VertexBoneAssignmentList::iterator i, iend;
  i = mBoneAssignments.begin();
  iend = mBoneAssignments.end();
  for (; i != iend; ++i) {
    if (lastVertIdx != i->second.vertexIndex) {
      // change in vertex
      if (maxBones < currBones)
        maxBones = currBones;
      currBones = 0;
    }

    currBones++;

    lastVertIdx = i->second.vertexIndex;

  }

  if (maxBones == 0) {
    // No bone assignments
    sharedGeometry.numBlendWeightsPerVertex = 0;
    return;
  }
  // Allocate a buffer for bone weights
  sharedGeometry.numBlendWeightsPerVertex = maxBones;
  sharedGeometry.pBlendingWeights =
    new RenderOperation::VertexBlendData[sharedGeometry.numVertices * maxBones];

  // Assign data
  unsigned short v;
  i = mBoneAssignments.begin();
  RenderOperation::VertexBlendData *pBlend = sharedGeometry.pBlendingWeights;
  // Iterate by vertex
  for (v = 0; v < sharedGeometry.numVertices; ++v) {
    for (unsigned short bone = 0; bone < maxBones; ++bone) {
      // Do we still have data for this vertex?
      if (i->second.vertexIndex == v) {
        // If so, assign
        pBlend->matrixIndex = i->second.boneIndex;
        pBlend->blendWeight = i->second.weight;
        ++i;
      } else {
        // Ran out of assignments for this vertex, use weight 0 to indicate empty
        pBlend->blendWeight = 0;
        pBlend->matrixIndex = 0;
      }
      ++pBlend;
    }
  }

  mBoneAssignmentsOutOfDate = false;

}
//---------------------------------------------------------------------
void Mesh::_notifySkeleton(Skeleton* pSkel) {
  mSkeleton = pSkel;
  mSkeletonName = pSkel->getName();
}
//---------------------------------------------------------------------
Mesh::BoneAssignmentIterator Mesh::getBoneAssignmentIterator(void) {
  return BoneAssignmentIterator(mBoneAssignments.begin(),
                                mBoneAssignments.end());
}
//---------------------------------------------------------------------
const String& Mesh::getSkeletonName(void) const {
  return mSkeletonName;
}
//---------------------------------------------------------------------
void Mesh::generateLodLevels(const LodDistanceList& lodDistances,
                             ProgressiveMesh::VertexReductionQuota reductionMethod, Real reductionValue) {
  mMeshLodUsageList.clear();
  mIsLodManual = false;

  char msg[128];
  sprintf(msg, "Generating %d lower LODs for mesh %s.",
          lodDistances.size(), mName.c_str());
  LogManager::getSingleton().logMessage(msg);

  SubMeshList::iterator isub, isubend;
  isubend = mSubMeshList.end();
  for (isub = mSubMeshList.begin(); isub != isubend; ++isub) {
    // Set up data for reduction
    GeometryData* pGeom = (*isub)->useSharedVertices ? &sharedGeometry : &((*isub)->geometry);

    ProgressiveMesh pm(pGeom, (*isub)->faceVertexIndices, (*isub)->numFaces * 3);
    pm.build(
      static_cast<ushort>(lodDistances.size()),
      &((*isub)->mLodFaceList),
      reductionMethod, reductionValue);

  }

  // Iterate over the lods and record usage
  LodDistanceList::const_iterator idist, idistend;
  idistend = lodDistances.end();
  // Record first LOD (full detail)
  MeshLodUsage lod;
  lod.fromDepthSquared = 0.0f;
  mMeshLodUsageList.push_back(lod);

  for (idist = lodDistances.begin(); idist != idistend; ++idist) {
    // Record usage
    lod.fromDepthSquared = (*idist) * (*idist);
    mMeshLodUsageList.push_back(lod);

  }
  mNumLods = static_cast<ushort>(lodDistances.size() + 1);
}
//---------------------------------------------------------------------
ushort Mesh::getNumLodLevels(void) const {
  return mNumLods;
}
//---------------------------------------------------------------------
const Mesh::MeshLodUsage& Mesh::getLodLevel(ushort index) const {
  assert(index < mMeshLodUsageList.size());
  if (mIsLodManual && mMeshLodUsageList[index].manualMesh == NULL) {
    // Load the mesh now
    mMeshLodUsageList[index].manualMesh =
      MeshManager::getSingleton().load(mMeshLodUsageList[index].manualName);
  }
  return mMeshLodUsageList[index];
}
//---------------------------------------------------------------------
struct ManualLodSortLess :
  public std::binary_function<const Mesh::MeshLodUsage&, const Mesh::MeshLodUsage&, bool> {
  bool operator() (const Mesh::MeshLodUsage& mesh1, const Mesh::MeshLodUsage& mesh2) {
    // sort ascending by depth
    return mesh1.fromDepthSquared < mesh2.fromDepthSquared;
  }
};
void Mesh::createManualLodLevel(Real fromDepth, const String& meshName) {
  mIsLodManual = true;
  MeshLodUsage lod;
  lod.fromDepthSquared = fromDepth * fromDepth;
  lod.manualName = meshName;
  lod.manualMesh = NULL;
  mMeshLodUsageList.push_back(lod);
  ++mNumLods;

  std::sort(mMeshLodUsageList.begin(), mMeshLodUsageList.end(), ManualLodSortLess());
}
//---------------------------------------------------------------------
void Mesh::updateManualLodLevel(ushort index, const String& meshName) {
  // Basic prerequisites
  assert(mIsLodManual && "Not using manual LODs!");
  assert(index != 0 && "Can't modify first lod level (full detail)");
  assert(index < mMeshLodUsageList.size() && "Index out of bounds");
  // get lod
  MeshLodUsage* lod = &(mMeshLodUsageList[index]);

  lod->manualName = meshName;
  lod->manualMesh = NULL;
}
//---------------------------------------------------------------------
ushort Mesh::getLodIndex(Real depth) const {
  return getLodIndexSquaredDepth(depth * depth);
}
//---------------------------------------------------------------------
ushort Mesh::getLodIndexSquaredDepth(Real squaredDepth) const {
  MeshLodUsageList::const_iterator i, iend;
  iend = mMeshLodUsageList.end();
  ushort index = 0;
  for (i = mMeshLodUsageList.begin(); i != iend; ++i, ++index) {
    if (i->fromDepthSquared > squaredDepth) {
      return index - 1;
    }
  }

  // If we fall all the way through, use the highest value
  return static_cast<ushort>(mMeshLodUsageList.size() - 1);


}
//---------------------------------------------------------------------
void Mesh::_setLodInfo(unsigned short numLevels, bool isManual) {
  mNumLods = numLevels;
  mMeshLodUsageList.resize(numLevels);
  // Resize submesh face data lists too
  for (SubMeshList::iterator i = mSubMeshList.begin(); i != mSubMeshList.end(); ++i) {
    (*i)->mLodFaceList.resize(numLevels - 1);
  }
  mIsLodManual = isManual;
}
//---------------------------------------------------------------------
void Mesh::_setLodUsage(unsigned short level, Mesh::MeshLodUsage& usage) {
  mMeshLodUsageList[level] = usage;
}
//---------------------------------------------------------------------
void Mesh::_setSubMeshLodFaceList(unsigned short subIdx, unsigned short level,
                                  ProgressiveMesh::LODFaceData& facedata) {
  SubMesh* sm = mSubMeshList[subIdx];
  sm->mLodFaceList[level - 1] = facedata;

}
//---------------------------------------------------------------------
ushort Mesh::_getSubMeshIndex(const String& name) const {
  SubMeshNameMap::const_iterator i = mSubMeshNameMap.find(name) ;
  if (i == mSubMeshNameMap.end())
    Except(Exception::ERR_ITEM_NOT_FOUND, "No SubMesh named " + name + " found.",
           "Mesh::_getSubMeshIndex");

  return i->second;
}
//---------------------------------------------------------------------
void Mesh::removeLodLevels(void) {
  if (!mIsLodManual) {
    // Remove data from SubMeshes
    SubMeshList::iterator isub, isubend;
    isubend = mSubMeshList.end();
    for (isub = mSubMeshList.begin(); isub != isubend; ++isub) {
      (*isub)->removeLodLevels();
    }
  }

  mMeshLodUsageList.clear();

  // Reinitialise
  mNumLods = 1;
  // Init first (manual) lod
  MeshLodUsage lod;
  lod.fromDepthSquared = 0.0f;
  mMeshLodUsageList.push_back(lod);
  mIsLodManual = false;


}
//---------------------------------------------------------------------
Real Mesh::getBoundingSphereRadius(void) {
  return mBoundRadius;
}


}

