//
//  Vertex shader
//
//  Created by Fabrizio Duroni on 27/05/16.
//  Copyright © 2016 Fabrizio Duroni. All rights reserved.
//

#version 300 es

uniform mat4 mvpMatrix;
uniform mat4 normalMatrix;

layout(location = 0) in vec4 a_position;
layout(location = 1) in vec4 a_normal;

out vec4 v_color;

void main() {
    
    vec4 color = vec4(1.0, 0.0, 0.0, 1.0);
    
    vec4 eyeNormal = normalize(normalMatrix * a_normal);
    vec4 lightPosition = vec4(0.0, 0.0, 1.0, 0.0);
    float nDotVP = max(0.0, dot(eyeNormal, normalize(lightPosition)));
    
    vec4 ambient = color * 0.2;
    vec4 diffuse = color * nDotVP;
    
    //Output position and color for fragment shader.
    v_color = ambient + diffuse;
    gl_Position = mvpMatrix * a_position;
}
