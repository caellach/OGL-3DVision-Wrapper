/*
* OpenGL - 3D Vision Wrapper
* Copyright (c) Helifax 2015
*/

#ifndef H_OPENGL32
#define H_OPENGL32

#define ORIGINAL_DLL	"opengl32.dll"
#define FPSINJECT_DLL	"FPSInject.dll"

// DEVMSG
#if defined(WIN32)
#define __STR2__(x) #x
#define __STR1__(x) __STR2__(x)
#define __LOC__ __FILE__ "("__STR1__(__LINE__)"): "
#define __PRAGMAMSG__(msg) __pragma(message(__LOC__ msg))

#if !defined(HIDE_DEVMSG)
/// DEVMSG output when compiling. Disable by defining HIDE_DEVMSG before including this file.
#define DEVMSG(msg) __PRAGMAMSG__("DEVMSG: " msg)
#else
/// DEVMSG output when compiling. Disable by defining HIDE_DEVMSG before including this file.
#define DEVMSG(msg)
#endif

#if !defined(HIDE_DEVWARN)
/// DEVWARN output when compiling. Disable by defining HIDE_DEVWARN before including this file.
#define DEVWARN(msg) __PRAGMAMSG__("DEVWARN: " msg)
#else
/// DEVMSG output when compiling. Disable by defining HIDE_DEVWARN before including this file.
#define DEVWARN(msg)
#endif

#if !defined(HIDE_DEVERR)
/// DEVERR output when compiling. Disable by defining HIDE_DEVERR before including this file.
#define DEVERR(msg) __PRAGMAMSG__("DEVERR: " msg)
#else
/// DEVERR output when compiling. Disable by defining HIDE_DEVERR before including this file.
#define DEVERR(msg)
#endif

#if !defined(HIDE_DEVBUG)
/// DEVBUG output when compiling. Disable by defining HIDE_DEVBUG before including this file.
#define DEVBUG(msg) __PRAGMAMSG__("DEVBUG: " msg)
#else
/// DEVBUG output when compiling. Disable by defining HIDE_DEVBUG before including this file.
#define DEVBUG(msg)
#endif

#if !defined(HIDE_DEVTODO)
/// DEVTODO output when compiling. Disable by defining HIDE_DEVTODO before including this file.
#define DEVTODO(msg) __PRAGMAMSG__("DEVTODO: " msg)
#else
/// DEVTODO output when compiling. Disable by defining HIDE_DEVTODO before including this file.
#define DEVTODO(msg)
#endif
#else
#define __COMPILEMSG__(msg) _Pragma (#msg)
#define __PRAGMAMSG__(msg) __COMPILEMSG__(message (#msg))

#if !defined(HIDE_DEVMSG)
/// DEVMSG output when compiling. Disable by defining HIDE_DEVMSG before including this file.
#define DEVMSG(msg) __PRAGMAMSG__(DEVMSG: msg)
#else
/// DEVMSG output when compiling. Disable by defining HIDE_DEVMSG before including this file.
#define DEVMSG(msg)
#endif

#if !defined(HIDE_DEVWARN)
/// DEVWARN output when compiling. Disable by defining HIDE_DEVWARN before including this file.
#define DEVWARN(msg) __PRAGMAMSG__(DEVWARN: msg)
#else
/// DEVWARN output when compiling. Disable by defining HIDE_DEVWARN before including this file.
#define DEVWARN(msg)
#endif

#if !defined(HIDE_DEVERR)
/// DEVERR output when compiling. Disable by defining HIDE_DEVERR before including this file.
#define DEVERR(msg) __PRAGMAMSG__(DEVERR: msg)
#else
/// DEVERR output when compiling. Disable by defining HIDE_DEVERR before including this file.
#define DEVERR(msg)
#endif

#if !defined(HIDE_DEVBUG)
/// DEVBUG output when compiling. Disable by defining HIDE_DEVBUG before including this file.
#define DEVBUG(msg) __PRAGMAMSG__(DEVBUG: msg)
#else
/// DEVBUG output when compiling. Disable by defining HIDE_DEVBUG before including this file.
#define DEVBUG(msg)
#endif

#if !defined(HIDE_DEVTODO)
/// DEVTODO output when compiling. Disable by defining HIDE_DEVTODO before including this file.
#define DEVTODO(msg) __PRAGMAMSG__(DEVTODO: msg)
#else
/// DEVTODO output when compiling. Disable by defining HIDE_DEVTODO before including this file.
#define DEVTODO(msg)
#endif
#endif


/* Original function typedefs */
#include <windows.h>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <string>
#include <math.h>

extern BOOL Init (void);
extern HMODULE hOriginalDll;
void __cdecl add_log(const char * fmt, ...);

typedef void( *func_glUniformMatrix4fv_t)(GLint location, GLsizei count, GLboolean transpose, GLfloat *value);

// Stereo calls
typedef void(*func_glDrawArrays_t)(GLenum mode, GLint first, GLsizei count);
typedef void(*func_glBindMultiTexture_t)(GLenum mode, GLenum target, GLint texture);

// Stereo Calls ID5
typedef void(*func_glDrawElementsBaseVertex_t)(GLenum mode, GLsizei count, GLenum type, GLvoid *indices, GLint basevertex);
typedef void(*func_glBlitFramebuffer_t)(GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter);

// Shader Related
typedef GLuint( *funct_glCreateProgram_t)(void);
typedef void(*funct_glUseProgram_t)(GLuint program);
typedef void(*funct_glShaderSource_t)(GLuint shader,GLsizei count,const GLchar **string,const GLint *length);
typedef void(*funct_glGetAttachedShaders_t)(GLuint program, GLsizei maxCount, GLsizei *count,GLuint *shaders);
typedef void(*funct_glGetShaderSource_t)(GLuint shader,GLsizei bufSize,GLsizei *length, GLchar *source);
typedef void(*funct_glGetShaderiv_t)(GLuint shader, GLenum pname, GLint *params);
typedef GLint(*func_glGetUniformLocation_t)(GLuint program, const GLchar *name);
typedef void (*func_glUniform1f_t)(GLint location, GLfloat v0);
typedef void(*func_glLinkProgram_t)(GLuint program);
typedef void(*func_glCompileShader)(GLuint shader);
typedef void(*func_glGetProgramiv_t)(GLuint program, GLenum pname, GLint *params);
typedef void(*func_glDetachShader_t)(GLuint program, GLuint shader);
typedef void(*func_glDeleteShader_t)(GLuint shader);


// Framebuffers Related
typedef void(*func_glBindFramebuffer_t)(GLenum target, GLuint framebuffer);
typedef void(*func_glGenFramebuffers_t)(GLsizei n, GLuint *ids);
typedef void(*func_glFramebufferTexture_t)(GLenum target, GLenum attachment, GLuint texture, GLint level);
typedef void (*func_glFramebufferTexture2D_t)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
typedef void(*func_glTextureSubImage2D_t)(GLuint texture, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void* pixels);
typedef GLenum (*func_glCheckFramebufferStatus_t)(GLenum target);



// ARB Extension
typedef void(*funct_glUseProgramObjectARB_t)(GLhandleARB program);
typedef void(*func_glCompileShaderARB)(GLhandleARB shader);

// Shader Info related
typedef void(*func_glGetShaderiv_t)(GLuint shader, GLenum pname, GLint *params);
typedef void(*func_glGetShaderInfoLog_t)(GLuint shader, GLsizei maxLength, GLsizei *length, GLchar *infoLog);
typedef void(*func_glGetProgramInfoLog_t)(GLuint program, GLsizei maxLength, GLsizei *length, GLchar *infoLog);
typedef void(*func_glAttachShader_t)(GLuint program, GLuint shader);
typedef GLuint(*func_glCreateShader)(GLenum shaderType);

// Other Stuff
typedef GLenum(*func_glGetError_t)(void);
typedef void ( *func_glActiveTextureARB_t) (GLenum target);
typedef void ( *func_glActiveTexture_t) (GLenum target); 
typedef void ( *func_BindTextureEXT_t) (GLenum target, GLuint texture);
typedef void ( *func_glBindFrameBufferEXT_t) (GLenum e, GLuint uNumber); 
typedef void ( *func_glMultiTexCoord2fARB_t) (GLenum target, GLfloat s, GLfloat t);
typedef void ( *func_glAccum_t) (GLenum op,  GLfloat value);
typedef void ( *func_glAlphaFunc_t) (GLenum func,  GLclampf ref);
typedef GLboolean ( *func_glAreTexturesResident_t) (GLsizei n,  const GLuint *textures,  GLboolean *residences);
typedef void ( *func_glArrayElement_t) (GLint i);
typedef void ( *func_glBegin_t) (GLenum mode);
typedef void ( *func_glBindTexture_t) (GLenum target,  GLuint texture);
typedef void ( *func_glBitmap_t) (GLsizei width,  GLsizei height,  GLfloat xorig,  GLfloat yorig,  GLfloat xmove,  GLfloat ymove,  const GLubyte *bitmap);
typedef void ( *func_glBlendFunc_t) (GLenum sfactor,  GLenum dfactor);
typedef void ( *func_glCallList_t) (GLuint list);
typedef void ( *func_glCallLists_t) (GLsizei n,  GLenum type,  const GLvoid *lists);
typedef void ( *func_glClear_t) (GLbitfield mask);
typedef void ( *func_glClearAccum_t) (GLfloat red,  GLfloat green,  GLfloat blue,  GLfloat alpha);
typedef void ( *func_glClearColor_t) (GLclampf red,  GLclampf green,  GLclampf blue,  GLclampf alpha);
typedef void ( *func_glClearDepth_t) (GLclampd depth);
typedef void ( *func_glClearIndex_t) (GLfloat c);
typedef void ( *func_glClearStencil_t) (GLint s);
typedef void ( *func_glClipPlane_t) (GLenum plane,  const GLdouble *equation);
typedef void ( *func_glColor3b_t) (GLbyte red,  GLbyte green,  GLbyte blue);
typedef void ( *func_glColor3bv_t) (const GLbyte *v);
typedef void ( *func_glColor3d_t) (GLdouble red,  GLdouble green,  GLdouble blue);
typedef void ( *func_glColor3dv_t) (const GLdouble *v);
typedef void ( *func_glColor3f_t) (GLfloat red,  GLfloat green,  GLfloat blue);
typedef void ( *func_glColor3fv_t) (const GLfloat *v);
typedef void ( *func_glColor3i_t) (GLint red,  GLint green,  GLint blue);
typedef void ( *func_glColor3iv_t) (const GLint *v);
typedef void ( *func_glColor3s_t) (GLshort red,  GLshort green,  GLshort blue);
typedef void ( *func_glColor3sv_t) (const GLshort *v);
typedef void ( *func_glColor3ub_t) (GLubyte red,  GLubyte green,  GLubyte blue);
typedef void ( *func_glColor3ubv_t) (const GLubyte *v);
typedef void ( *func_glColor3ui_t) (GLuint red,  GLuint green,  GLuint blue);
typedef void ( *func_glColor3uiv_t) (const GLuint *v);
typedef void ( *func_glColor3us_t) (GLushort red,  GLushort green,  GLushort blue);
typedef void ( *func_glColor3usv_t) (const GLushort *v);
typedef void ( *func_glColor4b_t) (GLbyte red,  GLbyte green,  GLbyte blue,  GLbyte alpha);
typedef void ( *func_glColor4bv_t) (const GLbyte *v);
typedef void ( *func_glColor4d_t) (GLdouble red,  GLdouble green,  GLdouble blue,  GLdouble alpha);
typedef void ( *func_glColor4dv_t) (const GLdouble *v);
typedef void ( *func_glColor4f_t) (GLfloat red,  GLfloat green,  GLfloat blue,  GLfloat alpha);
typedef void ( *func_glColor4fv_t) (const GLfloat *v);
typedef void ( *func_glColor4i_t) (GLint red,  GLint green,  GLint blue,  GLint alpha);
typedef void ( *func_glColor4iv_t) (const GLint *v);
typedef void ( *func_glColor4s_t) (GLshort red,  GLshort green,  GLshort blue,  GLshort alpha);
typedef void ( *func_glColor4sv_t) (const GLshort *v);
typedef void ( *func_glColor4ub_t) (GLubyte red,  GLubyte green,  GLubyte blue,  GLubyte alpha);
typedef void ( *func_glColor4ubv_t) (const GLubyte *v);
typedef void ( *func_glColor4ui_t) (GLuint red,  GLuint green,  GLuint blue,  GLuint alpha);
typedef void ( *func_glColor4uiv_t) (const GLuint *v);
typedef void ( *func_glColor4us_t) (GLushort red,  GLushort green,  GLushort blue,  GLushort alpha);
typedef void ( *func_glColor4usv_t) (const GLushort *v);
typedef void ( *func_glColorMask_t) (GLboolean red,  GLboolean green,  GLboolean blue,  GLboolean alpha);
typedef void ( *func_glColorMaterial_t) (GLenum face,  GLenum mode);
typedef void ( *func_glColorPointer_t) (GLint size,  GLenum type,  GLsizei stride,  const GLvoid *pointer);
typedef void ( *func_glCopyPixels_t) (GLint x,  GLint y,  GLsizei width,  GLsizei height,  GLenum type);
typedef void ( *func_glCopyTexImage1D_t) (GLenum target,  GLint level,  GLenum internalFormat,  GLint x,  GLint y,  GLsizei width,  GLint border);
typedef void ( *func_glCopyTexImage2D_t) (GLenum target,  GLint level,  GLenum internalFormat,  GLint x,  GLint y,  GLsizei width,  GLsizei height,  GLint border);
typedef void ( *func_glCopyTexSubImage1D_t) (GLenum target,  GLint level,  GLint xoffset,  GLint x,  GLint y,  GLsizei width);
typedef void ( *func_glCopyTexSubImage2D_t) (GLenum target,  GLint level,  GLint xoffset,  GLint yoffset,  GLint x,  GLint y,  GLsizei width,  GLsizei height);
typedef void ( *func_glCullFace_t) (GLenum mode);
typedef void ( *func_glDebugEntry_t) (void);
typedef void ( *func_glDeleteLists_t) (GLuint list,  GLsizei range);
typedef void ( *func_glDeleteTextures_t) (GLsizei n,  const GLuint *textures);
typedef void ( *func_glDepthFunc_t) (GLenum func);
typedef void ( *func_glDepthMask_t) (GLboolean flag);
typedef void ( *func_glDepthRange_t) (GLclampd zNear,  GLclampd zFar);
typedef void ( *func_glDisable_t) (GLenum cap);
typedef void ( *func_glDisableClientState_t) (GLenum array);
typedef void ( *func_glDrawArrays_t) (GLenum mode,  GLint first,  GLsizei count);
typedef void ( *func_glDrawBuffer_t) (GLenum mode);
typedef void (__stdcall *func_glDrawElements_t) (GLenum mode,  GLsizei count,  GLenum type,  const GLvoid *indices);
typedef void ( *func_glDrawPixels_t) (GLsizei width,  GLsizei height,  GLenum format,  GLenum type,  const GLvoid *pixels);
typedef void ( *func_glEdgeFlag_t) (GLboolean flag);
typedef void ( *func_glEdgeFlagPointer_t) (GLsizei stride,  const GLvoid *pointer);
typedef void ( *func_glEdgeFlagv_t) (const GLboolean *flag);
typedef void ( *func_glEnable_t) (GLenum cap);
typedef void ( *func_glEnableClientState_t) (GLenum array);
typedef void ( *func_glEnd_t) (void);
typedef void ( *func_glEndList_t) (void);
typedef void ( *func_glEvalCoord1d_t) (GLdouble u);
typedef void ( *func_glEvalCoord1dv_t) (const GLdouble *u);
typedef void ( *func_glEvalCoord1f_t) (GLfloat u);
typedef void ( *func_glEvalCoord1fv_t) (const GLfloat *u);
typedef void ( *func_glEvalCoord2d_t) (GLdouble u,  GLdouble v);
typedef void ( *func_glEvalCoord2dv_t) (const GLdouble *u);
typedef void ( *func_glEvalCoord2f_t) (GLfloat u,  GLfloat v);
typedef void ( *func_glEvalCoord2fv_t) (const GLfloat *u);
typedef void ( *func_glEvalMesh1_t) (GLenum mode,  GLint i1,  GLint i2);
typedef void ( *func_glEvalMesh2_t) (GLenum mode,  GLint i1,  GLint i2,  GLint j1,  GLint j2);
typedef void ( *func_glEvalPoint1_t) (GLint i);
typedef void ( *func_glEvalPoint2_t) (GLint i,  GLint j);
typedef void ( *func_glFeedbackBuffer_t) (GLsizei size,  GLenum type,  GLfloat *buffer);
typedef void ( *func_glFinish_t) (void);
typedef void ( *func_glFlush_t) (void);
typedef void ( *func_glFogf_t) (GLenum pname,  GLfloat param);
typedef void ( *func_glFogfv_t) (GLenum pname,  const GLfloat *params);
typedef void ( *func_glFogi_t) (GLenum pname,  GLint param);
typedef void ( *func_glFogiv_t) (GLenum pname,  const GLint *params);
typedef void ( *func_glFrontFace_t) (GLenum mode);
typedef void ( *func_glFrustum_t) (GLdouble left,  GLdouble right,  GLdouble bottom,  GLdouble top,  GLdouble zNear,  GLdouble zFar);
typedef GLuint ( *func_glGenLists_t) (GLsizei range);
typedef void ( *func_glGenTextures_t) (GLsizei n,  GLuint *textures);
typedef void ( *func_glGetBooleanv_t) (GLenum pname,  GLboolean *params);
typedef void ( *func_glGetClipPlane_t) (GLenum plane,  GLdouble *equation);
typedef void ( *func_glGetDoublev_t) (GLenum pname,  GLdouble *params);
typedef GLenum ( *func_glGetError_t) (void);
typedef void ( *func_glGetFloatv_t) (GLenum pname,  GLfloat *params);
typedef void ( *func_glGetIntegerv_t) (GLenum pname,  GLint *params);
typedef void ( *func_glGetLightfv_t) (GLenum light,  GLenum pname,  GLfloat *params);
typedef void ( *func_glGetLightiv_t) (GLenum light,  GLenum pname,  GLint *params);
typedef void ( *func_glGetMapdv_t) (GLenum target,  GLenum query,  GLdouble *v);
typedef void ( *func_glGetMapfv_t) (GLenum target,  GLenum query,  GLfloat *v);
typedef void ( *func_glGetMapiv_t) (GLenum target,  GLenum query,  GLint *v);
typedef void ( *func_glGetMaterialfv_t) (GLenum face,  GLenum pname,  GLfloat *params);
typedef void ( *func_glGetMaterialiv_t) (GLenum face,  GLenum pname,  GLint *params);
typedef void ( *func_glGetPixelMapfv_t) (GLenum map,  GLfloat *values);
typedef void ( *func_glGetPixelMapuiv_t) (GLenum map,  GLuint *values);
typedef void ( *func_glGetPixelMapusv_t) (GLenum map,  GLushort *values);
typedef void ( *func_glGetPointerv_t) (GLenum pname,  GLvoid* *params);
typedef void ( *func_glGetPolygonStipple_t) (GLubyte *mask);
typedef const GLubyte * ( *func_glGetString_t) (GLenum name);
typedef void ( *func_glGetTexEnvfv_t) (GLenum target,  GLenum pname,  GLfloat *params);
typedef void ( *func_glGetTexEnviv_t) (GLenum target,  GLenum pname,  GLint *params);
typedef void ( *func_glGetTexGendv_t) (GLenum coord,  GLenum pname,  GLdouble *params);
typedef void ( *func_glGetTexGenfv_t) (GLenum coord,  GLenum pname,  GLfloat *params);
typedef void ( *func_glGetTexGeniv_t) (GLenum coord,  GLenum pname,  GLint *params);
typedef void ( *func_glGetTexImage_t) (GLenum target,  GLint level,  GLenum format,  GLenum type,  GLvoid *pixels);
typedef void ( *func_glGetTexLevelParameterfv_t) (GLenum target,  GLint level,  GLenum pname,  GLfloat *params);
typedef void ( *func_glGetTexLevelParameteriv_t) (GLenum target,  GLint level,  GLenum pname,  GLint *params);
typedef void ( *func_glGetTexParameterfv_t) (GLenum target,  GLenum pname,  GLfloat *params);
typedef void ( *func_glGetTexParameteriv_t) (GLenum target,  GLenum pname,  GLint *params);
typedef void ( *func_glHint_t) (GLenum target,  GLenum mode);
typedef void ( *func_glIndexMask_t) (GLuint mask);
typedef void ( *func_glIndexPointer_t) (GLenum type,  GLsizei stride,  const GLvoid *pointer);
typedef void ( *func_glIndexd_t) (GLdouble c);
typedef void ( *func_glIndexdv_t) (const GLdouble *c);
typedef void ( *func_glIndexf_t) (GLfloat c);
typedef void ( *func_glIndexfv_t) (const GLfloat *c);
typedef void ( *func_glIndexi_t) (GLint c);
typedef void ( *func_glIndexiv_t) (const GLint *c);
typedef void ( *func_glIndexs_t) (GLshort c);
typedef void ( *func_glIndexsv_t) (const GLshort *c);
typedef void ( *func_glIndexub_t) (GLubyte c);
typedef void ( *func_glIndexubv_t) (const GLubyte *c);
typedef void ( *func_glInitNames_t) (void);
typedef void ( *func_glInterleavedArrays_t) (GLenum format,  GLsizei stride,  const GLvoid *pointer);
typedef GLboolean ( *func_glIsEnabled_t) (GLenum cap);
typedef GLboolean ( *func_glIsList_t) (GLuint list);
typedef GLboolean ( *func_glIsTexture_t) (GLuint texture);
typedef void ( *func_glLightModelf_t) (GLenum pname,  GLfloat param);
typedef void ( *func_glLightModelfv_t) (GLenum pname,  const GLfloat *params);
typedef void ( *func_glLightModeli_t) (GLenum pname,  GLint param);
typedef void ( *func_glLightModeliv_t) (GLenum pname,  const GLint *params);
typedef void ( *func_glLightf_t) (GLenum light,  GLenum pname,  GLfloat param);
typedef void ( *func_glLightfv_t) (GLenum light,  GLenum pname,  const GLfloat *params);
typedef void ( *func_glLighti_t) (GLenum light,  GLenum pname,  GLint param);
typedef void ( *func_glLightiv_t) (GLenum light,  GLenum pname,  const GLint *params);
typedef void ( *func_glLineStipple_t) (GLint factor,  GLushort pattern);
typedef void ( *func_glLineWidth_t) (GLfloat width);
typedef void ( *func_glListBase_t) (GLuint base);
typedef void ( *func_glLoadIdentity_t) (void);
typedef void ( *func_glLoadMatrixd_t) (const GLdouble *m);
typedef void ( *func_glLoadMatrixf_t) (const GLfloat *m);
typedef void ( *func_glLoadName_t) (GLuint name);
typedef void ( *func_glLogicOp_t) (GLenum opcode);
typedef void ( *func_glMap1d_t) (GLenum target,  GLdouble u1,  GLdouble u2,  GLint stride,  GLint order,  const GLdouble *points);
typedef void ( *func_glMap1f_t) (GLenum target,  GLfloat u1,  GLfloat u2,  GLint stride,  GLint order,  const GLfloat *points);
typedef void ( *func_glMap2d_t) (GLenum target,  GLdouble u1,  GLdouble u2,  GLint ustride,  GLint uorder,  GLdouble v1,  GLdouble v2,  GLint vstride,  GLint vorder,  const GLdouble *points);
typedef void ( *func_glMap2f_t) (GLenum target,  GLfloat u1,  GLfloat u2,  GLint ustride,  GLint uorder,  GLfloat v1,  GLfloat v2,  GLint vstride,  GLint vorder,  const GLfloat *points);
typedef void ( *func_glMapGrid1d_t) (GLint un,  GLdouble u1,  GLdouble u2);
typedef void ( *func_glMapGrid1f_t) (GLint un,  GLfloat u1,  GLfloat u2);
typedef void ( *func_glMapGrid2d_t) (GLint un,  GLdouble u1,  GLdouble u2,  GLint vn,  GLdouble v1,  GLdouble v2);
typedef void ( *func_glMapGrid2f_t) (GLint un,  GLfloat u1,  GLfloat u2,  GLint vn,  GLfloat v1,  GLfloat v2);
typedef void ( *func_glMaterialf_t) (GLenum face,  GLenum pname,  GLfloat param);
typedef void ( *func_glMaterialfv_t) (GLenum face,  GLenum pname,  const GLfloat *params);
typedef void ( *func_glMateriali_t) (GLenum face,  GLenum pname,  GLint param);
typedef void ( *func_glMaterialiv_t) (GLenum face,  GLenum pname,  const GLint *params);
typedef void ( *func_glMatrixMode_t) (GLenum mode);
typedef void ( *func_glMultMatrixd_t) (const GLdouble *m);
typedef void ( *func_glMultMatrixf_t) (const GLfloat *m);
typedef void ( *func_glNewList_t) (GLuint list,  GLenum mode);
typedef void ( *func_glNormal3b_t) (GLbyte nx,  GLbyte ny,  GLbyte nz);
typedef void ( *func_glNormal3bv_t) (const GLbyte *v);
typedef void ( *func_glNormal3d_t) (GLdouble nx,  GLdouble ny,  GLdouble nz);
typedef void ( *func_glNormal3dv_t) (const GLdouble *v);
typedef void ( *func_glNormal3f_t) (GLfloat nx,  GLfloat ny,  GLfloat nz);
typedef void ( *func_glNormal3fv_t) (const GLfloat *v);
typedef void ( *func_glNormal3i_t) (GLint nx,  GLint ny,  GLint nz);
typedef void ( *func_glNormal3iv_t) (const GLint *v);
typedef void ( *func_glNormal3s_t) (GLshort nx,  GLshort ny,  GLshort nz);
typedef void ( *func_glNormal3sv_t) (const GLshort *v);
typedef void ( *func_glNormalPointer_t) (GLenum type,  GLsizei stride,  const GLvoid *pointer);
typedef void ( *func_glOrtho_t) (GLdouble left,  GLdouble right,  GLdouble bottom,  GLdouble top,  GLdouble zNear,  GLdouble zFar);
typedef void ( *func_glPassThrough_t) (GLfloat token);
typedef void ( *func_glPixelMapfv_t) (GLenum map,  GLsizei mapsize,  const GLfloat *values);
typedef void ( *func_glPixelMapuiv_t) (GLenum map,  GLsizei mapsize,  const GLuint *values);
typedef void ( *func_glPixelMapusv_t) (GLenum map,  GLsizei mapsize,  const GLushort *values);
typedef void ( *func_glPixelStoref_t) (GLenum pname,  GLfloat param);
typedef void ( *func_glPixelStorei_t) (GLenum pname,  GLint param);
typedef void ( *func_glPixelTransferf_t) (GLenum pname,  GLfloat param);
typedef void ( *func_glPixelTransferi_t) (GLenum pname,  GLint param);
typedef void ( *func_glPixelZoom_t) (GLfloat xfactor,  GLfloat yfactor);
typedef void ( *func_glPointSize_t) (GLfloat size);
typedef void ( *func_glPolygonMode_t) (GLenum face,  GLenum mode);
typedef void ( *func_glPolygonOffset_t) (GLfloat factor,  GLfloat units);
typedef void ( *func_glPolygonStipple_t) (const GLubyte *mask);
typedef void ( *func_glPopAttrib_t) (void);
typedef void ( *func_glPopClientAttrib_t) (void);
typedef void ( *func_glPopMatrix_t) (void);
typedef void ( *func_glPopName_t) (void);
typedef void ( *func_glPrioritizeTextures_t) (GLsizei n,  const GLuint *textures,  const GLclampf *priorities);
typedef void ( *func_glPushAttrib_t) (GLbitfield mask);
typedef void ( *func_glPushClientAttrib_t) (GLbitfield mask);
typedef void ( *func_glPushMatrix_t) (void);
typedef void ( *func_glPushName_t) (GLuint name);
typedef void ( *func_glRasterPos2d_t) (GLdouble x,  GLdouble y);
typedef void ( *func_glRasterPos2dv_t) (const GLdouble *v);
typedef void ( *func_glRasterPos2f_t) (GLfloat x,  GLfloat y);
typedef void ( *func_glRasterPos2fv_t) (const GLfloat *v);
typedef void ( *func_glRasterPos2i_t) (GLint x,  GLint y);
typedef void ( *func_glRasterPos2iv_t) (const GLint *v);
typedef void ( *func_glRasterPos2s_t) (GLshort x,  GLshort y);
typedef void ( *func_glRasterPos2sv_t) (const GLshort *v);
typedef void ( *func_glRasterPos3d_t) (GLdouble x,  GLdouble y,  GLdouble z);
typedef void ( *func_glRasterPos3dv_t) (const GLdouble *v);
typedef void ( *func_glRasterPos3f_t) (GLfloat x,  GLfloat y,  GLfloat z);
typedef void ( *func_glRasterPos3fv_t) (const GLfloat *v);
typedef void ( *func_glRasterPos3i_t) (GLint x,  GLint y,  GLint z);
typedef void ( *func_glRasterPos3iv_t) (const GLint *v);
typedef void ( *func_glRasterPos3s_t) (GLshort x,  GLshort y,  GLshort z);
typedef void ( *func_glRasterPos3sv_t) (const GLshort *v);
typedef void ( *func_glRasterPos4d_t) (GLdouble x,  GLdouble y,  GLdouble z,  GLdouble w);
typedef void ( *func_glRasterPos4dv_t) (const GLdouble *v);
typedef void ( *func_glRasterPos4f_t) (GLfloat x,  GLfloat y,  GLfloat z,  GLfloat w);
typedef void ( *func_glRasterPos4fv_t) (const GLfloat *v);
typedef void ( *func_glRasterPos4i_t) (GLint x,  GLint y,  GLint z,  GLint w);
typedef void ( *func_glRasterPos4iv_t) (const GLint *v);
typedef void ( *func_glRasterPos4s_t) (GLshort x,  GLshort y,  GLshort z,  GLshort w);
typedef void ( *func_glRasterPos4sv_t) (const GLshort *v);
typedef void ( *func_glReadBuffer_t) (GLenum mode);
typedef void ( *func_glReadPixels_t) (GLint x,  GLint y,  GLsizei width,  GLsizei height,  GLenum format,  GLenum type,  GLvoid *pixels);
typedef void ( *func_glRectd_t) (GLdouble x1,  GLdouble y1,  GLdouble x2,  GLdouble y2);
typedef void ( *func_glRectdv_t) (const GLdouble *v1,  const GLdouble *v2);
typedef void ( *func_glRectf_t) (GLfloat x1,  GLfloat y1,  GLfloat x2,  GLfloat y2);
typedef void ( *func_glRectfv_t) (const GLfloat *v1,  const GLfloat *v2);
typedef void ( *func_glRecti_t) (GLint x1,  GLint y1,  GLint x2,  GLint y2);
typedef void ( *func_glRectiv_t) (const GLint *v1,  const GLint *v2);
typedef void ( *func_glRects_t) (GLshort x1,  GLshort y1,  GLshort x2,  GLshort y2);
typedef void ( *func_glRectsv_t) (const GLshort *v1,  const GLshort *v2);
typedef GLint ( *func_glRenderMode_t) (GLenum mode);
typedef void ( *func_glRotated_t) (GLdouble angle,  GLdouble x,  GLdouble y,  GLdouble z);
typedef void ( *func_glRotatef_t) (GLfloat angle,  GLfloat x,  GLfloat y,  GLfloat z);
typedef void ( *func_glScaled_t) (GLdouble x,  GLdouble y,  GLdouble z);
typedef void ( *func_glScalef_t) (GLfloat x,  GLfloat y,  GLfloat z);
typedef void ( *func_glScissor_t) (GLint x,  GLint y,  GLsizei width,  GLsizei height);
typedef void ( *func_glSelectBuffer_t) (GLsizei size,  GLuint *buffer);
typedef void ( *func_glShadeModel_t) (GLenum mode);
typedef void ( *func_glStencilFunc_t) (GLenum func,  GLint ref,  GLuint mask);
typedef void ( *func_glStencilMask_t) (GLuint mask);
typedef void ( *func_glStencilOp_t) (GLenum fail,  GLenum zfail,  GLenum zpass);
typedef void ( *func_glTexCoord1d_t) (GLdouble s);
typedef void ( *func_glTexCoord1dv_t) (const GLdouble *v);
typedef void ( *func_glTexCoord1f_t) (GLfloat s);
typedef void ( *func_glTexCoord1fv_t) (const GLfloat *v);
typedef void ( *func_glTexCoord1i_t) (GLint s);
typedef void ( *func_glTexCoord1iv_t) (const GLint *v);
typedef void ( *func_glTexCoord1s_t) (GLshort s);
typedef void ( *func_glTexCoord1sv_t) (const GLshort *v);
typedef void ( *func_glTexCoord2d_t) (GLdouble s,  GLdouble t);
typedef void ( *func_glTexCoord2dv_t) (const GLdouble *v);
typedef void ( *func_glTexCoord2f_t) (GLfloat s,  GLfloat t);
typedef void ( *func_glTexCoord2fv_t) (const GLfloat *v);
typedef void ( *func_glTexCoord2i_t) (GLint s,  GLint t);
typedef void ( *func_glTexCoord2iv_t) (const GLint *v);
typedef void ( *func_glTexCoord2s_t) (GLshort s,  GLshort t);
typedef void ( *func_glTexCoord2sv_t) (const GLshort *v);
typedef void ( *func_glTexCoord3d_t) (GLdouble s,  GLdouble t,  GLdouble r);
typedef void ( *func_glTexCoord3dv_t) (const GLdouble *v);
typedef void ( *func_glTexCoord3f_t) (GLfloat s,  GLfloat t,  GLfloat r);
typedef void ( *func_glTexCoord3fv_t) (const GLfloat *v);
typedef void ( *func_glTexCoord3i_t) (GLint s,  GLint t,  GLint r);
typedef void ( *func_glTexCoord3iv_t) (const GLint *v);
typedef void ( *func_glTexCoord3s_t) (GLshort s,  GLshort t,  GLshort r);
typedef void ( *func_glTexCoord3sv_t) (const GLshort *v);
typedef void ( *func_glTexCoord4d_t) (GLdouble s,  GLdouble t,  GLdouble r,  GLdouble q);
typedef void ( *func_glTexCoord4dv_t) (const GLdouble *v);
typedef void ( *func_glTexCoord4f_t) (GLfloat s,  GLfloat t,  GLfloat r,  GLfloat q);
typedef void ( *func_glTexCoord4fv_t) (const GLfloat *v);
typedef void ( *func_glTexCoord4i_t) (GLint s,  GLint t,  GLint r,  GLint q);
typedef void ( *func_glTexCoord4iv_t) (const GLint *v);
typedef void ( *func_glTexCoord4s_t) (GLshort s,  GLshort t,  GLshort r,  GLshort q);
typedef void ( *func_glTexCoord4sv_t) (const GLshort *v);
typedef void ( *func_glTexCoordPointer_t) (GLint size,  GLenum type,  GLsizei stride,  const GLvoid *pointer);
typedef void ( *func_glTexEnvf_t) (GLenum target,  GLenum pname,  GLfloat param);
typedef void ( *func_glTexEnvfv_t) (GLenum target,  GLenum pname,  const GLfloat *params);
typedef void ( *func_glTexEnvi_t) (GLenum target,  GLenum pname,  GLint param);
typedef void ( *func_glTexEnviv_t) (GLenum target,  GLenum pname,  const GLint *params);
typedef void ( *func_glTexGend_t) (GLenum coord,  GLenum pname,  GLdouble param);
typedef void ( *func_glTexGendv_t) (GLenum coord,  GLenum pname,  const GLdouble *params);
typedef void ( *func_glTexGenf_t) (GLenum coord,  GLenum pname,  GLfloat param);
typedef void ( *func_glTexGenfv_t) (GLenum coord,  GLenum pname,  const GLfloat *params);
typedef void ( *func_glTexGeni_t) (GLenum coord,  GLenum pname,  GLint param);
typedef void ( *func_glTexGeniv_t) (GLenum coord,  GLenum pname,  const GLint *params);
typedef void ( *func_glTexImage1D_t) (GLenum target,  GLint level,  GLint internalformat,  GLsizei width,  GLint border,  GLenum format,  GLenum type,  const GLvoid *pixels);
typedef void ( *func_glTexImage2D_t) (GLenum target,  GLint level,  GLint internalformat,  GLsizei width,  GLsizei height,  GLint border,  GLenum format,  GLenum type,  const GLvoid *pixels);
typedef void ( *func_glTexParameterf_t) (GLenum target,  GLenum pname,  GLfloat param);
typedef void ( *func_glTexParameterfv_t) (GLenum target,  GLenum pname,  const GLfloat *params);
typedef void ( *func_glTexParameteri_t) (GLenum target,  GLenum pname,  GLint param);
typedef void ( *func_glTexParameteriv_t) (GLenum target,  GLenum pname,  const GLint *params);
typedef void ( *func_glTexSubImage1D_t) (GLenum target,  GLint level,  GLint xoffset,  GLsizei width,  GLenum format,  GLenum type,  const GLvoid *pixels);
typedef void ( *func_glTexSubImage2D_t) (GLenum target,  GLint level,  GLint xoffset,  GLint yoffset,  GLsizei width,  GLsizei height,  GLenum format,  GLenum type,  const GLvoid *pixels);
typedef void ( *func_glTranslated_t) (GLdouble x,  GLdouble y,  GLdouble z);
typedef void ( *func_glTranslatef_t) (GLfloat x,  GLfloat y,  GLfloat z);
typedef void ( *func_glVertex2d_t) (GLdouble x,  GLdouble y);
typedef void ( *func_glVertex2dv_t) (const GLdouble *v);
typedef void ( *func_glVertex2f_t) (GLfloat x,  GLfloat y);
typedef void ( *func_glVertex2fv_t) (const GLfloat *v);
typedef void ( *func_glVertex2i_t) (GLint x,  GLint y);
typedef void ( *func_glVertex2iv_t) (const GLint *v);
typedef void ( *func_glVertex2s_t) (GLshort x,  GLshort y);
typedef void ( *func_glVertex2sv_t) (const GLshort *v);
typedef void ( *func_glVertex3d_t) (GLdouble x,  GLdouble y,  GLdouble z);
typedef void ( *func_glVertex3dv_t) (const GLdouble *v);
typedef void ( *func_glVertex3f_t) (GLfloat x,  GLfloat y,  GLfloat z);
typedef void ( *func_glVertex3fv_t) (const GLfloat *v);
typedef void ( *func_glVertex3i_t) (GLint x,  GLint y,  GLint z);
typedef void ( *func_glVertex3iv_t) (const GLint *v);
typedef void ( *func_glVertex3s_t) (GLshort x,  GLshort y,  GLshort z);
typedef void ( *func_glVertex3sv_t) (const GLshort *v);
typedef void ( *func_glVertex4d_t) (GLdouble x,  GLdouble y,  GLdouble z,  GLdouble w);
typedef void ( *func_glVertex4dv_t) (const GLdouble *v);
typedef void ( *func_glVertex4f_t) (GLfloat x,  GLfloat y,  GLfloat z,  GLfloat w);
typedef void ( *func_glVertex4fv_t) (const GLfloat *v);
typedef void ( *func_glVertex4i_t) (GLint x,  GLint y,  GLint z,  GLint w);
typedef void ( *func_glVertex4iv_t) (const GLint *v);
typedef void ( *func_glVertex4s_t) (GLshort x,  GLshort y,  GLshort z,  GLshort w);
typedef void ( *func_glVertex4sv_t) (const GLshort *v);
typedef void ( *func_glVertexPointer_t) (GLint size,  GLenum type,  GLsizei stride,  const GLvoid *pointer);
typedef void ( *func_glViewport_t) (GLint x,  GLint y,  GLsizei width,  GLsizei height);
typedef void ( *func_wglChoosePixelFormat_t) (void);
typedef void ( *func_wglCopyContext_t) (void);
typedef void ( *func_wglCreateContext_t) (void);
typedef void ( *func_wglCreateLayerContext_t) (void);
typedef void ( *func_wglDeleteContext_t) (void);
typedef void ( *func_wglDescribeLayerPlane_t) (void);
typedef void ( *func_wglDescribePixelFormat_t) (void);
typedef void ( *func_wglGetCurrentContext_t) (void);
typedef HDC  ( *func_wglGetCurrentDC_t) (void);
typedef void ( *func_wglGetDefaultProcAddress_t) (void);
typedef void ( *func_wglGetLayerPaletteEntries_t) (void);
typedef void ( *func_wglGetPixelFormat_t) (void);
typedef PROC ( *func_wglGetProcAddress_t) (LPCSTR);
typedef void ( *func_wglMakeCurrent_t) (void);
typedef void ( *func_wglRealizeLayerPalette_t) (void);
typedef void ( *func_wglSetLayerPaletteEntries_t) (void);
typedef void ( *func_wglSetPixelFormat_t) (void);
typedef void ( *func_wglShareLists_t) (void);
typedef void ( *func_wglSwapBuffers_t) (HDC hDC);
typedef void ( *func_wglSwapLayerBuffers_t) (HDC hdc, UINT fuPlanes);
typedef void ( *func_wglUseFontBitmapsA_t) (HDC, DWORD, DWORD, DWORD);
typedef void ( *func_wglUseFontBitmapsW_t) (void);
typedef void ( *func_wglUseFontOutlinesA_t) (HDC, DWORD, DWORD, DWORD, FLOAT, FLOAT, int, LPGLYPHMETRICSFLOAT);
typedef void ( *func_wglUseFontOutlinesW_t) (void);


// More textures
typedef void(*func_glTexImage2DMultisample_t)(GLenum target, GLsizei samples, GLint internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations);



// Shader Related
extern funct_glCreateProgram_t orig_glCreateProgram;
extern funct_glGetShaderSource_t orig_glGetShaderSource;
extern funct_glShaderSource_t orig_glShaderSource;
extern funct_glUseProgram_t orig_glUseProgram;
extern funct_glUseProgramObjectARB_t orig_glUseProgramObjectARB;
extern func_glCompileShaderARB orig_glCompileShaderARB;

extern funct_glGetShaderiv_t orig_glGetShader;
extern func_glGetUniformLocation_t orig_glGetUniformLocation;
extern func_glUniform1f_t orig_glUniform1f;
extern func_glBindFramebuffer_t orig_glBindFramebuffer;
extern func_glGenFramebuffers_t orig_glGenFramebuffers;
extern func_glFramebufferTexture_t orig_glFramebufferTexture;
extern func_glFramebufferTexture2D_t orig_glFramebufferTexture2D;
extern func_glCheckFramebufferStatus_t orig_glCheckFramebufferStatus;
extern func_glLinkProgram_t orig_glLinkProgram;
extern func_glGetProgramiv_t orig_glGetProgramiv;
extern funct_glGetAttachedShaders_t orig_glGetAttachedShaders;
extern func_glCompileShader orig_glCompileShader;
extern func_glDetachShader_t orig_glDetachShader;
extern func_glDeleteShader_t orig_glDeleteShader;
extern func_glGetShaderiv_t orig_glGetShaderiv;
extern func_glGetShaderInfoLog_t orig_glGetShaderInfoLog;
extern func_glGetProgramInfoLog_t orig_glGetProgramInfoLog;
extern func_glAttachShader_t orig_glAttachShader;
extern func_glCreateShader orig_glCreateShader;
extern func_glListBase_t orig_glListBase;

// Stereo Calls ID5
extern func_glDrawElementsBaseVertex_t orig_glDrawElementsBaseVertex;
extern func_glBlitFramebuffer_t	orig_glBlitFramebuffer;

// Texture
extern func_glBindMultiTexture_t orig_glBindMultiTexture;
extern func_glTexImage2DMultisample_t orig_glTexImage2DMultisample;
extern func_glTextureSubImage2D_t orig_glTextureSubImage2D;

// Shader info + other
extern func_glGetError_t orig_glGetError;
extern func_glUniformMatrix4fv_t orig_glUniformMatrix4fv;
extern func_glActiveTextureARB_t orig_glActiveTextureARB;
extern func_glActiveTexture_t orig_glActiveTexture;
extern func_BindTextureEXT_t orig_BindTextureEXT;
extern func_glMultiTexCoord2fARB_t orig_glMultiTexCoord2fARB;
extern func_glAccum_t	orig_glAccum;
extern func_glAlphaFunc_t	orig_glAlphaFunc;
extern func_glAreTexturesResident_t	orig_glAreTexturesResident;
extern func_glArrayElement_t	orig_glArrayElement;
extern func_glBegin_t	orig_glBegin;
extern func_glBindTexture_t	orig_glBindTexture;
extern func_glBitmap_t	orig_glBitmap;
extern func_glBlendFunc_t	orig_glBlendFunc;
extern func_glCallList_t	orig_glCallList;
extern func_glCallLists_t	orig_glCallLists;
extern func_glClear_t	orig_glClear;
extern func_glClearAccum_t	orig_glClearAccum;
extern func_glClearColor_t	orig_glClearColor;
extern func_glClearDepth_t	orig_glClearDepth;
extern func_glClearIndex_t	orig_glClearIndex;
extern func_glClearStencil_t	orig_glClearStencil;
extern func_glClipPlane_t	orig_glClipPlane;
extern func_glColor3b_t	orig_glColor3b;
extern func_glColor3bv_t	orig_glColor3bv;
extern func_glColor3d_t	orig_glColor3d;
extern func_glColor3dv_t	orig_glColor3dv;
extern func_glColor3f_t	orig_glColor3f;
extern func_glColor3fv_t	orig_glColor3fv;
extern func_glColor3i_t	orig_glColor3i;
extern func_glColor3iv_t	orig_glColor3iv;
extern func_glColor3s_t	orig_glColor3s;
extern func_glColor3sv_t	orig_glColor3sv;
extern func_glColor3ub_t	orig_glColor3ub;
extern func_glColor3ubv_t	orig_glColor3ubv;
extern func_glColor3ui_t	orig_glColor3ui;
extern func_glColor3uiv_t	orig_glColor3uiv;
extern func_glColor3us_t	orig_glColor3us;
extern func_glColor3usv_t	orig_glColor3usv;
extern func_glColor4b_t	orig_glColor4b;
extern func_glColor4bv_t	orig_glColor4bv;
extern func_glColor4d_t	orig_glColor4d;
extern func_glColor4dv_t	orig_glColor4dv;
extern func_glColor4f_t	orig_glColor4f;
extern func_glColor4fv_t	orig_glColor4fv;
extern func_glColor4i_t	orig_glColor4i;
extern func_glColor4iv_t	orig_glColor4iv;
extern func_glColor4s_t	orig_glColor4s;
extern func_glColor4sv_t	orig_glColor4sv;
extern func_glColor4ub_t	orig_glColor4ub;
extern func_glColor4ubv_t	orig_glColor4ubv;
extern func_glColor4ui_t	orig_glColor4ui;
extern func_glColor4uiv_t	orig_glColor4uiv;
extern func_glColor4us_t	orig_glColor4us;
extern func_glColor4usv_t	orig_glColor4usv;
extern func_glColorMask_t	orig_glColorMask;
extern func_glColorMaterial_t	orig_glColorMaterial;
extern func_glColorPointer_t	orig_glColorPointer;
extern func_glCopyPixels_t	orig_glCopyPixels;
extern func_glCopyTexImage1D_t	orig_glCopyTexImage1D;
extern func_glCopyTexImage2D_t	orig_glCopyTexImage2D;
extern func_glCopyTexSubImage1D_t	orig_glCopyTexSubImage1D;
extern func_glCopyTexSubImage2D_t	orig_glCopyTexSubImage2D;
extern func_glCullFace_t	orig_glCullFace;
extern func_glDebugEntry_t	orig_glDebugEntry;
extern func_glDeleteLists_t	orig_glDeleteLists;
extern func_glDeleteTextures_t	orig_glDeleteTextures;
extern func_glDepthFunc_t	orig_glDepthFunc;
extern func_glDepthMask_t	orig_glDepthMask;
extern func_glDepthRange_t	orig_glDepthRange;
extern func_glDisable_t	orig_glDisable;
extern func_glDisableClientState_t	orig_glDisableClientState;
extern func_glDrawArrays_t	orig_glDrawArrays;
extern func_glDrawBuffer_t	orig_glDrawBuffer;
extern func_glDrawElements_t	orig_glDrawElements;
extern func_glDrawPixels_t	orig_glDrawPixels;
extern func_glEdgeFlag_t	orig_glEdgeFlag;
extern func_glEdgeFlagPointer_t	orig_glEdgeFlagPointer;
extern func_glEdgeFlagv_t	orig_glEdgeFlagv;
extern func_glEnable_t	orig_glEnable;
extern func_glEnableClientState_t	orig_glEnableClientState;
extern func_glEnd_t	orig_glEnd;
extern func_glEndList_t	orig_glEndList;
extern func_glEvalCoord1d_t	orig_glEvalCoord1d;
extern func_glEvalCoord1dv_t	orig_glEvalCoord1dv;
extern func_glEvalCoord1f_t	orig_glEvalCoord1f;
extern func_glEvalCoord1fv_t	orig_glEvalCoord1fv;
extern func_glEvalCoord2d_t	orig_glEvalCoord2d;
extern func_glEvalCoord2dv_t	orig_glEvalCoord2dv;
extern func_glEvalCoord2f_t	orig_glEvalCoord2f;
extern func_glEvalCoord2fv_t	orig_glEvalCoord2fv;
extern func_glEvalMesh1_t	orig_glEvalMesh1;
extern func_glEvalMesh2_t	orig_glEvalMesh2;
extern func_glEvalPoint1_t	orig_glEvalPoint1;
extern func_glEvalPoint2_t	orig_glEvalPoint2;
extern func_glFeedbackBuffer_t	orig_glFeedbackBuffer;
extern func_glFinish_t	orig_glFinish;
extern func_glFlush_t	orig_glFlush;
extern func_glFogf_t	orig_glFogf;
extern func_glFogfv_t	orig_glFogfv;
extern func_glFogi_t	orig_glFogi;
extern func_glFogiv_t	orig_glFogiv;
extern func_glFrontFace_t	orig_glFrontFace;
extern func_glFrustum_t	orig_glFrustum;
extern func_glGenLists_t	orig_glGenLists;
extern func_glGenTextures_t	orig_glGenTextures;
extern func_glGetBooleanv_t	orig_glGetBooleanv;
extern func_glGetClipPlane_t	orig_glGetClipPlane;
extern func_glGetDoublev_t	orig_glGetDoublev;
extern func_glGetError_t	orig_glGetError;
extern func_glGetFloatv_t	orig_glGetFloatv;
extern func_glGetIntegerv_t	orig_glGetIntegerv;
extern func_glGetLightfv_t	orig_glGetLightfv;
extern func_glGetLightiv_t	orig_glGetLightiv;
extern func_glGetMapdv_t	orig_glGetMapdv;
extern func_glGetMapfv_t	orig_glGetMapfv;
extern func_glGetMapiv_t	orig_glGetMapiv;
extern func_glGetMaterialfv_t	orig_glGetMaterialfv;
extern func_glGetMaterialiv_t	orig_glGetMaterialiv;
extern func_glGetPixelMapfv_t	orig_glGetPixelMapfv;
extern func_glGetPixelMapuiv_t	orig_glGetPixelMapuiv;
extern func_glGetPixelMapusv_t	orig_glGetPixelMapusv;
extern func_glGetPointerv_t	orig_glGetPointerv;
extern func_glGetPolygonStipple_t	orig_glGetPolygonStipple;
extern func_glGetString_t	orig_glGetString;
extern func_glGetTexEnvfv_t	orig_glGetTexEnvfv;
extern func_glGetTexEnviv_t	orig_glGetTexEnviv;
extern func_glGetTexGendv_t	orig_glGetTexGendv;
extern func_glGetTexGenfv_t	orig_glGetTexGenfv;
extern func_glGetTexGeniv_t	orig_glGetTexGeniv;
extern func_glGetTexImage_t	orig_glGetTexImage;
extern func_glGetTexLevelParameterfv_t	orig_glGetTexLevelParameterfv;
extern func_glGetTexLevelParameteriv_t	orig_glGetTexLevelParameteriv;
extern func_glGetTexParameterfv_t	orig_glGetTexParameterfv;
extern func_glGetTexParameteriv_t	orig_glGetTexParameteriv;
extern func_glHint_t	orig_glHint;
extern func_glIndexMask_t	orig_glIndexMask;
extern func_glIndexPointer_t	orig_glIndexPointer;
extern func_glIndexd_t	orig_glIndexd;
extern func_glIndexdv_t	orig_glIndexdv;
extern func_glIndexf_t	orig_glIndexf;
extern func_glIndexfv_t	orig_glIndexfv;
extern func_glIndexi_t	orig_glIndexi;
extern func_glIndexiv_t	orig_glIndexiv;
extern func_glIndexs_t	orig_glIndexs;
extern func_glIndexsv_t	orig_glIndexsv;
extern func_glIndexub_t	orig_glIndexub;
extern func_glIndexubv_t	orig_glIndexubv;
extern func_glInitNames_t	orig_glInitNames;
extern func_glInterleavedArrays_t	orig_glInterleavedArrays;
extern func_glIsEnabled_t	orig_glIsEnabled;
extern func_glIsList_t	orig_glIsList;
extern func_glIsTexture_t	orig_glIsTexture;
extern func_glLightModelf_t	orig_glLightModelf;
extern func_glLightModelfv_t	orig_glLightModelfv;
extern func_glLightModeli_t	orig_glLightModeli;
extern func_glLightModeliv_t	orig_glLightModeliv;
extern func_glLightf_t	orig_glLightf;
extern func_glLightfv_t	orig_glLightfv;
extern func_glLighti_t	orig_glLighti;
extern func_glLightiv_t	orig_glLightiv;
extern func_glLineStipple_t	orig_glLineStipple;
extern func_glLineWidth_t	orig_glLineWidth;
extern func_glListBase_t	orig_glListBase;
extern func_glLoadIdentity_t	orig_glLoadIdentity;
extern func_glLoadMatrixd_t	orig_glLoadMatrixd;
extern func_glLoadMatrixf_t	orig_glLoadMatrixf;
extern func_glLoadName_t	orig_glLoadName;
extern func_glLogicOp_t	orig_glLogicOp;
extern func_glMap1d_t	orig_glMap1d;
extern func_glMap1f_t	orig_glMap1f;
extern func_glMap2d_t	orig_glMap2d;
extern func_glMap2f_t	orig_glMap2f;
extern func_glMapGrid1d_t	orig_glMapGrid1d;
extern func_glMapGrid1f_t	orig_glMapGrid1f;
extern func_glMapGrid2d_t	orig_glMapGrid2d;
extern func_glMapGrid2f_t	orig_glMapGrid2f;
extern func_glMaterialf_t	orig_glMaterialf;
extern func_glMaterialfv_t	orig_glMaterialfv;
extern func_glMateriali_t	orig_glMateriali;
extern func_glMaterialiv_t	orig_glMaterialiv;
extern func_glMatrixMode_t	orig_glMatrixMode;
extern func_glMultMatrixd_t	orig_glMultMatrixd;
extern func_glMultMatrixf_t	orig_glMultMatrixf;
extern func_glNewList_t	orig_glNewList;
extern func_glNormal3b_t	orig_glNormal3b;
extern func_glNormal3bv_t	orig_glNormal3bv;
extern func_glNormal3d_t	orig_glNormal3d;
extern func_glNormal3dv_t	orig_glNormal3dv;
extern func_glNormal3f_t	orig_glNormal3f;
extern func_glNormal3fv_t	orig_glNormal3fv;
extern func_glNormal3i_t	orig_glNormal3i;
extern func_glNormal3iv_t	orig_glNormal3iv;
extern func_glNormal3s_t	orig_glNormal3s;
extern func_glNormal3sv_t	orig_glNormal3sv;
extern func_glNormalPointer_t	orig_glNormalPointer;
extern func_glOrtho_t	orig_glOrtho;
extern func_glPassThrough_t	orig_glPassThrough;
extern func_glPixelMapfv_t	orig_glPixelMapfv;
extern func_glPixelMapuiv_t	orig_glPixelMapuiv;
extern func_glPixelMapusv_t	orig_glPixelMapusv;
extern func_glPixelStoref_t	orig_glPixelStoref;
extern func_glPixelStorei_t	orig_glPixelStorei;
extern func_glPixelTransferf_t	orig_glPixelTransferf;
extern func_glPixelTransferi_t	orig_glPixelTransferi;
extern func_glPixelZoom_t	orig_glPixelZoom;
extern func_glPointSize_t	orig_glPointSize;
extern func_glPolygonMode_t	orig_glPolygonMode;
extern func_glPolygonOffset_t	orig_glPolygonOffset;
extern func_glPolygonStipple_t	orig_glPolygonStipple;
extern func_glPopAttrib_t	orig_glPopAttrib;
extern func_glPopClientAttrib_t	orig_glPopClientAttrib;
extern func_glPopMatrix_t	orig_glPopMatrix;
extern func_glPopName_t	orig_glPopName;
extern func_glPrioritizeTextures_t	orig_glPrioritizeTextures;
extern func_glPushAttrib_t	orig_glPushAttrib;
extern func_glPushClientAttrib_t	orig_glPushClientAttrib;
extern func_glPushMatrix_t	orig_glPushMatrix;
extern func_glPushName_t	orig_glPushName;
extern func_glRasterPos2d_t	orig_glRasterPos2d;
extern func_glRasterPos2dv_t	orig_glRasterPos2dv;
extern func_glRasterPos2f_t	orig_glRasterPos2f;
extern func_glRasterPos2fv_t	orig_glRasterPos2fv;
extern func_glRasterPos2i_t	orig_glRasterPos2i;
extern func_glRasterPos2iv_t	orig_glRasterPos2iv;
extern func_glRasterPos2s_t	orig_glRasterPos2s;
extern func_glRasterPos2sv_t	orig_glRasterPos2sv;
extern func_glRasterPos3d_t	orig_glRasterPos3d;
extern func_glRasterPos3dv_t	orig_glRasterPos3dv;
extern func_glRasterPos3f_t	orig_glRasterPos3f;
extern func_glRasterPos3fv_t	orig_glRasterPos3fv;
extern func_glRasterPos3i_t	orig_glRasterPos3i;
extern func_glRasterPos3iv_t	orig_glRasterPos3iv;
extern func_glRasterPos3s_t	orig_glRasterPos3s;
extern func_glRasterPos3sv_t	orig_glRasterPos3sv;
extern func_glRasterPos4d_t	orig_glRasterPos4d;
extern func_glRasterPos4dv_t	orig_glRasterPos4dv;
extern func_glRasterPos4f_t	orig_glRasterPos4f;
extern func_glRasterPos4fv_t	orig_glRasterPos4fv;
extern func_glRasterPos4i_t	orig_glRasterPos4i;
extern func_glRasterPos4iv_t	orig_glRasterPos4iv;
extern func_glRasterPos4s_t	orig_glRasterPos4s;
extern func_glRasterPos4sv_t	orig_glRasterPos4sv;
extern func_glReadBuffer_t	orig_glReadBuffer;
extern func_glReadPixels_t	orig_glReadPixels;
extern func_glRectd_t	orig_glRectd;
extern func_glRectdv_t	orig_glRectdv;
extern func_glRectf_t	orig_glRectf;
extern func_glRectfv_t	orig_glRectfv;
extern func_glRecti_t	orig_glRecti;
extern func_glRectiv_t	orig_glRectiv;
extern func_glRects_t	orig_glRects;
extern func_glRectsv_t	orig_glRectsv;
extern func_glRenderMode_t	orig_glRenderMode;
extern func_glRotated_t	orig_glRotated;
extern func_glRotatef_t	orig_glRotatef;
extern func_glScaled_t	orig_glScaled;
extern func_glScalef_t	orig_glScalef;
extern func_glScissor_t	orig_glScissor;
extern func_glSelectBuffer_t	orig_glSelectBuffer;
extern func_glShadeModel_t	orig_glShadeModel;
extern func_glStencilFunc_t	orig_glStencilFunc;
extern func_glStencilMask_t	orig_glStencilMask;
extern func_glStencilOp_t	orig_glStencilOp;
extern func_glTexCoord1d_t	orig_glTexCoord1d;
extern func_glTexCoord1dv_t	orig_glTexCoord1dv;
extern func_glTexCoord1f_t	orig_glTexCoord1f;
extern func_glTexCoord1fv_t	orig_glTexCoord1fv;
extern func_glTexCoord1i_t	orig_glTexCoord1i;
extern func_glTexCoord1iv_t	orig_glTexCoord1iv;
extern func_glTexCoord1s_t	orig_glTexCoord1s;
extern func_glTexCoord1sv_t	orig_glTexCoord1sv;
extern func_glTexCoord2d_t	orig_glTexCoord2d;
extern func_glTexCoord2dv_t	orig_glTexCoord2dv;
extern func_glTexCoord2f_t	orig_glTexCoord2f;
extern func_glTexCoord2fv_t	orig_glTexCoord2fv;
extern func_glTexCoord2i_t	orig_glTexCoord2i;
extern func_glTexCoord2iv_t	orig_glTexCoord2iv;
extern func_glTexCoord2s_t	orig_glTexCoord2s;
extern func_glTexCoord2sv_t	orig_glTexCoord2sv;
extern func_glTexCoord3d_t	orig_glTexCoord3d;
extern func_glTexCoord3dv_t	orig_glTexCoord3dv;
extern func_glTexCoord3f_t	orig_glTexCoord3f;
extern func_glTexCoord3fv_t	orig_glTexCoord3fv;
extern func_glTexCoord3i_t	orig_glTexCoord3i;
extern func_glTexCoord3iv_t	orig_glTexCoord3iv;
extern func_glTexCoord3s_t	orig_glTexCoord3s;
extern func_glTexCoord3sv_t	orig_glTexCoord3sv;
extern func_glTexCoord4d_t	orig_glTexCoord4d;
extern func_glTexCoord4dv_t	orig_glTexCoord4dv;
extern func_glTexCoord4f_t	orig_glTexCoord4f;
extern func_glTexCoord4fv_t	orig_glTexCoord4fv;
extern func_glTexCoord4i_t	orig_glTexCoord4i;
extern func_glTexCoord4iv_t	orig_glTexCoord4iv;
extern func_glTexCoord4s_t	orig_glTexCoord4s;
extern func_glTexCoord4sv_t	orig_glTexCoord4sv;
extern func_glTexCoordPointer_t	orig_glTexCoordPointer;
extern func_glTexEnvf_t	orig_glTexEnvf;
extern func_glTexEnvfv_t	orig_glTexEnvfv;
extern func_glTexEnvi_t	orig_glTexEnvi;
extern func_glTexEnviv_t	orig_glTexEnviv;
extern func_glTexGend_t	orig_glTexGend;
extern func_glTexGendv_t	orig_glTexGendv;
extern func_glTexGenf_t	orig_glTexGenf;
extern func_glTexGenfv_t	orig_glTexGenfv;
extern func_glTexGeni_t	orig_glTexGeni;
extern func_glTexGeniv_t	orig_glTexGeniv;
extern func_glTexImage1D_t	orig_glTexImage1D;
extern func_glTexImage2D_t	orig_glTexImage2D;
extern func_glTexParameterf_t	orig_glTexParameterf;
extern func_glTexParameterfv_t	orig_glTexParameterfv;
extern func_glTexParameteri_t	orig_glTexParameteri;
extern func_glTexParameteriv_t	orig_glTexParameteriv;
extern func_glTexSubImage1D_t	orig_glTexSubImage1D;
extern func_glTexSubImage2D_t	orig_glTexSubImage2D;
extern func_glTranslated_t	orig_glTranslated;
extern func_glTranslatef_t orig_glTranslatef;
extern func_glVertex2d_t	orig_glVertex2d;
extern func_glVertex2dv_t	orig_glVertex2dv;
extern func_glVertex2f_t	orig_glVertex2f;
extern func_glVertex2fv_t	orig_glVertex2fv;
extern func_glVertex2i_t	orig_glVertex2i;
extern func_glVertex2iv_t	orig_glVertex2iv;
extern func_glVertex2s_t	orig_glVertex2s;
extern func_glVertex2sv_t	orig_glVertex2sv;
extern func_glVertex3d_t	orig_glVertex3d;
extern func_glVertex3dv_t	orig_glVertex3dv;
extern func_glVertex3f_t	orig_glVertex3f;
extern func_glVertex3fv_t	orig_glVertex3fv;
extern func_glVertex3i_t	orig_glVertex3i;
extern func_glVertex3iv_t	orig_glVertex3iv;
extern func_glVertex3s_t	orig_glVertex3s;
extern func_glVertex3sv_t	orig_glVertex3sv;
extern func_glVertex4d_t	orig_glVertex4d;
extern func_glVertex4dv_t	orig_glVertex4dv;
extern func_glVertex4f_t	orig_glVertex4f;
extern func_glVertex4fv_t	orig_glVertex4fv;
extern func_glVertex4i_t	orig_glVertex4i;
extern func_glVertex4iv_t	orig_glVertex4iv;
extern func_glVertex4s_t	orig_glVertex4s;
extern func_glVertex4sv_t	orig_glVertex4sv;
extern func_glVertexPointer_t	orig_glVertexPointer;
extern func_glViewport_t	orig_glViewport;
extern func_wglChoosePixelFormat_t	orig_wglChoosePixelFormat;
extern func_wglCopyContext_t	orig_wglCopyContext;
extern func_wglCreateContext_t	orig_wglCreateContext;
extern func_wglCreateLayerContext_t	orig_wglCreateLayerContext;
extern func_wglDeleteContext_t	orig_wglDeleteContext;
extern func_wglDescribeLayerPlane_t	orig_wglDescribeLayerPlane;
extern func_wglDescribePixelFormat_t	orig_wglDescribePixelFormat;
extern func_wglGetCurrentContext_t	orig_wglGetCurrentContext;
extern func_wglGetCurrentDC_t	orig_wglGetCurrentDC;
extern func_wglGetDefaultProcAddress_t	orig_wglGetDefaultProcAddress;
extern func_wglGetLayerPaletteEntries_t	orig_wglGetLayerPaletteEntries;
extern func_wglGetPixelFormat_t	orig_wglGetPixelFormat;
extern func_wglGetProcAddress_t	orig_wglGetProcAddress;
extern func_wglMakeCurrent_t	orig_wglMakeCurrent;
extern func_wglRealizeLayerPalette_t	orig_wglRealizeLayerPalette;
extern func_wglSetLayerPaletteEntries_t	orig_wglSetLayerPaletteEntries;
extern func_wglSetPixelFormat_t	orig_wglSetPixelFormat;
extern func_wglShareLists_t	orig_wglShareLists;
extern func_wglSwapBuffers_t	orig_wglSwapBuffers;
extern func_wglSwapLayerBuffers_t	orig_wglSwapLayerBuffers;
extern func_wglUseFontBitmapsA_t	orig_wglUseFontBitmapsA;
extern func_wglUseFontBitmapsW_t	orig_wglUseFontBitmapsW;
extern func_wglUseFontOutlinesA_t	orig_wglUseFontOutlinesA;
extern func_wglUseFontOutlinesW_t	orig_wglUseFontOutlinesW;

// Function to get the main thread handle
DWORD GetMainThreadId(void);

#endif
