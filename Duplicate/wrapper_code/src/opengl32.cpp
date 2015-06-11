/*
* Game-Deception Blank Wrapper v2
* Copyright (c) Helifax 2015
*/

extern "C" {
#include "opengl_3dv.h"
}
#include <stdio.h>
#include <direct.h>
#include <iostream>
#include <fstream>
#include <memory>
#include "opengl32.h"
#include "include\ShaderManager.h"
#include "windows.h"
#include "d3d9.h"
#include "NVAPI_343\nvapi.h"
#include <thread>
#include "FPSInject/FPSInject.h"
#include "MessageBox\MessageBox.h"
#include "ConfigReader.h"
#include "opengl32_ARB_Variant.h"
#include "wrapper_version.h"
#include "WindowManager.h"
#include "FboManager.h"
#include "TextureManager.h"
#include "crc32.h"

// FPSInject
HINSTANCE FPSHandle;
typedef FPSInject* (__cdecl *create)();
typedef void(*destroy)(FPSInject*);
create FPScreate;
destroy FPSDestroy;
static void LoadFPSInject(void);
static bool isFPSInjectHandleValid(void);
static bool m_areExtensionsLoade = false;

// Our object
FPSInject* m_FPSInjector;

// Window Information
unsigned int g_width = 0;
unsigned int g_height = 0;
HWND g_MainWindowHandle;
bool g_windowResizeCheck = false;

// 3D Vision
GLD3DBuffers * gl_d3d_buffers;

//Config reader
extern configReader *g_reader;

// normal buffer swapping on BindFrameBuffer
static void bindFrameBufferSwap(void);

// Automatic GL_SWAPBUFFERs - Auto hook point
static void AutomaticGlSwapBuffers(void);

// End frame method used in automatic hooking 
static void EndFrames(void);

// buffer swapping on SwapBuffer
static void swapBuffersSwap(void);

#ifdef DEBUG_WRAPPER
int debugVertexIndex = -1;
int debugPixelIndex = -1;
bool printCurrentShader = false;
bool isCurrentShaderVertex = true;
bool isCompileEnable = false;
#endif

//********************************************************************************************
// DEBUG FIND ENTRY POINT
//********************************************************************************************

//log_glBindFramebuffer
void log_glBindFramebuffer(GLenum target, GLuint framebuffer)
{
	char text[100];
	sprintf_s(text, "glBindFramebuffer %d Called !", framebuffer);
	if (framebuffer == 0)
	{
		//this is our ScreenFramebuffer
		strcat_s(text, 100, " This is the SCREEN FRAMEBUFFER !!!");
	}
	else
	{
		strcat_s(text, 100, " Ignore this one");
	}
	add_log(text);
	if (orig_glBindFramebuffer != 0)
	{
		(*orig_glBindFramebuffer)(target, framebuffer);
	}
}
///-------------------------------------------------------------------------------------------

//log_glBindFramebufferEXT
void log_glBindFramebufferEXT(GLenum target, GLuint framebuffer)
{
	char text[100];
	sprintf_s(text, "glBindFramebufferEXT %d Called !", framebuffer);
	if (framebuffer == 0)
	{
		//this is our ScreenFramebuffer
		strcat_s(text, 100, " This is the SCREEN FRAMEBUFFER !!!");
	}
	else
	{
		strcat_s(text, 100, " Ignore this one");
	}
	add_log(text);
	if (orig_glBindFramebuffer != 0)
	{
		(*orig_glBindFramebuffer)(target, framebuffer);
	}
}
///-------------------------------------------------------------------------------------------

//log_glBindFramebufferARB
void log_glBindFramebufferARB(GLenum target, GLuint framebuffer)
{
	char text[100];
	sprintf_s(text, 100, "glBindFramebufferARB %d Called !", framebuffer);
	if (framebuffer == 0)
	{
		//this is our ScreenFramebuffer
		strcat_s(text, 100, " This is the SCREEN FRAMEBUFFER !!!");
	}
	else
	{
		strcat_s(text, 100, " Ignore this one");
	}
	add_log(text);
	if (orig_glBindFramebuffer != 0)
	{
		(*orig_glBindFramebuffer)(target, framebuffer);
	}
}
///-------------------------------------------------------------------------------------------

//********************************************************************************************
// END DEBUG FIND ENTRY POINT
//********************************************************************************************

//sys_glBindFramebuffer
void sys_glBindFramebuffer(GLenum target, GLuint framebuffer)
{
	// If we have the injection point enabled
	if (g_reader->Get3DVisionEnabledStatus())
	{
		// We are rendering the left frame, so let it run natively
		if (framebuffer == GLuint(0) && GetInterop() == false)
		{
			if ((NV3DVisionGetCurrentFrame() == 1))
			{
				NV3DVisionBuffers_activate_depth(gl_d3d_buffers);
				NV3DVisionBuffers_activate_left(gl_d3d_buffers);
			}
			else if ((NV3DVisionGetCurrentFrame() == 2))
			{

				NV3DVisionBuffers_activate_right(gl_d3d_buffers);
			}
		}
		else //not our framebuffer 0
		{
			//Original Frame buffer
			(*orig_glBindFramebuffer)(target, framebuffer);
		}
	}
	else
	{
		//Original Frame buffer
		(*orig_glBindFramebuffer)(target, framebuffer);
	}

}
///-------------------------------------------------------------------------------------------

// normal buffer swapping on BindFrameBuffer
static void bindFrameBufferSwap(void)
{
	//initializes
	if (NV3DVisionIsNotInit())
	{
		// Load our extensions
		if (!m_areExtensionsLoade)
		{
			if (ogl_LoadFunctions() == ogl_LOAD_FAILED)
			{
				add_log("Failed to get OpenGL extensions (ogl_LOAD_FAILED)");
			}
			else
			{
				add_log("OpenGL extensions Retrieved !!!");
			}
			m_areExtensionsLoade = true;
		}
		//Create our buffers
		gl_d3d_buffers = new GLD3DBuffers;

		g_MainWindowHandle = FindWindowFromProcessId();
		NV3DVisionBuffers_create(gl_d3d_buffers, g_MainWindowHandle, true);

		// Start our Threads
		StartResizeThread();
		StartNVAPIThread();

		//start the rendering
		NV3DVisionSetCurrentFrame(1);
		add_log("3D Vision successfully initialised !");

		// FPS Inject
		if (isFPSInjectHandleValid())
		{
			m_FPSInjector->synchronizeRendering();
		}
	}

	else if (NV3DVisionGetCurrentFrame() == 1)
	{
		// Sync Left eye
		if (isFPSInjectHandleValid())
		{
			m_FPSInjector->syncLeft();
		}
		NV3DVisionSetCurrentFrame(2);

	}
	else if (NV3DVisionGetCurrentFrame() == 2)
	{
		// Sync Right eye
		if (isFPSInjectHandleValid())
		{
			m_FPSInjector->syncRight();
		}
		NV3DVisionSetCurrentFrame(1);

		// both eyes are rendered. So swap buffers and reset counter
		//Flip the framebuffers vetically
		NV3DVisionBuffers_flip_left(gl_d3d_buffers);
		NV3DVisionBuffers_flip_right(gl_d3d_buffers);

		// Do the rendering
		NV3DVisionBuffers_deactivate(gl_d3d_buffers);
		NV3DVisionBuffers_flush(gl_d3d_buffers);
		
		//check for resize
		if (g_windowResizeCheck == true)
		{
			// Create the new buffers
			g_MainWindowHandle = FindWindowFromProcessId();
			// create DirectX 3D Vision buffers
			NV3DVisionReInitBuffers();
			NV3DVisionBuffers_destroy(gl_d3d_buffers);

			//Resize complete
			g_windowResizeCheck = false;

			NV3DVisionBuffers_create(gl_d3d_buffers, g_MainWindowHandle, true);
		}
	}
}
///-------------------------------------------------------------------------------------------

// buffer swapping on SwapBuffer
static void swapBuffersSwap(void)
{
	if (NV3DVisionGetCurrentFrame() == 1)
	{
		// Initialize the WRAPPER!!! BANG BANG! 3D Vision Active baby!!!!
		if (NV3DVisionIsNotInit())
		{
			// Load our extensions
			if (!m_areExtensionsLoade)
			{
				if (ogl_LoadFunctions() == ogl_LOAD_FAILED)
				{
					add_log("Failed to get OpenGL extensions (ogl_LOAD_FAILED)");
				}
				else
				{
					add_log("OpenGL extensions Retrieved !!!");
				}
				m_areExtensionsLoade = true;
			}

			//Create our buffers
			gl_d3d_buffers = new GLD3DBuffers;

			g_MainWindowHandle = FindWindowFromProcessId();
			NV3DVisionBuffers_create(gl_d3d_buffers, g_MainWindowHandle, true);

			// Start our Threads
			StartResizeThread();
			StartNVAPIThread();

			add_log("3D Vision successfully initialised !");

			// FPS Inject
			if (isFPSInjectHandleValid())
			{
				m_FPSInjector->synchronizeRendering();
			}
		}
		else
		{
			// Render what we have and continue rendering with the left eye
			//Flip the framebuffers vertically
			NV3DVisionBuffers_flip_left(gl_d3d_buffers);
			NV3DVisionBuffers_flip_right(gl_d3d_buffers);

			// Do the rendering
			NV3DVisionBuffers_deactivate(gl_d3d_buffers);
			NV3DVisionBuffers_flush(gl_d3d_buffers);

			//check for resize
			if (g_windowResizeCheck == true)
			{
				// Create the new buffers
				g_MainWindowHandle = FindWindowFromProcessId();
				// create DirectX 3D Vision buffers
				NV3DVisionReInitBuffers();
				NV3DVisionBuffers_destroy(gl_d3d_buffers);

				//Resize complete
				g_windowResizeCheck = false;

				NV3DVisionBuffers_create(gl_d3d_buffers, g_MainWindowHandle, true);
			}
			//start the rendering
			NV3DVisionSetCurrentFrame(2);
			//activate left buffer so everything is copied in it
			NV3DVisionBuffers_activate_depth(gl_d3d_buffers);
			NV3DVisionBuffers_activate_left(gl_d3d_buffers);

			// Sync Left eye
			if (isFPSInjectHandleValid())
			{
				m_FPSInjector->syncLeft();
			}
		}
	}
	else if (NV3DVisionGetCurrentFrame() == 2)
	{
		//activate right buffer so everything is copied in it
		NV3DVisionSetCurrentFrame(1);
		NV3DVisionBuffers_activate_right(gl_d3d_buffers);

		// Sync RIGHT eye
		if (isFPSInjectHandleValid())
		{
			m_FPSInjector->syncRight();
		}
	}
}
///-------------------------------------------------------------------------------------------

//********************************************************************************************
// SHADER MANIPULATION
//********************************************************************************************

GLuint sys_glCreateProgram(void)
{
	GLuint program = (*orig_glCreateProgram)();
	return program;
}
///-------------------------------------------------------------------------------------------

#include <mutex>
static std::recursive_mutex m_opengl32Mutex;

#include <fstream>
#include <string>
#include <sstream>


// We will inject the stereo here 
void sys_glAttachShader(GLuint program, GLuint shader)
{
	m_opengl32Mutex.lock();

	orig_glCompileShader = (func_glCompileShader)orig_wglGetProcAddress("glCompileShader");
	if (orig_glCompileShader == 0x0)
		add_log("glCompileShader not found !!!!");
	orig_glGetShaderiv = (func_glGetShaderiv_t)orig_wglGetProcAddress("glGetShaderiv");
	if (orig_glGetShaderiv == 0x0)
		add_log("glGetShaderiv not found !!!!");

	// Add the CRC of the program
	std::string programSring = std::to_string(program);
	DWORD progCRC32 = crc32buf(programSring.c_str(), programSring.length());
	
	// Get the correct shader type
	// Get the instance
	ShaderManager *shaderManager = ShaderManager::getInstance();
	
	// Depends on what type of shader it is we do stuff
	if (shaderManager->isShaderType(shader, GL_VERTEX_SHADER))
	{
		//get the original shader Source
		std::string shaderSource = shaderManager->getShaderSource(shader);

		// Calculate the CRC32 before we do any injection
		// Otherwise the CRC32 will change
		DWORD crc32 = crc32buf(shaderSource.c_str(), shaderSource.length());
		crc32 += progCRC32;

		if (shaderManager->GetVertexInjectionState())
		{
			//Apply the custom shaders
			// If we failed apply normal injection
			if (shaderManager->ApplyExceptionShaders(shaderSource, GL_VERTEX_SHADER, crc32) == false)
			{
				//Insert Stereo
				shaderSource = shaderManager->injectStereoScopy(shaderSource, program);
			}
		}

		// Swap the Source
		shaderManager->applyShaderSource(shader, shaderSource, NULL);

		// Store it as an existing shader
		EXISTING_SHADER_T currentShader;
		currentShader.m_CRC32 = crc32;
		currentShader.m_programId = program;
		currentShader.m_shaderId = shader;
		currentShader.m_shaderSourceCode = shaderSource;
		currentShader.m_shaderType = GL_VERTEX_SHADER;
		// Push it back
		shaderManager->addExistingShaderInfo(currentShader);
		
		// Export the Source
		ExportShader("Vertex", currentShader.m_programId, currentShader.m_CRC32, currentShader.m_shaderSourceCode);

		// We also need to compile the shader before attaching it
		//Compile shader
		(*orig_glCompileShader)(shader);

		//Test compile
		GLint statusOk = 0;
		(*orig_glGetShaderiv)(shader, GL_COMPILE_STATUS, &statusOk);

		if (statusOk == GL_FALSE)
		{
			CheckCompileState(shader);
		}
	}
	else if (shaderManager->isShaderType(shader, GL_FRAGMENT_SHADER))
	{
		//get the original shader Source
		std::string shaderSource = shaderManager->getShaderSource(shader);

		// Calculate the CRC32
		DWORD crc32 = crc32buf(shaderSource.c_str(), shaderSource.length());
		crc32 += progCRC32;

		if (shaderManager->GetVertexInjectionState())
		{
			//Apply the custom shaders
			// If we failed apply normal injection
			if (shaderManager->ApplyExceptionShaders(shaderSource, GL_FRAGMENT_SHADER, crc32) == false)
			{
				// This only happens in development mode.
#ifdef DEBUG_WRAPPER
				//Insert Stereo
				shaderSource = shaderManager->injectFragmentModification(shaderSource, program);
#endif
			}
		}

		// Swap the Source
		shaderManager->applyShaderSource(shader, shaderSource, NULL);

		// Store it as an existing shader
		EXISTING_SHADER_T currentShader;
		currentShader.m_CRC32 = crc32;
		currentShader.m_programId = program;
		currentShader.m_shaderId = shader;
		currentShader.m_shaderSourceCode = shaderSource;
		currentShader.m_shaderType = GL_FRAGMENT_SHADER;
		// Push it back
		shaderManager->addExistingShaderInfo(currentShader);

		// Export the Source
		ExportShader("Pixel", currentShader.m_programId, currentShader.m_CRC32, currentShader.m_shaderSourceCode);

		//Compile shader
		(*orig_glCompileShader)(shader);

		// We also need to compile the shader before attaching it
		//Test compile
		GLint statusOk = 0;
		(*orig_glGetShaderiv)(shader, GL_COMPILE_STATUS, &statusOk);

		if (statusOk == GL_FALSE)
		{
			CheckCompileState(shader);
		}
	}
	
	// We attach after we swapped the sources
	(*orig_glAttachShader)(program, shader);

	m_opengl32Mutex.unlock();
}
///-------------------------------------------------------------------------------------------

void sys_glShaderSource(GLuint shader, GLsizei count, const GLchar **string, const GLint *length)
{
	// Apply the original source
	(*orig_glShaderSource)(shader, count, string, length);
}
///-------------------------------------------------------------------------------------------

void sys_glCompileShader(GLuint shader)
{
	// Compile the shader
	(*orig_glCompileShader)(shader);

	// Check to see if the compile was ok
	GLint OK = GL_FALSE;
	orig_glGetShaderiv = (func_glGetShaderiv_t)orig_wglGetProcAddress("glGetShaderiv");
	(*orig_glGetShaderiv)(shader, GL_COMPILE_STATUS, &OK);

	if (OK == GL_FALSE)
	{
		CheckCompileState(shader);
		exit(0);
	}
}
///-------------------------------------------------------------------------------------------

void sys_glLinkProgram(GLuint program)
{
	// Link the original program now that the correct shaders are swapped
	(*orig_glLinkProgram)(program);

	//Check for error
	GLint LinkOK = 0;
	orig_glGetProgramiv = (func_glGetProgramiv_t)orig_wglGetProcAddress("glGetProgramiv");
	(*orig_glGetProgramiv)(program, GL_LINK_STATUS, &LinkOK);
	if (LinkOK == GL_FALSE)
	{
		CheckLinkState(program);
	}
}
///-------------------------------------------------------------------------------------------

// add the stereo values
void sys_glUseProgram(GLuint program)
{
	m_opengl32Mutex.lock();

	// Get the shaderManager
	ShaderManager * shaderManager = ShaderManager::getInstance();

#ifdef DEBUG_WRAPPER
	if (shaderManager->GetVertexInjectionState())
	{
		shaderManager->ApplyDebugExceptionShaders();
	}
#endif

	// Use the original program
	(*orig_glUseProgram)(program);
	// Set the program as being current
	shaderManager->SetActiveShaderProgram(program);

	if (shaderManager->GetVertexInjectionState())
	{
		// Get the locations of the uniforms
		GLfloat value;
		const GLchar *uniform_eye = (GLchar*)"g_eye";
		const GLchar *uniform_eye_spearation = (GLchar*)"g_eye_separation";
		const GLchar *uniform_convergence = (GLchar*)"g_convergence";
		const GLchar *uniform_vertexEnabled = (GLchar*)"g_vertexEnabled";
		const GLchar *uniform_pixelEnabled = (GLchar*)"g_pixelEnabled";


		// Get our functions from the driver. Be sure to only take them once
		if (orig_glGetUniformLocation == NULL)
		{
			orig_glGetUniformLocation = (func_glGetUniformLocation_t)(orig_wglGetProcAddress)("glGetUniformLocation");
		}
		//glUniform1f
		if (orig_glUniform1f == NULL)
		{

			orig_glUniform1f = (func_glUniform1f_t)(orig_wglGetProcAddress)("glUniform1f");
		}

		if ((GLint)program >= 0)
		{
			GLint location_eye = (*orig_glGetUniformLocation)(program, uniform_eye);
			GLint location_eye_separation = (*orig_glGetUniformLocation)(program, uniform_eye_spearation);
			GLint location_convergence = (*orig_glGetUniformLocation)(program, uniform_convergence);
			GLint location_vertexEnabled = (*orig_glGetUniformLocation)(program, uniform_vertexEnabled);
			GLint location_pixelEnabled = (*orig_glGetUniformLocation)(program, uniform_pixelEnabled);

			//set the uniform inside the shaders
			if (NV3DVisionGetCurrentFrame() == 1)
			{
				//left eye
				value = -1.0f;
				//(*orig_glUniform1f)(location_eye, value);
			}
			else
			{
				//right eye
				value = +1.0f;
				//(*orig_glUniform1f)(location_eye, value);
			}
			//Set the Separation and convergence
			(*orig_glUniform1f)(location_eye_separation, Get3DEyeSeparation());
			(*orig_glUniform1f)(location_convergence, Get3DConvergence());
			(*orig_glUniform1f)(location_vertexEnabled, 1.0f);
			(*orig_glUniform1f)(location_pixelEnabled, 1.0f);
		}

		/////////////////////////////////////
		// APPLY CUSTOM SHADER PARAMS
		/////////////////////////////////////
		shaderManager->ApplyCustomShaderPairs(program);
		
		/////////////////////////////////////
		// USED TO DISABLE ALOT OF SHADERS
		/////////////////////////////////////
#ifdef	DEBUG_WRAPPER
		if (shaderManager->VertexShadersExceptionsEnabled())
		{
			if (((GLint)program >= (GLint)shaderManager->GetExceptionShaderStart() && (GLint)program <= (GLint)shaderManager->GetExceptionShaderEnd()))
			{
				GLint location_eye_separation = (*orig_glGetUniformLocation)(program, uniform_eye_spearation);
				GLint location_convergence = (*orig_glGetUniformLocation)(program, uniform_convergence);

				//Set the Separation and convergence
				(*orig_glUniform1f)(location_eye_separation, 0);
				(*orig_glUniform1f)(location_convergence, 0);

				// Do we disable the vertex shaders ???
				if (shaderManager->VertexShaderExceptionsDisableShader())
				{
					GLint location_enabled = (*orig_glGetUniformLocation)(program, uniform_vertexEnabled);
					(*orig_glUniform1f)(location_enabled, 0.0f);
				}
				else
				{
					GLint location_enabled = (*orig_glGetUniformLocation)(program, uniform_vertexEnabled);
					(*orig_glUniform1f)(location_enabled, 1.0f);
				}
			}
			if (shaderManager->DisableCurrentShader())
			{
				// If we are navigating vertexes
				if (isCurrentShaderVertex)
				{
					// Disable the current Shader
					if (debugVertexIndex == (int)program)
					{
						GLint location_enabled = (*orig_glGetUniformLocation)(program, uniform_vertexEnabled);
						(*orig_glUniform1f)(location_enabled, 0.0f);
					}
				}
				else // Pixel Shader
				{
					// Disable the current Shader
					if (debugPixelIndex == (int)program)
					{
						GLint location_enabled = (*orig_glGetUniformLocation)(program, uniform_pixelEnabled);
						(*orig_glUniform1f)(location_enabled, 0.0f);
					}
				}
			}
		}
#endif	
	}

	m_opengl32Mutex.unlock();
}

//********************************************************************************************
// END SHADER MANIPULATION
//********************************************************************************************

///-------------------------------------------------------------------------------------------

const GLubyte * sys_glGetString(GLenum name)
{
	return (*orig_glGetString) (name);
}
///-------------------------------------------------------------------------------------------

//Initializes and renders
void sys_wglSwapBuffers(HDC hDC)
{
	//Avoid warning
	(void)hDC;

	if (g_reader->GetInjectionPoint() == "DUPLICATE_BUFFERS")
	{
		//initializes
		if (NV3DVisionIsNotInit() && g_reader->Get3DVisionEnabledStatus())
		{
			// Load our extensions
			if (!m_areExtensionsLoade)
			{
				if (ogl_LoadFunctions() == ogl_LOAD_FAILED)
				{
					add_log("Failed to get OpenGL extensions (ogl_LOAD_FAILED)");
				}
				else
				{
					add_log("OpenGL extensions Retrieved !!!");
				}
				m_areExtensionsLoade = true;
			}

			//Create our buffers
			gl_d3d_buffers = new GLD3DBuffers;

			g_MainWindowHandle = FindWindowFromProcessId();
			NV3DVisionBuffers_create(gl_d3d_buffers, g_MainWindowHandle, true);

			// Start our Threads
			StartResizeThread();
			StartNVAPIThread();

			//start the rendering
			NV3DVisionSetCurrentFrame(1);

			// FPS Inject
			if (isFPSInjectHandleValid())
			{
				m_FPSInjector->synchronizeRendering();
			}
			add_log("3D Vision Wrapper working in FBO Duplication Mode !!!");
			add_log("3D Vision successfully initialized !");

			//////////////////////////////////////////////////////////////////////////
			// Get our FBO manager and Duplicate the BACK FBO
			FBOManager * fboManager = FBOManager::getInstance();
			fboManager->AddStereoFBOs(0, TRUE);
			if (fboManager->IsRightEyeFBOComplete(0) == FALSE)
			{
				// Something Failed
				add_log("Could not generate Right Eye BACK FrameBuffer... Abort!");
			}
		}
		else if (g_reader->Get3DVisionEnabledStatus())
		{
			FBOManager *fboManager = FBOManager::getInstance();
			GLint currentFBO = -1;
			glGetIntegerv(GL_FRAMEBUFFER_BINDING, &currentFBO);
			GLuint rightFBO = fboManager->GetRightEyeFBO(currentFBO);

			// Copy the content from our FBOs in the 3DVision buffers and present them
			NV3DVisionBuffers_flipAndFlush(gl_d3d_buffers, rightFBO);
			
			// Reset the bound textures
			fboManager->ResetFBOBoundMode();
			TextureManager *texManager = TextureManager::getInstance();
			texManager->ResetTextureBoundMode();
		}
		else
		{
			// Disabled
			(*orig_wglSwapBuffers)(hDC);
		}

	}
	else
	{
		// If we have the injection point enabled
		if (g_reader->Get3DVisionEnabledStatus())
		{
			if (g_reader->IsAutomaticHookMode())
			{
				// If Automatic Hooking point is enabled
				AutomaticGlSwapBuffers();
			}
			else
			{
				add_log("Manual Hooking Enabled SwapBuffers !!!");

				// If Manual Hooking point is enabled
				std::string injectionPoint = g_reader->GetInjectionPoint();

				// if we swap buffer on framebuffer bind
				if (injectionPoint != "glSwapBuffers" && injectionPoint != "SCREEN_BUFFER")
				{
					bindFrameBufferSwap();
				}
				// if we swap buffer on wgl_swapbuffers
				else if (injectionPoint == "glSwapBuffers")
				{
					swapBuffersSwap();
				}
			}
		}
		else
		{
			if (g_reader->GetPrintDebugInfoState())
			{
				add_log("glSwapBuffers Called !");
			}
			(*orig_wglSwapBuffers)(hDC);
		}
	}
}
///-------------------------------------------------------------------------------------------

/// Windows Variant of SwapBuffers

void sys_wglSwapLayerBuffers(HDC hdc, UINT fuPlanes)
{
	//Avoid warning
	(void)hdc;
	(void)fuPlanes;

	sys_wglSwapBuffers(hdc);
}
///-------------------------------------------------------------------------------------------

//////////////////////////////////////////////////////////////////////////
/// AUTOMATIC SWAPPING
//////////////////////////////////////////////////////////////////////////

// The last Step before actually swapping the buffers
static void EndFrames(void)
{
	// Copy the right eye
	if (g_windowResizeCheck == false)
	{
		std::string injectionPoint = g_reader->GetInjectionPoint();
		if (injectionPoint != "glSwapBuffers" && injectionPoint != "SCREEN_BUFFER")
		{
			NV3DVisionBuffers_flip_right(gl_d3d_buffers);
		}
		else if (injectionPoint == "SCREEN_BUFFER")
		{
			NV3DVisionBuffers_copy_right(gl_d3d_buffers);
		}
	}

	// Deactivate 
	NV3DVisionBuffers_deactivate_right(gl_d3d_buffers);
	NV3DVisionBuffers_deactivate(gl_d3d_buffers);

	// We rendered everything so first we need to flush.
	NV3DVisionBuffers_flush(gl_d3d_buffers);

	//check for resize
	if (g_windowResizeCheck == true)
	{
		// Create the new buffers
		g_MainWindowHandle = FindWindowFromProcessId();
		// create DirectX 3D Vision buffers
		NV3DVisionReInitBuffers();
		NV3DVisionBuffers_destroy(gl_d3d_buffers);

		//Resize complete
		g_windowResizeCheck = false;

		NV3DVisionBuffers_create(gl_d3d_buffers, g_MainWindowHandle, true);
	}
}
///-------------------------------------------------------------------------------------------

//Initializes and renders
static void AutomaticGlSwapBuffers(void)
{
	//initializes
	if (NV3DVisionIsNotInit())
	{
		// Print just once:)
		add_log("Automatic Hooking Enabled !!!");

		// Load our extensions
		if (!m_areExtensionsLoade)
		{
			if (ogl_LoadFunctions() == ogl_LOAD_FAILED)
			{
				add_log("Failed to get OpenGL extensions (ogl_LOAD_FAILED)");
			}
			else
			{
				add_log("OpenGL extensions Retrieved !!!");
			}
			m_areExtensionsLoade = true;
		}

		//Create our buffers
		gl_d3d_buffers = new GLD3DBuffers;

		g_MainWindowHandle = FindWindowFromProcessId();
		NV3DVisionBuffers_create(gl_d3d_buffers, g_MainWindowHandle, true);

		// Start our Threads
		StartResizeThread();
		StartNVAPIThread();

		//start the rendering
		NV3DVisionSetCurrentFrame(1);

		// FPS Inject
		if (isFPSInjectHandleValid())
		{
			m_FPSInjector->synchronizeRendering();
		}
	}
	// If we are initialised
	if (!NV3DVisionIsNotInit())
	{
		if (NV3DVisionGetCurrentFrame() == 2)
		{
			EndFrames();
			NV3DVisionBuffers_activate_depth(gl_d3d_buffers);
			NV3DVisionBuffers_activate_left(gl_d3d_buffers);
			NV3DVisionSetCurrentFrame(1);

			// Sync Left eye
			if (isFPSInjectHandleValid())
			{
				m_FPSInjector->syncLeft();
			}
		}
		else if (NV3DVisionGetCurrentFrame() == 1)
		{
			// Sync Right eye
			if (isFPSInjectHandleValid())
			{
				m_FPSInjector->syncRight();
			}

			if (g_windowResizeCheck == false)
			{
				std::string injectionPoint = g_reader->GetInjectionPoint();
				if (injectionPoint != "glSwapBuffers" && injectionPoint != "SCREEN_BUFFER")
				{
					NV3DVisionBuffers_flip_left(gl_d3d_buffers);
				}
				else if (injectionPoint == "SCREEN_BUFFER")
				{
					NV3DVisionBuffers_copy_left(gl_d3d_buffers);
				}
			}
			NV3DVisionBuffers_deactivate_left(gl_d3d_buffers);
			NV3DVisionBuffers_activate_right(gl_d3d_buffers);

			NV3DVisionSetCurrentFrame(2);
		}
	}
}
///-------------------------------------------------------------------------------------------

//////////////////////////////////////////////////////////////////////////
/// END OF AUTOMATIC SWAPPING
//////////////////////////////////////////////////////////////////////////

void sys_glGenTextures(GLsizei n, GLuint *textures)
{
	TextureManager *texManager = TextureManager::getInstance();

	// Generate the textures
	(*orig_glGenTextures) (n, textures);

	// Store the results
	GLuint *returnedIds = new GLuint[n];
	for (GLsizei i = 0; i < n; i++)
	{
		returnedIds[i] = *textures;
		textures += sizeof(GLuint);

		/// Stereo texture generation
		texManager->AddStereoTextures(returnedIds[i]);
	}
}
///-------------------------------------------------------------------------------------------

void sys_glBindMultiTextureEXT(GLenum mode, GLenum target, GLint texture)
{
	// Run the original Binding Mode
	(*orig_glBindMultiTexture)(mode, target, texture);

	// Add the current texture as active
	TextureManager *texManager = TextureManager::getInstance();
	texManager->AddTextureBoundMode(mode, target, texture);
}
///-------------------------------------------------------------------------------------------

void sys_glBindTexture(GLenum target, GLuint texture)
{
	(*orig_glBindTexture) (target, texture);

	// Add the current texture as active
	TextureManager *texManager = TextureManager::getInstance();
	texManager->AddTextureBoundMode(GL_INVALID_ENUM, target, texture);
}
///-------------------------------------------------------------------------------------------

void sys_glActiveTexture(GLenum target)
{
	orig_glActiveTexture(target);
}
///-------------------------------------------------------------------------------------------

void sys_glTexImage2DMultisample(GLenum target, GLsizei samples, GLint internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations)
{
	// Run the original Function
	(*orig_glTexImage2DMultisample)(target, samples, internalformat, width, height, fixedsamplelocations);

	// Set the same for the pair Texture
	TextureManager *texManager = TextureManager::getInstance();
	GLenum mode;
	GLenum targetBound;
	GLint leftEyeTex;
	
	// Get the current bound texture
	texManager->GetTextureBoundMode(&mode, &targetBound, &leftEyeTex);

	// Get the stereo Texture pair
	GLuint rightEyeTex = texManager->GetRightEyeTextureId(leftEyeTex);

	// Bind the right eye Texture
	(*orig_glBindMultiTexture)(mode, targetBound, rightEyeTex);

	// Run the same command on the Right Eye texture
	(*orig_glTexImage2DMultisample)(target, samples, internalformat, width, height, fixedsamplelocations);

	// Need to return to the left eye texture binding
	(*orig_glBindMultiTexture)(mode, targetBound, leftEyeTex);
}
///-------------------------------------------------------------------------------------------

void sys_glTextureSubImage2D(GLuint texture, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void* pixels)
{
	(*orig_glTextureSubImage2D)(texture, target, level, xoffset, yoffset, width, height, format, type, pixels);
	// Set the same for the pair Texture
	TextureManager *texManager = TextureManager::getInstance();
	GLenum mode;
	GLenum targetBound;
	GLint leftEyeTex;

	// Get the current bound texture
	texManager->GetTextureBoundMode(&mode, &targetBound, &leftEyeTex);

	// Get the stereo Texture pair
	GLuint rightEyeTex = texManager->GetRightEyeTextureId(leftEyeTex);

	// Bind the right eye Texture
	(*orig_glBindMultiTexture)(mode, targetBound, rightEyeTex);

	//////////////////////////////////////////////////////////////////////////
	// Run the same command on the Right Eye texture
	(*orig_glTextureSubImage2D)(rightEyeTex, target, level, xoffset, yoffset, width, height, format, type, pixels);

	// Need to return to the left eye texture binding
	(*orig_glBindMultiTexture)(mode, targetBound, leftEyeTex);
}
///-------------------------------------------------------------------------------------------

//////////////////////////////////////////////////////////////////////////
/// END OF TEXTURE MANAGEMENT
//////////////////////////////////////////////////////////////////////////

void sys_glGenFramebuffers(GLsizei n, GLuint *ids)
{
	FBOManager *fboManager = FBOManager::getInstance();

	// Generate the original Frame Buffer calls
	(*orig_glGenFramebuffers)(n, ids);

	// Store the results
	GLuint *returnedIds = new GLuint[n];
	for (GLsizei i = 0; i < n; i++)
	{
		returnedIds[i] = *ids;
		ids += sizeof(GLuint);

		/// Stereo buffers generation
		fboManager->AddStereoFBOs(returnedIds[i], FALSE);
	}
	
	// Clean up
	delete[] returnedIds;
}
///-------------------------------------------------------------------------------------------

void sys_glBindFramebufferAlternative(GLenum target, GLuint framebuffer)
{
	// Run the original Command
	(*orig_glBindFramebuffer)(target, framebuffer);

	FBOManager * fboManager = FBOManager::getInstance();
	fboManager->AddFBOBoundMode(target, framebuffer);
}
///-------------------------------------------------------------------------------------------

void sys_glFramebufferTexture(GLenum target, GLenum attachment, GLuint texture, GLint level)
{
	FBOManager *fboManager = FBOManager::getInstance();

	// Run original Function
	(*orig_glFramebufferTexture)(target, attachment, texture, level);

	// Get Current Bound FBO information
	GLenum currentTarget;
	GLuint currentFBO;
	fboManager->GetFBOBoundMode(&currentTarget, &currentFBO);

	// Get Right Eye FBO
	GLuint rightEyeFramebuffer = fboManager->GetRightEyeIncompleteFBO(currentFBO);

	// Bind the Right Eye FBO so we can duplicate the stuff
	(*orig_glBindFramebuffer)(currentTarget, rightEyeFramebuffer);

	// Duplicate the call
	// But we need the correct TEXTURE !!!!!
	TextureManager *texManager = TextureManager::getInstance();
	GLuint rightEyeTex = texManager->GetRightEyeTextureId(texture);

	(*orig_glFramebufferTexture)(target, attachment, rightEyeTex, level);

	// Check and see if the right eye FBO is complete
	if ((*orig_glCheckFramebufferStatus)(currentTarget) == GL_FRAMEBUFFER_COMPLETE)
	{
		// Mark the right eye FBO as Complete
		fboManager->MarkFBOAsComplete(rightEyeFramebuffer);
	}
	// Rebind the left eye FBO
	(*orig_glBindFramebuffer)(currentTarget, currentFBO);
}
///-------------------------------------------------------------------------------------------

void sys_glFramebufferTexture2D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level)
{
	FBOManager *fboManager = FBOManager::getInstance();

	// Run original Function
	(*orig_glFramebufferTexture2D)(target, attachment, textarget, texture, level);

	// Get Current Bound FBO information
	GLenum currentTarget;
	GLuint currentFBO;
	fboManager->GetFBOBoundMode(&currentTarget, &currentFBO);
	
	// Get Right Eye FBO
	GLuint rightEyeFramebuffer = fboManager->GetRightEyeIncompleteFBO(currentFBO);

	// Bind the Right Eye FBO so we can duplicate the stuff
	(*orig_glBindFramebuffer)(currentTarget, rightEyeFramebuffer);

	// Duplicate the call
	// But we need the correct TEXTURE !!!!!
	TextureManager *texManager = TextureManager::getInstance();
	GLuint rightEyeTex = texManager->GetRightEyeTextureId(texture);

	(*orig_glFramebufferTexture2D)(target, attachment, textarget, rightEyeTex, level);

	// Check and see if the right eye FBO is complete
	//if ((*orig_glCheckFramebufferStatus)(currentTarget) == GL_FRAMEBUFFER_COMPLETE)
	{
		// Mark the right eye FBO as Complete
		fboManager->MarkFBOAsComplete(rightEyeFramebuffer);
	}
	// Rebind the left eye FBO
	(*orig_glBindFramebuffer)(currentTarget, currentFBO);
}
///-------------------------------------------------------------------------------------------

GLenum sys_glCheckFramebufferStatus(GLenum target)
{
	return (*orig_glCheckFramebufferStatus)(target);
}
///-------------------------------------------------------------------------------------------

//////////////////////////////////////////////////////////////////////////
/// END OF FRAMEBUFFER MANAGEMENT
//////////////////////////////////////////////////////////////////////////

//********************************************************************************************
// START STEREO CALLS
//********************************************************************************************
void sys_glClear(GLbitfield mask)
{
	m_opengl32Mutex.lock();

	if (g_reader->GetInjectionPoint() == "DUPLICATE_BUFFERS")
	{
		if (!NV3DVisionIsNotInit())
		{
			if (orig_glBindFramebuffer == NULL)
			{
				orig_glBindFramebuffer = (func_glBindFramebuffer_t)(orig_wglGetProcAddress)("glBindFramebufferEXT");
			}

			// Get the FBO Manager
			FBOManager * fboManager = FBOManager::getInstance();
			GLint currentFBO = -1;
			glGetIntegerv(GL_FRAMEBUFFER_BINDING, &currentFBO);
			GLint currentRightFBO = fboManager->GetRightEyeFBO(currentFBO);

			if (!GetInterop())
			{
				SetInterop(true);
				// Left eye
				(*orig_glBindFramebuffer)(GL_FRAMEBUFFER, currentFBO);
				(*orig_glClear)(mask);

				// Right eye
				(*orig_glBindFramebuffer)(GL_FRAMEBUFFER, currentRightFBO);
				(*orig_glClear)(mask);

				// Swap back to normal FB
				(*orig_glBindFramebuffer)(GL_FRAMEBUFFER, currentFBO);
				SetInterop(false);
			}
			else
			{
				(*orig_glClear)(mask);
			}
		}
		else
		{
			(*orig_glClear)(mask);
		}
	}
	else
	{
		(*orig_glClear)(mask);
	}
	m_opengl32Mutex.unlock();
}
///-------------------------------------------------------------------------------------------

void sys_glClearColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha)
{
	m_opengl32Mutex.lock();
	if (g_reader->GetInjectionPoint() == "DUPLICATE_BUFFERS")
	{
		if (!NV3DVisionIsNotInit())
		{
			if (orig_glBindFramebuffer == NULL)
			{
				orig_glBindFramebuffer = (func_glBindFramebuffer_t)(orig_wglGetProcAddress)("glBindFramebufferEXT");
			}

			// Get the FBO Manager
			FBOManager * fboManager = FBOManager::getInstance();
			GLint currentFBO = -1;
			glGetIntegerv(GL_FRAMEBUFFER_BINDING, &currentFBO);
			GLint currentRightFBO = fboManager->GetRightEyeFBO(currentFBO);

			if (!GetInterop())
			{
				SetInterop(true);
				// Left eye
				(*orig_glBindFramebuffer)(GL_FRAMEBUFFER, currentFBO);
				(*orig_glClearColor) (red, green, blue, alpha);

				// Right eye
				(*orig_glBindFramebuffer)(GL_FRAMEBUFFER, currentRightFBO);
				(*orig_glClearColor) (red, green, blue, alpha);

				// Swap back to normal FB
				(*orig_glBindFramebuffer)(GL_FRAMEBUFFER, currentFBO);
				SetInterop(false);
			}
			else
			{
				(*orig_glClearColor) (red, green, blue, alpha);
			}
		}
		else
		{
			(*orig_glClearColor) (red, green, blue, alpha);
		}
	}
	else
	{
		(*orig_glClearColor) (red, green, blue, alpha);
	}
	m_opengl32Mutex.unlock();
}
///-------------------------------------------------------------------------------------------

void sys_glDrawArrays(GLenum mode, GLint first, GLsizei count)
{
	m_opengl32Mutex.lock();
	if (g_reader->GetInjectionPoint() == "DUPLICATE_BUFFERS")
	{
		if (!NV3DVisionIsNotInit())
		{
			if (orig_glBindFramebuffer == NULL)
			{
				orig_glBindFramebuffer = (func_glBindFramebuffer_t)(orig_wglGetProcAddress)("glBindFramebufferEXT");
			}
			// Get our functions from the driver. Be sure to only take them once
			if (orig_glGetUniformLocation == NULL)
			{
				orig_glGetUniformLocation = (func_glGetUniformLocation_t)(orig_wglGetProcAddress)("glGetUniformLocation");
			}
			//glUniform1f
			if (orig_glUniform1f == NULL)
			{
				orig_glUniform1f = (func_glUniform1f_t)(orig_wglGetProcAddress)("glUniform1f");
			}

			// Get the shaderManager
			ShaderManager * shaderManager = ShaderManager::getInstance();
			GLfloat valueLeft = 0;
			GLfloat valueRight = 0;
			GLint location_eye = 0;

			// Get the FBO Manager
			FBOManager * fboManager = FBOManager::getInstance();
			GLint currentFBO = -1;
			glGetIntegerv(GL_FRAMEBUFFER_BINDING, &currentFBO);
			GLint currentRightFBO = fboManager->GetRightEyeFBO(currentFBO);


			if (shaderManager->GetVertexInjectionState())
			{
				const GLchar *uniform_eye = (GLchar*)"g_eye";
				// Get the uniform location
				location_eye = (*orig_glGetUniformLocation)(shaderManager->GetActiveShaderProgram(), uniform_eye);
				valueLeft = -1.0f;
				valueRight = 1.0f;
			}

			// Left eye
			SetInterop(true);
			(*orig_glBindFramebuffer)(GL_FRAMEBUFFER, currentFBO);
			(*orig_glUniform1f)(location_eye, valueLeft);
			(*orig_glDrawArrays) (mode, first, count);

			// Right eye
			(*orig_glBindFramebuffer)(GL_FRAMEBUFFER, currentRightFBO);
			(*orig_glUniform1f)(location_eye, valueRight);
			(*orig_glDrawArrays) (mode, first, count);


			// Swap back to normal FB
			(*orig_glBindFramebuffer)(GL_FRAMEBUFFER, currentFBO);
			SetInterop(false);
		}
		else
		{
			(*orig_glDrawArrays) (mode, first, count);
		}
	}
	else
	{
		(*orig_glDrawArrays) (mode, first, count);
	}
	m_opengl32Mutex.unlock();
}
///-------------------------------------------------------------------------------------------

void sys_glDrawElements(GLenum mode, GLsizei count, GLenum type, const GLvoid *indices)
{
	m_opengl32Mutex.lock();
	if (g_reader->GetInjectionPoint() == "DUPLICATE_BUFFERS")
	{
		if (!NV3DVisionIsNotInit())
		{
			if (orig_glBindFramebuffer == NULL)
			{
				orig_glBindFramebuffer = (func_glBindFramebuffer_t)(orig_wglGetProcAddress)("glBindFramebufferEXT");
			}
			// Get our functions from the driver. Be sure to only take them once
			if (orig_glGetUniformLocation == NULL)
			{
				orig_glGetUniformLocation = (func_glGetUniformLocation_t)(orig_wglGetProcAddress)("glGetUniformLocation");
			}
			//glUniform1f
			if (orig_glUniform1f == NULL)
			{
				orig_glUniform1f = (func_glUniform1f_t)(orig_wglGetProcAddress)("glUniform1f");
			}
			// Get the shaderManager
			ShaderManager * shaderManager = ShaderManager::getInstance();
			FBOManager * fboManager = FBOManager::getInstance();
			TextureManager *texManager = TextureManager::getInstance();

			GLfloat valueLeft = 0;
			GLfloat valueRight = 0;
			GLint location_eye = 0;

			// Get the FBO Manager
			GLint currentFBO = -1;
			glGetIntegerv(GL_FRAMEBUFFER_BINDING, &currentFBO);
			GLint currentRightFBO = fboManager->GetRightEyeFBO(currentFBO);

			if (shaderManager->GetVertexInjectionState())
			{
				const GLchar *uniform_eye = (GLchar*)"g_eye";
				// Get the uniform location
				location_eye = (*orig_glGetUniformLocation)(shaderManager->GetActiveShaderProgram(), uniform_eye);
				valueLeft = -1.0f;
				valueRight = 1.0f;
			}


			// Left eye
			SetInterop(true);
			(*orig_glBindFramebuffer)(GL_FRAMEBUFFER, currentFBO);
			(*orig_glUniform1f)(location_eye, valueLeft);
			(*orig_glDrawElements) (mode, count, type, indices);

			// Right eye
			GLenum mode = 0;
			GLenum texTarget = 0;
			GLint texId = 0;
			GLuint rightEyeTex;
			texManager->GetTextureBoundMode(&mode, &texTarget, &texId);
			rightEyeTex = texManager->GetRightEyeTextureId(texId);

			// Bind FBO
			(*orig_glBindFramebuffer)(GL_FRAMEBUFFER, currentRightFBO);
			
			// Bind Texture
			if (mode == GL_INVALID_ENUM)
			{
				(*orig_glBindTexture)(texTarget, rightEyeTex);
			}
			else
			{
				(orig_glBindMultiTexture)(mode, texTarget, rightEyeTex);
			}
			
			(*orig_glUniform1f)(location_eye, valueRight);
			(*orig_glDrawElements) (mode, count, type, indices);

			// Swap back to normal FB
			(*orig_glBindFramebuffer)(GL_FRAMEBUFFER, currentFBO);


			if (mode == GL_INVALID_ENUM)
			{
				(*orig_glBindTexture)(texTarget, texId);
			}
			else
			{
				(orig_glBindMultiTexture)(mode, texTarget, texId);
			}

			SetInterop(false);
		}
		else
		{
			(*orig_glDrawElements) (mode, count, type, indices);
		}
	}
	else
	{
		(*orig_glDrawElements) (mode, count, type, indices);
	}
	m_opengl32Mutex.unlock();
}
///-------------------------------------------------------------------------------------------

void sys_glDrawElementsBaseVertex(GLenum mode, GLsizei count, GLenum type, GLvoid *indices, GLint basevertex)
{
	m_opengl32Mutex.lock();

	if (g_reader->GetInjectionPoint() == "DUPLICATE_BUFFERS")
	{
		if (!NV3DVisionIsNotInit())
		{
			if (orig_glBindFramebuffer == NULL)
			{
				orig_glBindFramebuffer = (func_glBindFramebuffer_t)(orig_wglGetProcAddress)("glBindFramebufferEXT");
			}
			// Get our functions from the driver. Be sure to only take them once
			if (orig_glGetUniformLocation == NULL)
			{
				orig_glGetUniformLocation = (func_glGetUniformLocation_t)(orig_wglGetProcAddress)("glGetUniformLocation");
			}
			//glUniform1f
			if (orig_glUniform1f == NULL)
			{
				orig_glUniform1f = (func_glUniform1f_t)(orig_wglGetProcAddress)("glUniform1f");
			}

			// Get the shaderManager
			ShaderManager * shaderManager = ShaderManager::getInstance();
			GLfloat valueLeft = -1.0f;
			GLfloat valueRight = 1.0f;
			GLint location_eye = 0;

			// Get the FBO Manager
			FBOManager * fboManager = FBOManager::getInstance();
			GLint currentFBO = -1;
			glGetIntegerv(GL_FRAMEBUFFER_BINDING, &currentFBO);


			GLint currentRightFBO = fboManager->GetRightEyeFBO(currentFBO);

			if (shaderManager->GetVertexInjectionState())
			{
				const GLchar *uniform_eye = (GLchar*)"g_eye";
				// Get the uniform location
				location_eye = (*orig_glGetUniformLocation)(shaderManager->GetActiveShaderProgram(), uniform_eye);
			}

			// Left eye
			SetInterop(true);

			(*orig_glBindFramebuffer)(GL_FRAMEBUFFER, currentFBO);
			(*orig_glUniform1f)(location_eye, valueLeft);
			(*orig_glDrawElementsBaseVertex)(mode, count, type, indices, basevertex);

			// Right eye
			(*orig_glBindFramebuffer)(GL_FRAMEBUFFER, currentRightFBO);
			(*orig_glUniform1f)(location_eye, valueRight);
			(*orig_glDrawElementsBaseVertex)(mode, count, type, indices, basevertex);


			// Swap back to normal FB
			(*orig_glBindFramebuffer)(GL_FRAMEBUFFER, currentFBO);
			SetInterop(false);
		}
		else
		{
			(*orig_glDrawElements) (mode, count, type, indices);
		}
	}
	else
	{
		(*orig_glDrawElements) (mode, count, type, indices);
	}
	m_opengl32Mutex.unlock();
}
///-------------------------------------------------------------------------------------------

void sys_glBlitFramebuffer(GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter)
{
	m_opengl32Mutex.lock();
	
	if (g_reader->GetInjectionPoint() == "DUPLICATE_BUFFERS")
	{
		if (!NV3DVisionIsNotInit())
		{
			if (orig_glBindFramebuffer == NULL)
			{
				orig_glBindFramebuffer = (func_glBindFramebuffer_t)(orig_wglGetProcAddress)("glBindFramebufferEXT");
			}
			GLenum error = 0;

			// Get the FBO Manager
			FBOManager * fboManager = FBOManager::getInstance();
			GLint currentFBO = -1;
			GLenum target = 0;
			
			glGetIntegerv(GL_FRAMEBUFFER_BINDING, &currentFBO);
			error = glGetError();
			GLint currentRightFBO = fboManager->GetRightEyeFBO(currentFBO);
			error = glGetError();

			GLint currentReadFBO = -1;
			GLint currentDrawFBO = -1;
			glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING, &currentReadFBO);
			error = glGetError();
			glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &currentDrawFBO);
			error = glGetError();
			GLint currentReadRightFBO = fboManager->GetRightEyeFBO(currentReadFBO);
			error = glGetError();
			GLint currentDrawRightFBO = fboManager->GetRightEyeFBO(currentDrawFBO);
			error = glGetError();

			// Left eye
			SetInterop(true);


			if (currentDrawFBO != -1)
			{
				(*orig_glBlitFramebuffer)(srcX0, srcY0, srcX1, srcY1, dstX0, dstY0, dstX1, dstY1, mask, filter);
				error = glGetError();
			}

			GLenum mode, texTarget;
			GLint texture;
			TextureManager *texManager = TextureManager::getInstance();
			texManager->GetTextureBoundMode(&mode, &texTarget, &texture);

			GLuint rightEyeTex = texManager->GetRightEyeTextureId(texture);
			(*orig_glBindMultiTexture)(mode, texTarget, rightEyeTex);

			// Right eye
			//(*orig_glBindFramebuffer)(GL_FRAMEBUFFER, currentFBO);
			error = glGetError();
			if (currentReadFBO != -1)
				(*orig_glBindFramebuffer)(GL_READ_FRAMEBUFFER, currentReadRightFBO);
			error = glGetError();
			if (currentDrawFBO != -1)
			{
				(*orig_glBindFramebuffer)(GL_DRAW_FRAMEBUFFER, currentDrawRightFBO);
				error = glGetError();
				(*orig_glBlitFramebuffer)(srcX0, srcY0, srcX1, srcY1, dstX0, dstY0, dstX1, dstY1, mask, filter);
				error = glGetError();
			}

			// Swap back to normal FB
			(*orig_glBindMultiTexture)(mode, texTarget, texture);
			(*orig_glBindFramebuffer)(GL_READ_FRAMEBUFFER, 0);
			(*orig_glBindFramebuffer)(GL_DRAW_FRAMEBUFFER, 0);
			SetInterop(false);
		}
		else
		{
			(*orig_glBlitFramebuffer)(srcX0, srcY0, srcX1, srcY1, dstX0, dstY0, dstX1, dstY1, mask, filter);
		}
	}
	else
	{
		(*orig_glBlitFramebuffer)(srcX0, srcY0, srcX1, srcY1, dstX0, dstY0, dstX1, dstY1, mask, filter);
	}
	m_opengl32Mutex.unlock();
}
///-------------------------------------------------------------------------------------------

//********************************************************************************************
// END STEREO CALLS
//********************************************************************************************

//////////////////////////////////////////////////////////////////////////
// Function Hooking
PROC sys_wglGetProcAddress(LPCSTR ProcName)
{
	std::string injectionPoint = g_reader->GetInjectionPoint();

	if (!strcmp(ProcName, "glUseProgram"))
	{
		orig_glUseProgram = (funct_glUseProgram_t)orig_wglGetProcAddress(ProcName);
		return (FARPROC)sys_glUseProgram;
	}
	// ARB Variant
	else if (!strcmp(ProcName, "glUseProgramObjectARB"))
	{
		orig_glUseProgramObjectARB = (funct_glUseProgram_t)orig_wglGetProcAddress(ProcName);
		return (FARPROC)sys_glUseProgramObjectARB;
	}
	else if (!strcmp(ProcName, "glCreateProgram"))
	{
		orig_glCreateProgram = (funct_glCreateProgram_t)orig_wglGetProcAddress(ProcName);
		return (FARPROC)sys_glCreateProgram;
	}
	else if (!strcmp(ProcName, "glAttachShader"))
	{
		orig_glAttachShader = (func_glAttachShader_t)orig_wglGetProcAddress(ProcName);
		return (FARPROC)sys_glAttachShader;
	}
	else if (!strcmp(ProcName, "glShaderSource"))
	{
		orig_glShaderSource = (funct_glShaderSource_t)orig_wglGetProcAddress(ProcName);
		return (FARPROC)sys_glShaderSource;
	}
	// we do the normal rendering Not printing the debug info
	else if (!strcmp(ProcName, injectionPoint.c_str()) && (!g_reader->GetPrintDebugInfoState()))
	{
		char result[100];
		sprintf_s(result, "3D Vision Hooking point is: %s", injectionPoint.c_str());
		add_log(result);
		orig_glBindFramebuffer = (func_glBindFramebuffer_t)orig_wglGetProcAddress(ProcName);
		return (FARPROC)sys_glBindFramebuffer;
	}
	else if (!strcmp(ProcName, "glBindFramebuffer"))
	{
		orig_glBindFramebuffer = (func_glBindFramebuffer_t)orig_wglGetProcAddress(ProcName);
		if ((g_reader->GetPrintDebugInfoState()))
		{
			return (FARPROC)log_glBindFramebuffer;
		}
		else
		{
			return (FARPROC)sys_glBindFramebufferAlternative;
		}
	}
	else if (!strcmp(ProcName, "glBindFramebufferEXT"))
	{
		orig_glBindFramebuffer = (func_glBindFramebuffer_t)orig_wglGetProcAddress(ProcName);
		if ((g_reader->GetPrintDebugInfoState()))
		{
			return (FARPROC)log_glBindFramebufferEXT;
		}
		else
		{
			return (FARPROC)sys_glBindFramebufferAlternative;
		}
	}
	else if (!strcmp(ProcName, "glBindFramebufferARB"))
	{
		orig_glBindFramebuffer = (func_glBindFramebuffer_t)orig_wglGetProcAddress(ProcName);
		if ((g_reader->GetPrintDebugInfoState()))
		{
			return (FARPROC)log_glBindFramebufferARB;
		}
		else
		{
			return (FARPROC)sys_glBindFramebufferAlternative;
		}
	}

	else if (!strcmp(ProcName, "glTextureSubImage2D"))
	{
		orig_glTextureSubImage2D = (func_glTextureSubImage2D_t)orig_wglGetProcAddress(ProcName);
		return (FARPROC)sys_glTextureSubImage2D;
	}
	else if (!strcmp(ProcName, "glTextureSubImage2DEXT"))
	{
		orig_glTextureSubImage2D = (func_glTextureSubImage2D_t)orig_wglGetProcAddress(ProcName);
		return (FARPROC)sys_glTextureSubImage2D;
	}

	else if (!strcmp(ProcName, "glFramebufferTexture"))
	{
		orig_glFramebufferTexture = (func_glFramebufferTexture_t)orig_wglGetProcAddress(ProcName);
		return (FARPROC)sys_glFramebufferTexture;
	}
	else if (!strcmp(ProcName, "glFramebufferTextureEXT"))
	{
		orig_glFramebufferTexture = (func_glFramebufferTexture_t)orig_wglGetProcAddress(ProcName);
		return (FARPROC)sys_glFramebufferTexture;
	}

	else if (!strcmp(ProcName, "glFramebufferTexture2D"))
	{
		orig_glFramebufferTexture2D = (func_glFramebufferTexture2D_t)orig_wglGetProcAddress(ProcName);
		return (FARPROC)sys_glFramebufferTexture2D;
	}
	else if (!strcmp(ProcName, "glFramebufferTexture2DEXT"))
	{
		orig_glFramebufferTexture2D = (func_glFramebufferTexture2D_t)orig_wglGetProcAddress(ProcName);
		return (FARPROC)sys_glFramebufferTexture2D;
	}

	else if (!strcmp(ProcName, "glLinkProgram"))
	{
		orig_glLinkProgram = (func_glLinkProgram_t)orig_wglGetProcAddress(ProcName);
		return (FARPROC)sys_glLinkProgram;
	}
	else if (!strcmp(ProcName, "glCompileShader"))
	{
		orig_glCompileShader = (func_glCompileShader)orig_wglGetProcAddress(ProcName);
		return (FARPROC)sys_glCompileShader;
	}
	//////////////////////////////////////////////////////////////////////////
	// DUPLICATE FRAMEBUFFERS Method
	//////////////////////////////////////////////////////////////////////////
	else if (!strcmp(ProcName, "glGenFramebuffers"))
	{
		orig_glGenFramebuffers = (func_glGenFramebuffers_t)orig_wglGetProcAddress(ProcName);
		return (FARPROC)sys_glGenFramebuffers;
	}
	else if (!strcmp(ProcName, "glGenFramebuffersEXT"))
	{
		orig_glGenFramebuffers = (func_glGenFramebuffers_t)orig_wglGetProcAddress(ProcName);
		return (FARPROC)sys_glGenFramebuffers;
	}
	else if (!strcmp(ProcName, "glTexImage2DMultisample"))
	{
		orig_glTexImage2DMultisample = (func_glTexImage2DMultisample_t)orig_wglGetProcAddress(ProcName);
		return (FARPROC)sys_glTexImage2DMultisample;
	}
	else if (!strcmp(ProcName, "glTexImage2DMultisampleEXT"))
	{
		orig_glTexImage2DMultisample = (func_glTexImage2DMultisample_t)orig_wglGetProcAddress(ProcName);
		return (FARPROC)sys_glTexImage2DMultisample;
	}

	else if (!strcmp(ProcName, "glDrawElementsBaseVertex"))
	{
		orig_glDrawElementsBaseVertex = (func_glDrawElementsBaseVertex_t)orig_wglGetProcAddress(ProcName);
		return (FARPROC)sys_glDrawElementsBaseVertex;
	}
	else if (!strcmp(ProcName, "glDrawArrays"))
	{
		orig_glDrawArrays = (func_glDrawArrays_t)orig_wglGetProcAddress(ProcName);
		return (FARPROC)sys_glDrawArrays;
	}
	else if (!strcmp(ProcName, "glBlitFramebuffer"))
	{
		orig_glBlitFramebuffer = (func_glBlitFramebuffer_t)orig_wglGetProcAddress(ProcName);
		return (FARPROC)sys_glBlitFramebuffer;
	}
	else if (!strcmp(ProcName, "glBlitFramebufferEXT"))
	{
		orig_glBlitFramebuffer = (func_glBlitFramebuffer_t)orig_wglGetProcAddress(ProcName);
		return (FARPROC)sys_glBlitFramebuffer;
	}
	else if (!strcmp(ProcName, "glGenTextures"))
	{
		orig_glGenTextures = (func_glGenTextures_t)orig_wglGetProcAddress(ProcName);
		return (FARPROC)sys_glGenTextures;
	}

	else if (!strcmp(ProcName, "glBindMultiTexture"))
	{
		orig_glBindMultiTexture = (func_glBindMultiTexture_t)orig_wglGetProcAddress(ProcName);
		return (FARPROC)sys_glBindMultiTextureEXT;
	}
	else if (!strcmp(ProcName, "glBindMultiTextureEXT"))
	{
		orig_glBindMultiTexture = (func_glBindMultiTexture_t)orig_wglGetProcAddress(ProcName);
		return (FARPROC)sys_glBindMultiTextureEXT;
	}
	else if (!strcmp(ProcName, "glActiveTexture"))
	{
		orig_glActiveTexture = (func_glActiveTexture_t)orig_wglGetProcAddress(ProcName);
		return (FARPROC)sys_glActiveTexture;
	}
	else if (!strcmp(ProcName, "glCheckFramebufferStatus"))
	{
		orig_glCheckFramebufferStatus = (func_glCheckFramebufferStatus_t)orig_wglGetProcAddress(ProcName);
		return (FARPROC)sys_glCheckFramebufferStatus;
	}
	else if (!strcmp(ProcName, "glCheckFramebufferStatusEXT"))
	{
		orig_glCheckFramebufferStatus = (func_glCheckFramebufferStatus_t)orig_wglGetProcAddress(ProcName);
		return (FARPROC)sys_glCheckFramebufferStatus;
	}

	return orig_wglGetProcAddress(ProcName);
}

// MAIN ENTRY
BOOL __stdcall DllMain(HINSTANCE hinstDll, DWORD fdwReason, LPVOID lpvReserved)
{
	(void)lpvReserved;
	(void)hinstDll;

	//Read the config file
	if (!g_reader)
		g_reader = new configReader;

	switch (fdwReason)
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hOriginalDll);

		// Splash Screen enabled?
		if (g_reader->GetEnableSplashScreen())
		{
			char info[255];
			sprintf_s(info, "OpenGL3DVision Wrapper v.%s", OGL3DVISION_VERSION_STR);
			// SDK Message box
			CustomMessageBox(NULL, _T("  \t\t\t\t\t\t               \n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n"),
				_T(info), MB_OK);
		}
		// Init the wrapper
		LoadFPSInject();
		Init();
		break;

	case DLL_PROCESS_DETACH:
		if (hOriginalDll != NULL)
		{
			// Stop the threads
			StopResizeThread();
			StoptNVAPIThread();
			Sleep(500);

			NV3DVisionBuffers_destroy(gl_d3d_buffers);
			add_log("\nDisabling 3D Vision... Cleaning up and freeing the resources...");
			// Removing the FPS inject
			if (FPSDestroy)
			{
				(FPSDestroy)(m_FPSInjector);
			}
			FreeLibrary(FPSHandle);
			add_log("Application closed successfully !\nEND\n");
			// The the openGL32.dll lib
			FreeLibrary(hOriginalDll);
			hOriginalDll = NULL;
			
		}
		break;
	}
	return TRUE;
}
#pragma warning(default:4100)
///-------------------------------------------------------------------------------------------

void __cdecl add_log(const char * fmt, ...)
{
	if (g_reader->GetEnableLog())
	{
		va_list va_alist;
		char logbuf[5000] = "";
		FILE * fp;
		errno_t err;

		va_start(va_alist, fmt);
		_vsnprintf_s(logbuf + strlen(logbuf), 5000, sizeof(logbuf)-strlen(logbuf), fmt, va_alist);
		va_end(va_alist);

		//if ((fp = fopen("3DVisionWrapper.log", "ab")) != NULL)
		err = fopen_s(&fp, "3DVisionWrapper.log", "ab");
		if (err == 0)
		{
			fprintf(fp, "%s", logbuf);
			fprintf(fp, "\n");
			fclose(fp);
		}
	}
}
///-------------------------------------------------------------------------------------------

static void LoadFPSInject(void)
{
	// Load the library
	FPSHandle = LoadLibrary(FPSINJECT_DLL);

	if (FPSHandle != 0)
	{
		// Get the pointers
		FPScreate = (create)GetProcAddress(FPSHandle, "create_object");
		FPSDestroy = (destroy)GetProcAddress(FPSHandle, "destroy_object");

		// Assign the pointer
		m_FPSInjector = (FPScreate)();
	}
}
///-------------------------------------------------------------------------------------------

static bool isFPSInjectHandleValid(void)
{
	bool ret = false;

	if (FPSHandle != 0)
	{
		ret = true;
	}

	return ret;
}
///-------------------------------------------------------------------------------------------

DWORD GetMainThreadId(void) 
{
	const std::tr1::shared_ptr<void> hThreadSnapshot(CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0), CloseHandle);
	if (hThreadSnapshot.get() == INVALID_HANDLE_VALUE) 
	{
		add_log("GetMainThreadId failed");
		return 0;
	}
	THREADENTRY32 tEntry;
	tEntry.dwSize = sizeof(THREADENTRY32);
	DWORD result = 0;
	DWORD currentPID = GetCurrentProcessId();
	for (BOOL success = Thread32First(hThreadSnapshot.get(), &tEntry);
		!result && success && GetLastError() != ERROR_NO_MORE_FILES;
		success = Thread32Next(hThreadSnapshot.get(), &tEntry))
	{
		if (tEntry.th32OwnerProcessID == currentPID) 
		{
			result = tEntry.th32ThreadID;
		}
	}
	return result;
}
///-------------------------------------------------------------------------------------------