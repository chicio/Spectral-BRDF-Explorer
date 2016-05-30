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

    struct vector3 {
        float x;
        float y;
        float z;
    };
    
    GLuint _programObject;
    GLuint _vboIds[2];
    GLint _stride;
    
    GLKMatrix4 _mvpMatrix;
    GLKMatrix4 _normalMatrix;
    GLint _mvpLocation;
    GLint _normalLocation;
    
    float _rotation;
    
    std::vector<float> vertexData;
    std::vector<unsigned int> vertexIndices, normalIndices;
    std::vector<vector3> vertices;
    std::vector<vector3> normals;
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

        FILE* file = fopen([filePath cStringUsingEncoding:NSUTF8StringEncoding], "r");
        
        if(file == NULL){
            
            printf("Impossible to open the file !\n");
        }
        
        while( 1 ){
            
            char lineHeader[128];
            // read the first word of the line
            int res = fscanf(file, "%s", lineHeader);
            
            if (res == EOF) {
                
                //End of file, exit parser.
                break;
            }
            
            if (strcmp(lineHeader, "v") == 0){
                
                struct vector3 vertex;
                fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z );
                vertices.push_back(vertex);
            }else if(strcmp(lineHeader, "vn") == 0){
                
                struct vector3 normal;
                fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z );
                normals.push_back(normal);
            }else if ( strcmp( lineHeader, "f" ) == 0 ){
                
                std::string vertex1, vertex2, vertex3;
                unsigned int vertexIndex[3], normalIndex[3];
                int matches = fscanf(file,
                                     "%d//%d %d//%d %d//%d\n",
                                     &vertexIndex[0],
                                     &normalIndex[0],
                                     &vertexIndex[1],
                                     &normalIndex[1],
                                     &vertexIndex[2],
                                     &normalIndex[2]);
                
                if (matches != 6){
                    printf("File can't be read by our simple parser : ( Try exporting with other options\n");
                }
                
                //Indices array.
                vertexIndices.push_back(vertexIndex[0]);
                vertexIndices.push_back(vertexIndex[1]);
                vertexIndices.push_back(vertexIndex[2]);
                normalIndices.push_back(normalIndex[0]);
                normalIndices.push_back(normalIndex[1]);
                normalIndices.push_back(normalIndex[2]);
                
                //Vertex data.
                for (int i = 0 ; i < 3; i++) {
                    
                    struct vector3 vertex = vertices[vertexIndex[i] - 1];
                    struct vector3 normal = normals[normalIndex[i] - 1];
                    
                    vertexData.push_back(vertex.x);
                    vertexData.push_back(vertex.y);
                    vertexData.push_back(vertex.z);
                    vertexData.push_back(normal.x);
                    vertexData.push_back(normal.y);
                    vertexData.push_back(normal.z);
                }
            }
        }
        
        //Init opengl.
        [self initOpenGL];
        
        //Load opengl data.
        _stride = sizeof(GLfloat) * (VERTEX_POS_SIZE + VERTEX_NORMAL_SIZE);
        int numberOfTotalVertex = ((int)vertexData.size() / (VERTEX_POS_SIZE + VERTEX_NORMAL_SIZE));
        int vertexDataSize = _stride * numberOfTotalVertex;
        int vertexIndicesSize = (int)vertexIndices.size() * sizeof(GLuint);
        
        [self loadDataOpenGL:vertexData.data()
            verticesDataSize:vertexDataSize
                   andStride:_stride
                  andIndices:vertexIndices.data()
              andIndicesSize:vertexIndicesSize];
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
    _mvpLocation = glGetUniformLocation(_programObject, "mvpMatrix");
    _normalLocation = glGetUniformLocation(_programObject, "normalMatrix");
    
    // Free up no longer needed shader resources
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

#pragma mark Data OpenGL

- (void)loadDataOpenGL:(GLfloat *)verticesArray
        verticesDataSize:(int)verticesSize
             andStride:(GLint)vertexStride
            andIndices:(GLuint *)indicesArray
        andIndicesSize:(int)indicesSize {
    
    //Generate buffer objects: one for vertex data and one for vertex indices.
    glGenBuffers(1, _vboIds);
    
    //Start buffer for vertex data.
    glBindBuffer(GL_ARRAY_BUFFER, _vboIds[0]);
    glBufferData(GL_ARRAY_BUFFER, verticesSize, verticesArray, GL_STATIC_DRAW);
}

#pragma mark Draw OpenGL

- (void)update {
    
    //Projection matrix.
    float aspect = fabs(self.view.bounds.size.width / self.view.bounds.size.height);
    GLKMatrix4 projectionMatrix = GLKMatrix4MakePerspective(GLKMathDegreesToRadians(65.0f), aspect, 0.1f, 10.0f);
    
    //Modelview matrix.
    GLKMatrix4 modelViewMatrix = GLKMatrix4MakeTranslation(0.0f, 0.0f, -5.0f);
    modelViewMatrix = GLKMatrix4Rotate(modelViewMatrix, _rotation, 0.0f, 1.0f, 0.0f);
    
    //Set inverse transpose matrix for normal.
    _normalMatrix = GLKMatrix4InvertAndTranspose(modelViewMatrix, NULL);

    //Set uniform modelviewprojection matrix.
    _mvpMatrix = GLKMatrix4Multiply(projectionMatrix, modelViewMatrix);
    
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
    GLuint offset = VERTEX_POS_SIZE * sizeof(GLfloat);
    glEnableVertexAttribArray(VERTEX_POS_INDX);
    glEnableVertexAttribArray(VERTEX_NORMAL_INDX);
    glVertexAttribPointer(VERTEX_POS_INDX, VERTEX_POS_SIZE, GL_FLOAT, GL_FALSE, _stride, 0);
    glVertexAttribPointer(VERTEX_NORMAL_INDX, VERTEX_NORMAL_SIZE, GL_FLOAT, GL_FALSE, _stride, (const void *)offset);
    
    //Load uniforms.
    glUniformMatrix4fv(_mvpLocation, 1, GL_FALSE, (GLfloat *)_mvpMatrix.m);
    glUniformMatrix4fv(_normalLocation, 1, GL_FALSE, (GLfloat *)_normalMatrix.m);
    
    //Draw model.
    glDrawArrays(GL_TRIANGLES, 0, ((int)vertexData.size() / (VERTEX_POS_SIZE + VERTEX_NORMAL_SIZE)));
    
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
