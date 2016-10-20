//
//  OpenGLESHeader.h
//  SpectralBRDFExplorer
//
//  Created by Fabrizio Duroni on 16/10/2016.
//  
//

#ifndef OpenGLESHeader_h
#define OpenGLESHeader_h

/*!
 Header for the various mobile platform supperted:
 
 - iOS declares OpenGL ES headers in <OpenGLES/ES3/gl.h>
 - Android declares OpenGL ES headers in <GLES3/gl3.h>
 */
#ifdef __APPLE__
#include <OpenGLES/ES3/gl.h>
#else
#include <GLES3/gl3.h>
#endif

#endif
