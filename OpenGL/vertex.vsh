//
//  Vertex shader
//
//  Created by Fabrizio Duroni on 27/05/16.
//  Copyright © 2016 Fabrizio Duroni. All rights reserved.
//

#version 300 es

struct material {
    vec4 ka;
    vec4 kd;
    vec4 ks;
    float sh;
};

struct pointLight {
    vec4 position;
    vec4 color;
};

uniform mat4 mvpMatrix;
uniform mat4 normalMatrix;
uniform pointLight light;
uniform material surfaceMaterial;

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 normal;

out vec4 v_color;

void main() {
    
    //Convert vertex and normal to camera space.
    vec4 eyeNormal = normalize(normalMatrix * normal);
    vec4 eyePosition = mvpMatrix * position;
    
    //Calculate light direction for vertex in camera space.
    vec4 eyeVertexLightDirection = normalize(light.position - eyePosition);
    
    //Cosine theta diffuse lambertian component.
    float cosTheta = max(0.0, dot(eyeNormal, normalize(light.position)));
    
    //H vector for specular component.
    vec4 viewPosition = vec4(0.0, 0.0, 1.0, 0.0);
    vec4 h = normalize(eyeVertexLightDirection + viewPosition);
    
    vec4 ambient = surfaceMaterial.ka * light.color;
    vec4 diffuse = surfaceMaterial.kd * light.color * cosTheta;
    vec4 specular = surfaceMaterial.ks * light.color * pow(max(0.0, dot(h, eyeNormal)), surfaceMaterial.sh);
    
    //Output position and color for fragment shader.
    v_color = ambient + diffuse + specular;
//    v_color = vec4(dot(h, eyeNormal), 0.0, 0.0, 0.0);
//    v_color = vec4(h.x, h.y, h.z, 1.0);
    gl_Position = eyePosition;
}
