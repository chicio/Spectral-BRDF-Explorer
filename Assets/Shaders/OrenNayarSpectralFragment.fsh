#version 300 es

precision mediump float;

/// Spectrum samples.
const int samples = 31;

/**
 Spectral material definition.
 Uses percentage to define the quantity spectrum
 of spectrum used for lighting.
 */
struct SpectralMaterial {
    
    /// Material spectrum.
    float spectrum[samples];
    /// Ambient percentage.
    float ambientPercentage;
    /// Diffuse percentage.
    float diffusePercentage;
    /// Specular percentage.
    float specularPercentage;
    /// Shiness.
    float sh;
};

/**
 Spectral light definition.
 It it composed of a direction and a spectrum.
 */
struct DirectionalSpectralLight {
    
    /// Light direction.
    vec3 direction;
    /// Light (illuminant) spectrum.
    float spectrum[samples];
};

/**
 Color matching function.
 Used in tristimulus conversion.
 */
struct ColorMatchingFunction {
    
    /// X component.
    float x[samples];
    /// Y component.
    float y[samples];
    /// Z component.
    float z[samples];
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
uniform DirectionalSpectralLight light;
/// Material data.
uniform SpectralMaterial surfaceMaterial;
/// Shadow sampler.
uniform lowp sampler2DShadow shadowMapSampler;

/*************** Spectrum ***************/

/**
 Calculate result spectrum of the object using spectral data
 for illuminant and object surface using the lighting model of
 the shader.
 
 @param light spectral light.
 @param surfaceMaterial surface material.
 @param cosTheta attenuation factor.
 @param shadowPercentage the shadow percentage.
 @param phongModelComponent phong specular component.
 */
float[samples] calculateSpectrum(DirectionalSpectralLight light,
                                 SpectralMaterial surfaceMaterial,
                                 float cosTheta,
                                 float shadowPercentage,
                                 float orenNayarComponent) {
    
    float objectIlluminantSpectrum[samples];
    float spectrumAmbient[samples];
    float spectrumDiffuse[samples];
    float resultSpectrum[samples];
    
    float diffuseComponent = orenNayarComponent * surfaceMaterial.diffusePercentage;
    
    for (int i = 0; i < samples; i++) {
        
        objectIlluminantSpectrum[i] = light.spectrum[i] * surfaceMaterial.spectrum[i];
        spectrumAmbient[i] = objectIlluminantSpectrum[i] * surfaceMaterial.ambientPercentage;
        spectrumDiffuse[i] = objectIlluminantSpectrum[i] * diffuseComponent * cosTheta * shadowPercentage;
        resultSpectrum[i] = spectrumAmbient[i] + spectrumDiffuse[i];
    }
    
    return resultSpectrum;
}

/*************** Tristimulus ***************/

/**
 Create a color matching function using 2 standard observer.
 This data is used in tristimulus conversion.
 
 @returns a ColorMatchingFunction struct with 2 observer data.
 */
ColorMatchingFunction standardObserver2() {
    
    float x[samples] = float[samples](
        0.014310,0.043510,0.134380,0.283900,0.348280,0.336200,0.290800,0.195360,0.095640,0.032010,0.004900,0.009300,
        0.063270,0.165500,0.290400,0.433450,0.594500,0.762100,0.916300,1.026300,1.062200,1.002600,0.854450,0.642400,
        0.447900,0.283500,0.164900,0.087400,0.046770,0.022700,0.011359
    );
    
    float y[samples] = float[samples](
        0.000396,0.001210,0.004000,0.011600,0.023000,0.038000,0.060000,0.090980,0.139020,0.208020,0.323000,0.503000,
        0.710000,0.862000,0.954000,0.994950,0.995000,0.952000,0.870000,0.757000,0.631000,0.503000,0.381000,0.265000,
        0.175000,0.107000,0.061000,0.032000,0.017000,0.008210,0.004102
    );
    
    float z[samples] = float[samples](
        0.067850,0.207400,0.645600,1.385600,1.747060,1.772110,1.669200,1.287640,0.812950,0.465180,0.272000,0.158200,
        0.078250,0.042160,0.020300,0.008750,0.003900,0.002100,0.001650,0.001100,0.000800,0.000340,0.000190,0.000050,
        0.000020,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000
    );
    
    ColorMatchingFunction cmf = ColorMatchingFunction(x, y, z);
    
    return cmf;
}

/**
 Convert spectrum to CIE 1931 XYZ tristimulus values.
 
 @param result object * illuminant with lighting model applied.
 @param light spectral light.
 
 @returns vec3 with tristimulus components.
 */
vec3 tristimulus(float resultSpectrum[samples], DirectionalSpectralLight light) {
    
    ColorMatchingFunction cmfStdObserver2 = standardObserver2();
    
    //Tristimulus values.
    float X = 0.0f;
    float Y = 0.0f;
    float Z = 0.0f;
    
    //Normalizing constant.
    float luminanceSum = 0.0f;
    
    //Integration approximation (Riemann).
    for (int i = 0; i < samples; i++) {
        
        X += resultSpectrum[i] * cmfStdObserver2.x[i];
        Y += resultSpectrum[i] * cmfStdObserver2.y[i];
        Z += resultSpectrum[i] * cmfStdObserver2.z[i];
        luminanceSum += light.spectrum[i] * cmfStdObserver2.y[i];
    }
    
    float k = 1.0f / luminanceSum;
    
    X = X * k;
    Y = Y * k;
    Z = Z * k;
    
    vec3 tristimulus = vec3(X, Y, Z);
    
    return tristimulus;
}

/**
 Convert tristimulus to sRGB color space.
 
 @param tristimulus the tristimulus values.
 
 @returns vec4 with sRGB values.
 */
vec4 tristimulusTosRGB(vec3 tristimulus) {
    
    float r =  3.240479f * tristimulus.x - 1.537150f * tristimulus.y - 0.498535f * tristimulus.z;
    float g = -0.969256f * tristimulus.x + 1.875991f * tristimulus.y + 0.041556f * tristimulus.z;
    float b =  0.055648f * tristimulus.x - 0.204043f * tristimulus.y + 1.057311f * tristimulus.z;
    
    vec4 rgbColor = vec4(r, g, b, 1.0);
    
    return rgbColor;
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

/*************** Oren-Nayar lighting model ***************/

/**
 Oren Nayar lighting model.
 
 @param lightDirection.
 @param viewDirection.
 @param normal.
 
 @returns Oren-Nayar diffuse model.
 */
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

/*************** Main ***************/

void main() {

    //Calculate light direction and view direction.
    vec3 lightDirection = normalize(light.direction);
    vec3 viewDirection = normalize(viewPosition - vertPos);
    //Attenuation factor (lambertian).
    float cosTheta = max(0.0, dot(normalInterp, normalize(lightDirection)));
    //Shadow percentage.
    float shadowPercentage = shadow(shadowCoordinate);
    
    //Apply cook torrance model for specular component.
    float orenNayarDiffuseComponent = orenNayar(lightDirection, viewDirection, normalInterp);
    
    //Calculate spectrum.
    float result[samples] = calculateSpectrum(light,
                                              surfaceMaterial,
                                              cosTheta,
                                              shadowPercentage,
                                              orenNayarDiffuseComponent);
    
    //Convert to tristimulus.
    vec3 tristimulus = tristimulus(result, light);
    
    //Convert to sRGB.
    fragmentColor = tristimulusTosRGB(tristimulus);
}
