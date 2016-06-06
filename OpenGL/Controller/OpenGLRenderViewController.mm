//
//  GLViewController.m
//  OpenGL
//
//  Created by Fabrizio Duroni on 27/05/16.
//  Copyright © 2016 Fabrizio Duroni. All rights reserved.
//

#include <vector>
#include <iostream>

#include "OpenGLShader.hpp"
#include "OpenGLProgram.hpp"
#include "OpenGLObjParser.hpp"
#include "OpenGLRenderer.hpp"

#import "OpenGLRenderViewController.h"

@interface OpenGLRenderViewController () {
    
    OpenGLRenderer openGLRenderer;
}

@property (strong, nonatomic) EAGLContext *context;
@end

@implementation OpenGLRenderViewController

#pragma mark Apple View Lifecycle

- (void)viewDidLoad {
    
    [super viewDidLoad];
    
    //Set title.
    self.title = [NSString stringWithUTF8String:Scene::instance().model.getName().c_str()];
    
    //Create context.
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
        
        self.preferredFramesPerSecond = 60;
        
        GLKView *view = (GLKView *)self.view;
        view.context = self.context;
        view.drawableDepthFormat = GLKViewDrawableDepthFormat24;
        
        //Init opengl.
        NSString* currentLighting = [NSString stringWithUTF8String:Scene::instance().lighting.c_str()];
        NSString* vertexShaderName = [NSString stringWithFormat:@"%@Vertex", currentLighting];
        NSString* fragmentShaderName = [NSString stringWithFormat:@"%@Fragment", currentLighting];
        const char* vertexShaderSource = [self shaderSource:vertexShaderName andExtension:@"vsh"];
        const char* fragmentShaderSource = [self shaderSource:fragmentShaderName andExtension:@"fsh"];
        
        std::string error;
        bool rendererStarted = openGLRenderer.startRenderer(vertexShaderSource, fragmentShaderSource, error);
        
        if(rendererStarted) {
            
            openGLRenderer.loadScene();
        } else {
            
            //Failed to load OpenGL ES 3 context.
            UIAlertController* alert = [UIAlertController
                                        alertControllerWithTitle:@"OpenGL"
                                        message:[NSString stringWithCString:error.c_str() encoding:NSUTF8StringEncoding]
                                        preferredStyle:UIAlertControllerStyleAlert];
            
            [self presentViewController:alert animated:YES completion:nil];
        }
    }
}

#pragma mark Draw OpenGL

- (void)update {
    
    //Update modelview and projection matrix if needed.
    openGLRenderer.update(self.view.bounds.size.width, self.view.bounds.size.height, self.timeSinceLastUpdate);
}

- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect {
    
    //Draw current model using renderer created.
    openGLRenderer.draw();
}

#pragma mark Shader OpenGL

- (const char *)shaderSource:(NSString *)shaderName andExtension:(NSString *)shaderExtension {
    
    NSString *shaderPath = [[NSBundle mainBundle] pathForResource:shaderName ofType:shaderExtension];
    const char *shaderSource = [[NSString stringWithContentsOfFile:shaderPath
                                                          encoding:NSUTF8StringEncoding
                                                             error:nil] UTF8String];
    return shaderSource;
}

#pragma mark Terminate OpenGL

- (void)tearDownGL {
    
    [EAGLContext setCurrentContext:self.context];

    //Shutdown renderer.
    openGLRenderer.shutdown();
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
