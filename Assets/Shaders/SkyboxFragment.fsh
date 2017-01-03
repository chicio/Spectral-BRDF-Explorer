#version 300 es

precision mediump float;

/// Texture coordinate (position obatined from vertex shader).
in vec3 textureCoordinate;

/// Fragment color.
out vec4 fragColor;

/// Skybox texture sampler.
uniform samplerCube skyboxSampler;

void main() {

    fragColor = texture(skyboxSampler, textureCoordinate);
}
