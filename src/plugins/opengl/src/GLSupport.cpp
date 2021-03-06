
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


#include "GLSupport.h"

namespace renderer {

template<> GLSupport* Singleton<GLSupport>::ms_Singleton = 0;

void GLSupport::setConfigOption(const String &name, const String &value) {
  ConfigOptionMap::iterator it = mOptions.find(name);

  if (it != mOptions.end())
    it->second.currentValue = value;
}

ConfigOptionMap& GLSupport::getConfigOptions(void) {
  return mOptions;
}

void GLSupport::initialiseExtensions(void) {
  // Set version string
  const GLubyte* pcVer = glGetString(GL_VERSION);

  assert(pcVer && "Problems getting GL version string using glGetString");

  if(pcVer) {
    String tmpStr = (const char*)pcVer;
    version = tmpStr.substr(0, tmpStr.find(" "));
  }

  // Set extension list
  std::stringstream ext;
  String str;

  const GLubyte* pcExt = glGetString(GL_EXTENSIONS);

  assert(pcExt && "Problems getting GL extension string using glGetString");

  if (pcExt) {

    ext << pcExt;

    while(ext >> str) {
      extensionList.insert(str);
    }
  }

  ext.str("");
}

bool GLSupport::checkMinGLVersion(const String& v) {
  unsigned int first, second, third;
  unsigned int cardFirst, cardSecond, cardThird;
  if(v == version)
    return true;

  String::size_type pos = v.find(".");
  if(pos == String::npos)
    return false;

  String::size_type pos1 = v.rfind(".");
  if(pos1 == String::npos)
    return false;

  first = ::atoi(v.substr(0, pos).c_str());
  second = ::atoi(v.substr(pos + 1, pos1 - (pos + 1)).c_str());
  third = ::atoi(v.substr(pos1 + 1, v.length()).c_str());

  pos = version.find(".");
  if(pos == String::npos)
    return false;

  pos1 = version.rfind(".");
  if(pos1 == String::npos)
    return false;

  cardFirst  = ::atoi(version.substr(0, pos).c_str());
  cardSecond = ::atoi(version.substr(pos + 1, pos1 - (pos + 1)).c_str());
  cardThird  = ::atoi(version.substr(pos1 + 1, version.length()).c_str());

  if(first <= cardFirst && second <= cardSecond && third <= cardThird)
    return true;

  return false;
}

bool GLSupport::checkExtension(const String& ext) {
  if(extensionList.find(ext) == extensionList.end())
    return false;

  return true;
}

GLSupport& GLSupport::getSingleton(void) {
  return Singleton<GLSupport>::getSingleton();
}

void* GLSupport::getProcAddress(const String& procname) {
  return (void*)wglGetProcAddress( procname.c_str() );
}

}
