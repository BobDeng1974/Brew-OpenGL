//=============================================================================
//@file		Cube.h
//@brief	Cube shape
//@author	VerMan
//@date		April 20, 2009
//=============================================================================
#ifndef __CUBE_H__
#define __CUBE_H__

#include "tga_image.h"

class Cube 
{
public:
	void Init(IShell *shell);
	void Destroy();
	void LoadTexture();
	void Render();
	boolean KeyEvent(uint16 wParam);

	boolean IsBlendEnabled;
	boolean IsLightEnabled;
	boolean IsTextureEnabled;
	boolean IsFogEnabled;

private:
	//Application shell
	IShell		*m_pIShell;

	//Cube data information
	GLfixed		m_Vertices[72];
	GLfixed		m_Normals[72];
	GLfixed		m_Colors[96];
	GLubyte		m_Indices[24];
	GLshort		m_TexCoords[48];

	//textures
	TGA_IMAGE	m_Texture;
	GLuint		m_TexID;

	GLint		m_RotAngle;
	GLint		m_zPos;
	GLint		m_xPos;
};

#endif