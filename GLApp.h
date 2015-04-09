//=============================================================================
//@file		GLApp.h
//@brief	Open GL application class header
//@author	VerMan
//@date		April 6, 2009
//=============================================================================
#ifndef __GLAPP_H__
#define __GLAPP_H__

#include "Cube.h"

#define ITOX(i) ((i)<<16)
#define FTOX(f) ((int32)(f * 65535))
#define ZFAR	-70
#define ZNEAR	-15

class GLApp
{
public:
	boolean Init(IShell *shell, IDisplay *display);
	void Destroy();
	void Start();
	boolean KeyEvent(uint16 wParam);

	//for better response to key events
	int		m_KeyState;
	uint16	m_KeyParam;

private:
	void Render();
	void CleanUp();
	static void OnTimer(void *data);
	boolean SetupEGL(IDisplay *display);
	boolean SetupGL();

	//Application shell
	IShell			*m_pIShell;
	IDisplay		*m_pIDisplay;

	//Display
	IBitmap			*m_pDBitmap;
	AEEBitmapInfo	m_DBitmapInfo;

	//GL interfaces
	IGL				*m_pIGL;
	IEGL			*m_pIEGL;

	//EGL variables
	EGLDisplay		m_eglDisplay;
	EGLSurface		m_eglSurface;
	EGLContext		m_eglContext;

	//Shapes
	Cube			m_Cube;
};

#endif __GLAPP_H__