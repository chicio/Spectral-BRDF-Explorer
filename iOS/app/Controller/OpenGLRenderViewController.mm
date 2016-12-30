//
//  GLViewController.m
//  SpectralBRDFExplorer
//
//  Created by Fabrizio Duroni on 27/05/16.
//  
//

#include <vector>
#include <iostream>

#include "OpenGLShader.hpp"
#include "OpenGLProgram.hpp"
#include "OpenGLRenderer.hpp"

#import "OpenGLRenderViewController.h"

@interface OpenGLRenderViewController () {
    
    /// OpenGL ES Renderer.
    OpenGLRenderer openGLRenderer;
}

/// Pinch gesture recognizer.
@property (strong, nonatomic) UIPinchGestureRecognizer *pinchRecognizer;
/// Pan gesture recognizer.
@property (strong, nonatomic) UIPanGestureRecognizer *panRecognizer;
/// custom apple OpenGL ES context.
@property (strong, nonatomic) EAGLContext *context;

@end

@implementation OpenGLRenderViewController

#pragma mark Apple View Lifecycle

- (void)viewDidLoad {
    
    [super viewDidLoad];
    
    //Set title.
    self.title = @"Spectral BRDF Explorer";
    
    //Create context.
    self.context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES3];
    
    if (!self.context) {
        
        //Failed to load OpenGL ES 3 context.
        UIAlertController* alert = [UIAlertController
                                    alertControllerWithTitle:@"OpenGL"
                                    message:@"OpenGL ES 3 not available"
                                    preferredStyle:UIAlertControllerStyleAlert];
        
        [self presentViewController:alert animated:YES completion:nil];
    }
    
    if ([EAGLContext setCurrentContext:self.context]) {
        
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
        bool rendererStarted = openGLRenderer.start(OpenGLCamera(glm::vec3(0.0f, 7.0f, 1.0f),
                                                                 glm::vec3(0.0f, 0.0f, -12.0f),
                                                                 glm::vec3(0.0f, 1.0f, 0.0f)),
                                                            error);
        
        if (!rendererStarted) {
            
            //Shutdown OpenGL.
            openGLRenderer.shutdown();
            
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

    //Update camera.
    openGLRenderer.openGLCamera.updateCamera();
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
    
    //Update camera.
    openGLRenderer.openGLCamera.updateCamera();
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
