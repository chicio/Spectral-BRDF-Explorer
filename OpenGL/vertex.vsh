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
    vec3 position;
    vec4 color;
};

uniform mat4 mvMatrix;
uniform mat4 mvpMatrix;
uniform mat4 normalMatrix;
uniform pointLight light;
uniform material surfaceMaterial;

layout(location = 0) in vec3 inputPosition;
layout(location = 1) in vec3 inputNormal;

out vec4 v_color;

void main() {
    
    //Convert vertex and normal to camera space.
    vec3 normal = vec3(normalize(normalMatrix * vec4(inputNormal, 0.0)));
    vec4 vertexPositionHomogeneous = mvMatrix * vec4(inputPosition, 1.0);
    vec3 vertexPosition = vec3(vertexPositionHomogeneous) / vertexPositionHomogeneous.w;
    
    //Calculate light direction and view direction.
    vec3 lightDirection = normalize(light.position - vertexPosition);
    vec3 viewPosition = vec3(0.0, 0.0, 1.0);
    vec3 viewDirection = normalize(-1.0 *  vertexPosition);
    
    //Cosine theta diffuse lambertian component.
    float cosTheta = max(0.0, dot(normal, normalize(lightDirection)));
    
    //H vector for specular component.
    vec3 h = normalize(lightDirection + viewDirection);
    
    vec4 ambient = surfaceMaterial.ka * light.color;
    vec4 diffuse = surfaceMaterial.kd * light.color * cosTheta;
    vec4 specular = surfaceMaterial.ks * light.color * pow(max(0.0, dot(h, normal)), surfaceMaterial.sh);
    
    //Output position and color for fragment shader.
    v_color = specular;
    gl_Position = mvpMatrix * vec4(inputPosition, 1.0);
}





