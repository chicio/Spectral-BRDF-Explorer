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
const int samples = 81;

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

    //EXAMPLE TO BE REMOVED.
    float testArray[8] = float[8](0.0, 0.3, 0.6, 1.0, 1.0, 1.0, 1.0, 1.0);
    vec4 color = vec4(testArray[0], testArray[1], testArray[2], 0.0);
    
    //Calculate light direction and view direction.
    vec3 lightDirection = normalize(light.direction);
    vec3 viewDirection = normalize(viewPosition - vertPos);
    float cosTheta = max(0.0, dot(normalInterp, normalize(lightDirection)));
    float shadowPercentage = shadow();
    
    /****** Spectrum *******/
    ///////Spectrum  calculation
    float illuminant[samples] = float[samples](
        49.9755, 52.3118, 54.6482, 68.7015, 82.7549, 87.1204,  91.486, 92.4589, 93.4318,
        90.057 , 86.6823, 95.7736, 104.865, 110.936, 117.008,  117.41, 117.812, 116.336, 114.861, 115.392,
        115.923, 112.367, 108.811, 109.082, 109.354, 108.578, 107.802, 106.296,  104.79, 106.239,107.689,
        106.047, 104.405, 104.225, 104.046, 102.023,   100.0, 98.1671, 96.3342, 96.0611, 95.788, 92.2368,
        88.6856, 89.3459, 90.0062, 89.8026, 89.5991, 88.6489, 87.6987, 85.4936, 83.2886, 83.4939,
        83.6992,  81.863, 80.0268, 80.1207, 80.2146, 81.2462, 82.2778,  80.281, 78.2842,
        74.0027, 69.7213, 70.6652, 71.6091,  72.979,  74.349, 67.9765,  61.604, 65.7448,
        69.8856, 72.4863,  75.087, 69.3398, 63.5927, 55.0054, 46.4182, 56.6118, 66.8054,
        65.0941, 63.3828
    );
    
    float object[samples] = float[samples](
        0.052, 0.052, 0.052, 0.052, 0.051, 0.051, 0.05,0.05,0.049,0.049,0.049,0.049,
        0.049, 0.049, 0.049, 0.048, 0.048, 0.047, 0.047,0.046,0.045,0.045,0.044,0.044,
        0.044, 0.044, 0.044, 0.044, 0.044, 0.044, 0.044,0.044,0.045,0.046,0.047,0.048,
         0.05, 0.053, 0.057, 0.063, 0.072, 0.086, 0.109,0.143,0.192,0.256,0.332,0.413,
        0.486,  0.55, 0.598, 0.631, 0.654, 0.672, 0.686,0.694,0.7,0.704,0.707,0.712,
        0.718, 0.721, 0.724, 0.727, 0.729, 0.73,  0.73,0.729,0.727,0.728,0.729,0.729,
        0.727, 0.723, 0.721, 0.724, 0.728, 0.727, 0.702,0.68,0.664
    );
    
    float objectIlluminantSpectrum[samples];

    for (int i = 0; i < samples; i++) {
        
        objectIlluminantSpectrum[i] = illuminant[i] * object[i];
    }
    
    float spectrumAmbient[samples];
    float spectrumDiffuse[samples];
    float resultSpectrum[samples];
    
    for (int i = 0; i < samples; i++) {
        
        spectrumAmbient[i] = objectIlluminantSpectrum[i] * surfaceMaterial.ambientPercentage;
        spectrumDiffuse[i] = objectIlluminantSpectrum[i] * surfaceMaterial.diffusePercentage * cosTheta * shadowPercentage;
        resultSpectrum[i] = spectrumAmbient[i] + spectrumDiffuse[i];
    }
    
    ///////Tristimulus conversion.
    float x[samples] = float[samples](
                                    0.001368,0.002236,0.004243,0.00765,0.01431,0.02319,0.04351,0.07763,0.13438,0.21477,0.2839,
                                    0.3285,0.34828,0.34806,0.3362,0.3187,0.2908,0.2511,0.19536,0.1421,0.09564,0.05795001,0.03201,
                                    0.0147,0.0049,0.0024,0.0093,0.0291,0.06327,0.1096,0.1655,0.2257499,0.2904,0.3597,0.4334499,
                                    0.5120501,0.5945,0.6784,0.7621,0.8425,0.9163,0.9786,1.0263,1.0567,1.0622,1.0456,1.0026,0.9384,
                                    0.8544499,0.7514,0.6424,0.5419,0.4479,0.3608,0.2835,0.2187,0.1649,0.1212,0.0874,0.0636,0.04677,
                                    0.0329,0.0227,0.01584,0.01135916,0.008110916,0.005790346,0.004106457,0.002899327,0.00204919,
                                    0.001439971,0.0009999493,0.0006900786,0.0004760213,0.0003323011,0.0002348261,0.0001661505,
                                    0.000117413,0.00008307527,0.00005870652,0.00004150994
                                    );
    
    float y[samples] = float[samples](
                                    0.0000390,0.0000640,0.000120,0.0002170,0.0003960,0.000640,0.001210,0.002180,0.0040,0.00730,
                                    0.01160,0.016840,0.0230,0.02980,0.0380,0.0480,0.060,0.07390,0.090980,0.11260,0.139020,0.16930,
                                    0.208020,0.25860,0.3230,0.40730,0.5030,0.60820,0.710,0.79320,0.8620,0.91485010,0.9540,0.98030,
                                    0.99495010,1.0,0.995,0.9786,0.952,0.9154,0.87,0.8163,0.757,0.6949,0.631,0.5668,0.503,0.4412,
                                    0.381,0.321,0.265,0.217,0.175,0.1382,0.107,0.0816,0.061,0.04458,0.032,0.0232,0.017,0.01192,
                                    0.00821,0.005723,0.004102,0.002929,0.002091,0.001484,0.001047,0.00074,0.00052,0.0003611,
                                    0.0002492,0.0001719,0.00012,0.0000848,0.00006,0.0000424,0.00003,0.0000212,0.00001499
                                    );
    
    float z[samples] = float[samples](
                                    0.006450001,0.01054999,0.02005001,0.03621,0.06785001,0.1102,0.2074,0.3713,0.6456,1.0390501,
                                    1.3856,1.62296,1.74706,1.7826,1.77211,1.7441,1.6692,1.5281,1.28764,1.0419,0.8129501,0.6162,
                                    0.46518,0.3533,0.272,0.2123,0.1582,0.1117,0.07824999,0.05725001,0.04216,0.02984,0.0203,0.0134,
                                    0.008749999,0.005749999,0.0039,0.002749999,0.0021,0.0018,0.001650001,0.0014,0.0011,0.001,0.0008,
                                    0.0006,0.00034,0.00024,0.00019,0.0001,0.00004999999,0.00003,0.00002,0.00001,0.0,0.0,0.0,0.0,
                                    0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,
                                    0.0,0.0
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
    
    /////Gamma correction.
//    float power = 1.0f/2.2f;
//    rgbColor.x = pow(rgbColor.x, power);
//    rgbColor.y = pow(rgbColor.y, power);
//    rgbColor.z = pow(rgbColor.z, power);
    /*****************/

    o_fragColor = rgbColor;
}
