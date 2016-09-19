#version 300 es

precision mediump float;

struct material {
    vec4 ka;
    vec4 kd;
    vec4 ks;
    float sh;
};

struct directionalLight {
    vec3 direction;
    vec4 color;
};

in vec3 normalInterp;
in vec3 vertPos;
in vec2 textureCoordinate;
in vec4 shadowCoord;

out vec4 o_fragColor;

uniform directionalLight light;
uniform material surfaceMaterial;
uniform lowp sampler2DShadow shadowMapSampler;
uniform sampler2D textureSampler;
uniform int textureActive;

float shadow() {
    
    //Shadow calculate using PCF (percentage closer filtering).
    vec4 offset;
    float pixelSize = 1.0/512.0;
    float pixelSizeHomogenous = pixelSize * shadowCoord.w;
    float bias = -0.001 * shadowCoord.w;
    float shadowPercentage = 0.0;
    
    for(float x = -2.0; x <= 2.0; x += 1.0) {
        
        for(float y = -2.0; y <= 2.0; y += 1.0) {
            
            offset = vec4(x * pixelSizeHomogenous, y * pixelSizeHomogenous, bias, 0.0);
            shadowPercentage += textureProj(shadowMapSampler, shadowCoord + offset);
        }
    }
    
    shadowPercentage = shadowPercentage / 16.0;
    
    return shadowPercentage;
}

float cookTorrance(vec3 lightDirection, vec3 viewDirection, vec3 normal) {
    
    //Input.
    float roughness = 0.2;
    float fresnel = pow((1.0 - 1.180) / (1.0 + 1.180), 2.0);
    
    vec3 halfVector = normalize(lightDirection + viewDirection);
    
    float nDotV = max(0.0, dot(normal, viewDirection));
    float nDotL = max(0.0, dot(normal, lightDirection));
    float vDotH = max(0.0, dot(viewDirection, halfVector));
    float lDotH = max(0.0, dot(lightDirection, halfVector));
    
    //D term.
    float nDotH = max(0.0, dot(halfVector, normal));
    float cos2Alpha = pow(nDotH, 2.0);
    float tan2Alpha = (cos2Alpha - 1.0) / cos2Alpha; //already * -1
    float D = exp(tan2Alpha / pow(roughness, 2.0)) / (pow(roughness, 2.0) * pow(nDotH, 4.0));
    
    //G term.
    float g1 = (2.0 * nDotH * nDotV) / vDotH;
    float g2 = (2.0 * nDotH * nDotL) / lDotH;
    float G = min(1.0, min(g1, g2));
    
    //F term.
    float F = fresnel + (1.0 - fresnel) * pow(vDotH, 5.0);
    
    float cookTorranceValue = (D * G * F) / (4.0 * nDotV * nDotL);
 
    return cookTorranceValue;
}

void main() {
    
    //Calculate light direction and view direction.
    vec3 viewPosition = vec3(0.0, 0.0, 1.0);
    vec3 lightDirection = normalize(light.direction);
    vec3 viewDirection = normalize(viewPosition - vertPos);
    
    //Cosine theta diffuse lambertian component.
    float cosTheta = max(0.0, dot(normalInterp, normalize(lightDirection)));
    
    vec4 ambient = surfaceMaterial.ka * light.color;
    vec4 diffuse = vec4(0.0, 0.0, 0.0, 1.0);
    vec4 specular = vec4(0.0, 0.0, 0.0, 1.0);

    if(cosTheta > 0.0) {
        
        if(textureActive == 0) {
            
            //No texture. Standard lighting.
            diffuse = surfaceMaterial.kd * light.color * cosTheta;
        } else {
            
            //Lighting using texture.
            diffuse = surfaceMaterial.kd * texture(textureSampler, textureCoordinate) * cosTheta;
        }
        
        specular = surfaceMaterial.ks * light.color * cookTorrance(lightDirection, viewDirection, normalInterp);
    }
    
    o_fragColor = ambient + (diffuse + specular) * shadow();
}
