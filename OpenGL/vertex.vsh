//
//  Vertex shader
//
//  Created by Fabrizio Duroni on 27/05/16.
//  Copyright © 2016 Fabrizio Duroni. All rights reserved.
//

#version 300 es

uniform mat4 u_mvpMatrix;

layout(location = 0) in vec4 a_position;

out vec4 v_color;

void main() {
    
   v_color = vec4(1.0, 0.0, 0.0, 1.0);
    gl_Position = u_mvpMatrix * a_position;
}