#version 300 es

precision mediump float;

in vec3 textureCoordinate;

out vec4 o_fragColor;

uniform samplerCube skyboxSampler;

void main() {

    o_fragColor = texture(skyboxSampler, textureCoordinate);
}
