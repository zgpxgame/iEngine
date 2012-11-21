#ifndef __RenderTargetTexture_H__
#define __RenderTargetTexture_H__

#include "Prerequisites.h"
#include "Texture.h"
#include "RenderTarget.h"

namespace renderer {
class RenderTargetTexture : public Texture, public RenderTarget {
public:
  /// @copydoc Texture::isRenderTarget
  virtual bool isRenderTarget() {
    return false;
  }
};
}

#endif
