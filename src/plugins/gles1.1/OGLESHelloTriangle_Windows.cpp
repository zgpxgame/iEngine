/******************************************************************************

 @File         OGLESHelloTriangle_Windows.cpp

 @Title        OpenGL ES 1.x Hello Triangle Tutorial

 @Version      

 @Copyright    Copyright (c) Imagination Technologies Limited.

 @Platform     .

 @Description  Basic Tutorial that shows step-by-step how to initialize OpenGL ES
               1.x, use it for drawing a triangle and terminate it.

******************************************************************************/
#include <stdio.h>
#include <windows.h>
#include <TCHAR.h>

#include <EGL/egl.h>
#include <GLES/gl.h>

/******************************************************************************
 Defines
******************************************************************************/
//#define NO_GDI 1 /* Remove the GDI functions */

#ifndef NO_GDI
// Windows class name to register
#define	WINDOW_CLASS _T("PVRShellClass")

// Width and height of the window
#define WINDOW_WIDTH	640
#define WINDOW_HEIGHT	480
#endif

/******************************************************************************
 Global variables
******************************************************************************/

// Variable set in the message handler to finish the demo
bool	g_bDemoDone = false;

/*!****************************************************************************
 @Function		WndProc
 @Input			hWnd		Handle to the window
 @Input			message		Specifies the message
 @Input			wParam		Additional message information
 @Input			lParam		Additional message information
 @Return		LRESULT		result code to OS
 @Description	Processes messages for the main window
******************************************************************************/
#ifndef NO_GDI
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		// Handles the close message when a user clicks the quit icon of the window
		case WM_CLOSE:
			g_bDemoDone = true;
			PostQuitMessage(0);
			return 1;

		default:
			break;
	}

	// Calls the default window procedure for messages we did not handle
	return DefWindowProc(hWnd, message, wParam, lParam);
}
#endif
/*!****************************************************************************
 @Function		TestEGLError
 @Input			pszLocation		location in the program where the error took
								place. ie: function name
 @Return		bool			true if no EGL error was detected
 @Description	Tests for an EGL error and prints it
******************************************************************************/
bool TestEGLError(HWND hWnd, char* pszLocation)
{
	/*
		eglGetError returns the last error that has happened using egl,
		not the status of the last called function. The user has to
		check after every single egl call or at least once every frame.
	*/
	EGLint iErr = eglGetError();
	if (iErr != EGL_SUCCESS)
	{
#ifndef NO_GDI
		TCHAR pszStr[256];
		_stprintf(pszStr, _T("%s failed (%d).\n"), pszLocation, iErr);
		MessageBox(hWnd, pszStr, _T("Error"), MB_OK|MB_ICONEXCLAMATION);
#endif
		return false;
	}

	return true;
}

/*!****************************************************************************
 @Function		WinMain
 @Input			hInstance		Application instance from OS
 @Input			hPrevInstance	Always NULL
 @Input			lpCmdLine		command line from OS
 @Input			nCmdShow		Specifies how the window is to be shown
 @Return		int				result code to OS
 @Description	Main function of the program
******************************************************************************/
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, TCHAR *lpCmdLine, int nCmdShow)
{
	int				i;

	// Remove unreferenced formal parameter warning by referencing these parameters.
	(void) nCmdShow;
	(void) lpCmdLine;
	(void) hPrevInstance;

	// Windows variables
	HWND				hWnd	= 0;
	HDC					hDC		= 0;

	// EGL variables
	EGLDisplay			eglDisplay	= 0;
	EGLConfig			eglConfig	= 0;
	EGLSurface			eglSurface	= 0;
	EGLContext			eglContext	= 0;
	NativeWindowType	eglWindow	= 0;
	EGLint				pi32ConfigAttribs[128];
	GLuint	ui32Vbo = 0; // Vertex buffer object handle

	/*
		Step 0 - Create a NativeWindowType that we can use for OpenGL ES output
	*/
#ifndef NO_GDI
	// Register the windows class
	WNDCLASS sWC;
    sWC.style = CS_HREDRAW | CS_VREDRAW;
	sWC.lpfnWndProc = WndProc;
    sWC.cbClsExtra = 0;
    sWC.cbWndExtra = 0;
    sWC.hInstance = hInstance;
    sWC.hIcon = 0;
    sWC.hCursor = LoadCursor(NULL, IDC_ARROW);
    sWC.lpszMenuName = 0;
	sWC.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
    sWC.lpszClassName = WINDOW_CLASS;
	unsigned int nWidth = WINDOW_WIDTH;
	unsigned int nHeight = WINDOW_HEIGHT;

	ATOM registerClass = RegisterClass(&sWC);
	if (!registerClass)
	{
		MessageBox(0, _T("Failed to register the window class"), _T("Error"), MB_OK | MB_ICONEXCLAMATION);
	}
#if defined(UNDER_CE)
	// Get the monitor dimensions
	{
		HMONITOR	hMonitor;
		BOOL		bRet;
		POINT		p;
		MONITORINFO sMInfo;

		p.x			= 0;
		p.y			= 0;
		hMonitor	= MonitorFromPoint(p, MONITOR_DEFAULTTOPRIMARY);
		sMInfo.cbSize = sizeof(sMInfo);
		bRet = GetMonitorInfo(hMonitor, &sMInfo);
		if (!bRet)
		{
			MessageBox(0, _T("Failed to get monitor info"), _T("Error"), MB_OK|MB_ICONEXCLAMATION);
			goto cleanup;
		}

		nWidth = sMInfo.rcMonitor.right - sMInfo.rcMonitor.left;
		nHeight = sMInfo.rcMonitor.bottom - sMInfo.rcMonitor.top;
	}
#endif
	// Create the eglWindow
	RECT	sRect;
	SetRect(&sRect, 0, 0, nWidth, nHeight);
	AdjustWindowRectEx(&sRect, WS_CAPTION | WS_SYSMENU, false, 0);
	hWnd = CreateWindow( WINDOW_CLASS, _T("HelloTriangle"), WS_VISIBLE | WS_SYSMENU,
						 0, 0, nWidth, nHeight, NULL, NULL, hInstance, NULL);
	eglWindow = hWnd;

	// Get the associated device context
	hDC = GetDC(hWnd);
	if (!hDC)
	{
		MessageBox(0, _T("Failed to create the device context"), _T("Error"), MB_OK|MB_ICONEXCLAMATION);
		goto cleanup;
	}
#endif
	/*
		Step 1 - Get the default display.
		EGL uses the concept of a "display" which in most environments
		corresponds to a single physical screen. Since we usually want
		to draw to the main screen or only have a single screen to begin
		with, we let EGL pick the default display.
		Querying other displays is platform specific.
	*/
	eglDisplay = eglGetDisplay((NativeDisplayType) hDC);

    if(eglDisplay == EGL_NO_DISPLAY)
         eglDisplay = eglGetDisplay((NativeDisplayType) EGL_DEFAULT_DISPLAY);
	/*
		Step 2 - Initialize EGL.
		EGL has to be initialized with the display obtained in the
		previous step. We cannot use other EGL functions except
		eglGetDisplay and eglGetError before eglInitialize has been
		called.
		If we're not interested in the EGL version number we can just
		pass NULL for the second and third parameters.
	*/
	EGLint iMajorVersion, iMinorVersion;
	if (!eglInitialize(eglDisplay, &iMajorVersion, &iMinorVersion))
	{
#ifndef NO_GDI
		MessageBox(0, _T("eglInitialize() failed."), _T("Error"), MB_OK|MB_ICONEXCLAMATION);
#endif
		goto cleanup;
	}

	/*
		Step 3 - Specify the required configuration attributes.
		An EGL "configuration" describes the pixel format and type of
		surfaces that can be used for drawing.
		For now we just want to use a 16 bit RGB surface that is a
		Window surface, i.e. it will be visible on screen. The list
		has to contain key/value pairs, terminated with EGL_NONE.
	*/
	i = 0;
	pi32ConfigAttribs[i++] = EGL_RED_SIZE;
	pi32ConfigAttribs[i++] = 5;
	pi32ConfigAttribs[i++] = EGL_GREEN_SIZE;
	pi32ConfigAttribs[i++] = 6;
	pi32ConfigAttribs[i++] = EGL_BLUE_SIZE;
	pi32ConfigAttribs[i++] = 5;
	pi32ConfigAttribs[i++] = EGL_ALPHA_SIZE;
	pi32ConfigAttribs[i++] = 0;
	pi32ConfigAttribs[i++] = EGL_SURFACE_TYPE;
	pi32ConfigAttribs[i++] = EGL_WINDOW_BIT;
	pi32ConfigAttribs[i++] = EGL_NONE;

	/*
		Step 4 - Find a config that matches all requirements.
		eglChooseConfig provides a list of all available configurations
		that meet or exceed the requirements given as the second
		argument. In most cases we just want the first config that meets
		all criteria, so we can limit the number of configs returned to 1.
	*/
	EGLint iConfigs;
	if (!eglChooseConfig(eglDisplay, pi32ConfigAttribs, &eglConfig, 1, &iConfigs) || (iConfigs != 1))
	{
#ifndef NO_GDI
		MessageBox(0, _T("eglChooseConfig() failed."), _T("Error"), MB_OK|MB_ICONEXCLAMATION);
#endif
		goto cleanup;
	}

	/*
		Step 5 - Create a surface to draw to.
		Use the config picked in the previous step and the native window
		handle when available to create a window surface. A window surface
		is one that will be visible on screen inside the native display (or
		fullscreen if there is no windowing system).
		Pixmaps and pbuffers are surfaces which only exist in off-screen
		memory.
	*/
	eglSurface = eglCreateWindowSurface(eglDisplay, eglConfig, eglWindow, NULL);

    if(eglSurface == EGL_NO_SURFACE)
    {
        eglGetError(); // Clear error
        eglSurface = eglCreateWindowSurface(eglDisplay, eglConfig, NULL, NULL);
	}

	if (!TestEGLError(hWnd, "eglCreateWindowSurface"))
	{
		goto cleanup;
	}

	/*
		Step 6 - Create a context.
		EGL has to create a context for OpenGL ES. Our OpenGL ES resources
		like textures will only be valid inside this context
		(or shared contexts)
	*/
	eglContext = eglCreateContext(eglDisplay, eglConfig, NULL, NULL);
	if (!TestEGLError(hWnd, "eglCreateContext"))
	{
		goto cleanup;
	}

	/*
		Step 7 - Bind the context to the current thread and use our
		window surface for drawing and reading.
		Contexts are bound to a thread. This means you don't have to
		worry about other threads and processes interfering with your
		OpenGL ES application.
		We need to specify a surface that will be the target of all
		subsequent drawing operations, and one that will be the source
		of read operations. They can be the same surface.
	*/
	eglMakeCurrent(eglDisplay, eglSurface, eglSurface, eglContext);
	if (!TestEGLError(hWnd, "eglMakeCurrent"))
	{
		goto cleanup;
	}

	/*
		Step 8 - Draw something with OpenGL ES.
		At this point everything is initialized and we're ready to use
		OpenGL ES to draw something on the screen.
	*/

	// Sets the clear color.
	// The colours are passed per channel (red,green,blue,alpha) as float values from 0.0 to 1.0
	glClearColor(0.6f, 0.8f, 1.0f, 1.0f); // clear blue

	// We're going to draw a triangle to the screen so create a vertex buffer object for our triangle
	{
		// Interleaved vertex data
		float afVertices[] = {	-0.4f,-0.4f,0.0f, // Position
								1.0f,1.0f,0.66f,1.0f, // Colour
								0.4f,-0.4f,0.0f,
								1.0f,1.0f,0.66f,1.0f,
								0.0f,0.4f,0.0f,
								1.0f,1.0f,0.66f,1.0f};

		// Generate the vertex buffer object (VBO)
		glGenBuffers(1, &ui32Vbo);

		// Bind the VBO so we can fill it with data
		glBindBuffer(GL_ARRAY_BUFFER, ui32Vbo);

		// Set the buffer's data
		unsigned int uiSize = 3 * (sizeof(float) * 7); // Calc afVertices size (3 vertices * stride (7 verttypes per vertex (3 pos + 4 colour)))
		glBufferData(GL_ARRAY_BUFFER, uiSize, afVertices, GL_STATIC_DRAW);
	}

	// Draws a triangle for 800 frames
	//for(i = 0; i < 800; ++i)
    while (1)
	{
		// Check if the message handler finished the demo
		if (g_bDemoDone) break;

		/*
			Clears the color buffer.
			glClear() can also be used to clear the depth or stencil buffer
			(GL_DEPTH_BUFFER_BIT or GL_STENCIL_BUFFER_BIT)
		*/
		glClear(GL_COLOR_BUFFER_BIT);
		if (!TestEGLError(hWnd, "glClear"))
		{
			goto cleanup;
		}

		/*
			Draw a triangle
		*/

		// Enable vertex arrays
		glEnableClientState(GL_VERTEX_ARRAY);

		/*
		   Set the vertex pointer.
		   param 1: Number of coordinates per vertex; must be 2, 3, or 4.
		   param 2: GL_FIXED for CommonLite and GL_FLOAT for Common profile.
		   param 3: Specifies the byte offset between consecutive vertexes.
		   param 4: Offset to the start of the first vertex into the VBO.
		 */
		glVertexPointer(3, GL_FLOAT, sizeof(float) * 7, 0);

		// Set color data in the same way
		glEnableClientState(GL_COLOR_ARRAY);
		glColorPointer(4,GL_FLOAT,sizeof(float) * 7, (GLvoid*) (sizeof(float) * 3) /*The color starts after the 3 position values (x,y,z)*/);

		/*
			Draws a non-indexed triangle array from the pointers previously given.
			param 1: Primitive type: GL_POINTS, GL_LINES, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN, GL_TRIANGLES, etc.
			param 2: The starting index in the array.
			param 3: The number of indices to be rendered.
		*/
		glDrawArrays(GL_TRIANGLES, 0, 3);
		if (!TestEGLError(hWnd, "glDrawArrays"))
		{
			goto cleanup;
		}

		/*
			Swap Buffers.
			Brings to the native display the current render surface.
		*/
		eglSwapBuffers(eglDisplay, eglSurface);
		if (!TestEGLError(hWnd, "eglSwapBuffers"))
		{
			goto cleanup;
		}
#ifndef NO_GDI
		// Managing the window messages
		MSG msg;
		PeekMessage(&msg, hWnd, NULL, NULL, PM_REMOVE);
		TranslateMessage(&msg);
		DispatchMessage(&msg);
#endif
	}


cleanup:
	// Delete the VBO as it is no longer needed
	glDeleteBuffers(1, &ui32Vbo);

	/*
		Step 9 - Terminate OpenGL ES and destroy the window (if present).
		eglTerminate takes care of destroying any context or surface created
		with this display, so we don't need to call eglDestroySurface or
		eglDestroyContext here.
	*/
	eglMakeCurrent(eglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
	eglTerminate(eglDisplay);

	/*
		Step 10 - Destroy the eglWindow.
		Again, this is platform specific and delegated to a separate function.
	*/
#ifndef NO_GDI
	// Release the device context
	if (hDC) ReleaseDC(hWnd, hDC);

	// Destroy the eglWindow
	if (hWnd) DestroyWindow(hWnd);
#endif
	return 0;
}

/******************************************************************************
 End of file (OGLESHelloTriangleWindows.cpp)
******************************************************************************/

