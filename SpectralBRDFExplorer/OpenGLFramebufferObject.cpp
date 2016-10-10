//
//  OpenGLFrameBuffer.cpp
//  SpectralBRDFExplorer
//
//  Created by Fabrizio Duroni on 10/10/2016.
//  Copyright © 2016 Fabrizio Duroni. All rights reserved.
//

#include "OpenGLFramebufferObject.hpp"

bool OpenGLFramebufferObject::attach2DTexture(GLuint _textureId, GLenum attachment) {
    
    //Get default framebuffer handle.
    GLint defaultFramebuffer = 0;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &defaultFramebuffer);
    
    //Setup FBO.
    GLenum none = GL_NONE;
    glGenFramebuffers(1, &framebufferObjectId);
    glBindFramebuffer(GL_FRAMEBUFFER, framebufferObjectId);
    
    //No color buffer needed (only depth).
    glDrawBuffers(1, &none);
    //Set texture.
    //glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _textureId, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, _textureId, 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _textureId);
    
    if(GL_FRAMEBUFFER_COMPLETE != glCheckFramebufferStatus(GL_FRAMEBUFFER)) {
        
        std::cout << "ERROR FRAMEBUFFER OBJECT " << glCheckFramebufferStatus(GL_FRAMEBUFFER);
        return false;
    }
    
    //Restore default framebuffer after setup.
    glBindFramebuffer(GL_FRAMEBUFFER, defaultFramebuffer);
    
    return true;
}
