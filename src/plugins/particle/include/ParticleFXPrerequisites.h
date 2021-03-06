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
#ifndef __SfxPluginPrerequisites_H__
#define __SfxPluginPrerequisites_H__

#include "Prerequisites.h"

//-----------------------------------------------------------------------
// Windows Settings
//-----------------------------------------------------------------------
#if OGRE_PLATFORM == PLATFORM_WIN32
#   if OGRE_DYNAMIC_LINKAGE == 0
#       pragma warn( "No dynamic linkage" )
#       define _ParticleFXExport
#   else
#       ifdef PLUGIN_ParticleFX_EXPORTS
#           define _ParticleFXExport __declspec(dllexport)
#       else
#           define _ParticleFXExport __declspec(dllimport)
#       endif
#   endif
#else
#   define _ParticleFXExport
#endif


namespace renderer {

// Predeclare classes
class PointEmitter;
class PointEmitterFactory;
class LinearForceAffector;
class LinearForceAffectorFactory;



}


#endif
