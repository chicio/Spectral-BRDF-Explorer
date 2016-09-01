//
//  Fragment shader
//
//  Created by Fabrizio Duroni on 27/05/16.
//  Copyright © 2016 Fabrizio Duroni. All rights reserved.
//

#version 300 es

precision mediump float;

struct material {
    vec4 ka;
    vec4 kd;
    vec4 ks;
    float sh;
};

struct pointLight {
    vec3 position;
    vec4 color;
};

in vec3 normalInterp;
in vec3 vertPos;
in vec2 textureCoordinate;
in vec4 shadowCoord;

out vec4 o_fragColor;

uniform pointLight light;
uniform material surfaceMaterial;
//uniform sampler2D textureSampler;
uniform int textureActive;
uniform lowp sampler2D shadowMapSampler;

//float lookup ( float x, float y )
//{
//    float pixelSize = 0.002; // 1/500
//    vec4 offset = vec4 ( x * pixelSize * shadowCoord.w,
//                        y * pixelSize * shadowCoord.w,
//                        0.0, 0.0 );
//    return textureProj ( shadowMapSampler, shadowCoord + offset );
//}

void main() {

//    float shadow = 1.0;
//    if (textureProj(shadowMapSampler, shadowCoord)  <  shadowCoord.z){
//        shadow = 0.5;
//    }
    
    // perform perspective divide
    vec3 projCoords = shadowCoord.xyz / shadowCoord.w;
    // Transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // Get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMapSampler, projCoords.xy).r;
    // Get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // Check whether current frag pos is in shadow
    float shadow = currentDepth < closestDepth + 0.001  ? 1.0 : 0.0;

//    // 3x3 kernel with 4 taps per sample, effectively 6x6 PCF
//    float shadow = 0.0;
//    float x, y;
//    for ( x = -2.0; x <= 2.0; x += 2.0 )
//        for ( y = -2.0; y <= 2.0; y += 2.0 )
//            shadow += lookup ( x, y );
//    
//    // divide sum by 9.0
//    shadow = shadow * 0.11;
    
    //Calculate light direction and view direction.
    vec3 viewPosition = vec3(0.0, 0.0, 1.0);
    vec3 lightDirection = normalize(light.position - vertPos);
    vec3 viewDirection = normalize(viewPosition - vertPos);
    
    //Cosine theta diffuse lambertian component.
    float cosTheta = max(0.0, dot(normalInterp, normalize(lightDirection)));
    
    vec4 ambient = surfaceMaterial.ka * light.color;
    vec4 diffuse = vec4(0.0, 0.0, 0.0, 1.0);
    vec4 specular = vec4(0.0, 0.0, 0.0, 1.0);
    
    if(cosTheta > 0.0) {
        
        //Reflection vector around normal
        vec3 reflectionDirection = reflect(-lightDirection, normalInterp);
        
        if(textureActive == 0) {
            
            //No texture. Standard lighting.
            diffuse = surfaceMaterial.kd * light.color * cosTheta;
        } else {
            
            //Lighting using texture.
            diffuse = surfaceMaterial.kd * cosTheta;
//            diffuse = surfaceMaterial.kd * texture(textureSampler, textureCoordinate) * cosTheta;
        }
        
        specular = surfaceMaterial.ks * light.color * pow(max(0.0, dot(reflectionDirection, normalInterp)), surfaceMaterial.sh);
    }
    
    //o_fragColor = vec4(shadow, 0,0,1);
    o_fragColor = ambient + (diffuse + specular) * shadow;
}
