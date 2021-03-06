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

#include "window_win32.h"
#include "renderer/include/Camera.h"
#include "renderer/include/LogManager.h"
#include "renderer/include/RenderSystem.h"
#include "renderer/include/ImageCodec.h"
#include "engine/engine.h"
#include <tchar.h>

using namespace renderer;

namespace engine {

OSWindow::OSWindow() {
  mIsFullScreen = false;
  mHWnd = 0;
  mActive = false;
  mReady = false;
  mClosed = false;
  mExternalHandle = NULL;
}

OSWindow::~OSWindow() {
  destroy();
}

void OSWindow::create(String name, int width, int height, int colourDepth,
                         bool fullScreen, int left, int top, bool depthBuffer) {
  //HWND parentHWnd;
  HINSTANCE hInst = GetModuleHandle(NULL);
  long tempPtr;

  // Get variable-length params
  // miscParam[0] = parent HWND
  // miscParam[1] = bool vsync
  // miscParam[2] = int displayFrequency

  //va_list marker;
  //va_start( marker, depthBuffer );

  //tempPtr = va_arg( marker, long );
  //Win32Window* parentRW = reinterpret_cast<Win32Window*>(tempPtr);
  //if( parentRW == NULL )
  //  parentHWnd = 0;
  //else
  //  parentHWnd = parentRW->getWindowHandle();

  //tempPtr = va_arg( marker, long );
  //bool vsync = static_cast<bool>(tempPtr);
  bool vsync = false;

  //tempPtr = va_arg( marker, long );
  //int displayFrequency = static_cast<int>(tempPtr);
  int displayFrequency = 0;
  //va_end( marker );

  // Destroy current window if any
  if( mHWnd )
    destroy();

  if (fullScreen) {
    mColourDepth = colourDepth;
    mLeft = 0;
    mTop = 0;
  } else {
    // Get colour depth from display
    mColourDepth = GetDeviceCaps(GetDC(0), BITSPIXEL);
    mTop = top;
    mLeft = left;
  }

  if (!mExternalHandle) {
    // Register the window class
    // NB allow 4 bytes of window data for Win32Window pointer
    WNDCLASS wndClass = { CS_HREDRAW | CS_VREDRAW, WndProc, 0, 4, hInst,
                          LoadIcon( NULL, _T("IDI_ICON1") ),
                          LoadCursor( NULL, IDC_ARROW ),
                          (HBRUSH)GetStockObject( BLACK_BRUSH ), NULL,
                          name.c_str()
                        };
    RegisterClass( &wndClass );

    // Create our main window
    // Pass pointer to self
    HWND hWnd = CreateWindowEx(fullScreen?WS_EX_TOPMOST:0, name.c_str(), name.c_str(),
                               (fullScreen?WS_POPUP:WS_OVERLAPPEDWINDOW)|WS_CLIPCHILDREN|WS_CLIPSIBLINGS, left, top,
                               width, height, 0L, 0L, hInst, this);

    mHWnd = hWnd;
    // Store info
    mWidth = width;
    mHeight = height;

    if (fullScreen) {
      DEVMODE DevMode;
      DevMode.dmSize = sizeof(DevMode);
      DevMode.dmBitsPerPel = mColourDepth;
      DevMode.dmPelsWidth = mWidth;
      DevMode.dmPelsHeight = mHeight;
      DevMode.dmDisplayFrequency = displayFrequency;
      DevMode.dmFields = DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT|DM_DISPLAYFREQUENCY;
      if (ChangeDisplaySettings(&DevMode, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
        LogManager::getSingleton().logMessage(LML_CRITICAL, "ChangeDisplaySettingsEx");
    }

  } else {
    mHWnd = mExternalHandle;
    RECT rc;
    GetClientRect(mHWnd, &rc);
    mWidth = rc.right;
    mHeight = rc.bottom;
  }
  ShowWindow(mHWnd, SW_SHOWNORMAL);
  UpdateWindow(mHWnd);
  mName = name;
  mIsDepthBuffered = depthBuffer;
  mIsFullScreen = fullScreen;

  /*if (!vsync) {
  	mOldSwapIntervall = wglGetSwapIntervalEXT();
  	wglSwapIntervalEXT(0);
  }
  else
  	mOldSwapIntervall = -1;*/
  HDC hdc = GetDC(mHWnd);

  LogManager::getSingleton().logMessage(
    LML_NORMAL, "Created Win32Window '%s' : %ix%i, %ibpp",
    mName.c_str(), mWidth, mHeight, mColourDepth );

  PIXELFORMATDESCRIPTOR pfd = {
    sizeof(PIXELFORMATDESCRIPTOR),
    1,
    PFD_DRAW_TO_WINDOW |
    PFD_SUPPORT_OPENGL |
    PFD_DOUBLEBUFFER,
    PFD_TYPE_RGBA,
    mColourDepth,
    0, 0, 0, 0, 0, 0,
    0,
    0,
    0,
    0, 0, 0, 0,
    32, 			     // 32-bit z-buffer
    0,				     // no stencil buffer
    0,
    PFD_MAIN_PLANE,
    0,
    0, 0, 0
  };
  int iPixelFormat = ChoosePixelFormat(hdc, &pfd);
  if (!iPixelFormat)
    Except(0, "ChoosePixelFormat failed", "Win32Window::create");
  if (!SetPixelFormat(hdc, iPixelFormat, &pfd))
    Except(0, "SetPixelFormat failed", "Win32Window::create");

  HGLRC glrc = wglCreateContext(hdc);
  if (!glrc)
    Except(0, "wglCreateContext", "Win32Window::create");
  if (!wglMakeCurrent(hdc, glrc))
    Except(0, "wglMakeCurrent", "Win32Window::create");

  mGlrc = glrc;
  mHDC = hdc;

  mReady = true;
}

void OSWindow::destroy(void) {
  if (mGlrc) {
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(mGlrc);
    mGlrc = NULL;
  }
  if (mHDC) {
    ReleaseDC(mHWnd, mHDC);
    mHDC = NULL;
  }
  /*if (mOldSwapIntervall >= 0)
  	wglSwapIntervalEXT(mOldSwapIntervall);*/
  if (mIsFullScreen)
    ChangeDisplaySettings(NULL, 0);
  DestroyWindow(mHWnd);
  mActive = false;
}

bool OSWindow::isActive() {
  return mActive;
}

bool OSWindow::isClosed() {
  return mClosed;
}

void OSWindow::reposition(int left, int top) {
  // XXX FIXME
}

void OSWindow::resize(int width, int height) {
  mWidth = width;
  mHeight = height;

  // Notify viewports of resize
  ViewportList::iterator it, itend;
  itend = mViewportList.end();
  for( it = mViewportList.begin(); it != itend; ++it ) {
    (*it).second->getCamera()->setAspectRatio(Real(width) / height);
    (*it).second->_updateDimensions();
  }
  // TODO - resize window
}

void OSWindow::WindowMovedOrResized() {
  RECT temprect;
  ::GetClientRect(getWindowHandle(), &temprect);
  resize(temprect.right - temprect.left, temprect.bottom - temprect.top);
}

void OSWindow::swapBuffers(bool waitForVSync) {
  SwapBuffers(mHDC);
}

void OSWindow::outputText(int x, int y, const String& text) {
  //deprecated
}
void OSWindow::writeContentsToFile(const String& filename) {
#if 0
  ImageCodec::ImageData imgData;
  imgData.width = mWidth;
  imgData.height = mHeight;
  imgData.format = PF_R8G8B8;

  // Allocate buffer
  uchar* pBuffer = new uchar[mWidth * mHeight * 3];

  // Read pixels
  // I love GL: it does all the locking & colour conversion for us
  glReadPixels(0,0, mWidth-1, mHeight-1, GL_RGB, GL_UNSIGNED_BYTE, pBuffer);

  // Wrap buffer in a chunk
  DataChunk chunk(pBuffer, mWidth * mHeight * 3);

  // Need to flip the read data over in Y though
  Image img;
  img.loadRawData(chunk, mWidth, mHeight, PF_R8G8B8 );
  img.flipAroundX();

  DataChunk chunkFlipped(img.getData(), chunk.getSize());

  // Get codec
  size_t pos = filename.find_last_of(".");
  String extension;
  if( pos == String::npos )
    Except(
      Exception::ERR_INVALIDPARAMS,
      "Unable to determine image type for '" + filename + "' - invalid extension.",
      "D3D8RenderWindow::writeContentsToFile" );

  while( pos != filename.length() - 1 )
    extension += filename[++pos];

  // Write out
  ImageCodec img_codec;
  img_codec.SaveToFile(chunkFlipped, filename, &imgData);

  delete [] pBuffer;
#endif
}

// Window procedure callback
// This is a static member, so applies to all windows but we store the
// Win32Window instance in the window data GetWindowLog/SetWindowLog
LRESULT OSWindow::WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ) {
  OSWindow* win;

  // look up window instance
  if( WM_CREATE != uMsg )
    win = (OSWindow*)GetWindowLong( hWnd, 0 );

  switch( uMsg ) {
  case WM_ACTIVATE:
    if( WA_INACTIVE == LOWORD( wParam ) )
      win->mActive = false;
    else
      win->mActive = true;
    break;

  case WM_CREATE: {
    // Log the new window
    // Get CREATESTRUCT
    LPCREATESTRUCT lpcs = (LPCREATESTRUCT)lParam;
    win = (OSWindow*)(lpcs->lpCreateParams);
    // Store pointer in window user data area
    SetWindowLong( hWnd, 0, (long)win );
    win->mActive = true;

    return 0;
  }
  break;

  case WM_PAINT:
    // If we get WM_PAINT messges, it usually means our window was
    // comvered up, so we need to refresh it by re-showing the contents
    // of the current frame.
    if( win->mActive && win->mReady )
      win->update();
    break;

  case WM_MOVE:
    // Move messages need to be tracked to update the screen rects
    // used for blitting the backbuffer to the primary
    // *** This doesn't need to be used to Direct3D9 ***
    break;

  case WM_ENTERSIZEMOVE:
    // Previent rendering while moving / sizing
    win->mReady = false;
    break;

  case WM_EXITSIZEMOVE:
    win->WindowMovedOrResized();
    win->mReady = true;
    break;

  case WM_SIZE:
    // Check to see if we are losing or gaining our window.  Set the
    // active flag to match
    if( SIZE_MAXHIDE == wParam || SIZE_MINIMIZED == wParam )
      win->mActive = false;
    else {
      win->mActive = true;
      if( win->mReady )
        win->WindowMovedOrResized();
    }
    break;

  case WM_GETMINMAXINFO:
    // Prevent the window from going smaller than some minimu size
    ((MINMAXINFO*)lParam)->ptMinTrackSize.x = 100;
    ((MINMAXINFO*)lParam)->ptMinTrackSize.y = 100;
    break;

  case WM_DESTROY:
    PostQuitMessage(0);
    break;

  case WM_CLOSE:

    DestroyWindow( win->mHWnd );
    win->mClosed = true;
    engine::iEngine::GetInstance()->RequestExit();
    return 0;
  }

  return DefWindowProc( hWnd, uMsg, wParam, lParam );
}

void OSWindow::RunMessageLoop() {
  MSG msg;
  while (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
}

}
