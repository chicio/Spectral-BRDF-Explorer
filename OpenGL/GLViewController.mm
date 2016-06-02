//
//  GLViewController.m
//  OpenGL
//
//  Created by Fabrizio Duroni on 27/05/16.
//  Copyright © 2016 Fabrizio Duroni. All rights reserved.
//

#import <OpenGLES/ES3/gl.h>
#include <vector>
#include <iostream>

#import "OpenGLObj.hpp"
#import "GLViewController.h"

@interface GLViewController () {
    
    NSString *_modelFilePath;
    
    //Program.
    GLuint _programObject;
    GLuint _vboIds[2];
    
    //Matrices.
    GLKMatrix4 _mvMatrix;
    GLKMatrix4 _mvpMatrix;
    GLKMatrix4 _normalMatrix;
    
    //Uniforms.
    GLint _mvLocation;
    GLint _mvpLocation;
    GLint _normalLocation;
    GLint _lightPosition;
    GLint _lightColor;
    GLint _materialAmbient;
    GLint _materialDiffuse;
    GLint _materialSpecular;
    GLint _materialSpecularExponent;
    
    float _rotation;

    OpenGLObj openglObj;
}

@property (strong, nonatomic) EAGLContext *context;
@end

@implementation GLViewController

#pragma setter

- (void)setModel:(NSString *)name {
    
    _modelFilePath = [[NSBundle mainBundle] pathForResource:name ofType:@"obj"];
}

#pragma mark Data OpenGL

#define VERTEX_POS_INDX       0
#define VERTEX_NORMAL_INDX    1

#pragma mark Apple View Lifecycle

- (void)viewDidLoad {
    
    [super viewDidLoad];
    
    self.context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES3];
    
    if(!self.context) {
        
        //Failed to load OpenGL ES 3 context.
        UIAlertController* alert = [UIAlertController
                                    alertControllerWithTitle:@"OpenGL"
                                    message:@"OpenGL ES 3 not available"
                                    preferredStyle:UIAlertControllerStyleAlert];
        
        [self presentViewController:alert animated:YES completion:nil];
    }
    
    if([EAGLContext setCurrentContext:self.context]) {
        
        GLKView *view = (GLKView *)self.view;
        view.context = self.context;
        view.drawableDepthFormat = GLKViewDrawableDepthFormat24;
        
        //Init obj.
        std::string error;
        openglObj.parseObj([_modelFilePath cStringUsingEncoding:NSUTF8StringEncoding], error);
        
        //Init opengl.
        [self initOpenGL];

        //Load obj data.
        [self loadOpenGLObjData];
    }
}

#pragma mark Init OpenGL

- (void)initOpenGL {
    
    const char *vertexShaderSource = [self shaderSource:@"vertex" andExtension:@"vsh"];
    const char *fragmentShaderSource = [self shaderSource:@"fragment" andExtension:@"fsh"];
    
    //Load shaders.
    GLuint vertexShader = [self loadShader:GL_VERTEX_SHADER source:vertexShaderSource];
    GLuint fragmentShader = [self loadShader:GL_FRAGMENT_SHADER source:fragmentShaderSource];
    
    //Create program.
    _programObject = glCreateProgram();
    
    //Attach shader to program.
    glAttachShader(_programObject, vertexShader);
    glAttachShader(_programObject, fragmentShader);
    
    //Link the program.
    glLinkProgram(_programObject);
    
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    
    // Check the link status
    GLint linked;
    glGetProgramiv (_programObject, GL_LINK_STATUS, &linked );
    
    if (!linked) {
        
        GLint infoLen = 0;
        glGetProgramiv(_programObject, GL_INFO_LOG_LENGTH, &infoLen);
        
        if(infoLen > 1) {
            
            char *infoLog = (char *)malloc (sizeof(char) * infoLen);
            glGetProgramInfoLog(_programObject, infoLen, NULL, infoLog);
            printf("%s", infoLog);
        }
        
        glDeleteProgram(_programObject);
    }
    
    //Prepare uniform to be loaded.
    _mvLocation = glGetUniformLocation(_programObject, "mvMatrix");
    _mvpLocation = glGetUniformLocation(_programObject, "mvpMatrix");
    _normalLocation = glGetUniformLocation(_programObject, "normalMatrix");
    _lightPosition = glGetUniformLocation(_programObject, "light.position");
    _lightColor = glGetUniformLocation(_programObject, "light.color");
    _materialAmbient = glGetUniformLocation(_programObject, "surfaceMaterial.ka");
    _materialDiffuse = glGetUniformLocation(_programObject, "surfaceMaterial.kd");
    _materialSpecular = glGetUniformLocation(_programObject, "surfaceMaterial.ks");
    _materialSpecularExponent = glGetUniformLocation(_programObject, "surfaceMaterial.sh");
    
    // Free up no longer needed shader resources
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

#pragma mark Data OpenGL

- (void)loadOpenGLObjData {
    
    //Generate buffer objects: one for vertex data and one for vertex indices.
    glGenBuffers(1, _vboIds);
    
    //Start buffer for vertex data.
    glBindBuffer(GL_ARRAY_BUFFER, _vboIds[0]);
    glBufferData(GL_ARRAY_BUFFER, openglObj.getVerticesDataSize(), openglObj.getVerticesData().data(), GL_STATIC_DRAW);
}

#pragma mark Draw OpenGL

- (void)update {
    
    //Projection matrix.
    float aspect = fabs(self.view.bounds.size.width / self.view.bounds.size.height);
    GLKMatrix4 projectionMatrix = GLKMatrix4MakePerspective(GLKMathDegreesToRadians(65.0f), aspect, 0.1f, 10.0f);
    
    //Modelview matrix.
    _mvMatrix = GLKMatrix4MakeTranslation(0.0f, 0.0f, -5.0f);
    _mvMatrix = GLKMatrix4Rotate(_mvMatrix, _rotation, 0.0f, 1.0f, 0.0f);
    
    //Set inverse transpose matrix for normal.
    _normalMatrix = GLKMatrix4InvertAndTranspose(_mvMatrix, NULL);

    //Set uniform modelviewprojection matrix.
    _mvpMatrix = GLKMatrix4Multiply(projectionMatrix, _mvMatrix);
    
    _rotation += self.timeSinceLastUpdate * 0.5f;
}

- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect {
    
    //Set states.
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    //Install program.
    glUseProgram(_programObject);
    
    //Bind buffers.
    glBindBuffer(GL_ARRAY_BUFFER, _vboIds[0]);
    
    //Enable vertex attribute.
    GLvoid *offset = (GLvoid *)(VERTEX_POS_SIZE * sizeof(GLfloat));
    glEnableVertexAttribArray(VERTEX_POS_INDX);
    glEnableVertexAttribArray(VERTEX_NORMAL_INDX);
    glVertexAttribPointer(VERTEX_POS_INDX, VERTEX_POS_SIZE, GL_FLOAT, GL_FALSE, openglObj.getStride(), 0);
    glVertexAttribPointer(VERTEX_NORMAL_INDX, VERTEX_NORMAL_SIZE, GL_FLOAT, GL_FALSE, openglObj.getStride(), offset);
    
    //Load uniforms.
    glUniformMatrix4fv(_mvLocation, 1, GL_FALSE, (GLfloat *)_mvMatrix.m);
    glUniformMatrix4fv(_mvpLocation, 1, GL_FALSE, (GLfloat *)_mvpMatrix.m);
    glUniformMatrix4fv(_normalLocation, 1, GL_FALSE, (GLfloat *)_normalMatrix.m);
    
    glUniform3f(_lightPosition, 1.0, 1.0, 1.0);
    glUniform4f(_lightColor, 1.0, 1.0, 1.0, 1.0);
    
    glUniform4f(_materialAmbient, 0.1745, 0.01175, 0.01175, 1.0);
    glUniform4f(_materialDiffuse, 0.61424, 0.04136, 0.04136, 1.0);
    glUniform4f(_materialSpecular, 1.0, 1.0, 1.0, 1.0);
    glUniform1f(_materialSpecularExponent, 76);
    
    //Draw model.
    glDrawArrays(GL_TRIANGLES, 0, openglObj.getNumberOfVerticesToDraw());
    
    glDisableVertexAttribArray ( VERTEX_POS_INDX );
    glDisableVertexAttribArray ( VERTEX_NORMAL_INDX );
    
    glBindBuffer ( GL_ARRAY_BUFFER, 0 );
}

#pragma mark Shader OpenGL

- (const char *)shaderSource:(NSString *)shaderName andExtension:(NSString *)shaderExtension {
    
    NSString *shaderPath = [[NSBundle mainBundle] pathForResource:shaderName ofType:shaderExtension];
    const char *shaderSource = [[NSString stringWithContentsOfFile:shaderPath
                                                          encoding:NSUTF8StringEncoding
                                                             error:nil] UTF8String];
    return shaderSource;
}

- (GLuint)loadShader:(GLenum)type source:(const char *)shaderSrc {
    
    //Create the shader object.
    GLuint shader = glCreateShader(type);
    
    if (shader == 0) {
        
        return 0;
    }
    
    //Load the shader source.
    glShaderSource(shader, 1, &shaderSrc, NULL);
    
    //Compile the shader.
    glCompileShader(shader);
    
    // Check the compile status
    GLint compiled;
    glGetShaderiv ( shader, GL_COMPILE_STATUS, &compiled );
    
    if (!compiled){
        
        GLint infoLen = 0;
        glGetShaderiv ( shader, GL_INFO_LOG_LENGTH, &infoLen );
        
        if ( infoLen > 1 ){
            
            char *infoLog = (char *)malloc (sizeof(char) * infoLen);
            glGetShaderInfoLog(shader, infoLen, NULL, infoLog);
            printf("%s", infoLog);
        }
        
        glDeleteShader ( shader );
        return 0;
    }
    
    return shader;
}

#pragma mark Terminate OpenGL

- (void)tearDownGL {
    
    [EAGLContext setCurrentContext:self.context];
    
    glDeleteProgram(_programObject);
    glDeleteBuffers(2, _vboIds);
}

- (void)didReceiveMemoryWarning {
    
    [super didReceiveMemoryWarning];
    
    if ([self isViewLoaded] && ([[self view] window] == nil)) {
        
        self.view = nil;
        
        [self tearDownGL];
        
        if ([EAGLContext currentContext] == self.context) {
            
            [EAGLContext setCurrentContext:nil];
        }
        
        self.context = nil;
    }
}

@end
