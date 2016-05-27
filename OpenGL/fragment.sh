//
//  Fragment shader
//
//  Created by Fabrizio Duroni on 27/05/16.
//  Copyright © 2016 Fabrizio Duroni. All rights reserved.
//

#version 300 es                                

in vec4 v_color;

layout(location = 0) out vec4 outColor;

void main() {
    
  outColor = v_color;
}