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
#ifndef __GLRenderSystem_H__
#define __GLRenderSystem_H__

#include "GLPrerequisites.h"
#include "Platform.h"
#include "RenderSystem.h"

#include "GLSupport.h"

namespace renderer {
/**
  Implementation of SDL as a rendering system.
 */
class GLRenderSystem : public RenderSystem {
private:
  // Array of up to 8 lights, indexed as per API
  // Note that a null value indicates a free slot
#define MAX_LIGHTS 8
  Light* mLights[MAX_LIGHTS];

  // view matrix to set world against
  Matrix4 mViewMatrix;
  Matrix4 mWorldMatrix;
  Matrix4 mTextureMatrix;

  // XXX 8 max texture units?
  int mTextureCoordIndex[OGRE_MAX_TEXTURE_COORD_SETS];

  void initConfigOptions(void);

  void setGLLight(int index, Light* lt);
  void makeGLMatrix(GLfloat gl_matrix[16], const Matrix4& m);

  GLint getBlendMode(SceneBlendFactor ogreBlend);

  void setLights();

  // Store last depth write state
  bool mDepthWrite;

  GLint convertCompareFunction(CompareFunction func);
  GLint convertStencilOp(StencilOperation op);

  // Save stencil settings since GL insists on having them in groups
  // Means we have to call functions more than once, but what the hey
  GLint mStencilFunc, mStencilRef;
  GLuint mStencilMask;
  GLint mStencilFail, mStencilZFail, mStencilPass;

  // internal method for anisotrophy validation
  GLfloat _getCurrentAnisotropy();

  /// GL support class, used for creating windows etc
  GLSupport* mGLSupport;

  /// Internal method to set pos / direction of a light
  void setGLLightPositionDirection(Light* lt, int lightindex);

public:
  // Default constructor / destructor
  GLRenderSystem();
  ~GLRenderSystem();

  // ----------------------------------
  // Overridden RenderSystem functions
  // ----------------------------------
  /** See
    RenderSystem
   */
  const String& getName(void) const;
  /** See
    RenderSystem
   */
  ConfigOptionMap& getConfigOptions(void);
  /** See
    RenderSystem
   */
  void setConfigOption(const String &name, const String &value);
  /** See
    RenderSystem
   */
  String validateConfigOptions(void);
  /** See
    RenderSystem
   */
  void initialise();
  /** See
    RenderSystem
   */
  void reinitialise(void); // Used if settings changed mid-rendering
  /** See
    RenderSystem
   */
  void shutdown(void);

  /** See
    RenderSystem
   */
  void setAmbientLight(float r, float g, float b);
  /** See
    RenderSystem
   */
  void setShadingType(ShadeOptions so);
  /** See
    RenderSystem
   */
  void setTextureFiltering(TextureFilterOptions fo);
  /** See
    RenderSystem
   */
  void setLightingEnabled(bool enabled);
  /** See
    RenderSystem
   */
  //RenderWindow* createRenderWindow(const String &name, int width, int height, int colourDepth,
  //                                 bool fullScreen, int left = 0, int top = 0, bool depthBuffer = true,
  //                                 RenderWindow* parentWindowHandle = 0);

  RenderTexture * createRenderTexture( const String & name, int width, int height );

  /** See
    RenderSystem
   */
  void destroyRenderWindow(RenderWindow* pWin);
  /** See
    RenderSystem
   */
  String getErrorDescription(long errorNumber);

  /** See
    RenderSystem
   */
  void convertColourValue(const ColourValue& colour, unsigned long* pDest);

  // -----------------------------
  // Low-level overridden members
  // -----------------------------
  /** See
    RenderSystem
   */
  void _addLight(Light *lt);
  /** See
    RenderSystem
   */
  void _removeLight(Light *lt);
  /** See
    RenderSystem
   */
  void _modifyLight(Light* lt);
  /** See
    RenderSystem
   */
  void _removeAllLights(void);
  /** See
    RenderSystem
   */
  void _pushRenderState(void);
  /** See
    RenderSystem
   */
  void _popRenderState(void);
  /** See
    RenderSystem
   */
  void _setWorldMatrix(const Matrix4 &m);
  /** See
    RenderSystem
   */
  void _setViewMatrix(const Matrix4 &m);
  /** See
    RenderSystem
   */
  void _setProjectionMatrix(const Matrix4 &m);
  /** See
    RenderSystem
   */
  void _setSurfaceParams(const ColourValue &ambient,
                         const ColourValue &diffuse, const ColourValue &specular,
                         const ColourValue &emissive, Real shininess);
  /** See
    RenderSystem
   */
  unsigned short _getNumTextureUnits(void);
  /** See
    RenderSystem
   */
  void _setTexture(int unit, bool enabled, const String &texname);
  /** See
    RenderSystem
   */
  void _setTextureCoordSet(int stage, int index);
  /** See
    RenderSystem
   */
  void _setTextureCoordCalculation(int stage, TexCoordCalcMethod m);
  /** See
    RenderSystem
   */
  void _setTextureBlendMode(int stage, const LayerBlendModeEx& bm);
  /** See
    RenderSystem
   */
  void _setTextureAddressingMode(int stage, Material::TextureLayer::TextureAddressingMode tam);
  /** See
    RenderSystem
   */
  void _setTextureMatrix(int stage, const Matrix4& xform);
  /** See
    RenderSystem
   */
  void _setSceneBlending(SceneBlendFactor sourceFactor, SceneBlendFactor destFactor);
  /** See
    RenderSystem
   */
  void _setAlphaRejectSettings(CompareFunction func, unsigned char value);
  /** See
    RenderSystem
   */
  void _setViewport(Viewport *vp);
  /** See
    RenderSystem
   */
  void _beginFrame(void);
  /** See
    RenderSystem
   */
  void _render(RenderOperation& op);
  /** See
    RenderSystem
   */
  void _endFrame(void);
  /** See
    RenderSystem
   */
  void _setCullingMode(CullingMode mode);
  /** See
    RenderSystem
   */
  void _setDepthBufferParams(bool depthTest = true, bool depthWrite = true, CompareFunction depthFunction = CMPF_LESS_EQUAL);
  /** See
    RenderSystem
   */
  void _setDepthBufferCheckEnabled(bool enabled = true);
  /** See
    RenderSystem
   */
  void _setDepthBufferWriteEnabled(bool enabled = true);
  /** See
    RenderSystem
   */
  void _setDepthBufferFunction(CompareFunction func = CMPF_LESS_EQUAL);
  /** See
    RenderSystem
   */
  void _setDepthBias(ushort bias);
  /** See
    RenderSystem
   */
  void _setFog(FogMode mode, ColourValue colour, Real density, Real start, Real end);
  /** See
    RenderSystem
   */
  void _makeProjectionMatrix(Real fovy, Real aspect, Real nearPlane, Real farPlane, Matrix4& dest);
  /** See
    RenderSystem
   */
  void _setRasterisationMode(SceneDetailLevel level);
  /** See
    RenderSystem
   */
  void setStencilCheckEnabled(bool enabled);
  /** See
    RenderSystem
   */
  bool hasHardwareStencil(void);
  /** See
    RenderSystem
   */
  ushort getStencilBufferBitDepth(void);
  /** See
    RenderSystem
   */
  void setStencilBufferFunction(CompareFunction func);
  /** See
    RenderSystem
   */
  void setStencilBufferReferenceValue(ulong refValue);
  /** See
    RenderSystem
   */
  void setStencilBufferMask(ulong mask);
  /** See
    RenderSystem
   */
  void setStencilBufferFailOperation(StencilOperation op);
  /** See
    RenderSystem
   */
  void setStencilBufferDepthFailOperation(StencilOperation op);
  /** See
    RenderSystem
   */
  void setStencilBufferPassOperation(StencilOperation op);
  /** See RenderSystem.
  @remarks
      This is overridden because GL likes to set stencil options together, so we can
      provide a better custom implementation of this than using the superclass.
   */
  void setStencilBufferParams(CompareFunction func = CMPF_ALWAYS_PASS,
                              ulong refValue = 0, ulong mask = 0xFFFFFFFF,
                              StencilOperation stencilFailOp = SOP_KEEP,
                              StencilOperation depthFailOp = SOP_KEEP,
                              StencilOperation passOp = SOP_KEEP);
  /** See
    RenderSystem
   */
  void _setTextureLayerFiltering(int unit, const TextureFilterOptions texLayerFilterOps);
  /** See
    RenderSystem
   */
  void _setAnisotropy(int maxAnisotropy);
  /** See
    RenderSystem
   */
  void _setTextureLayerAnisotropy(int unit, int maxAnisotropy);
  // ----------------------------------
  // End Overridden members
  // ----------------------------------
};
}
#endif

