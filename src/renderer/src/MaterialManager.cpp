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
#include "MaterialManager.h"

#include "base/string_util.h"
#include "base/string_split.h"
#include "Material.h"
#include "StringVector.h"
#include "LogManager.h"
#include "SDDataChunk.h"
#include "ArchiveEx.h"
#include "StringConverter.h"

namespace renderer {

//-----------------------------------------------------------------------
// Internal parser methods
// Material Attributes
ColourValue _parseColourValue(StringVector::iterator& params, int numParams) {
  ColourValue colour(
    atof(params[1].c_str()) ,
    atof(params[2].c_str()) ,
    atof(params[3].c_str()) ,
    (numParams==5) ? atof(params[4].c_str()) : 1.0f ) ;
  return colour ;
}
void parseAmbient(StringVector::iterator& params, int numParams, Material* pMat) {
  // Must be 3 or 4parameters (+ command = 4 or 5)
  if (numParams != 4 && numParams != 5) {
    LogManager::getSingleton().logMessage("Bad ambient attribute line in "
                                          + pMat->getName() + ", wrong number of parameters (expected 3 or 4)");
  } else {
    pMat->setAmbient( _parseColourValue(params, numParams) );
  }
}
//-----------------------------------------------------------------------
void parseDiffuse(StringVector::iterator& params, int numParams, Material* pMat) {
  // Must be 3 or 4 parameters (+ command = 4 or 5)
  if (numParams != 4 && numParams != 5) {
    LogManager::getSingleton().logMessage("Bad diffuse attribute line in "
                                          + pMat->getName() + ", wrong number of parameters (expected 3 or 4)");
  } else {
    pMat->setDiffuse( _parseColourValue(params, numParams) );
  }
}
//-----------------------------------------------------------------------
void parseSpecular(StringVector::iterator& params, int numParams, Material* pMat) {
  // Must be 4 or 5 parameters (+ command = 5 or 6)
  if (numParams != 5 && numParams != 6) {
    LogManager::getSingleton().logMessage("Bad specular attribute line in "
                                          + pMat->getName() + ", wrong number of parameters (expected 4 or 5)");
  } else {
    pMat->setSpecular( _parseColourValue(params, numParams-1) );
    pMat->setShininess(atof(params[numParams-1].c_str()));
  }
}
//-----------------------------------------------------------------------
void parseEmissive(StringVector::iterator& params, int numParams, Material* pMat) {
  // Must be 3 or 4 parameters (+ command = 4 or 5)
  if (numParams != 4 && numParams != 5) {
    LogManager::getSingleton().logMessage("Bad emissive attribute line in "
                                          + pMat->getName() + ", wrong number of parameters (expected 3 or 4)");
  } else {
    pMat->setSelfIllumination( _parseColourValue(params, numParams) );
  }
}
//-----------------------------------------------------------------------
SceneBlendFactor convertBlendFactor(const String& param) {
  if (param == "one")
    return SBF_ONE;
  else if (param == "zero")
    return SBF_ZERO;
  else if (param == "dest_colour")
    return SBF_DEST_COLOUR;
  else if (param == "src_colour")
    return SBF_SOURCE_COLOUR;
  else if (param == "one_minus_dest_colour")
    return SBF_ONE_MINUS_DEST_COLOUR;
  else if (param == "one_minus_src_colour")
    return SBF_ONE_MINUS_SOURCE_COLOUR;
  else if (param == "dest_alpha")
    return SBF_DEST_ALPHA;
  else if (param == "src_alpha")
    return SBF_SOURCE_ALPHA;
  else if (param == "one_minus_dest_alpha")
    return SBF_ONE_MINUS_DEST_ALPHA;
  else if (param == "one_minus_src_alpha")
    return SBF_ONE_MINUS_SOURCE_ALPHA;
  else {
    Except(Exception::ERR_INVALIDPARAMS, "Invalid blend factor.", "convertBlendFactor");
  }


}
//-----------------------------------------------------------------------
void parseSceneBlend(StringVector::iterator& params, int numParams, Material* pMat) {
  // Should be 1 or 2 params (+ command)
  if (numParams == 2) {
    //simple
    SceneBlendType stype;
    if (params[1] == "add")
      stype = SBT_ADD;
    else if (params[1] == "modulate")
      stype = SBT_TRANSPARENT_COLOUR;
    else if (params[1] == "alpha_blend")
      stype = SBT_TRANSPARENT_ALPHA;
    else {
      LogManager::getSingleton().logMessage("Bad scene_blend attribute line in "
                                            + pMat->getName() + ", unrecognised parameter '" + params[1] + "'");
    }
    pMat->setSceneBlending(stype);

  } else if (numParams == 3) {
    //src/dest
    SceneBlendFactor src, dest;

    try {
      src = convertBlendFactor(params[1]);
      dest = convertBlendFactor(params[2]);
      pMat->setSceneBlending(src,dest);
    } catch (Exception& e) {
      LogManager::getSingleton().logMessage("Bad scene_blend attribute line in "
                                            + pMat->getName() + ", " + e.getFullDescription());
    }

  } else {
    LogManager::getSingleton().logMessage("Bad scene_blend attribute line in "
                                          + pMat->getName() + ", wrong number of parameters (expected 1 or 2)");
  }


}
//-----------------------------------------------------------------------
CompareFunction convertCompareFunction(const String& param) {
  if (param == "always_fail")
    return CMPF_ALWAYS_FAIL;
  else if (param == "always_pass")
    return CMPF_ALWAYS_PASS;
  else if (param == "less")
    return CMPF_LESS;
  else if (param == "less_equal")
    return CMPF_LESS_EQUAL;
  else if (param == "equal")
    return CMPF_EQUAL;
  else if (param == "not_equal")
    return CMPF_NOT_EQUAL;
  else if (param == "greater_equal")
    return CMPF_GREATER_EQUAL;
  else if (param == "greater")
    return CMPF_GREATER;
  else
    Except(Exception::ERR_INVALIDPARAMS, "Invalid compare function", "convertCompareFunction");

}
//-----------------------------------------------------------------------
void parseDepthParams(StringVector::iterator& params, int numParams, Material* pMat) {
  if (numParams != 2) {
    LogManager::getSingleton().logMessage("Bad " + params[0] + " attribute line in "
                                          + pMat->getName() + ", wrong number of parameters (expected 1)");
    return;
  }

  if (params[0] == "depth_check") {
    if (params[1] == "on")
      pMat->setDepthCheckEnabled(true);
    else if (params[1] == "off")
      pMat->setDepthCheckEnabled(false);
    else
      LogManager::getSingleton().logMessage("Bad depth_check attribute line in "
                                            + pMat->getName() + ", valid parameters are 'on' or 'off'.");
  } else if (params[0] == "depth_write") {
    if (params[1] == "on")
      pMat->setDepthWriteEnabled(true);
    else if (params[1] == "off")
      pMat->setDepthWriteEnabled(false);
    else
      LogManager::getSingleton().logMessage("Bad depth_write attribute line in "
                                            + pMat->getName() + ", valid parameters are 'on' or 'off'.");
  } else if (params[0] == "depth_func") {
    try {
      CompareFunction func = convertCompareFunction(params[1]);
      pMat->setDepthFunction(func);
    } catch (...) {
      LogManager::getSingleton().logMessage("Bad depth_func attribute line in "
                                            + pMat->getName() + ", invalid function parameter.");
    }
  }

}
//-----------------------------------------------------------------------
void parseCullMode(StringVector::iterator& params, int numParams, Material* pMat) {

  if (numParams != 2) {
    LogManager::getSingleton().logMessage("Bad " + params[0] + " attribute line in "
                                          + pMat->getName() + ", wrong number of parameters (expected 1)");
    return;
  }

  if (params[0] == "cull_hardware") {
    if (params[1]=="none")
      pMat->setCullingMode(CULL_NONE);
    else if (params[1]=="anticlockwise")
      pMat->setCullingMode(CULL_ANTICLOCKWISE);
    else if (params[1]=="clockwise")
      pMat->setCullingMode(CULL_CLOCKWISE);
    else
      LogManager::getSingleton().logMessage("Bad cull_hardware attribute line in "
                                            + pMat->getName() + ", valid parameters are 'none', 'clockwise' or 'anticlockwise'.");

  } else { // cull_software
    if (params[1]=="none")
      pMat->setManualCullingMode(MANUAL_CULL_NONE);
    else if (params[1]=="back")
      pMat->setManualCullingMode(MANUAL_CULL_BACK);
    else if (params[1]=="front")
      pMat->setManualCullingMode(MANUAL_CULL_FRONT);
    else
      LogManager::getSingleton().logMessage("Bad cull_software attribute line in "
                                            + pMat->getName() + ", valid parameters are 'none', 'front' or 'back'.");


  }
}
//-----------------------------------------------------------------------
void parseLighting(StringVector::iterator& params, int numParams, Material* pMat) {
  if (numParams != 2) {
    LogManager::getSingleton().logMessage("Bad " + params[0] + " attribute line in "
                                          + pMat->getName() + ", wrong number of parameters (expected 1)");
    return;
  }
  if (params[1]=="on")
    pMat->setLightingEnabled(true);
  else if (params[1]=="off")
    pMat->setLightingEnabled(false);
  else
    LogManager::getSingleton().logMessage("Bad lighting attribute line in "
                                          + pMat->getName() + ", valid parameters are 'on' or 'off'.");
}
//-----------------------------------------------------------------------
void parseFogging(StringVector::iterator& params, int numParams, Material* pMat) {
  if (numParams < 2) {
    LogManager::getSingleton().logMessage("Bad " + params[0] + " attribute line in "
                                          + pMat->getName() + ", wrong number of parameters (expected 1)");
    return;
  }
  if (params[1]=="true") {
    // if true, we need to see if they supplied all arguments, or just the 1... if just the one,
    // Assume they want to disable the default fog from effecting this material.
    if( numParams == 9 ) {
      FogMode mFogtype;
      if( params[2] == "none" )
        mFogtype = FOG_NONE;
      else if( params[2] == "linear" )
        mFogtype = FOG_LINEAR;
      else if( params[2] == "exp" )
        mFogtype = FOG_EXP;
      else if( params[2] == "exp2" )
        mFogtype = FOG_EXP2;
      else
        LogManager::getSingleton().logMessage("Bad fogging attribute line in "
                                              + pMat->getName() + ", valid parameters are 'none', 'linear', 'exp', or 'exp2'.");

      pMat->setFog(true,mFogtype,ColourValue(atof(params[3].c_str()),atof(params[4].c_str()),atof(params[5].c_str())),atof(params[6].c_str()),atof(params[7].c_str()),atof(params[8].c_str()));
    } else {
      pMat->setFog(true);
    }
  } else if (params[1]=="false")
    pMat->setFog(false);
  else
    LogManager::getSingleton().logMessage("Bad fog_override attribute line in "
                                          + pMat->getName() + ", valid parameters are 'true' or 'false'.");
}
//-----------------------------------------------------------------------
void parseShading(StringVector::iterator& params, int numParams, Material* pMat) {
  if (numParams != 2) {
    LogManager::getSingleton().logMessage("Bad " + params[0] + " attribute line in "
                                          + pMat->getName() + ", wrong number of parameters (expected 1)");
    return;
  }
  if (params[1]=="flat")
    pMat->setShadingMode(SO_FLAT);
  else if (params[1]=="gouraud")
    pMat->setShadingMode(SO_GOURAUD);
  else if (params[1]=="phong")
    pMat->setShadingMode(SO_PHONG);
  else
    LogManager::getSingleton().logMessage("Bad shading attribute line in "
                                          + pMat->getName() + ", valid parameters are 'flat', 'gouraud' or 'phong'.");

}
//-----------------------------------------------------------------------
void parseFiltering(StringVector::iterator& params, int numParams, Material* pMat) {
  if (numParams != 2) {
    LogManager::getSingleton().logMessage("Bad " + params[0] + " attribute line in "
                                          + pMat->getName() + ", wrong number of parameters (expected 1)");
    return;
  }
  if (params[1]=="none")
    pMat->setTextureFiltering(TFO_NONE);
  else if (params[1]=="bilinear")
    pMat->setTextureFiltering(TFO_BILINEAR);
  else if (params[1]=="trilinear")
    pMat->setTextureFiltering(TFO_TRILINEAR);
  else if (params[1]=="anisotropic")
    pMat->setTextureFiltering(TFO_ANISOTROPIC);
  else
    LogManager::getSingleton().logMessage("Bad filtering attribute line in "
                                          + pMat->getName() + ", valid parameters are 'none', 'bilinear', 'trilinear' or 'anisotropic'.");
}
//-----------------------------------------------------------------------
// Texture layer attributes
void parseTexture(StringVector::iterator& params, int numParams, Material* pMat, Material::TextureLayer* pTex) {
  if (numParams != 2) {
    LogManager::getSingleton().logMessage("Bad " + params[0] + " attribute line in "
                                          + pMat->getName() + ", wrong number of parameters (expected 1)");
    return;
  }
  pTex->setTextureName(params[1]);
}
//-----------------------------------------------------------------------
void parseAnimTexture(StringVector::iterator& params, int numParams, Material* pMat, Material::TextureLayer* pTex) {
  // Determine which form it is
  // Must have at least 3 params though
  if (numParams < 4) {
    LogManager::getSingleton().logMessage("Bad " + params[0] + " attribute line in "
                                          + pMat->getName() + ", wrong number of parameters (expected at least 3)");
    return;
  }
  if (numParams == 4 && atoi(params[2].c_str()) != 0 ) {
    // First form using base name & number of frames
    pTex->setAnimatedTextureName(params[1], atoi(params[2].c_str()), atof(params[3].c_str()));
  } else {
    // Second form using individual names
    // Can use params[1] as array start point
    pTex->setAnimatedTextureName((String*)&params[1], numParams-2, atof(params[numParams-1].c_str()));
  }

}
//-----------------------------------------------------------------------
void parseCubicTexture(StringVector::iterator& params, int numParams, Material* pMat, Material::TextureLayer* pTex) {

  // Get final param
  bool useUVW;
  String uvOpt = StringToLowerASCII(params[numParams-1]);
  if (uvOpt == "combineduvw")
    useUVW = true;
  else if (uvOpt == "separateuv")
    useUVW = false;
  else {
    LogManager::getSingleton().logMessage("Bad " + params[0] + " attribute line in "
                                          + pMat->getName() + ", final parameter must be 'combinedUVW' or 'separateUV'.");
    return;
  }
  // Determine which form it is
  if (numParams == 3) {
    // First form using base name
    pTex->setCubicTextureName(params[1], useUVW);
  } else if (numParams == 8) {
    // Second form using individual names
    // Can use params[1] as array start point
    pTex->setCubicTextureName((String*)&params[1], useUVW);
  } else {
    LogManager::getSingleton().logMessage("Bad " + params[0] + " attribute line in "
                                          + pMat->getName() + ", wrong number of parameters (expected 2 or 7)");
    return;
  }

}
//-----------------------------------------------------------------------
void parseTexCoord(StringVector::iterator& params, int numParams, Material* pMat, Material::TextureLayer* pTex) {
  if (numParams != 2) {
    LogManager::getSingleton().logMessage("Bad " + params[0] + " attribute line in "
                                          + pMat->getName() + ", wrong number of parameters (expected 1)");
    return;
  }
  pTex->setTextureCoordSet(atoi(params[1].c_str()));

}
//-----------------------------------------------------------------------
void parseTexAddressMode(StringVector::iterator& params, int numParams, Material* pMat, Material::TextureLayer* pTex) {
  if (numParams != 2) {
    LogManager::getSingleton().logMessage("Bad " + params[0] + " attribute line in "
                                          + pMat->getName() + ", wrong number of parameters (expected 1)");
    return;
  }
  if (params[1]=="wrap")
    pTex->setTextureAddressingMode(Material::TextureLayer::TAM_WRAP);
  else if (params[1]=="mirror")
    pTex->setTextureAddressingMode(Material::TextureLayer::TAM_MIRROR);
  else if (params[1]=="clamp")
    pTex->setTextureAddressingMode(Material::TextureLayer::TAM_CLAMP);
  else
    LogManager::getSingleton().logMessage("Bad " + params[0] + " attribute line in "
                                          + pMat->getName() + ", valid parameters are 'wrap', 'clamp' or 'mirror'.");

}
//-----------------------------------------------------------------------
void parseColourOp(StringVector::iterator& params, int numParams, Material* pMat, Material::TextureLayer* pTex) {
  if (numParams != 2) {
    LogManager::getSingleton().logMessage("Bad " + params[0] + " attribute line in "
                                          + pMat->getName() + ", wrong number of parameters (expected 1)");
    return;
  }
  if (params[1]=="replace")
    pTex->setColourOperation(LBO_REPLACE);
  else if (params[1]=="add")
    pTex->setColourOperation(LBO_ADD);
  else if (params[1]=="modulate")
    pTex->setColourOperation(LBO_MODULATE);
  else if (params[1]=="alpha_blend")
    pTex->setColourOperation(LBO_ALPHA_BLEND);
  else
    LogManager::getSingleton().logMessage("Bad " + params[0] + " attribute line in "
                                          + pMat->getName() + ", valid parameters are 'replace', 'add', 'modulate' or 'alpha_blend'.");
}
//-----------------------------------------------------------------------
void parseAlphaRejection(StringVector::iterator& params, int numParams, Material* pMat, Material::TextureLayer* pTex) {
  if (numParams != 3) {
    LogManager::getSingleton().logMessage("Bad " + params[0] + " attribute line in "
                                          + pMat->getName() + ", wrong number of parameters (expected 2)");
    return;
  }

  CompareFunction cmp;
  try {
    cmp = convertCompareFunction(params[1]);
  } catch (...) {
    LogManager::getSingleton().logMessage("Bad " + params[0] + " attribute line in "
                                          + pMat->getName() + ", invalid compare function.");
    return;
  }

  pTex->setAlphaRejectSettings(cmp, atoi(params[2].c_str()));

}
//-----------------------------------------------------------------------
LayerBlendOperationEx convertBlendOpEx(const String& param) {
  if (param == "source1")
    return LBX_SOURCE1;
  else if (param == "source2")
    return LBX_SOURCE2;
  else if (param == "modulate")
    return LBX_MODULATE;
  else if (param == "modulate_x2")
    return LBX_MODULATE_X2;
  else if (param == "modulate_x4")
    return LBX_MODULATE_X4;
  else if (param == "add")
    return LBX_ADD;
  else if (param == "add_signed")
    return LBX_ADD_SIGNED;
  else if (param == "add_smooth")
    return LBX_ADD_SMOOTH;
  else if (param == "subtract")
    return LBX_SUBTRACT;
  else if (param == "blend_diffuse_alpha")
    return LBX_BLEND_DIFFUSE_ALPHA;
  else if (param == "blend_texture_alpha")
    return LBX_BLEND_TEXTURE_ALPHA;
  else if (param == "blend_current_alpha")
    return LBX_BLEND_CURRENT_ALPHA;
  else if (param == "blend_manual")
    return LBX_BLEND_MANUAL;
  else if (param == "dotproduct")
    return LBX_DOTPRODUCT;
  else
    Except(Exception::ERR_INVALIDPARAMS, "Invalid blend function", "convertBlendOpEx");
}
//-----------------------------------------------------------------------
LayerBlendSource convertBlendSource(const String& param) {
  if (param == "src_current")
    return LBS_CURRENT;
  else if (param == "src_texture")
    return LBS_TEXTURE;
  else if (param == "src_diffuse")
    return LBS_DIFFUSE;
  else if (param == "src_specular")
    return LBS_SPECULAR;
  else if (param == "src_manual")
    return LBS_MANUAL;
  else
    Except(Exception::ERR_INVALIDPARAMS, "Invalid blend source", "convertBlendSource");
}
//-----------------------------------------------------------------------
void parseLayerFiltering(StringVector::iterator& params, int numParams, Material* pMat, Material::TextureLayer* pTex) {
  if (numParams != 2) {
    LogManager::getSingleton().logMessage("Bad " + params[0] + " attribute line in "
                                          + pMat->getName() + ", wrong number of parameters (expected 1)");
    return;
  }
  if (params[1]=="none")
    pTex->setTextureLayerFiltering(TFO_NONE);
  else if (params[1]=="bilinear")
    pTex->setTextureLayerFiltering(TFO_BILINEAR);
  else if (params[1]=="trilinear")
    pTex->setTextureLayerFiltering(TFO_TRILINEAR);
  else if (params[1]=="anisotropic")
    pTex->setTextureLayerFiltering(TFO_ANISOTROPIC);
  else
    LogManager::getSingleton().logMessage("Bad texture layer filtering attribute line in "
                                          + pMat->getName() + ", valid parameters are 'none', 'bilinear', 'trilinear' or 'anisotropic'.");
}
//-----------------------------------------------------------------------
void parseColourOpEx(StringVector::iterator& params, int numParams, Material* pMat, Material::TextureLayer* pTex) {
  if (numParams < 4 || numParams > 13) {
    LogManager::getSingleton().logMessage("Bad " + params[0] + " attribute line in "
                                          + pMat->getName() + ", wrong number of parameters (expected 3 to 10)");
    return;
  }
  LayerBlendOperationEx op;
  LayerBlendSource src1, src2;
  Real manual = 0.0;
  ColourValue colSrc1 = ColourValue::White;
  ColourValue colSrc2 = ColourValue::White;

  try {
    op = convertBlendOpEx(params[1]);
    src1 = convertBlendSource(params[2]);
    src2 = convertBlendSource(params[3]);

    if (op == LBX_BLEND_MANUAL) {
      if (numParams < 5) {
        LogManager::getSingleton().logMessage("Bad " + params[0] + " attribute line in "
                                              + pMat->getName() + ", wrong number of parameters (expected 4 for manual blend)");
        return;
      }
      manual = atof(params[4].c_str());
    }

    if (src1 == LBS_MANUAL) {
      int parIndex = 4;
      if (op == LBX_BLEND_MANUAL)
        parIndex++;

      if (numParams < parIndex + 3) {
        LogManager::getSingleton().logMessage("Bad " + params[0] + " attribute line in "
                                              + pMat->getName() + ", wrong number of parameters (expected " + StringConverter::toString(parIndex + 2) + ")");
        return;
      }

      colSrc1.r = atof(params[parIndex++].c_str());
      colSrc1.g = atof(params[parIndex++].c_str());
      colSrc1.b = atof(params[parIndex].c_str());
    }

    if (src2 == LBS_MANUAL) {
      int parIndex = 4;
      if (op == LBX_BLEND_MANUAL)
        parIndex++;
      if (src1 == LBS_MANUAL)
        parIndex += 3;

      if (numParams < parIndex + 3) {
        LogManager::getSingleton().logMessage("Bad " + params[0] + " attribute line in "
                                              + pMat->getName() + ", wrong number of parameters (expected " + StringConverter::toString(parIndex + 2) + ")");
        return;
      }

      colSrc2.r = atof(params[parIndex++].c_str());
      colSrc2.g = atof(params[parIndex++].c_str());
      colSrc2.b = atof(params[parIndex].c_str());
    }
  } catch (Exception& e) {
    LogManager::getSingleton().logMessage("Bad " + params[0] + " attribute line in "
                                          + pMat->getName() + ", " + e.getFullDescription());
    return;
  }

  pTex->setColourOperationEx(op, src1, src2, colSrc1, colSrc2, manual);
}
//-----------------------------------------------------------------------
void parseColourOpFallback(StringVector::iterator& params, int numParams, Material* pMat, Material::TextureLayer* pTex) {
  if (numParams != 3) {
    LogManager::getSingleton().logMessage("Bad " + params[0] + " attribute line in "
                                          + pMat->getName() + ", wrong number of parameters (expected 2)");
    return;
  }

  //src/dest
  SceneBlendFactor src, dest;

  try {
    src = convertBlendFactor(params[1]);
    dest = convertBlendFactor(params[2]);
    pTex->setColourOpMultipassFallback(src,dest);
  } catch (Exception& e) {
    LogManager::getSingleton().logMessage("Bad "+ params[0] +" attribute line in "
                                          + pMat->getName() + ", " + e.getFullDescription());
  }
}
//-----------------------------------------------------------------------
void parseAlphaOpEx(StringVector::iterator& params, int numParams, Material* pMat, Material::TextureLayer* pTex) {
  if (numParams < 4 || numParams > 7) {
    LogManager::getSingleton().logMessage("Bad " + params[0] + " attribute line in "
                                          + pMat->getName() + ", wrong number of parameters (expected 3 or 4)");
    return;
  }
  LayerBlendOperationEx op;
  LayerBlendSource src1, src2;
  Real manual = 0.0;
  Real arg1 = 1.0, arg2 = 1.0;

  try {
    op = convertBlendOpEx(params[1]);
    src1 = convertBlendSource(params[2]);
    src2 = convertBlendSource(params[3]);
    if (op == LBX_BLEND_MANUAL) {
      if (numParams != 5) {
        LogManager::getSingleton().logMessage("Bad " + params[0] + " attribute line in "
                                              + pMat->getName() + ", wrong number of parameters (expected 4 for manual blend)");
        return;
      }
      manual = atof(params[4].c_str());
    }
    if (src1 == LBS_MANUAL) {
      int parIndex = 4;
      if (op == LBX_BLEND_MANUAL)
        parIndex++;

      if (numParams < parIndex) {
        LogManager::getSingleton().logMessage("Bad " + params[0] + " attribute line in "
                                              + pMat->getName() + ", wrong number of parameters (expected " + StringConverter::toString(parIndex - 1) + ")");
        return;
      }

      arg1 = atof(params[parIndex].c_str());
    }

    if (src2 == LBS_MANUAL) {
      int parIndex = 4;
      if (op == LBX_BLEND_MANUAL)
        parIndex++;
      if (src1 == LBS_MANUAL)
        parIndex++;

      if (numParams < parIndex) {
        LogManager::getSingleton().logMessage("Bad " + params[0] + " attribute line in "
                                              + pMat->getName() + ", wrong number of parameters (expected " + StringConverter::toString(parIndex - 1) + ")");
        return;
      }

      arg2 = atof(params[parIndex].c_str());
    }
  } catch (Exception& e) {
    LogManager::getSingleton().logMessage("Bad " + params[0] + " attribute line in "
                                          + pMat->getName() + ", " + e.getFullDescription());
    return;
  }

  pTex->setAlphaOperation(op, src1, src2, arg1, arg2, manual);
}
//-----------------------------------------------------------------------
void parseEnvMap(StringVector::iterator& params, int numParams, Material* pMat, Material::TextureLayer* pTex) {
  if (numParams != 2) {
    LogManager::getSingleton().logMessage("Bad " + params[0] + " attribute line in "
                                          + pMat->getName() + ", wrong number of parameters (expected 2)");
    return;
  }
  if (params[1]=="off")
    pTex->setEnvironmentMap(false);
  else if (params[1]=="spherical")
    pTex->setEnvironmentMap(true, Material::TextureLayer::ENV_CURVED);
  else if (params[1]=="planar")
    pTex->setEnvironmentMap(true, Material::TextureLayer::ENV_PLANAR);
  else if (params[1]=="cubic_reflection")
    pTex->setEnvironmentMap(true, Material::TextureLayer::ENV_REFLECTION);
  else if (params[1]=="cubic_normal")
    pTex->setEnvironmentMap(true, Material::TextureLayer::ENV_NORMAL);
  else
    LogManager::getSingleton().logMessage("Bad " + params[0] + " attribute line in "
                                          + pMat->getName() + ", valid parameters are 'off', 'spherical', 'planar', 'cubic_reflection' and 'cubic_normal'.");

}
//-----------------------------------------------------------------------
void parseScroll(StringVector::iterator& params, int numParams, Material* pMat, Material::TextureLayer* pTex) {
  if (numParams != 3) {
    LogManager::getSingleton().logMessage("Bad " + params[0] + " attribute line in "
                                          + pMat->getName() + ", wrong number of parameters (expected 3)");
    return;
  }
  if (params[0]=="scroll") {
    pTex->setTextureScroll(atof(params[1].c_str()), atof(params[2].c_str()));
  } else { // scroll_anim
    pTex->setScrollAnimation(atof(params[1].c_str()), atof(params[2].c_str()));
  }
}
//-----------------------------------------------------------------------
void parseRotate(StringVector::iterator& params, int numParams, Material* pMat, Material::TextureLayer* pTex) {
  if (numParams != 2) {
    LogManager::getSingleton().logMessage("Bad " + params[0] + " attribute line in "
                                          + pMat->getName() + ", wrong number of parameters (expected 2)");
    return;
  }
  if (params[0]=="rotate") {
    pTex->setTextureRotate(atof(params[1].c_str()));
  } else { // rotate_anim
    pTex->setRotateAnimation(atof(params[1].c_str()));
  }
}
//-----------------------------------------------------------------------
void parseScale(StringVector::iterator& params, int numParams, Material* pMat, Material::TextureLayer* pTex) {
  if (numParams != 3) {
    LogManager::getSingleton().logMessage("Bad " + params[0] + " attribute line in "
                                          + pMat->getName() + ", wrong number of parameters (expected 3)");
    return;
  }
  pTex->setTextureScale(atof(params[1].c_str()), atof(params[2].c_str()) );
}
//-----------------------------------------------------------------------
void parseWaveXform(StringVector::iterator& params, int numParams, Material* pMat, Material::TextureLayer* pTex) {
  if (numParams != 7) {
    LogManager::getSingleton().logMessage("Bad " + params[0] + " attribute line in "
                                          + pMat->getName() + ", wrong number of parameters (expected 6)");
    return;
  }
  Material::TextureLayer::TextureTransformType ttype;
  WaveformType waveType;
  // Check transform type
  if (params[1]=="scroll_x")
    ttype = Material::TextureLayer::TT_TRANSLATE_U;
  else if (params[1]=="scroll_y")
    ttype = Material::TextureLayer::TT_TRANSLATE_V;
  else if (params[1]=="rotate")
    ttype = Material::TextureLayer::TT_ROTATE;
  else if (params[1]=="scale_x")
    ttype = Material::TextureLayer::TT_SCALE_U;
  else if (params[1]=="scale_y")
    ttype = Material::TextureLayer::TT_SCALE_V;
  else {
    LogManager::getSingleton().logMessage("Bad " + params[0] + " attribute line in "
                                          + pMat->getName() + ", parameter 1 must be 'scroll_x', 'scroll_y', 'rotate', 'scale_x' or 'scale_y'");
    return;
  }
  // Check wave type
  if (params[2]=="sine")
    waveType = WFT_SINE;
  else if (params[2]=="triangle")
    waveType = WFT_TRIANGLE;
  else if (params[2]=="square")
    waveType = WFT_SQUARE;
  else if (params[2]=="sawtooth")
    waveType = WFT_SAWTOOTH;
  else if (params[2]=="inverse_sawtooth")
    waveType = WFT_INVERSE_SAWTOOTH;
  else {
    LogManager::getSingleton().logMessage("Bad " + params[0] + " attribute line in "
                                          + pMat->getName() + ", parameter 2 must be 'sine', 'triangle', 'square', 'sawtooth' or 'inverse_sawtooth'");
    return;
  }

  pTex->setTransformAnimation(ttype, waveType, atof(params[3].c_str()), atof(params[4].c_str()),
                              atof(params[5].c_str()), atof(params[6].c_str()) );

}
//-----------------------------------------------------------------------
void parseDepthBias(StringVector::iterator& params, int numParams, Material* pMat) {
  if (numParams != 2) {
    LogManager::getSingleton().logMessage("Bad " + params[0] + " attribute line in "
                                          + pMat->getName() + ", wrong number of parameters (expected 2)");
    return;
  }
  pMat->setDepthBias(atoi(params[1].c_str()));
}
//-----------------------------------------------------------------------
void parseAnisotropy(StringVector::iterator& params, int numParams, Material* pMat) {
  if (numParams != 2) {
    LogManager::getSingleton().logMessage("Bad " + params[0] + " attribute line in "
                                          + pMat->getName() + ", wrong number of parameters (expected 2)");
    return;
  }
  pMat->setAnisotropy(atoi(params[1].c_str()));
}
//-----------------------------------------------------------------------
void parseLayerAnisotropy(StringVector::iterator& params, int numParams, Material* pMat, Material::TextureLayer* pTex) {
  if (numParams != 2) {
    LogManager::getSingleton().logMessage("Bad " + params[0] + " attribute line in "
                                          + pMat->getName() + ", wrong number of parameters (expected 2)");
    return;
  }
  pTex->setTextureAnisotropy(atoi(params[1].c_str()));
}
//-----------------------------------------------------------------------
template<> MaterialManager* Singleton<MaterialManager>::ms_Singleton = 0;
//-----------------------------------------------------------------------
MaterialManager::MaterialManager() {
  mDefTextureFiltering = TFO_BILINEAR;
  mDefAniso = 1;

  // Set up default material - don't use name contructor as we want to avoid applying defaults
  Material::mDefaultSettings = new Material();
  Material::mDefaultSettings->mName = "DefaultSettings";

  // Set up a base white material
  this->create("BaseWhite");

  // Set up material attribute parsers
  mMatAttribParsers.insert(MatAttribParserList::value_type("ambient", (MATERIAL_ATTRIB_PARSER)parseAmbient));
  mMatAttribParsers.insert(MatAttribParserList::value_type("diffuse", (MATERIAL_ATTRIB_PARSER)parseDiffuse));
  mMatAttribParsers.insert(MatAttribParserList::value_type("specular", (MATERIAL_ATTRIB_PARSER)parseSpecular));
  mMatAttribParsers.insert(MatAttribParserList::value_type("emissive", (MATERIAL_ATTRIB_PARSER)parseEmissive));
  mMatAttribParsers.insert(MatAttribParserList::value_type("scene_blend", (MATERIAL_ATTRIB_PARSER)parseSceneBlend));
  mMatAttribParsers.insert(MatAttribParserList::value_type("depth_check", (MATERIAL_ATTRIB_PARSER)parseDepthParams));
  mMatAttribParsers.insert(MatAttribParserList::value_type("depth_write", (MATERIAL_ATTRIB_PARSER)parseDepthParams));
  mMatAttribParsers.insert(MatAttribParserList::value_type("depth_func", (MATERIAL_ATTRIB_PARSER)parseDepthParams));
  mMatAttribParsers.insert(MatAttribParserList::value_type("cull_hardware", (MATERIAL_ATTRIB_PARSER)parseCullMode));
  mMatAttribParsers.insert(MatAttribParserList::value_type("cull_software", (MATERIAL_ATTRIB_PARSER)parseCullMode));
  mMatAttribParsers.insert(MatAttribParserList::value_type("lighting", (MATERIAL_ATTRIB_PARSER)parseLighting));
  mMatAttribParsers.insert(MatAttribParserList::value_type("fog_override", (MATERIAL_ATTRIB_PARSER)parseFogging));
  mMatAttribParsers.insert(MatAttribParserList::value_type("shading", (MATERIAL_ATTRIB_PARSER)parseShading));
  mMatAttribParsers.insert(MatAttribParserList::value_type("filtering", (MATERIAL_ATTRIB_PARSER)parseFiltering));
  mMatAttribParsers.insert(MatAttribParserList::value_type("depth_bias", (MATERIAL_ATTRIB_PARSER)parseDepthBias));
  mMatAttribParsers.insert(MatAttribParserList::value_type("anisotropy", (MATERIAL_ATTRIB_PARSER)parseAnisotropy));

  // Set up layer attribute parsers
  mLayerAttribParsers.insert(LayerAttribParserList::value_type("texture", (TEXLAYER_ATTRIB_PARSER)parseTexture));
  mLayerAttribParsers.insert(LayerAttribParserList::value_type("anim_texture", (TEXLAYER_ATTRIB_PARSER)parseAnimTexture));
  mLayerAttribParsers.insert(LayerAttribParserList::value_type("cubic_texture", (TEXLAYER_ATTRIB_PARSER)parseCubicTexture));
  mLayerAttribParsers.insert(LayerAttribParserList::value_type("tex_coord_set", (TEXLAYER_ATTRIB_PARSER)parseTexCoord));
  mLayerAttribParsers.insert(LayerAttribParserList::value_type("tex_address_mode", (TEXLAYER_ATTRIB_PARSER)parseTexAddressMode));
  mLayerAttribParsers.insert(LayerAttribParserList::value_type("colour_op", (TEXLAYER_ATTRIB_PARSER)parseColourOp));
  mLayerAttribParsers.insert(LayerAttribParserList::value_type("alpha_rejection", (TEXLAYER_ATTRIB_PARSER)parseAlphaRejection));
  mLayerAttribParsers.insert(LayerAttribParserList::value_type("colour_op_ex", (TEXLAYER_ATTRIB_PARSER)parseColourOpEx));
  mLayerAttribParsers.insert(LayerAttribParserList::value_type("colour_op_multipass_fallback", (TEXLAYER_ATTRIB_PARSER)parseColourOpFallback));
  mLayerAttribParsers.insert(LayerAttribParserList::value_type("alpha_op_ex", (TEXLAYER_ATTRIB_PARSER)parseAlphaOpEx));
  mLayerAttribParsers.insert(LayerAttribParserList::value_type("env_map", (TEXLAYER_ATTRIB_PARSER)parseEnvMap));
  mLayerAttribParsers.insert(LayerAttribParserList::value_type("scroll", (TEXLAYER_ATTRIB_PARSER)parseScroll));
  mLayerAttribParsers.insert(LayerAttribParserList::value_type("scroll_anim", (TEXLAYER_ATTRIB_PARSER)parseScroll));
  mLayerAttribParsers.insert(LayerAttribParserList::value_type("rotate", (TEXLAYER_ATTRIB_PARSER)parseRotate));
  mLayerAttribParsers.insert(LayerAttribParserList::value_type("rotate_anim", (TEXLAYER_ATTRIB_PARSER)parseRotate));
  mLayerAttribParsers.insert(LayerAttribParserList::value_type("scale", (TEXLAYER_ATTRIB_PARSER)parseScale));
  mLayerAttribParsers.insert(LayerAttribParserList::value_type("wave_xform", (TEXLAYER_ATTRIB_PARSER)parseWaveXform));
  mLayerAttribParsers.insert(LayerAttribParserList::value_type("tex_filtering", (TEXLAYER_ATTRIB_PARSER)parseLayerFiltering));
  mLayerAttribParsers.insert(LayerAttribParserList::value_type("tex_anisotropy", (TEXLAYER_ATTRIB_PARSER)parseLayerAnisotropy));
}
//-----------------------------------------------------------------------
MaterialManager::~MaterialManager() {
  delete Material::mDefaultSettings;
  // Resources cleared by superclass
}
//-----------------------------------------------------------------------
void MaterialManager::parseScript(DataChunk& chunk) {
  String line;
  Material* pMat;
  char tempBuf[512];

  pMat = 0;

  while(!chunk.isEOF()) {
    line = chunk.getLine();
    // Ignore comments & blanks
    if (!(line.length() == 0 || line.substr(0,2) == "//")) {
      if (pMat == 0) {
        // No current material
        // So first valid data should be a material name
        // NB defer loading until later
        pMat = (Material*)createDeferred(line);
        // Skip to and over next {
        chunk.readUpTo(tempBuf, 511, "{");
      } else {
        // Already in a material
        if (line == "}") {
          // Finished material
          pMat = 0;
        } else if (line == "{") {
          // new pass
          parseNewTextureLayer(chunk, pMat);

        } else {
          // Attribute
          parseAttrib(StringToLowerASCII(line), pMat);
        }

      }

    }


  }

}
//-----------------------------------------------------------------------
void MaterialManager::parseAllSources(const String& extension) {
  StringVector materialFiles;
  DataChunk* pChunk;

  std::vector<ArchiveEx*>::iterator i = mVFS.begin();

  // Specific archives
  for (; i != mVFS.end(); ++i) {
    materialFiles = (*i)->getAllNamesLike( "./", extension);
    for (StringVector::iterator si = materialFiles.begin(); si != materialFiles.end(); ++si) {
      SDDataChunk dat;
      pChunk = &dat;
      (*i)->fileRead(si[0], &pChunk );
      LogManager::getSingleton().logMessage("Parsing material script: " + si[0]);
      parseScript(dat);
    }

  }
  // search common archives
  for (i = mCommonVFS.begin(); i != mCommonVFS.end(); ++i) {
    materialFiles = (*i)->getAllNamesLike( "./", extension);
    for (StringVector::iterator si = materialFiles.begin(); si != materialFiles.end(); ++si) {
      SDDataChunk dat;
      pChunk = &dat;
      (*i)->fileRead(si[0], &pChunk );
      LogManager::getSingleton().logMessage("Parsing material script: " + si[0]);
      parseScript(dat);
    }
  }


}
//-----------------------------------------------------------------------
Resource* MaterialManager::create( const String& name) {
  // Check name not already used
  if (getByName(name) != 0)
    Except(Exception::ERR_DUPLICATE_ITEM, "Material " + name + " already exists.",
           "MaterialManager::create");

  Material* m = new Material(name);
  m->assignNextHandle();
  // Load immediately
  m->load();

  mResources[name] = m;
  mHandles.insert(MaterialHandleList::value_type(m->mHandle, m));


  return m;
}
//-----------------------------------------------------------------------
void MaterialManager::parseNewTextureLayer(DataChunk& chunk, Material* pMat) {
  String line;
  Material::TextureLayer* pLayer;

  pLayer = pMat->addTextureLayer("");


  while (!chunk.isEOF()) {
    line = chunk.getLine();
    // Ignore comments & blanks
    if (line.length() != 0 && !(line.substr(0,2) == "//")) {
      if (line == "}") {
        // end of layer
        return;
      } else {
        parseLayerAttrib(line, pMat, pLayer);
      }
    }


  }
}
//-----------------------------------------------------------------------
void MaterialManager::parseAttrib( const String& line, Material* pMat) {
  StringVector vecparams;

  // Split params on space
  base::SplitString(line, &vecparams, " \t");
  StringVector::iterator params = vecparams.begin();

  // Look up first param (command setting)
  MatAttribParserList::iterator iparsers = mMatAttribParsers.find(params[0]);
  if (iparsers == mMatAttribParsers.end()) {
    // BAD command. BAD!
    LogManager::getSingleton().logMessage(
      "Bad material attribute line: '"
      + line + "' in " + pMat->getName() +
      ", unknown command '" + params[0] + "'");
  } else {
    // Use parser
    iparsers->second(
      params, static_cast< int >( vecparams.size() ), pMat );
  }


}
//-----------------------------------------------------------------------
void MaterialManager::parseLayerAttrib( const String& line, Material* pMat, Material::TextureLayer* pLayer) {
  StringVector vecparams;

  // Split params on space
  base::SplitString(line, &vecparams, " \t");
  StringVector::iterator params = vecparams.begin();

  // Look up first param (command setting)
  params[0] = StringToLowerASCII(params[0]);
  LayerAttribParserList::iterator iparsers = mLayerAttribParsers.find(params[0]);
  if (iparsers == mLayerAttribParsers.end()) {
    // BAD command. BAD!
    LogManager::getSingleton().logMessage("Bad texture layer attribute line: '"
                                          + line + "' in " + pMat->getName() + ", unknown command '" + params[0] + "'");
  } else {
    // Use parser
    if (params[0] != "texture" && params[0] != "cubic_texture" && params[0] != "anim_texture") {
      // Lower case all params if not texture
      for( size_t p = 1; p < vecparams.size(); ++p )
        params[p] = StringToLowerASCII(params[p]);

    }
    iparsers->second(params, (unsigned int)vecparams.size(), pMat, pLayer);
  }



}
//-----------------------------------------------------------------------
MaterialManager& MaterialManager::getSingleton(void) {
  return Singleton<MaterialManager>::getSingleton();
}
//-----------------------------------------------------------------------
Material* MaterialManager::add(const Material& mat) {
  // Check no material with that name already
  String name = mat.getName();
  Material* oldMat = (Material*)getByName(name);
  if (oldMat != 0) {
    Except(Exception::ERR_DUPLICATE_ITEM,
           "Material cannot be added since a "
           "Material already exists with the name " + name,
           "MaterialManager::add");
  }
  // Copy Material
  Material* newMat = new Material();
  *newMat = mat;
  if (newMat->getHandle() == -1)
    newMat->assignNextHandle();
  // Load immediately
  newMat->load();

  mResources[name] = newMat;
  mHandles.insert(MaterialHandleList::value_type(newMat->mHandle, newMat));

  return newMat;
}
//-----------------------------------------------------------------------
Material* MaterialManager::getByHandle(int handle) {
  MaterialHandleList::iterator i = mHandles.find(handle);

  if (i == mHandles.end()) {
    return 0;
  } else {
    return i->second;
  }
}
//-----------------------------------------------------------------------
Material* MaterialManager::createDeferred( const String& name) {
  // Check name not already used
  if (getByName(name) != 0)
    Except(Exception::ERR_DUPLICATE_ITEM, "Material " + name + " already exists.",
           "MaterialManager::create");

  Material* m = new Material(name, true);
  m->assignNextHandle();
  // Defer load until used

  mResources[name] = m;
  mHandles.insert(MaterialHandleList::value_type(m->mHandle, m));

  return m;
}

void MaterialManager::setDefaultTextureFiltering(TextureFilterOptions fo) {
  mDefTextureFiltering = fo;
}

TextureFilterOptions MaterialManager::getDefaultTextureFiltering() {
  return mDefTextureFiltering;
}

void MaterialManager::setDefaultAnisotropy(int maxAniso) {
  mDefAniso = maxAniso;
}

int MaterialManager::getDefaultAnisotropy() {
  return mDefAniso;
}
}
