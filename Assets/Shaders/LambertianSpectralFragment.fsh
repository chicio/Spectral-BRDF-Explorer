#version 300 es

precision mediump float;

struct material {
    float ambientPercentage;
    float diffusePercentage;
    float specularPercentage;
    float sh;
};

struct directionalLight {
    vec3 direction;
};

in vec3 normalInterp;
in vec3 vertPos;
in vec2 textureCoordinate;
in vec4 shadowCoord;

out vec4 o_fragColor;

uniform vec3 viewPosition;
uniform directionalLight light;
uniform material surfaceMaterial;
uniform lowp sampler2DShadow shadowMapSampler;

float shadow() {
    
    //Shadow calculate using PCF (percentage closer filtering).
    vec4 offset;
    float pixelSize = 0.00195; // 1.0/512.0
    float pixelSizeHomogenous = pixelSize * shadowCoord.w;
    float bias = -0.001 * shadowCoord.w;
    float shadowPercentage = 0.0;
    
    for (float x = -2.0; x <= 2.0; x += 2.0) {
        
        for (float y = -2.0; y <= 2.0; y += 2.0) {
            
            offset = vec4(x * pixelSizeHomogenous, y * pixelSizeHomogenous, bias, 0.0);
            shadowPercentage += textureProj(shadowMapSampler, shadowCoord + offset);
        }
    }
    
    shadowPercentage = shadowPercentage / 9.0;
    
    return shadowPercentage;
}

void main() {
    
    //EXAMPLE TO BE REMOVED.
    float samples[8] = float[8](0.0, 0.3, 0.6, 1.0, 1.0, 1.0, 1.0, 1.0);
    
    vec4 color = vec4(samples[0], samples[1], samples[2], 0.0);
    
    //Calculate light direction and view direction.
    vec3 lightDirection = normalize(light.direction);
    vec3 viewDirection = normalize(viewPosition - vertPos);
    
    vec4 ambient = surfaceMaterial.ambientPercentage * color;
    float cosTheta = max(0.0, dot(normalInterp, normalize(lightDirection)));
    vec4 diffuse = surfaceMaterial.diffusePercentage * color * cosTheta;
    
    o_fragColor = ambient + diffuse * shadow();
}
