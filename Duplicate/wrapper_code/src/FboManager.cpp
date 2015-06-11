#include <assert.h>
#include "FboManager.h"
#include "opengl32.h"
extern "C" {
#include "opengl_3dv.h"
}

// With and Height of the window
extern unsigned int g_width;
extern unsigned int g_height;

//------------------------------------------------------------------------>
FBOManager *FBOManager::instance = 0;
//------------------------------------------------------------------------>

FBOManager::FBOManager()
{
	// Constructor
	// To define stuff here if needed
}
//------------------------------------------------------------------------>


//////////////////////////////////////////////////////////////////////////
// Public Functions
//////////////////////////////////////////////////////////////////////////

GLboolean FBOManager::AddStereoFBOs(GLint _currentFBO, BOOL isBackBuffer)
{
	GLboolean ret = FALSE;
	GLint currentFBOIndex = isCurrentFBOInList(_currentFBO);

	// We have the FBO in the list
	if (currentFBOIndex != -1)
	{
		// We already have this FBO in the list.
		// Check to see Right Eye completeness. 
		ret = IsRightEyeFBOComplete(_currentFBO);

		// Right Eye FBO not generated 
		if (!ret)
		{
			// create a new Right Eye FBO
			RIGHT_EYE_FBO_T *newRightEyeFBO = new RIGHT_EYE_FBO_T;
			// If is BackBuffer framebuffer
			if (isBackBuffer)
			{
				ret = GenerateRightEyeBackBufferFBO(newRightEyeFBO);
			}
			else
			{
				ret = GenerateRightEyeFBO(newRightEyeFBO);
			}
		
			// Generated successfully
			if (ret)
			{
				m_stereoFBOs[currentFBOIndex].m_rightEyeFBO = newRightEyeFBO;
				ret = TRUE;
			}
		}
	}
	// We don't have it in the list so we need to 
	// Create Right Eye FBO
	// Push both FBOs in the list.
	else if (_currentFBO != -1)
	{
		// create a new Right Eye FBO
		RIGHT_EYE_FBO_T *newRightEyeFBO = new RIGHT_EYE_FBO_T;
		// If is BackBuffer framebuffer
		if (isBackBuffer)
		{
			ret = GenerateRightEyeBackBufferFBO(newRightEyeFBO);
		}
		else
		{
			ret = GenerateRightEyeFBO(newRightEyeFBO);
		}

		// Generated successfully
		if (ret)
		{
			STEREO_FBO_T stereoFBO;
			// Left Eye
			stereoFBO.m_leftEyeFBO = _currentFBO;
			// Right Eye
			stereoFBO.m_rightEyeFBO = newRightEyeFBO;

			//Push them in the vector
			m_stereoFBOs.push_back(stereoFBO);
			ret = TRUE;
		}
	}

	// Return the result
	return ret;
}
//------------------------------------------------------------------------>

void FBOManager::MarkFBOAsComplete(GLint _currentFBO)
{
	BOOL fboFound = FALSE;

	for (size_t i = 0; i < m_stereoFBOs.size(); i++)
	{
		if (m_stereoFBOs[i].m_rightEyeFBO->m_FBOId == _currentFBO)
		{
			fboFound = TRUE;
			m_stereoFBOs[i].m_rightEyeFBO->m_isComplete = TRUE;
			break;
		}
	}
	assert("FBO NOT FOUND so cannot set the Completeness of the FBO" && fboFound);
}
//------------------------------------------------------------------------>

GLboolean FBOManager::IsRightEyeFBOComplete(GLint _currentFBO)
{
	GLboolean ret = FALSE;
	GLint currentFBOIndex = isCurrentFBOInList(_currentFBO);

	// We have the FBO in the list
	if (currentFBOIndex != -1)
	{
		// Get the status of the Right Eye FBO
		ret = m_stereoFBOs[currentFBOIndex].m_rightEyeFBO->m_isComplete;
	}
	else
	{
		assert("FBO NOT FOUND so cannot check the Completeness of the FBO" && FALSE);
	}

	return ret;
}
//------------------------------------------------------------------------>

GLuint FBOManager::GetRightEyeIncompleteFBO(GLint _currentFBO)
{
	GLint FBONumber = -1;
	GLint currentFBOIndex = isCurrentFBOInList(_currentFBO);

	if (GetInterop() == false)
	{
		// We have the FBO in the list
		if (currentFBOIndex != -1)
		{
			FBONumber = m_stereoFBOs[currentFBOIndex].m_rightEyeFBO->m_FBOId;
		}
		else
		{
			assert("FBO Not FOUND !!! Cannot Return it" && FALSE);
		}
	}
	return FBONumber;
}
//------------------------------------------------------------------------>

GLuint FBOManager::GetRightEyeFBO(GLint _currentFBO)
{
	GLint FBONumber = -1;
	GLint currentFBOIndex = isCurrentFBOInList(_currentFBO);

	if (GetInterop() == false)
	{
		// We have the FBO in the list
		if (currentFBOIndex != -1)
		{
			// Get the status of the Right Eye FBO
			if (IsRightEyeFBOComplete(_currentFBO))
			{
				FBONumber = m_stereoFBOs[currentFBOIndex].m_rightEyeFBO->m_FBOId;
			}
			else
			{
				assert("FBO Not Complete !!! Cannot Return it" && FALSE);
			}
		}
		else 
		{
			assert("FBO Not FOUND !!! Cannot Return it" && FALSE);
		}
	}
	return FBONumber;
}
//------------------------------------------------------------------------>

void FBOManager::AddFBOBoundMode(GLenum target, GLuint framebuffer)
{
	m_currentTarget = target;
	m_currentFBO = framebuffer;
}
//------------------------------------------------------------------------>

BOOL FBOManager::GetFBOBoundMode(GLenum *target, GLuint *framebuffer)
{
	*target = m_currentTarget;
	*framebuffer = m_currentFBO;

	return TRUE;
}
//------------------------------------------------------------------------>

void FBOManager::ResetFBOBoundMode(void)
{
	m_currentTarget = 0;
	m_currentFBO = 0;
}
//------------------------------------------------------------------------>

//////////////////////////////////////////////////////////////////////////
// Private Functions
//////////////////////////////////////////////////////////////////////////

GLint FBOManager::isCurrentFBOInList(GLuint _currentFBO)
{
	GLint index = -1;
	// check and see if current FBO is in list.
	for (size_t i = 0; i < m_stereoFBOs.size(); i++)
	{
		if (_currentFBO == m_stereoFBOs[i].m_leftEyeFBO)
		{
			// Finished so we break;
			index = i;
			break;
		}
	}
	// Return the index
	return index;
}
//------------------------------------------------------------------------>

GLboolean FBOManager::GenerateRightEyeFBO(RIGHT_EYE_FBO_T* _rightEyeFBO)
{
	GLboolean ret = TRUE;

	// FBO ID
	GLuint auxFBO;
	// Generate the FBO	
	(*orig_glGenFramebuffers)(1, &auxFBO);
	_rightEyeFBO->m_FBOId = auxFBO;

	// Initializes the rest of the FBO
	for (size_t i = 0; i < 8; i++)
		_rightEyeFBO->m_colorTexId[i] = 0;
	_rightEyeFBO->m_stencilRenderBfrId = 0;
	_rightEyeFBO->m_isComplete = FALSE;
	return ret;
}
//------------------------------------------------------------------------>

GLboolean FBOManager::GenerateRightEyeBackBufferFBO(RIGHT_EYE_FBO_T* _rightEyeFBO)
{
	GLboolean ret = FALSE;

	// FBO ID
	GLuint auxFBO;
	// Color Texture ID
	GLuint auxTexture;
	// RenderBufferId
	GLuint steId;

	// Generate the RenderBuffer
	glGenRenderbuffers(1, &steId);
	glBindRenderbuffer(GL_RENDERBUFFER, steId);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, g_width, g_height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	// Generate the FBO
	GLenum status;
	(*orig_glGenFramebuffers)(1, &auxFBO);
	(*orig_glBindFramebuffer)(GL_FRAMEBUFFER, auxFBO);
	// Generate the Texture
	(*orig_glGenTextures)(1, &auxTexture);
	(*orig_glBindTexture)(GL_TEXTURE_2D, auxTexture);
	(*orig_glTexImage2D)(GL_TEXTURE_2D, 0, GL_RGBA, g_width, g_height, 0, GL_RGBA, GL_UNSIGNED_INT, NULL);
	(*orig_glTexParameteri)(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	(*orig_glTexParameteri)(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Attach the Color Texture
	(*orig_glFramebufferTexture2D)(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, auxTexture, 0);
	// attach a renderbuffer to depth attachment point
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, steId);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, steId);

	glDrawBuffer(GL_COLOR_ATTACHMENT0);
	// Check to see if complete
	status = (*orig_glCheckFramebufferStatus)(GL_FRAMEBUFFER);

	if (status == GL_FRAMEBUFFER_COMPLETE)
	{
		// Store the Information in our vector
		_rightEyeFBO->m_FBOId = auxFBO;
		_rightEyeFBO->m_colorTexId[0] = auxTexture;
		_rightEyeFBO->m_stencilRenderBfrId = steId;
		_rightEyeFBO->m_isComplete = TRUE;
		ret = TRUE;
	}

	// Revert to original FrameBuffer
	(*orig_glBindTexture)(GL_TEXTURE_2D, 0);
	(*orig_glBindFramebuffer)(GL_FRAMEBUFFER, 0);
	return ret;
}
//------------------------------------------------------------------------>