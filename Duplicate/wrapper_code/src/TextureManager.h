/*
* OpenGL - 3D Vision Wrapper V.3.19
* Copyright (c) Helifax 2015
*/

#ifndef __TEXTURE__MANAGER_H__
#define __TEXTURE__MANAGER_H__

#include <string>
#include <vector>
#include <stdlib.h> 
#include <gl_custom.h>

// Structure for a Right Eye Texture.
typedef struct 
{
	GLuint m_textureId;
	GLboolean m_isAttached;
} RIGHT_EYE_TEXTURE_T;

// Structure containing the pair of Duplicate Textures
typedef struct
{
	// Current Left Eye Tex
	GLuint m_leftEyeTextureId;
	// Pointer to a structure containing the Right Eye FBO information
	RIGHT_EYE_TEXTURE_T m_rightEyeTextureId;
} STEREO_TEXTURES_T;


// Texture Manager Singleton class
class TextureManager
{
	//////////////////////////////////////////////////////////////////////////
	// Singleton definitions
public:
	// Returns the instance of the FBO Manager
	static TextureManager* getInstance()
	{
		if (instance == 0)
		{
			instance = new TextureManager;
		}
		return instance;
	}

	//default dtor nothing to do 
	~TextureManager() { ; }

private:
	//default ctor 
	TextureManager();
	static TextureManager * instance;
	TextureManager(const TextureManager&);
	TextureManager& operator= (const TextureManager&);
	
	// Singleton definitions
	//////////////////////////////////////////////////////////////////////////

	// Definitions of class functions and variables
public:

	// Add the FBOs to the FBO stack/vector
	GLboolean AddStereoTextures(GLint _currentTex);
	//------------------------------------------------------------------------>

	GLuint GetRightEyeTextureId(GLint _currentTex);
	//------------------------------------------------------------------------>

	void AddTextureBoundMode(GLenum mode, GLenum target, GLint texture);
	//------------------------------------------------------------------------>

	BOOL GetTextureBoundMode(GLenum *mode, GLenum *target, GLint *texture);
	//------------------------------------------------------------------------>

	void ResetTextureBoundMode(void);
	//------------------------------------------------------------------------>

private:
	
	// vector containing all the FBOs
	std::vector <STEREO_TEXTURES_T> m_stereoTextures;

	// Current Texture Binding Mode
	GLenum m_currentMode;
	GLenum m_currentTarget;
	GLint m_currentTexture;

private:
	
};

#endif