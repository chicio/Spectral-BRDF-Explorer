#version 300 es

precision mediump float;

/**
 RGB material definition.
 */
struct RGBMaterial {
    
    /// Ambient component.
    vec4 ka;
    /// Diffuse component.
    vec4 kd;
    /// Specular component.
    vec4 ks;
    /// Shiness.
    float sh;
};

/**
 RGB light definition.
 It it composed of a direction and a color.
 */
struct DirectionalRGBLight {
    
    /// Light direction.
    vec3 direction;
    /// Light rgb color.
    vec4 color;
};

/// Normal (from vertex shader) interpolated per fragment.
in vec3 normalInterp;
/// Vertex position.
in vec3 vertPos;
/// Texture coordinated (If the 3D model has texture data).
in vec2 textureCoordinate;
/// Shadow coordinate (calculated using mvp from light position).
in vec4 shadowCoordinate;

/// Final fragment color.
out vec4 fragmentColor;

/// View position.
uniform vec3 viewPosition;
/// Light data.
uniform DirectionalRGBLight light;
/// Material data.
uniform RGBMaterial surfaceMaterial;
/// Shadow sampler.
uniform lowp sampler2DShadow shadowMapSampler;
/// Texture sampler.
uniform sampler2D textureSampler;
/// Flag used to check if the model has a texture.
uniform int textureActive;

/*************** Shadow ***************/

/**
 Calculate shadows using percentage closer filtering.
 Shadow texture has been obatined using shadow mapping
 technique (see OpenGLShadowMapProgram.cpp).
 
 @param shadowCoordinate the shadow coordinate on the shadow texture.
 
 @return percentage of shadow.
 */
float shadow(vec4 shadowCoordinate) {
    
    //Shadow calculate using PCF (percentage closer filtering).
    vec4 offset;
    float pixelSize = 0.00195; // 1.0/512.0
    float pixelSizeHomogenous = pixelSize * shadowCoordinate.w;
    float bias = -0.001 * shadowCoordinate.w;
    float shadowPercentage = 0.0;
    
    for (float x = -2.0; x <= 2.0; x += 2.0) {
        
        for (float y = -2.0; y <= 2.0; y += 2.0) {
            
            offset = vec4(x * pixelSizeHomogenous, y * pixelSizeHomogenous, bias, 0.0);
            shadowPercentage += textureProj(shadowMapSampler, shadowCoordinate + offset);
        }
    }
    
    shadowPercentage = shadowPercentage / 9.0;
    
    return shadowPercentage;
}

/*************** Main ***************/

void main() {
    
    //Calculate light direction and view direction.
    vec3 lightDirection = normalize(light.direction);
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
            diffuse = surfaceMaterial.kd * texture(textureSampler, textureCoordinate) * cosTheta;
        }
        
        specular = surfaceMaterial.ks * light.color * pow(max(0.0,dot(reflectionDirection, viewDirection)), surfaceMaterial.sh);
    }
    
    fragmentColor = ambient + (diffuse + specular) * shadow(shadowCoordinate);
}
