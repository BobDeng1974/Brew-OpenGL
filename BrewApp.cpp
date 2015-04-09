//=============================================================================
//@file		BrewApp.cpp
//@brief	BREW application class definition
//@author	VerMan
//@date		April 7, 2009
//=============================================================================
#include "BrewApp.h"

boolean BrewApp::EventHandler(BrewApp *pBrewApp, uint16 eCode, uint16 wParam, uint32 dwParam)
{
	return pBrewApp->OnEvent(eCode, wParam, dwParam);
}

void BrewApp::FreeAppData(BrewApp *pBrewApp)
{
	pBrewApp->OnFreeAppData();
}

//=============================================================================
//Event handler
//=============================================================================
boolean BrewApp::OnEvent(uint16 eCode, uint16 wParam, uint32 dwParam)
{
    switch (eCode)
    {
		case EVT_APP_START:
			m_GLApp.Init(m_pIShell, m_pIDisplay);
			m_GLApp.Start();
			return TRUE;

		case EVT_APP_STOP:
			return TRUE;

		case EVT_APP_SUSPEND:
			return TRUE;

		case EVT_APP_RESUME:
			return TRUE;

		case EVT_KEY_PRESS:
			m_GLApp.m_KeyState = 1;
			m_GLApp.m_KeyParam = wParam;

			if(m_GLApp.KeyEvent(wParam))
				return OnEvent(EVT_KEY, wParam, dwParam);
			else
				return TRUE;

		case EVT_KEY_RELEASE:
			m_GLApp.m_KeyState = 0;
			m_GLApp.m_KeyParam = wParam;
			return TRUE;

		case EVT_KEY:
			return m_GLApp.KeyEvent(wParam);

		default:
			break;
     }

     return FALSE;
}

//=============================================================================
//Frees application data
//=============================================================================
void BrewApp::OnFreeAppData()
{
	m_GLApp.Destroy();
}
