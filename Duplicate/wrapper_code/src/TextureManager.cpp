#include <assert.h>
#include "TextureManager.h"
#include "opengl32.h"
extern "C" {
#include "opengl_3dv.h"
}

// With and Height of the window
extern unsigned int g_width;
extern unsigned int g_height;

//------------------------------------------------------------------------>
TextureManager *TextureManager::instance = 0;
//------------------------------------------------------------------------>

TextureManager::TextureManager()
{
	// Constructor
	// To define stuff here if needed
}
//------------------------------------------------------------------------>


//////////////////////////////////////////////////////////////////////////
// Public Functions
//////////////////////////////////////////////////////////////////////////

GLboolean TextureManager::AddStereoTextures(GLint _currentTex)
{
	GLboolean ret = FALSE;
	GLuint newTex;
	// Generate the New Texture
	(*orig_glGenTextures)(1, &newTex);
	
	// Add the Pair
	STEREO_TEXTURES_T textures = { 0 };
	textures.m_leftEyeTextureId = _currentTex;
	textures.m_rightEyeTextureId.m_textureId = newTex;
	textures.m_rightEyeTextureId.m_isAttached = FALSE;
	
	// Push it
	m_stereoTextures.push_back(textures);

	// Return the result
	return ret;
}
//------------------------------------------------------------------------>

GLuint TextureManager::GetRightEyeTextureId(GLint _currentTex)
{
	GLuint texId = 0;
	for (size_t i = 0; i < m_stereoTextures.size(); i++)
	{
		if (m_stereoTextures[i].m_leftEyeTextureId == _currentTex)
		{
			texId = m_stereoTextures[i].m_rightEyeTextureId.m_textureId;
			break;
		}
	}
	return texId;
}
//------------------------------------------------------------------------>

void TextureManager::AddTextureBoundMode(GLenum mode, GLenum target, GLint texture)
{
	m_currentMode = mode;
	m_currentTarget = target;
	m_currentTexture = texture;
}
//------------------------------------------------------------------------>

BOOL TextureManager::GetTextureBoundMode(GLenum *mode, GLenum *target, GLint *texture)
{
	*mode = m_currentMode;
	*target = m_currentTarget;
	*texture = m_currentTexture;

	return TRUE;
}
//------------------------------------------------------------------------>

void TextureManager::ResetTextureBoundMode(void)
{
	m_currentMode = 0;
	m_currentTarget = 0;
	m_currentTexture = 0;
}

//////////////////////////////////////////////////////////////////////////
// Private Functions
//////////////////////////////////////////////////////////////////////////

