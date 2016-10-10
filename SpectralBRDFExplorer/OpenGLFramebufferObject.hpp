//
//  OpenGLFrameBuffer.hpp
//  SpectralBRDFExplorer
//
//  Created by Fabrizio Duroni on 10/10/2016.
//  Copyright © 2016 Fabrizio Duroni. All rights reserved.
//

#ifndef OpenGLFrameBuffer_hpp
#define OpenGLFrameBuffer_hpp

#include <iostream>

#ifdef __APPLE__
#include <OpenGLES/ES3/gl.h>
#else
#include <GLES3/gl3.h>
#endif

class OpenGLFramebufferObject {
public:
    
    GLuint framebufferObjectId;
    
    bool attach2DTexture(GLuint _textureId, GLenum attachment);
};

#endif /* OpenGLFrameBuffer_hpp */
