//
//  Macbeth.hpp
//  SpectralBRDFExplorer
//
//  Created by Fabrizio Duroni on 31/12/16.
//
//

#ifndef MacbethColorChecker_hpp
#define MacbethColorChecker_hpp

#include "SBEConfiguration.hpp"

namespace MacbethColorChecker {
    
    /// macbeth 1.
    const float darkSkin[SBEConfiguration::numberOfSamples] = {
        0.065f,0.068f,0.064f,0.059f,0.055f,0.053f,0.052f,0.052f,0.054f,0.057f,0.061f,
        0.065f,0.07f,0.074f,0.076f,0.079f,0.087f,0.1f,0.115f,0.129f,0.138f,0.146f,0.154f,
        0.163f,0.173f,0.188f,0.204f,0.222f,0.242f,0.261f,0.282f
    };

    /// macbeth 2.
    const float lightSkin[SBEConfiguration::numberOfSamples] = {
        0.182,0.197,0.201,0.205,0.212,0.224,0.24,0.262,0.282,0.293,0.301,0.321,
        0.322,0.298,0.292,0.3,0.295,0.305,0.358,0.435,0.494,0.53,0.55,0.564,0.574,
        0.59,0.605,0.624,0.652,0.682,0.713
    };

    /// macbeth 5.
    const float blueFlower[SBEConfiguration::numberOfSamples] = {
        0.328f,0.418f,0.446f,0.448f,0.444f,0.434f,0.421f,0.405f,0.381f,0.362f,0.342f,0.314f,
        0.271f,0.231f,0.211f,0.209f,0.201f,0.196f,0.206f,0.223f,0.235f,0.245f,0.243f,0.247f,
        0.269f,0.318f,0.384f,0.446f,0.49f,0.511f,0.52f
    };

    /// macbeth 7.
    const float orange[SBEConfiguration::numberOfSamples] = {
        0.054,0.053,0.052,0.052,0.052,0.052,0.052,0.053,0.055,0.057,0.061,0.068,0.086,0.12,
        0.17,0.228,0.297,0.38,0.452,0.503,0.532,0.552,0.566,0.578,0.587,0.599,0.604,0.608,
        0.615,0.622,0.628
    };
    
    /// macbeth 13.
    const float blue[SBEConfiguration::numberOfSamples] = {
        0.136f,0.175f,0.208f,0.244f,0.29f,0.335f,0.338f,0.302f,0.239f,0.172f,0.12f,0.086f,0.066f,
        0.054f,0.048f,0.045f,0.043f,0.041f,0.04f,0.04f,0.039f,0.04f,0.04f,0.041f,0.042f,0.042f,
        0.043f,0.044f,0.044f,0.045f,0.048f
    };
    
    /// macbeth 14.
    const float green[SBEConfiguration::numberOfSamples] = {
        0.058,0.059,0.059,0.062,0.065,0.07,0.078,0.091,0.113,0.14,0.18,0.244,0.324,0.363,0.355,
        0.323,0.281,0.238,0.196,0.158,0.124,0.101,0.089,0.084,0.08,0.077,0.075,0.075,0.078,0.082,
        0.088
    };
    
    /// macbeth 15.
    const float red[SBEConfiguration::numberOfSamples] = {
        0.051, 0.05, 0.049, 0.049, 0.049, 0.049, 0.048, 0.047, 0.045, 0.044,
        0.044, 0.044, 0.044, 0.044, 0.045, 0.047, 0.05, 0.057, 0.072, 0.109,
        0.192, 0.332, 0.486, 0.598, 0.654, 0.686, 0.7, 0.707, 0.718, 0.724,
        0.729
    };

    /// macbeth 19.
    const float white[SBEConfiguration::numberOfSamples] = {
        0.409f,0.671f,0.84f,0.878f,0.883f,0.886f,0.887f,0.888f,0.888f,0.888f,0.887f,0.887f,
        0.887f,0.887f,0.887f,0.886f,0.887f,0.888f,0.887f,0.886f,0.887f,0.889f,0.891f,0.891f,
        0.89f,0.889f,0.889f,0.888f,0.888f,0.888f,0.888f
    };
    
    /// macbeth 20.
    const float neutral8[SBEConfiguration::numberOfSamples] = {
        0.372f,0.529f,0.58f,0.585f,0.587f,0.588f,0.586f,0.583f,0.581f,0.58f,0.58f,0.58f,0.581f,
        0.582f,0.583f,0.583f,0.584f,0.586f,0.588f,0.588f,0.588f,0.586f,0.585f,0.583f,0.58f,
        0.578f,0.576f,0.574f,0.572f,0.57f,0.568f
    };
};

#endif /* Macbeth_hpp */
