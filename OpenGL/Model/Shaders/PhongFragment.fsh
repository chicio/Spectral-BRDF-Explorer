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
out vec4 o_fragColor;

uniform pointLight light;
uniform material surfaceMaterial;

void main() {

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
        
        diffuse = surfaceMaterial.kd * light.color * cosTheta;
        specular = surfaceMaterial.ks * light.color * pow(max(0.0, dot(reflectionDirection, normalInterp)), surfaceMaterial.sh);
    }
    
    o_fragColor = ambient + diffuse + specular;
}
