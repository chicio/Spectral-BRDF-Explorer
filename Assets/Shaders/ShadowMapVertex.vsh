#version 300 es

/// Model input position.
layout(location = 0) in vec3 inputPosition;

/// Output color (not used).
out vec4 color;

/// Model view projection matrix from light position.
uniform mat4 mvpLightMatrix;

void main() {
    
    //Get position from light position point of view.
    gl_Position = mvpLightMatrix * vec4(inputPosition, 1.0);
}
