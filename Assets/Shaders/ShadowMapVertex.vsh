#version 300 es

layout(location = 0) in vec3 inputPosition;

out vec4 v_color;

uniform mat4 mvpLightMatrix;

void main() {
    
    gl_Position = mvpLightMatrix * vec4(inputPosition, 1.0);
}
