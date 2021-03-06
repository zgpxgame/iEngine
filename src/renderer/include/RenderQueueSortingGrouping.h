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
#ifndef __RenderQueueSortingGrouping_H__
#define __RenderQueueSortingGrouping_H__

// Precompiler options
#include "Prerequisites.h"
#include "IteratorWrappers.h"
#include "Material.h"

namespace renderer {

/** Renderables in the queue grouped by priority.
@remarks
    This class simply groups renderables for rendering. All the
    renderables contained in this class are destined for the same
    RenderQueueGroup (coarse groupings like those between the main
    scene and overlays) and have the same priority (fine groupings
    for detailed overlap control).
@par
    This class optimises the grouped renderables by sorting them by
    material to reduce render state changes, and outsorts transparent
    objects.
*/
class RenderPriorityGroup {
  friend class renderer::SceneManager;
  /** Comparator to order transparent objects
  */
  struct TransparentQueueItemLess {
    const Camera* camera;

    _RendererExport bool operator()(const Renderable* x, const Renderable* y) const {
      // Sort DESCENDING by depth (ie far objects first)
      if (x->getSquaredViewDepth(camera) > y->getSquaredViewDepth(camera)) {
        return true;
      } else {
        return false;
      }

    }
  };
public:
  typedef std::vector<Renderable*> RenderableList;
  /// Map on material within each queue group, this is for non-transparent objects only
  /// 材质相同的不透明物体列表
  typedef std::map<Material*, RenderableList> MaterialGroupMap;
  /// Transparent object list, these are not grouped by material but will be sorted by descending Z
  /// 透明物体列表，不通过材质分组，按Z值降序排列（由远至近？）
  typedef std::vector<Renderable*> TransparentObjectList;
protected:
  MaterialGroupMap mMaterialGroups;
  TransparentObjectList mTransparentObjects;

public:
  RenderPriorityGroup() {}

  ~RenderPriorityGroup() {}

  /** Add a renderable to this group. */
  void addRenderable(Renderable* pRend) {
    std::pair<MaterialGroupMap::iterator, bool> retPair;
    RenderableList newList;

    Material* pMat = pRend->getMaterial();

    assert(pMat && "Can't add a renderable with a null material!");

    if (pMat->isTransparent()) {
      // Insert into transparent object vector
      mTransparentObjects.push_back(pRend);

    } else {

      // Try to insert, if already there existing will be returned
      retPair = mMaterialGroups.insert(MaterialGroupMap::value_type(pMat, newList));

      // Insert new Renderable
      // retPair.first is iterator on map (Material*, std::vector)
      // retPair.first->second is the vector of renderables
      retPair.first->second.push_back(pRend);
    }

  }

  /** Sorts the transparent objects which have been added to the queue by their depth
      in relation to the passed in Camera. */
  void sortTransparentObjects(const Camera* cam) {
    TransparentQueueItemLess lessFunctor;
    lessFunctor.camera = cam;

    std::sort(mTransparentObjects.begin(), mTransparentObjects.end(),
              lessFunctor);
  }


  /** Clears this group of renderables.
  */
  void clear(void) {
    mMaterialGroups.clear();
    mTransparentObjects.clear();

  }



};


/** A grouping level underneath RenderQueue which groups renderables
to be issued at coarsely the same time to the renderer.
RenderQueue下的优先级分组管理类，迭代器返回的分组按优先级升序
@remarks
    Each instance of this class itself hold RenderPriorityGroup instances,
    which are the groupings of renderables by priority for fine control
    of ordering (not required for most instances).
    每一个RenderQueueGroup实例拥有一个RenderPriorityGroup实例，通过优先级
    将可渲染物体分组，以更好的控制渲染
@par
    This is an internal OGRE class, not intended for apps to use.
*/
class RenderQueueGroup {
public:
  typedef std::map<ushort, RenderPriorityGroup*, std::less<ushort> > PriorityMap;
  typedef MapIterator<PriorityMap> PriorityMapIterator;
protected:
  // Map of RenderPriorityGroup objects
  PriorityMap mPriorityGroups;


public:
  RenderQueueGroup() {}

  ~RenderQueueGroup() {
    // destroy contents now
    PriorityMap::iterator i;
    for (i = mPriorityGroups.begin(); i != mPriorityGroups.end(); ++i) {
      delete i->second;
    }
  }

  /** Get an iterator for browsing through child contents. */
  PriorityMapIterator getIterator(void) {
    return PriorityMapIterator(mPriorityGroups.begin(), mPriorityGroups.end());
  }

  /** Add a renderable to this group, with the given priority. */
  void addRenderable(Renderable* pRend, ushort priority) {
    // Check if priority group is there
    PriorityMap::iterator i = mPriorityGroups.find(priority);
    RenderPriorityGroup* pPriorityGrp;
    if (i == mPriorityGroups.end()) {
      // Missing, create
      pPriorityGrp = new RenderPriorityGroup();
      mPriorityGroups.insert(PriorityMap::value_type(priority, pPriorityGrp));
    } else {
      pPriorityGrp = i->second;
    }

    // Add
    pPriorityGrp->addRenderable(pRend);

  }

  /** Clears this group of renderables.
  @remarks
      Doesn't delete any priority groups, just empties them. Saves on
      memory deallocations since the chances are rougly the same kinds of
      renderables are going to be sent to the queue again next time.
  */
  void clear(void) {
    PriorityMap::iterator i, iend;
    iend = mPriorityGroups.end();
    for (i = mPriorityGroups.begin(); i != iend; ++i) {
      i->second->clear();
    }

  }


};

}

#endif


