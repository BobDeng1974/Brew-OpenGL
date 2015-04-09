//=============================================================================
//@file		Cube.cpp
//@brief	Cube shape implementation. From OGL example #2.
//@author	VerMan
//@date		April 20, 2009
//=============================================================================
#include "BrewApp.h"

//=============================================================================
//Initializes the cube data
//=============================================================================
void Cube::Init(IShell *shell)
{
	m_pIShell = shell;
	m_RotAngle = 0;
	m_zPos = ZNEAR;
	m_xPos = 0;

	IsBlendEnabled = false;
	IsLightEnabled = false;
	IsTextureEnabled = false;
	IsFogEnabled = false;

	GLfixed vertices[72] = {
		//bottom
	    -ITOX(2), -ITOX(2),  ITOX(2),
		-ITOX(2), -ITOX(2), -ITOX(2),
		 ITOX(2), -ITOX(2), -ITOX(2),
		 ITOX(2), -ITOX(2),  ITOX(2),
		//top
		-ITOX(2),  ITOX(2),  ITOX(2),
		 ITOX(2),  ITOX(2),  ITOX(2),
		 ITOX(2),  ITOX(2), -ITOX(2),
		-ITOX(2),  ITOX(2), -ITOX(2),
		//left
		-ITOX(2), -ITOX(2), -ITOX(2),
		-ITOX(2), -ITOX(2),  ITOX(2),
		-ITOX(2),  ITOX(2),  ITOX(2),
		-ITOX(2),  ITOX(2), -ITOX(2),
		//right
		 ITOX(2), -ITOX(2),  ITOX(2),
		 ITOX(2), -ITOX(2), -ITOX(2),
		 ITOX(2),  ITOX(2), -ITOX(2),
		 ITOX(2),  ITOX(2),  ITOX(2),
		//front
		-ITOX(2), -ITOX(2),  ITOX(2),
		 ITOX(2), -ITOX(2),  ITOX(2),
		 ITOX(2),  ITOX(2),  ITOX(2),
		-ITOX(2),  ITOX(2),  ITOX(2),
		//back
		 ITOX(2), -ITOX(2), -ITOX(2),
		-ITOX(2), -ITOX(2), -ITOX(2),
		-ITOX(2),  ITOX(2), -ITOX(2),
		 ITOX(2),  ITOX(2), -ITOX(2)
	};

	GLfixed normals[72] = {
		//bottom
		 ITOX(0), -ITOX(1),  ITOX(0),
		 ITOX(0), -ITOX(1),  ITOX(0),
		 ITOX(0), -ITOX(1),  ITOX(0),
		 ITOX(0), -ITOX(1),  ITOX(0),
		//top
		 ITOX(0),  ITOX(1),  ITOX(0),
		 ITOX(0),  ITOX(1),  ITOX(0),
		 ITOX(0),  ITOX(1),  ITOX(0),
		 ITOX(0),  ITOX(1),  ITOX(0),
		//left
		 ITOX(0),  ITOX(0), -ITOX(1),
		 ITOX(0),  ITOX(0), -ITOX(1),
		 ITOX(0),  ITOX(0), -ITOX(1),
		 ITOX(0),  ITOX(0), -ITOX(1),
		//right
		 ITOX(0),  ITOX(0),  ITOX(1),
		 ITOX(0),  ITOX(0),  ITOX(1),
		 ITOX(0),  ITOX(0),  ITOX(1),
		 ITOX(0),  ITOX(0),  ITOX(1),
		//front
		-ITOX(1),  ITOX(0),  ITOX(0),
		-ITOX(1),  ITOX(0),  ITOX(0),
		-ITOX(1),  ITOX(0),  ITOX(0),
		-ITOX(1),  ITOX(0),  ITOX(0),
		//back
		 ITOX(1),  ITOX(0),  ITOX(0),
		 ITOX(1),  ITOX(0),  ITOX(0),
		 ITOX(1),  ITOX(0),  ITOX(0),
		 ITOX(1),  ITOX(0),  ITOX(0)
	};

	GLubyte indices[24] = {
		0, 1, 2, 3,
		4, 5, 6, 7,
		8, 9, 10, 11,
		12, 13, 14, 15, 
		16, 17, 18, 19,
		20, 21, 22, 23
	};
			
	GLfixed colors[96] = {
		//bottom
		FTOX(0.0f), FTOX(0.0f), FTOX(1.0f), FTOX(0.5f),
		FTOX(0.0f), FTOX(0.0f), FTOX(0.0f), FTOX(0.5f),
		FTOX(1.0f), FTOX(0.0f), FTOX(0.0f), FTOX(0.5f),
		FTOX(1.0f), FTOX(0.0f), FTOX(1.0f), FTOX(0.5f),
		//top
		FTOX(0.0f), FTOX(1.0f), FTOX(1.0f), FTOX(0.5f),
		FTOX(1.0f), FTOX(1.0f), FTOX(1.0f), FTOX(0.5f),
		FTOX(1.0f), FTOX(1.0f), FTOX(0.0f), FTOX(0.5f),
		FTOX(1.0f), FTOX(1.0f), FTOX(0.0f), FTOX(0.5f),
		//left
		FTOX(0.0f), FTOX(0.0f), FTOX(0.0f), FTOX(0.5f),
		FTOX(0.0f), FTOX(0.0f), FTOX(1.0f), FTOX(0.5f),
		FTOX(0.0f), FTOX(1.0f), FTOX(1.0f), FTOX(0.5f),
		FTOX(1.0f), FTOX(1.0f), FTOX(0.0f), FTOX(0.5f),
		//right
		FTOX(1.0f), FTOX(0.0f), FTOX(1.0f), FTOX(0.5f),
		FTOX(1.0f), FTOX(0.0f), FTOX(0.0f), FTOX(0.5f),
		FTOX(1.0f), FTOX(1.0f), FTOX(0.0f), FTOX(0.5f),
		FTOX(1.0f), FTOX(1.0f), FTOX(1.0f), FTOX(0.5f),
		//front
		FTOX(0.0f), FTOX(0.0f), FTOX(1.0f), FTOX(0.5f),
		FTOX(1.0f), FTOX(0.0f), FTOX(1.0f), FTOX(0.5f),
		FTOX(1.0f), FTOX(1.0f), FTOX(1.0f), FTOX(0.5f),
		FTOX(0.0f), FTOX(1.0f), FTOX(1.0f), FTOX(0.5f),
		//back
		FTOX(1.0f), FTOX(0.0f), FTOX(0.0f), FTOX(0.5f),
		FTOX(0.0f), FTOX(0.0f), FTOX(0.0f), FTOX(0.5f),
		FTOX(1.0f), FTOX(1.0f), FTOX(0.0f), FTOX(0.5f),
		FTOX(1.0f), FTOX(1.0f), FTOX(0.0f), FTOX(0.5f) 

	};

	GLshort texCoords[48] = {
		0, 0, 1, 0, 1, 1, 0, 1,
		0, 0, 1, 0, 1, 1, 0, 1,
		0, 0, 1, 0, 1, 1, 0, 1,
		0, 0, 1, 0, 1, 1, 0, 1,
		0, 0, 1, 0, 1, 1, 0, 1,
		0, 0, 1, 0, 1, 1, 0, 1	
	};

	for(GLint i=0; i<72; i++)
	{
		m_Vertices[i] = vertices[i];
		m_Normals[i] = normals[i];
	}

	for(GLint i=0; i<36; i++)
		m_Indices[i] = indices[i];

	for(GLint i=0; i<96; i++)
		m_Colors[i] = colors[i];

	for(GLint i=0; i<48; i++)
		m_TexCoords[i] = texCoords[i];

	//load cube texture
	LoadTexture();
}

//=============================================================================
//Destroys cube data
//=============================================================================
void Cube::Destroy()
{
	glDeleteTextures(1, &m_TexID);
	m_Texture.FreeData();
}

//=============================================================================
//Loads textures from file
//=============================================================================
void Cube::LoadTexture()
{
	//set the application shell to TGA image class
	m_Texture.m_pIShell = m_pIShell;

	//load the texture
	m_Texture.Load("res/cube.tga");

	//generate the texture
	glGenTextures(1, &m_TexID);
	glBindTexture(GL_TEXTURE_2D, m_TexID);

	glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexEnvx(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glTexImage2D(	GL_TEXTURE_2D, 
					0,
					m_Texture.GetDataFormat(),
					m_Texture.GetWidth(),
					m_Texture.GetHeight(),
					0,
					m_Texture.GetDataFormat(),
					m_Texture.GetDataType(),
					m_Texture.GetData()
	);
}

//=============================================================================
//Handles key events
//@param	wParam Event specific data
//@return	TRUE if the event has been processed, FALSE otherwise
//=============================================================================
boolean Cube::KeyEvent(uint16 wParam)
{
	switch(wParam)
	{
		//Zoom out
		case AVK_UP:
			if(m_zPos-- <= ZFAR)
				m_zPos++;
			return TRUE;
			break;

		//Zoom in
		case AVK_DOWN:
			if(m_zPos++ >= ZNEAR)
				m_zPos--;
			return TRUE;
			break;

		//move cube to the left
		case AVK_LEFT:
			m_xPos--;
			return TRUE;
			break;

		//move cube to the right
		case AVK_RIGHT:
			m_xPos++;
			return TRUE;
			break;

		//reset cube position
		case AVK_SELECT:
			m_zPos = ZNEAR;
			m_xPos = 0;
			return TRUE;
			break;

		case AVK_1:
			glEnable(GL_BLEND);
			IsBlendEnabled = true;
			return TRUE;
			break;

		case AVK_2:
			glDisable(GL_BLEND);
			IsBlendEnabled = false;
			return TRUE;
			break;

		case AVK_4:
			glEnable(GL_LIGHTING);
			IsLightEnabled = true;
			return TRUE;
			break;

		case AVK_5:
			glDisable(GL_LIGHTING);
			IsLightEnabled = false;
			return TRUE;
			break;

		case AVK_7:
			glEnable(GL_TEXTURE_2D);
			IsTextureEnabled = true;
			return TRUE;
			break;

		case AVK_8:
			glDisable(GL_TEXTURE_2D);
			IsTextureEnabled = false;
			return TRUE;
			break;

		case AVK_STAR:
			glEnable(GL_FOG);
			IsFogEnabled = true;
			return TRUE;
			break;

		case AVK_0:
			glDisable(GL_FOG);
			IsFogEnabled = false;
			return TRUE;
			break;

		default:
			return TRUE;
			break;
	}

	return FALSE;
}

//=============================================================================
//Renders the cube
//=============================================================================
void Cube::Render()
{
	m_RotAngle+=5;
	if(m_RotAngle > 360)
		m_RotAngle = 0;

	glPushMatrix();
	glLoadIdentity();
	glTranslatex(ITOX(m_xPos), 0, ITOX(m_zPos));
	glRotatex(ITOX(20), ITOX(1), 0, ITOX(1));
	glRotatex(ITOX(m_RotAngle), 0, ITOX(1), 0);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);

	glTexCoordPointer(2, GL_SHORT, 0, m_TexCoords);
	glVertexPointer(3, GL_FIXED, 0, m_Vertices);
	glColorPointer(4, GL_FIXED, 0, m_Colors);
	glNormalPointer(GL_FIXED, 0, m_Normals);

	for(int side=0; side<6; side++)
	{
		glBindTexture(GL_TEXTURE_2D, m_TexID);
		glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_BYTE, m_Indices + side * 4);
	}	
	
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);

	glPopMatrix();
}