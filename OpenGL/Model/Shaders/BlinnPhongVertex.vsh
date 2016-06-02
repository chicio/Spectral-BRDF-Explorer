//
//  Vertex shader
//
//  Created by Fabrizio Duroni on 27/05/16.
//  Copyright © 2016 Fabrizio Duroni. All rights reserved.
//

#version 300 es

layout(location = 0) in vec3 inputPosition;
layout(location = 1) in vec3 inputNormal;

uniform mat4 mvMatrix;
uniform mat4 mvpMatrix;
uniform mat4 normalMatrix;

out vec3 normalInterp;
out vec3 vertPos;

void main(){
    
    vec4 vertPosHomogenous = mvMatrix * vec4(inputPosition, 1.0);
    vertPos = vec3(vertPosHomogenous) / vertPosHomogenous.w;
    
    normalInterp = vec3(normalMatrix * vec4(inputNormal, 0.0));
    
    gl_Position = mvpMatrix * vec4(inputPosition, 1.0);
}
