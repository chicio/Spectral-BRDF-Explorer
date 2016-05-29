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

#import "tiny_obj_loader.h"
#import "GLViewController.h"

@interface GLViewController () {
    
    GLuint _programObject;
    GLuint _vboIds[2];
    GLint _stride;
    
    GLKMatrix4 _mvpMatrix;
    GLKMatrix4 _normalMatrix;
    GLint _mvpLocation;
    GLint _normalLocation;
    
    std::vector<tinyobj::shape_t> _shapes;
    std::vector<tinyobj::material_t> _materials;
}

@property (strong, nonatomic) EAGLContext *context;
@end

@implementation GLViewController

#pragma mark Data OpenGL

#define VERTEX_POS_SIZE       3
#define VERTEX_NORMAL_SIZE    3
#define VERTEX_POS_INDX       0
#define VERTEX_NORMAL_INDX    1

#pragma mark Apple View Lifecycle

GLfloat gCubeVertexData[216] =
{
    // Data layout for each line below is:
    // positionX, positionY, positionZ,     normalX, normalY, normalZ,
    0.5f, -0.5f, -0.5f,        1.0f, 0.0f, 0.0f,
    0.5f, 0.5f, -0.5f,         1.0f, 0.0f, 0.0f,
    0.5f, -0.5f, 0.5f,         1.0f, 0.0f, 0.0f,
    0.5f, -0.5f, 0.5f,         1.0f, 0.0f, 0.0f,
    0.5f, 0.5f, -0.5f,          1.0f, 0.0f, 0.0f,
    0.5f, 0.5f, 0.5f,         1.0f, 0.0f, 0.0f,
    
    0.5f, 0.5f, -0.5f,         0.0f, 1.0f, 0.0f,
    -0.5f, 0.5f, -0.5f,        0.0f, 1.0f, 0.0f,
    0.5f, 0.5f, 0.5f,          0.0f, 1.0f, 0.0f,
    0.5f, 0.5f, 0.5f,          0.0f, 1.0f, 0.0f,
    -0.5f, 0.5f, -0.5f,        0.0f, 1.0f, 0.0f,
    -0.5f, 0.5f, 0.5f,         0.0f, 1.0f, 0.0f,
    
    -0.5f, 0.5f, -0.5f,        -1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,       -1.0f, 0.0f, 0.0f,
    -0.5f, 0.5f, 0.5f,         -1.0f, 0.0f, 0.0f,
    -0.5f, 0.5f, 0.5f,         -1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,       -1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, 0.5f,        -1.0f, 0.0f, 0.0f,
    
    -0.5f, -0.5f, -0.5f,       0.0f, -1.0f, 0.0f,
    0.5f, -0.5f, -0.5f,        0.0f, -1.0f, 0.0f,
    -0.5f, -0.5f, 0.5f,        0.0f, -1.0f, 0.0f,
    -0.5f, -0.5f, 0.5f,        0.0f, -1.0f, 0.0f,
    0.5f, -0.5f, -0.5f,        0.0f, -1.0f, 0.0f,
    0.5f, -0.5f, 0.5f,         0.0f, -1.0f, 0.0f,
    
    0.5f, 0.5f, 0.5f,          0.0f, 0.0f, 1.0f,
    -0.5f, 0.5f, 0.5f,         0.0f, 0.0f, 1.0f,
    0.5f, -0.5f, 0.5f,         0.0f, 0.0f, 1.0f,
    0.5f, -0.5f, 0.5f,         0.0f, 0.0f, 1.0f,
    -0.5f, 0.5f, 0.5f,         0.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, 0.5f,        0.0f, 0.0f, 1.0f,
    
    0.5f, -0.5f, -0.5f,        0.0f, 0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f,       0.0f, 0.0f, -1.0f,
    0.5f, 0.5f, -0.5f,         0.0f, 0.0f, -1.0f,
    0.5f, 0.5f, -0.5f,         0.0f, 0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f,       0.0f, 0.0f, -1.0f,
    -0.5f, 0.5f, -0.5f,        0.0f, 0.0f, -1.0f
};

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
        
        //Load obj.
        NSString *filePath = [[NSBundle mainBundle] pathForResource:@"cube" ofType:@"obj"];
        std::string err;
        tinyobj::LoadObj(_shapes, _materials, err, [filePath cStringUsingEncoding:NSUTF8StringEncoding]);
        std::cout << "shapes: " << _shapes.size() << std::endl;

        for (size_t v = 0; v < _shapes[0].mesh.positions.size() / 3; v++) {
            printf("  v[%ld] = (%f, %f, %f)\n", v,
                   _shapes[0].mesh.positions[3*v+0],
                   _shapes[0].mesh.positions[3*v+1],
                   _shapes[0].mesh.positions[3*v+2]);
        }
        
        for (size_t v = 0; v < _shapes[0].mesh.normals.size() / 3; v++) {
            printf("  vn[%ld] = (%f, %f, %f)\n", v,
                   _shapes[0].mesh.normals[3*v+0],
                   _shapes[0].mesh.normals[3*v+1],
                   _shapes[0].mesh.normals[3*v+2]);
        }
        
        //Init opengl.
        [self initOpenGL];
        
        unsigned long vertexDataSize =_shapes[0].mesh.positions.size() + _shapes[0].mesh.normals.size();
        
        GLfloat vertexData[vertexDataSize];
        
        //Prepare data.
        unsigned long numVertices = _shapes[0].mesh.positions.size() / 3;
        for (int v = 0; v < numVertices; v++) {
            
            int currentVertexPosStart = v * (VERTEX_POS_SIZE + VERTEX_NORMAL_SIZE);
            
            vertexData[currentVertexPosStart] = _shapes[0].mesh.positions[3 * v + 0];
            vertexData[currentVertexPosStart + 1] = _shapes[0].mesh.positions[3 * v + 1];
            vertexData[currentVertexPosStart + 2] = _shapes[0].mesh.positions[3 * v + 2];
            
            int currentVertexNormStart = v * (VERTEX_POS_SIZE + VERTEX_NORMAL_SIZE) + VERTEX_NORMAL_SIZE;
            
            vertexData[currentVertexNormStart] = _shapes[0].mesh.normals[3 * v + 0];
            vertexData[currentVertexNormStart + 1] = _shapes[0].mesh.normals[3 * v + 1];
            vertexData[currentVertexNormStart + 2] = _shapes[0].mesh.normals[3 * v + 2];
        }
        
        //Load opengl data.
        _stride = sizeof(GLfloat) * (VERTEX_POS_SIZE + VERTEX_NORMAL_SIZE);
        
        [self loadDataOpenGL:vertexData
              andNumVertices:(GLuint)_shapes[0].mesh.positions.size()/3
                   andStride:_stride
                  andIndices:_shapes[0].mesh.indices.data()
               andNumIndices:(GLuint)_shapes[0].mesh.indices.size()];
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
    _mvpLocation = glGetUniformLocation(_programObject, "mvpMatrix");
    _normalLocation = glGetUniformLocation(_programObject, "normalMatrix");
    
    // Free up no longer needed shader resources
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

#pragma mark Data OpenGL

- (void)loadDataOpenGL:(GLfloat *)verticesArray
        andNumVertices:(GLuint)numVertices
             andStride:(GLint)vertexStride
            andIndices:(GLuint *)indicesArray
         andNumIndices:(GLuint)numIndices {
    
    //Generate buffer objects: one for vertex data and one for vertex indices.
    glGenBuffers(2, _vboIds);
    
    //Start buffer for vertex data.
    glBindBuffer(GL_ARRAY_BUFFER, _vboIds[0]);
    glBufferData(GL_ARRAY_BUFFER, vertexStride * numVertices, verticesArray, GL_STATIC_DRAW);
    
    //start buffer for vertex indices.
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _vboIds[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * numIndices, indicesArray, GL_STATIC_DRAW);
}

#pragma mark Draw OpenGL

- (void)update {
    
    //Projection matrix.
    float aspect = fabs(self.view.bounds.size.width / self.view.bounds.size.height);
    GLKMatrix4 projectionMatrix = GLKMatrix4MakePerspective(GLKMathDegreesToRadians(65.0f), aspect, 0.1f, 20.0f);
    
    //Modelview matrix.
    GLKMatrix4 modelViewMatrix = GLKMatrix4MakeTranslation(0.0f, 0.0f, -5.0f);
    modelViewMatrix = GLKMatrix4Rotate(modelViewMatrix, 1.1, 0.0f, 1.0f, 0.0f);
    
    //Set inverse transpose matrix for normal.
    _normalMatrix = GLKMatrix4InvertAndTranspose(modelViewMatrix, NULL);
//    _normalMatrix = GLKMatrix3InvertAndTranspose(GLKMatrix4GetMatrix3(modelViewMatrix), NULL);

    //Set uniform modelviewprojection matrix.
    _mvpMatrix = GLKMatrix4Multiply(projectionMatrix, modelViewMatrix);
}

- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect {
    
    //Set states.
    glClear(GL_COLOR_BUFFER_BIT);
    
    //Install program.
    glUseProgram(_programObject);
    
    //Bind buffers.
    glBindBuffer(GL_ARRAY_BUFFER, _vboIds[0]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _vboIds[1]);
    
    //Enable vertex attribute.
    GLuint offset = VERTEX_POS_SIZE * sizeof(GLfloat);
    glEnableVertexAttribArray(VERTEX_POS_INDX);
    glEnableVertexAttribArray(VERTEX_NORMAL_INDX);
    glVertexAttribPointer(VERTEX_POS_INDX, VERTEX_POS_SIZE, GL_FLOAT, GL_FALSE, _stride, 0);
    glVertexAttribPointer(VERTEX_NORMAL_INDX, VERTEX_NORMAL_SIZE, GL_FLOAT, GL_FALSE, _stride, (const void *)offset);
    
    //Load uniforms.
    glUniformMatrix4fv(_mvpLocation, 1, GL_FALSE, (GLfloat *)_mvpMatrix.m);
    glUniformMatrix4fv(_normalLocation, 1, GL_FALSE, (GLfloat *)_normalMatrix.m);
    
    //Draw model.
    glDrawElements(GL_TRIANGLES, (GLuint)_shapes[0].mesh.indices.size(), GL_UNSIGNED_INT, 0);
    
    glDisableVertexAttribArray ( VERTEX_POS_INDX );
    glDisableVertexAttribArray ( VERTEX_NORMAL_INDX );
    
    glBindBuffer ( GL_ARRAY_BUFFER, 0 );
    glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, 0 );
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
