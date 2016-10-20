//
//  OpenGLFrameBuffer.cpp
//  SpectralBRDFExplorer
//
//  Created by Fabrizio Duroni on 10/10/2016.
//  
//

#include "OpenGLFramebufferObject.hpp"

bool OpenGLFramebufferObject::attach2DTexture(GLuint textureId, GLenum attachment, GLenum drawBuffers) {
    
    //Get default framebuffer handle.
    GLint defaultFramebuffer = 0;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &defaultFramebuffer);
    
    //Setup FBO.
    //GLenum none = GL_NONE;
    glGenFramebuffers(1, &framebufferObjectId);
    glBindFramebuffer(GL_FRAMEBUFFER, framebufferObjectId);
    
    //No color buffer needed (only depth).
    glDrawBuffers(1, &drawBuffers);
    
    //Set texture.
    glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, textureId, 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureId);
    
    if(GL_FRAMEBUFFER_COMPLETE != glCheckFramebufferStatus(GL_FRAMEBUFFER)) {
        
        std::cout << "ERROR FRAMEBUFFER OBJECT " << glCheckFramebufferStatus(GL_FRAMEBUFFER);
        return false;
    }
    
    //Restore default framebuffer after setup.
    glBindFramebuffer(GL_FRAMEBUFFER, defaultFramebuffer);
    
    return true;
}
