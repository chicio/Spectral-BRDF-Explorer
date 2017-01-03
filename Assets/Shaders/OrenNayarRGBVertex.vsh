#version 300 es

/// Model input position.
layout(location = 0) in vec3 inputPosition;
/// Model input normal.
layout(location = 1) in vec3 inputNormal;
/// Model input texture coordinate.
layout(location = 2) in vec2 inputTextureCoordinate;

/// Normal * normalMatrix.
out vec3 normalInterp;
/// Vertex positition in world coordinate.
out vec3 vertPos;
/// Texture coordinate.
out vec2 textureCoordinate;
/// Shadow coordinate obtained using model view projection matrix from light position.
out vec4 shadowCoordinate;

/// Model view matrix.
uniform mat4 mvMatrix;
/// Model view projection matrix.
uniform mat4 mvpMatrix;
/// Model view projection matrix from light position.
uniform mat4 mvpLightMatrix;
/// Normal matrix (inverse of model view projection).
uniform mat4 normalMatrix;

void main() {
    
    //Position after transforms.
    gl_Position = mvpMatrix * vec4(inputPosition, 1.0);
    
    //World position after transform (no projection).
    vec4 vertPosWorldCoordinate = mvMatrix * vec4(inputPosition, 1.0);
    vertPos = vec3(vertPosWorldCoordinate);
    
    //Normal product with matrix.
    normalInterp = vec3(normalMatrix * vec4(inputNormal, 0.0));
    
    //Calculate shadow coordinate.
    shadowCoordinate = mvpLightMatrix * vec4(inputPosition, 1.0);
    shadowCoordinate = shadowCoordinate * 0.5 + 0.5;
    
    //Output coordinate.
    textureCoordinate = inputTextureCoordinate;
}
