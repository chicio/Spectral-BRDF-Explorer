//
//  GLViewController.m
//  SpectralBRDFExplorer
//
//  Created by Fabrizio Duroni on 27/05/16.
//  Copyright © 2016 Fabrizio Duroni. All rights reserved.
//

#include <vector>
#include <iostream>

#include "OpenGLShader.hpp"
#include "OpenGLProgram.hpp"
#include "OpenGLRenderer.hpp"

#import "OpenGLRenderViewController.h"

@interface OpenGLRenderViewController () {
    
    /// Renderer.
    OpenGLRenderer openGLRenderer;
}

@property (strong, nonatomic) UIPinchGestureRecognizer *pinchRecognizer;
@property (strong, nonatomic) UIPanGestureRecognizer *panRecognizer;
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
        
        self.preferredFramesPerSecond = 30;
        
        GLKView *view = (GLKView *)self.view;
        view.context = self.context;
        view.drawableDepthFormat = GLKViewDrawableDepthFormat24;
        view.userInteractionEnabled = YES;
        
        //Setup gestures.
        [self setupGestures];
        
        //Init opengl.
        std::string error;
        
        //Start renderer.
        bool rendererStarted = openGLRenderer.startRenderer(OpenGLCamera(glm::vec3(0.0f, 1.0f, 1.0f),
                                                                         glm::vec3(0.0f, 0.0f, -5.0f),
                                                                         glm::vec3(0.0f, 1.0f, 0.0f)),
                                                            error);
        
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

#pragma mark Gesture Recognizer

- (void)setupGestures {
 
    self.panRecognizer = [[UIPanGestureRecognizer alloc] initWithTarget:self action:@selector(pan:)];
    self.pinchRecognizer = [[UIPinchGestureRecognizer alloc] initWithTarget:self action:@selector(pinch:)];
    
    [self.view addGestureRecognizer:self.panRecognizer];
    [self.view addGestureRecognizer:self.pinchRecognizer];
}

- (void)pan:(UIPanGestureRecognizer *)recognizer {

    CGPoint rotation = [recognizer translationInView:recognizer.view];
    
    openGLRenderer.openGLCamera.setRotationFactors(rotation.x * -0.02, rotation.y * 0.01);
    
    NSLog(@"rotation %f %f", rotation.x, rotation.y);
}

- (void)pinch:(UIPinchGestureRecognizer *)recognizer {
    
    float scale = [recognizer scale];
    
    if(scale < 1.0) {
        
        //if < 1.0 zoom out.
        openGLRenderer.openGLCamera.setZoomFactor(scale * -0.30f);
    } else {
        
        //if > 1.0 zoom in.
        openGLRenderer.openGLCamera.setZoomFactor(scale * 0.08f);
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
