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

uniform vec3 viewPosition;
uniform directionalLight light;
uniform material surfaceMaterial;
uniform lowp sampler2DShadow shadowMapSampler;
uniform sampler2D textureSampler;
uniform int textureActive;

float shadow() {
    
    //Shadow calculate using PCF (percentage closer filtering).
    vec4 offset;
    float pixelSize = 0.00195; // 1.0/512.0
    float pixelSizeHomogenous = pixelSize * shadowCoord.w;
    float bias = -0.001 * shadowCoord.w;
    float shadowPercentage = 0.0;
    
    for(float x = -2.0; x <= 2.0; x += 2.0) {
        
        for(float y = -2.0; y <= 2.0; y += 2.0) {
            
            offset = vec4(x * pixelSizeHomogenous, y * pixelSizeHomogenous, bias, 0.0);
            shadowPercentage += textureProj(shadowMapSampler, shadowCoord + offset);
        }
    }
    
    shadowPercentage = shadowPercentage / 9.0;
    
    return shadowPercentage;
}

float orenNayar(vec3 lightDirection, vec3 viewDirection, vec3 normal) {
    
    float cosI = dot(lightDirection, normal);
    float cosO = dot(viewDirection, normal);
    
    float thetaI = acos(cosI);
    float thetaO = acos(cosO);
    
    float cosPhiDiff = length((lightDirection - normal * cosI)) * length((viewDirection - normal * cosO));
    
    float sigma = radians(45.0f);
    float sigmaSquared = pow(sigma, 2.0f);
    
    float A = 1.0f - (sigmaSquared / (2.0f * (sigmaSquared + 0.33f)));
    float B = (0.45f * sigmaSquared) / (sigmaSquared + 0.09f);
    
    float alpha = max(thetaI, thetaO);
    float beta = min(thetaI, thetaO);
    
    float orenNayarValue = A + B * max(0.0f, cosPhiDiff * sin(alpha) * tan(beta));
    
    return orenNayarValue;
}

void main() {

    //Calculate light direction and view direction.
    vec3 lightDirection = normalize(light.direction);
    vec3 viewDirection = normalize(viewPosition - vertPos);
        
    //Cosine theta diffuse lambertian component.
    float cosTheta = max(0.0, dot(normalInterp, normalize(lightDirection)));
    
    vec4 ambient = surfaceMaterial.ka * light.color;
    vec4 diffuse = vec4(0.0, 0.0, 0.0, 1.0);
    
    if(textureActive == 0) {
            
        //No texture. Standard lighting.
        diffuse =  surfaceMaterial.kd * light.color * orenNayar(lightDirection, viewDirection, normalize(normalInterp)) *  cosTheta;
    } else {
            
        //Lighting using texture.
        diffuse = surfaceMaterial.kd * texture(textureSampler, textureCoordinate) * orenNayar(lightDirection, viewDirection, normalInterp) * cosTheta;
    }
    
    o_fragColor = ambient + diffuse * shadow();
}
