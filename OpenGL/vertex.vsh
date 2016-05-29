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
    
    vec4 eyeNormal = normalize(normalMatrix * a_normal);
    vec4 lightPosition = vec4(0.0, 0.0, 1.0, 0.0);
    vec4 diffuseColor = vec4(1.0, 0.0, 0.0, 1.0);

    float nDotVP = max(0.0, dot(eyeNormal, normalize(lightPosition)));
    
    v_color = diffuseColor * nDotVP;
    gl_Position = mvpMatrix * a_position;
}
