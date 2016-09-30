#version 300 es

layout(location = 0) in vec3 inputPosition;
layout(location = 1) in vec3 inputNormal;

uniform mat4 mvpMatrix;

out vec3 textureCoordinate;

void main() {
    
    gl_Position = mvpMatrix * vec4(inputPosition, 1.0);
    textureCoordinate = inputPosition;
}
