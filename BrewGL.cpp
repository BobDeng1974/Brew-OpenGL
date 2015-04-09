//=============================================================================
//@file		main.cpp
//@brief	Brew OpenGL ES demo
//@author	VerMan
//@date		April 6, 2009
//=============================================================================
#include "BrewApp.h"
#include "BrewGL.bid"

//=============================================================================
//Creates the applet instance
//=============================================================================
extern "C" int AEEClsCreateInstance(AEECLSID ClsId, IShell *pIShell, IModule *pImodule, void **ppApplet)
{
	*ppApplet = NULL;

	if(AEEApplet_New(	sizeof(BrewApp),
						ClsId, 
						pIShell, 
						pImodule, 
						(IApplet**)ppApplet, 
						(AEEHANDLER)BrewApp::EventHandler,
						(PFNFREEAPPDATA)BrewApp::FreeAppData))
	{
		return SUCCESS;
	}
	
	return EFAILED;
}