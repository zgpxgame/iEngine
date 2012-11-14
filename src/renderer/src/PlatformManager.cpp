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
#include "PlatformManager.h"

#include "DynLibManager.h"
#include "DynLib.h"

namespace renderer {

    //-----------------------------------------------------------------------
    template<> PlatformManager* Singleton<PlatformManager>::ms_Singleton = 0;
    //-----------------------------------------------------------------------
    PlatformManager::PlatformManager()
    {
        // Load library
        String libraryName = "platform_manager";
        String debug_suffix;
#ifdef _DEBUG
        debug_suffix = "_d";
#endif
        #if OGRE_PLATFORM == PLATFORM_WIN32
            libraryName = libraryName + "_win32" + debug_suffix + ".dll";
        #else
            libraryName = "lib" + libraryName + debug_suffix + ".so";
        #endif

        DynLib* lib = DynLibManager::getSingleton().load(libraryName);

        mpfCreateConfigDialog = (DLL_CREATECONFIGDIALOG)lib->getSymbol("createPlatformConfigDialog");
        mpfCreateErrorDialog = (DLL_CREATEERRORDIALOG)lib->getSymbol("createPlatformErrorDialog");
		mpfCreateTimer = (DLL_CREATETIMER)lib->getSymbol("createTimer");

        mpfDestroyConfigDialog = (DLL_DESTROYCONFIGDIALOG)lib->getSymbol("destroyPlatformConfigDialog");
        mpfDestroyErrorDialog = (DLL_DESTROYERRORDIALOG)lib->getSymbol("destroyPlatformErrorDialog");
        mpfDestroyTimer = (DLL_DESTROYTIMER)lib->getSymbol("destroyTimer");

    }
    //-----------------------------------------------------------------------
    ConfigDialog* PlatformManager::createConfigDialog()
    {
        // Delegate
        ConfigDialog* pdlg;
        mpfCreateConfigDialog(&pdlg);
        return pdlg;
    }
    //-----------------------------------------------------------------------
    ErrorDialog* PlatformManager::createErrorDialog()
    {
        // Delegate
        ErrorDialog* pdlg;
        mpfCreateErrorDialog(&pdlg);
        return pdlg;
    }
    //-----------------------------------------------------------------------

    //-----------------------------------------------------------------------
    void PlatformManager::destroyConfigDialog(ConfigDialog*  dlg)
    {
        // Delegate
        mpfDestroyConfigDialog(dlg);
    }
    //-----------------------------------------------------------------------
    void PlatformManager::destroyErrorDialog(ErrorDialog* dlg)
    {
        // Delegate
        mpfDestroyErrorDialog(dlg);
    }

    //-----------------------------------------------------------------------
    Timer* PlatformManager::createTimer()
    {
        // Delegate
        Timer* pTimer;
        mpfCreateTimer(&pTimer);
        return pTimer;
    }
    //-----------------------------------------------------------------------
    PlatformManager& PlatformManager::getSingleton(void)
    {
        return Singleton<PlatformManager>::getSingleton();
    }
    //-----------------------------------------------------------------------
    void PlatformManager::destroyTimer(Timer* timer)
    {
        mpfDestroyTimer(timer);
    }



}
