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
#ifndef __RenderQueue_H__
#define __RenderQueue_H__

#include "Prerequisites.h"
#include "IteratorWrappers.h"

namespace renderer {

/** Enumeration of queue groups, by which the application may group queued renderables
    so that they are rendered together with events in between
	渲染队列组，用于将可渲染物体进行分组
*/
enum RenderQueueGroupID {
  /// Use this queue for objects which must be rendered first e.g. backgrounds
  RENDER_QUEUE_BACKGROUND = 0,
  /// First queue (after backgrounds), used for skyboxes if rendered first
  RENDER_QUEUE_1 = 10,
  RENDER_QUEUE_2 = 20,
  RENDER_QUEUE_3 = 30,
  RENDER_QUEUE_4 = 40,
  /// The default render queue
  RENDER_QUEUE_MAIN = 50,
  RENDER_QUEUE_6 = 60,
  RENDER_QUEUE_7 = 70,
  RENDER_QUEUE_8 = 80,
  /// Penultimate queue(before overlays), used for skyboxes if rendered last
  RENDER_QUEUE_9 = 90,
  /// Use this queue for objects which must be rendered last e.g. overlays
  RENDER_QUEUE_OVERLAY = 100
};

#define RENDERABLE_DEFAULT_PRIORITY  100

/** Class to manage the scene object rendering queue.
    管理场景中物体的渲染队列
    @remarks
        Objects are grouped by material to minimise rendering state changes. The map from
        material to renderable object is wrapped in a class for ease of use.
        场景中的物体通过材质进行分组，以达到最小化的渲染状态切换
    @par
        This class now includes the concept of 'queue groups' which allows the application
        adding the renderable to specifically schedule it so that it is included in
        a discrete group. Good for separating renderables into the main scene,
        backgrounds and overlays, and also could be used in the future for more
        complex multipass routines like stenciling.
*/
class _RendererExport RenderQueue {
public:
  typedef std::map< RenderQueueGroupID, RenderQueueGroup* > RenderQueueGroupMap;
  /// Iterator over queue groups
  typedef MapIterator<RenderQueueGroupMap> QueueGroupIterator;
protected:
  // 按不同类型进行分组（背景，主场景，OverLay等）
  RenderQueueGroupMap mGroups;
  // The current default queue group
  RenderQueueGroupID mDefaultQueueGroup;
public:
  RenderQueue();
  virtual ~RenderQueue();

  /** Empty the queue - should only be called by SceneManagers.
  */
  void clear(void);

  /** Add a renderable object to the queue.
      将物体添加到指定分组
  @remarks
      This methods adds a Renderable to the queue, which will be rendered later by
      the SceneManager. This is the advanced version of the call which allows the renderable
      to be added to any queue.
  @note
      Called by implementation of MovableObject::_updateRenderQueue.
  @param
      pRend Pointer to the Renderable to be added to the queue
  @param
      groupID The group the renderable is to be added to. This
      can be used to schedule renderable objects in separate groups such that the SceneManager
      respects the divisions between the groupings and does not reorder them outside these
      boundaries. This can be handy for overlays where no matter what you want the overlay to
      be rendered last.
  @param
      priority Controls the priority of the renderable within the queue group. If this number
      is raised, the renderable will be rendered later in the group compared to it's peers.
      Don't use this unless you really need to, manually ordering renderables prevents OGRE
      from sorting them for best efficiency. However this could be useful for ordering 2D
      elements manually for example.
  */
  void addRenderable(Renderable* pRend, RenderQueueGroupID groupID, ushort priority);

  /** Add a renderable object to the queue.
      将物体添加到当前的默认组
  @remarks
      This methods adds a Renderable to the queue, which will be rendered later by
      the SceneManager. This is the simplified version of the call which does not
      require a queue or priority to be specified. The queue group is taken from the
      current default (see setDefaultQueueGroup).
  @note
      Called by implementation of MovableObject::_updateRenderQueue.
  @param
      pRend Pointer to the Renderable to be added to the queue
  @param
      priority Controls the priority of the renderable within the queue group. If this number
      is raised, the renderable will be rendered later in the group compared to it's peers.
      Don't use this unless you really need to, manually ordering renderables prevents OGRE
      from sorting them for best efficiency. However this could be useful for ordering 2D
      elements manually for example.
  */
  void addRenderable(Renderable* pRend, ushort priority = RENDERABLE_DEFAULT_PRIORITY);

  /** Gets the current default queue group, which will be used for all renderable which do not
      specify which group they wish to be on.
  */
  RenderQueueGroupID getDefaultQueueGroup(void);

  /** Sets the current default queue group, which will be used for all renderable which do not
      specify which group they wish to be on.
  */
  void setDefaultQueueGroup(RenderQueueGroupID grp);

  /** Internal method, returns an iterator for the queue groups. */
  QueueGroupIterator _getQueueGroupIterator(void);
};


}


#endif
