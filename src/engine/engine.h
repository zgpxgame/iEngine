
#ifndef _IENGINE_H_
#define _IENGINE_H_

#include "base/basictypes.h"
#include "base/base_export.h"
#include <set>
#include <deque>

struct lua_State;
template <typename Type>
struct StaticMemorySingletonTraits;

namespace renderer {
  class Root;
  class RenderWindow;
}

namespace engine {

// 引擎框架
//   驱动所有子模块运行
  // FIXME! BASE_EXPORT
class BASE_EXPORT iEngine {
public:
  static iEngine* GetInstance();

  ~iEngine();

  bool Init();
  bool RunFrame(float delta_time);
  void Exit();
  void RequestExit();

  /** 主渲染窗口
  */
  renderer::RenderWindow* render_window() {
    return render_window_;
  }

private:
  iEngine();
  void SetupResource();

  /** Internal method for raising frame started events. */
  //bool FireFrameStarted(FrameEvent& evt);
  /** Internal method for raising frame ended events. */
  //bool FireFrameEnded(FrameEvent& evt);
  /** Internal method for raising frame started events. */
  //bool FireFrameStarted();
  /** Internal method for raising frame ended events. */
  //bool FireFrameEnded();

  /** Indicates the type of event to be considered by calculateEventTime(). */
  //enum FrameEventTimeType {
  //  FETT_ANY, FETT_STARTED, FETT_ENDED
  //};

  /** Internal method for calculating the average time between recently fired events.
  @param now The current time in ms.
  @param type The type of event to be considered.
  */
  //float CalculateEventTime(uint32 now, FrameEventTimeType type);
  
  // 清除已记录的帧事件时间
  //void ClearEventTimes();

  /** Contains the times of recently fired events */
  //std::deque<unsigned long> event_times_[3];

  renderer::Root *renderer_root_;
  renderer::RenderWindow* render_window_;
  bool exit_;
  
  // test
  lua_State* L;

  friend struct StaticMemorySingletonTraits<iEngine>;
  DISALLOW_COPY_AND_ASSIGN(iEngine);
  void ConfigRenderer();
};

}

#endif