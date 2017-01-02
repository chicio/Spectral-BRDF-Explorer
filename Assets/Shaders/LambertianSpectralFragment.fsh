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

/// Spectrum samples.
const int samples = 31;

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

    //Calculate light direction and view direction.
    vec3 lightDirection = normalize(light.direction);
    vec3 viewDirection = normalize(viewPosition - vertPos);
    float cosTheta = max(0.0, dot(normalInterp, normalize(lightDirection)));
    float shadowPercentage = shadow();
    
    /****** Spectrum *******/
    ///////Spectrum  calculation
    
//    ///Illuminant A
//    float illuminant[samples] = float[samples](
//        14.708000, 17.675300, 20.995000, 24.670900, 28.702700, 33.085900, 37.812100, 42.869300, 48.242300, 53.913200,
//        59.861100, 66.063500, 72.495900, 79.132600, 85.947000, 92.912000, 100.000000, 107.184000, 114.436000, 121.731000,
//        129.043000, 136.346000, 143.618000, 150.836000, 157.979000, 165.028000, 171.963000, 178.769000, 185.429000, 191.931000,
//        198.261000
//    );
    
    ///illuminant D65
    float illuminant[samples] = float[samples](
        82.754900,91.486000,93.431800,86.682300,104.865000,117.008000,117.812000,114.861000,115.923000,108.811000,109.354000,
        107.802000,104.790000,107.689000,104.405000,104.046000,100.000000,96.334200,95.788000,88.685600,90.006200,89.599100,
        87.698700,83.288600,83.699200,80.026800,80.214600,82.277800,78.284200,69.721300,71.609100
    );
    
    float object[samples] = float[samples](
        0.051, 0.05, 0.049, 0.049, 0.049, 0.049, 0.048, 0.047, 0.045, 0.044,
        0.044, 0.044, 0.044, 0.044, 0.045, 0.047, 0.05, 0.057, 0.072, 0.109,
        0.192, 0.332, 0.486, 0.598, 0.654, 0.686, 0.7, 0.707, 0.718, 0.724,
        0.729
    );
    
    float objectIlluminantSpectrum[samples];
    float spectrumAmbient[samples];
    float spectrumDiffuse[samples];
    float resultSpectrum[samples];
    
    for (int i = 0; i < samples; i++) {
        
        objectIlluminantSpectrum[i] = illuminant[i] * object[i];
        spectrumAmbient[i] = objectIlluminantSpectrum[i] * surfaceMaterial.ambientPercentage;
        spectrumDiffuse[i] = objectIlluminantSpectrum[i] * surfaceMaterial.diffusePercentage * cosTheta * shadowPercentage;
        resultSpectrum[i] = spectrumAmbient[i] + spectrumDiffuse[i];
    }
    
    ///////Tristimulus conversion.
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
    
    //Tristimulus values.
    float X = 0.0f;
    float Y = 0.0f;
    float Z = 0.0f;
    
    //Normalizing constant.
    float luminanceSum = 0.0f;

    //Integration approximation.
    for (int i = 0; i < samples; i++) {
        
        X += resultSpectrum[i] * x[i];
        Y += resultSpectrum[i] * y[i];
        Z += resultSpectrum[i] * z[i];
        luminanceSum += illuminant[i] * y[i];
    }
    
    //Scale to Y.
    //Usually k is chosen to give a value of 100 for luminance Y of samples with a unit reflectance spectrum.
    float k = 1.0f/luminanceSum;
    
    X = X * k;
    Y = Y * k;
    Z = Z * k;
    
    vec3 tristimulus = vec3(X, Y, Z);
    
    ////RGB conversion.
    float r =  3.240479f * tristimulus.x - 1.537150f * tristimulus.y - 0.498535f * tristimulus.z;
    float g = -0.969256f * tristimulus.x + 1.875991f * tristimulus.y + 0.041556f * tristimulus.z;
    float b =  0.055648f * tristimulus.x - 0.204043f * tristimulus.y + 1.057311f * tristimulus.z;

    vec4 rgbColor = vec4(r, g, b, 1.0);
    /*****************/

    o_fragColor = rgbColor;
}
