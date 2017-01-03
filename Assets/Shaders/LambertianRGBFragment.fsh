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

/*************** Texture ***************/

/**
 Procedural checker texture with antialiasing.
 
 Classical checker texture. Antialiasing is obtained 
 estimating the average value of the texture over an 
 area covered by the pixel. First of all the rate of 
 change of the texture over adjacent pixels (gradient
 vector). Its magnitude is used as filter width to
 decide if we need antialiasing.
 
 @see "Advanced RenderMan: Creating CGI for Motion Pictures" http://dl.acm.org/citation.cfm?id=555371
 @see "OpenGL ES 3.0 Programming Guide" http://opengles-book.com
 @see "OpenGL ES 3.0 Programming Guide" source code https://github.com/danginsburg/opengles3-book
 
 @param textureCoordinate texture coordinate.
 @param color0 first color of the checker.
 @param color1 second color of the checker.
 @param frequency the frequency of the checker.
 
 @returns the color of the checker texture.
 */
vec4 proceduralTextureChecker(vec2 textureCoordinate, vec4 color0, vec4 color1, float frequency) {
    
    vec4 color;
    
    vec2 textureFilterWidth = fwidth(textureCoordinate);
    vec2 fuzz = textureFilterWidth * float(frequency) * 2.0;
    vec2 check_pos = fract(textureCoordinate * float(frequency));
    float fuzz_max = max(fuzz.s, fuzz.t);
    
    if(fuzz_max <= 0.5) {
        
        vec2 p = smoothstep(vec2(0.5), fuzz + vec2(0.5), check_pos) + (1.0 - smoothstep(vec2(0.0), fuzz, check_pos));
        color = mix(color0, color1, p.x * p.y + (1.0 - p.x) * (1.0 - p.y));
        color = mix(color, (color0 + color1) / 2.0, smoothstep(0.125, 0.5, fuzz_max));
    } else {

        color = (color0 + color1) / 2.0;
    }
    
    return color;
}

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

    vec4 textureColor;
    vec4 ambient;
    vec4 diffuse;
    
    if(textureActive == 0) {
        
        ambient = surfaceMaterial.ka * light.color;
    } else {

        textureColor = proceduralTextureChecker(textureCoordinate,
                                                vec4(0.502, 0.0, 0.117, 1.0),
                                                vec4(0.9, 0.9, 0.9, 1.0),
                                                float(10));
        ambient = surfaceMaterial.ka * textureColor;
    }
    
    //Cosine theta diffuse lambertian component.
    float cosTheta = max(0.0, dot(normalInterp, normalize(lightDirection)));
    
    if(textureActive == 0) {

        //No texture. Standard lighting.
        diffuse = surfaceMaterial.kd * light.color * cosTheta;
    } else {
            
        //Lighting using texture.
        diffuse = surfaceMaterial.kd * textureColor * cosTheta;
    }
    
    fragmentColor = ambient + diffuse * shadow(shadowCoordinate);
}
