//
//  OpenGLFrameBuffer.hpp
//  SpectralBRDFExplorer
//
//  Created by Fabrizio Duroni on 10/10/2016.
//  
//

#ifndef OpenGLFrameBuffer_hpp
#define OpenGLFrameBuffer_hpp

#include <OpenGLESHeaders.h>
#include <iostream>

class OpenGLFramebufferObject {
public:
    
    /// Framebuffer object id.
    GLuint framebufferObjectId;
    
    /*!
     Attach a 2D texture to the framebuffer object.
     
     @params textureId the id of the texture.
     @params attachment framebuffer attachment type.
     @params drawBuffers array of draw buffers to be used. 
     
     @returns true if 2D texture has been attached, else false.
     */
    bool attach2DTexture(GLuint textureId, GLenum attachment, GLenum drawBuffers);
};

#endif /* OpenGLFrameBuffer_hpp */
