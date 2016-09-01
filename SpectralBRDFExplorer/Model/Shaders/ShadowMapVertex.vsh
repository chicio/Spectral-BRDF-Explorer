//
//  Shadow Mapping Vertex shader
//
//  Created by Fabrizio Duroni on 29/08/16.
//  Copyright © 2016 Fabrizio Duroni. All rights reserved.
//

#version 300 es

layout(location = 0) in vec3 inputPosition;

out vec4 v_color;

uniform mat4 mvpLightMatrix;

void main() {
    
    gl_Position = mvpLightMatrix * vec4(inputPosition, 1.0);
}
