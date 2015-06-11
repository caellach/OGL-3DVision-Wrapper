/*
* OpenGL - 3D Vision Wrapper V.1.01
* Copyright (c) Helifax 2014
*/

#ifndef __FBO__MANAGER_H__
#define __FBO__MANAGER_H__

#include <string>
#include <vector>
#include <stdlib.h> 
#include <gl_custom.h>

// Structure for a Right Eye FBO.
typedef struct struct_RightEyeFBO
{
	GLuint m_FBOId;
	GLuint m_colorTexId[8];
	GLuint m_stencilRenderBfrId;
	GLboolean m_isComplete;
} RIGHT_EYE_FBO_T;

// Structure containing the pair of FBOs
typedef struct struct_Stereo_FBO
{
	// Current FBO ID
	GLuint m_leftEyeFBO;
	// Pointer to a structure containing the Right Eye FBO information
	RIGHT_EYE_FBO_T* m_rightEyeFBO;
} STEREO_FBO_T;


// FBO Manager Singleton class
class FBOManager
{
	//////////////////////////////////////////////////////////////////////////
	// Singleton definitions
public:
	// Returns the instance of the FBO Manager
	static FBOManager* getInstance()
	{
		if (instance == 0)
		{
			instance = new FBOManager;
		}
		return instance;
	}

	//default dtor nothing to do 
	~FBOManager() { ; }

private:
	//default ctor 
	FBOManager();
	static FBOManager * instance;
	FBOManager(const FBOManager&);
	FBOManager& operator= (const FBOManager&);
	
	// Singleton definitions
	//////////////////////////////////////////////////////////////////////////

	// Definitions of class functions and variables

public:

	// Add the FBOs to the FBO stack/vector
	GLboolean AddStereoFBOs(GLint _currentFBO, BOOL isBackBuffer);
	//------------------------------------------------------------------------>

	void MarkFBOAsComplete(GLint _currentFBO);
	//------------------------------------------------------------------------>

	// Check if Right Eye FBO is complete for the current (Left Eye) FBO.
	GLboolean IsRightEyeFBOComplete(GLint _currentFBO);
	//------------------------------------------------------------------------>

	// Return the Right Eye FBO for the current Left FBO (no matter the state)
	GLuint GetRightEyeIncompleteFBO(GLint _currentFBO);
	//------------------------------------------------------------------------>s

	// Return the Right Eye FBO for the current Left FBO if complete!
	GLuint GetRightEyeFBO(GLint _currentFBO);
	//------------------------------------------------------------------------>s

	void AddFBOBoundMode(GLenum target, GLuint framebuffer);
	//------------------------------------------------------------------------>

	BOOL GetFBOBoundMode(GLenum *target, GLuint *framebuffer);
	//------------------------------------------------------------------------>

	void ResetFBOBoundMode(void);
	//------------------------------------------------------------------------>

private:
	
	// vector containing all the FBOs
	std::vector <STEREO_FBO_T> m_stereoFBOs;

	GLenum m_currentTarget;
	GLuint m_currentFBO;

private:
	//------------------------------------------------------------------------>
	// Generate the Right Eye FBO for the current FBO.
	// The returned values will be in _rightEyeFBO.
	GLboolean GenerateRightEyeFBO(RIGHT_EYE_FBO_T* _rightEyeFBO);
	//------------------------------------------------------------------------>

	GLboolean GenerateRightEyeBackBufferFBO(RIGHT_EYE_FBO_T* _rightEyeFBO);
	//------------------------------------------------------------------------>

	// Check to see if the Left Eye FBO is in the list
	// Returns the position in the vector were is found
	GLint isCurrentFBOInList(GLuint _currentFBO);
	//------------------------------------------------------------------------>
};

#endif