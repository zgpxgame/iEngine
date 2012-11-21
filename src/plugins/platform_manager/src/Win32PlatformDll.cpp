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
#include "Win32ConfigDialog.h"
#include "Win32Timer.h"
#include "Root.h"
#include "LogManager.h"

namespace renderer {

#define EXPORT __declspec(dllexport)

#ifdef _DEBUG
    int g_iCreatedConfigDiag = 0;
    int g_iCreatedRenderWindow = 0;
    int g_iCreatedInputReader = 0;
#endif

    /// Retrieves an instance of a config dialog for this platform
    extern "C" EXPORT void createPlatformConfigDialog(ConfigDialog** ppDlg)
    {
        // Must get HISTANCE
        HINSTANCE hInst = GetModuleHandle("plugin_platform_manager.dll");
        *ppDlg = new Win32ConfigDialog(hInst);

#ifdef _DEBUG
        g_iCreatedConfigDiag++;
#endif
    }

	/// Creates a Timer using default implementation
	extern "C" EXPORT void createTimer(Timer** ppTimer)
	{
		*ppTimer = new Win32Timer();
        (*ppTimer)->Reset();
	}

	extern "C" EXPORT void destroyTimer(Timer* ppTimer)
	{
		delete ppTimer;
	}


    /// Destroys
    extern "C" EXPORT void destroyPlatformConfigDialog(ConfigDialog* dlg)
    {
        delete dlg;

#ifdef _DEBUG
        g_iCreatedConfigDiag--;
#endif
    }

    /// Destroys
    extern "C" EXPORT void destroyPlatformRenderWindow(RenderWindow* wnd)
    {
        delete wnd;

#ifdef _DEBUG
        g_iCreatedRenderWindow--;
#endif
    }

#if 0//def _DEBUG
    BOOL WINAPI DllMain( HINSTANCE hinstDLL,  // handle to DLL module
                         DWORD fdwReason,     // reason for calling function
                         LPVOID lpvReserved   // reserved
                       )
    {
        if( fdwReason == DLL_THREAD_DETACH ) {
            if( g_iCreatedConfigDiag )
                LogManager::logMessage( "Memory Leak: Not all platform configuration dialogs were destroyed!!!", LML_CRITICAL );
            if( g_iCreatedConfigDiag )
                LogManager::logMessage( "Memory Leak: Not all platform error dialogs were destroyed!!!", LML_CRITICAL );
            if( g_iCreatedConfigDiag )
                LogManager::logMessage( "Memory Leak: Not all platform render windows were destroyed!!!", LML_CRITICAL );
            if( g_iCreatedConfigDiag )
                LogManager::logMessage( "Memory Leak: Not all platform input readers were destroyed!!!", LML_CRITICAL );
        }
    }
#endif
}
