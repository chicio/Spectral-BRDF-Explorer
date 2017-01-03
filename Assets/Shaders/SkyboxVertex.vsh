#version 300 es

/// Model input position.
layout(location = 0) in vec3 inputPosition;
/// Model input normal.
layout(location = 1) in vec3 inputNormal;

/// Texture coordinate to be used in fragment shader.
out vec3 textureCoordinate;

/// Model view projection matrix.
uniform mat4 mvpMatrix;

void main() {
    
    //Get position of the skybox model from point of view.
    gl_Position = mvpMatrix * vec4(inputPosition, 1.0);
    
    //Use texture coordinate as position.
    textureCoordinate = inputPosition;
}
