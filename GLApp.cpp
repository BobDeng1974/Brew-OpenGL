//=============================================================================
//@file		GLApp.cpp
//@brief	Open GL application class
//@author	VerMan
//@date		April 6, 2009
//=============================================================================
#include "BrewApp.h"

//=============================================================================
//Initializes the GL application
//=============================================================================
boolean GLApp::Init(IShell *shell, IDisplay *display)
{
	m_pIShell = shell;
	m_pIDisplay = display;

	//create GL interfaces
	if(ISHELL_CreateInstance(m_pIShell, AEECLSID_GL, (void**)&m_pIGL) != SUCCESS) 
	{
		return FALSE;
	}
	else 
	{
		IGL_Init(m_pIGL);
	}

	if(ISHELL_CreateInstance(m_pIShell, AEECLSID_EGL, (void**)&m_pIEGL) != SUCCESS) 
	{
		return FALSE;
	}
	else 
	{
		IEGL_Init(m_pIEGL);
	}

	//get device frame buffer info
	if(IDisplay_GetDeviceBitmap(display, &m_pDBitmap) != SUCCESS)
	{
		CleanUp();
		return FALSE;
	}

	if(IBitmap_GetInfo(m_pDBitmap, &m_DBitmapInfo, sizeof(AEEBitmapInfo)) != SUCCESS)
	{
		CleanUp();
		return FALSE;
	}

	if(!SetupEGL(display))
		return FALSE;

	if(!SetupGL())
		return FALSE;

	//setup shapes
	m_Cube.Init(shell);

	return TRUE;
}

//=============================================================================
//Setup EGL
//@returns TRUE on success, FALSE otherwise
//=============================================================================
boolean GLApp::SetupEGL(IDisplay *display)
{
	EGLConfig conf;
	EGLint ncfg = 1;
	EGLint params[5] = {EGL_NONE, EGL_NONE, EGL_NONE, EGL_NONE, EGL_NONE};
	IDIB *pDIB;

	m_eglDisplay = EGL_NO_DISPLAY;
	m_eglSurface = EGL_NO_SURFACE;
	m_eglContext = EGL_NO_CONTEXT;
	
	//set the display to the main display
	m_eglDisplay = eglGetDisplay(display);
	if(m_eglDisplay == EGL_NO_DISPLAY || eglGetError() != EGL_SUCCESS) 
		return FALSE;
	
	//initialize the display
	if(eglInitialize(m_eglDisplay, NULL, NULL) == EGL_FALSE || eglGetError() != EGL_SUCCESS)
		return FALSE;
	
	//get display configuration	
	eglGetConfigs(m_eglDisplay, &conf, 1, &ncfg);

	//create the window surface using our display’s bitmap
	if(IBITMAP_QueryInterface(m_pDBitmap, AEECLSID_DIB, (void**)&pDIB) != SUCCESS)
		return EFAILED;
		
	m_eglSurface = eglCreateWindowSurface(m_eglDisplay, conf, pDIB, params);
	IDIB_Release(pDIB);
		
	if(m_eglSurface == EGL_NO_SURFACE || eglGetError() != EGL_SUCCESS)
		return FALSE;
	
	//create the context used to render on the new window surface
	m_eglContext = eglCreateContext(m_eglDisplay, conf, 0, 0);
	if(m_eglContext == EGL_NO_CONTEXT || eglGetError() != EGL_SUCCESS)
		return FALSE;
	
	//make it the current context with the newly-created display and surface
	if(eglMakeCurrent(m_eglDisplay, m_eglSurface, m_eglSurface, m_eglContext) == EGL_FALSE || eglGetError() != EGL_SUCCESS)
		return FALSE;

	return TRUE;
}

//=============================================================================
//Setup GL
//@returns TRUE on success, FALSE otherwise
//=============================================================================
boolean GLApp::SetupGL()
{
	//smooth shading & depth test
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	
	//enable lighting operations
	GLfixed diffuse[] = { FTOX(1.0f), FTOX(1.0f), FTOX(1.0f), FTOX(1.0f) };
	GLfixed lightPos[] = { FTOX(0.0f), FTOX(5.0f), FTOX(-5.0f) };
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
	glLightxv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightxv(GL_LIGHT0, GL_POSITION, lightPos);
	
	//enable blending operations
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

	//enable fog operations
	GLfixed fogColor[] = { 0, 0, 0, ITOX(1) };
	glFogxv(GL_FOG_COLOR, fogColor);
	glFogx(GL_FOG_MODE, GL_LINEAR);
	glFogx(GL_FOG_START, ITOX(10));
	glFogx(GL_FOG_END, ITOX(70));	

	//perspective Correction
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	
	//GL Initialization
	glViewport(0, 0, m_DBitmapInfo.cx, m_DBitmapInfo.cy);
	
	//init Projection matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustumx(ITOX(-5), ITOX(5), ITOX(-5), ITOX(5), ITOX(10), ITOX(100));
	
	//init Model-View matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	if(glGetError() != GL_NO_ERROR) 
		return FALSE;
	
	return TRUE;
}

//=============================================================================
//Performs clean-up
//=============================================================================
void GLApp::CleanUp()
{
	eglMakeCurrent(EGL_NO_DISPLAY, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
	
	if(m_pIShell)
	{
		ISHELL_Release(m_pIShell);
		m_pIShell = NULL;
	}

	if(m_pDBitmap) 
	{
		IBITMAP_Release(m_pDBitmap);
		m_pDBitmap = NULL;
	}

	if(m_eglContext)
	{
		eglDestroyContext(m_eglDisplay, m_eglContext);
		m_eglContext = NULL;
	}

	if(m_eglSurface)
	{
		eglDestroySurface(m_eglDisplay, m_eglSurface);
		m_eglSurface = NULL;
	}

	if(m_eglDisplay)
	{
		eglTerminate(m_eglDisplay);
		m_eglDisplay = NULL;
	}

	if(m_pIEGL) 
	{
		IEGL_Release(m_pIEGL);
		m_pIEGL = NULL;
	}

	if(m_pIGL) 
	{
		IGL_Release(m_pIGL);
		m_pIGL = NULL;
	}
}

//=============================================================================
//Destroys the GL Application
//=============================================================================
void GLApp::Destroy()
{
	CleanUp();
}

//=============================================================================
//Starts the timer that triggers Render function
//@param	shell a pointer to the application ISHELL
//=============================================================================
void GLApp::Start()
{
	OnTimer(this);
}

//=============================================================================
//Repetedly executes our rendering loop
//@param	shell a pointer to the application ISHELL
//@param	data a pointer to data that will be passed to the callback
//=============================================================================
void GLApp::OnTimer(void *data)
{
	//call our render routine
	GLApp *pMe = (GLApp*)data;
	pMe->Render();

	if(pMe->m_KeyState == 1)
		pMe->KeyEvent(pMe->m_KeyParam);

	//repetedly call OnTimer again...
	ISHELL_SetTimer(pMe->m_pIShell, 50, (PFNNOTIFY)OnTimer, pMe);
}

//=============================================================================
//Perform rendering
//=============================================================================
void GLApp::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	AECHAR m_BlendingStr[10] = {'B','l','e','n','d','i','n','g',':','\0'};
	AECHAR m_LightingStr[10] = {'L','i','g','h','t','i','n','g',':','\0'};
	AECHAR m_TextureStr[9] = {'T','e','x','t','u','r','e',':','\0'};
	AECHAR m_FogStr[5] = {'F','o','g',':','\0'};
	AECHAR m_OnStr[3] = {'O','n','\0'};
	AECHAR m_OffStr[4] = {'O','f','f','\0'};

	IDISPLAY_SetColor(m_pIDisplay, CLR_USER_TEXT, MAKE_RGB(0xFF,0xFF,0xFF));
	IDISPLAY_SetColor(m_pIDisplay, CLR_USER_BACKGROUND, MAKE_RGB(0,0,0));
	IDISPLAY_DrawText(m_pIDisplay, AEE_FONT_NORMAL, m_BlendingStr, -1, 1, 0, 0, IDF_ALIGN_LEFT);
	IDISPLAY_DrawText(m_pIDisplay, AEE_FONT_NORMAL, (m_Cube.IsBlendEnabled)?m_OnStr:m_OffStr, -1, 1, 0, 0, IDF_ALIGN_RIGHT);
	IDISPLAY_DrawText(m_pIDisplay, AEE_FONT_NORMAL, m_LightingStr, -1, 1, 12, 0, IDF_ALIGN_LEFT);
	IDISPLAY_DrawText(m_pIDisplay, AEE_FONT_NORMAL, (m_Cube.IsLightEnabled)?m_OnStr:m_OffStr, -1, 1, 12, 0, IDF_ALIGN_RIGHT);
	IDISPLAY_DrawText(m_pIDisplay, AEE_FONT_NORMAL, m_TextureStr, -1, 1, 24, 0, IDF_ALIGN_LEFT);
	IDISPLAY_DrawText(m_pIDisplay, AEE_FONT_NORMAL, (m_Cube.IsTextureEnabled)?m_OnStr:m_OffStr, -1, 1, 24, 0, IDF_ALIGN_RIGHT);
	IDISPLAY_DrawText(m_pIDisplay, AEE_FONT_NORMAL, m_FogStr, -1, 1, 36, 0, IDF_ALIGN_LEFT);
	IDISPLAY_DrawText(m_pIDisplay, AEE_FONT_NORMAL, (m_Cube.IsFogEnabled)?m_OnStr:m_OffStr, -1, 1, 36, 0, IDF_ALIGN_RIGHT);
	IDISPLAY_Update(m_pIDisplay);

	m_Cube.Render();

	eglSwapBuffers(m_eglDisplay, m_eglSurface);
}

//=============================================================================
//Handles key events
//@param	wParam Event specific data
//@return	TRUE if the event has been processed, FALSE otherwise
//=============================================================================
boolean GLApp::KeyEvent(uint16 wParam)
{
	return m_Cube.KeyEvent(wParam);
}
