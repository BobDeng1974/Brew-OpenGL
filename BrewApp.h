//=============================================================================
//@file		BrewApp.h
//@brief	BREW application class header
//@author	VerMan
//@date		April 7, 2009
//=============================================================================
#ifndef __BREW_H__
#define __BREW_H__

//BREW
#include "AEEAppGen.h"
#include "AEEModGen.h"
#include "AEEShell.h"
#include "AEEStdLib.h"

//OpenGL
#include <IGL.H>
#include <AEEGL.h>

#include "GLApp.h"

class BrewApp : public AEEApplet
{
public:
	static boolean EventHandler(BrewApp *pBrewApp, AEEEvent eCode, uint16 wParam, uint32 dwParam);
	static void FreeAppData(BrewApp *pBrewApp);

protected:
	boolean OnEvent(AEEEvent eCode, uint16 wParam, uint32 dwParam);
	void OnFreeAppData(void);

private:
	GLApp m_GLApp;
};

#endif
