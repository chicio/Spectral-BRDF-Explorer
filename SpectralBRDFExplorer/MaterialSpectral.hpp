//
//  MaterialSpectral.hpp
//  SpectralBRDFExplorer
//
//  Created by Fabrizio Duroni on 03/01/2017.
//  
//

#ifndef MaterialSpectral_hpp
#define MaterialSpectral_hpp

#include <iterator>

#include "SBEConfiguration.hpp"
#include "Material.hpp"

class MaterialSpectral : public Material {
public:
    
    /// Spectrum samples.
    float spectrumSamples[SBEConfiguration::numberOfSamples];
    /// Ambient percentage.
    float ambientPercentage;
    /// Diffuse percentage.
    float diffusePercentage;
    /// Specular percentage.
    float specularPercentage;
    
    static Material* createDiffuseMacbethMaterial(const float macbethSpectrum[SBEConfiguration::numberOfSamples]) {
        
        MaterialSpectral* material = new MaterialSpectral();
        material->ambientPercentage = 0.2f;
        material->diffusePercentage = 0.8f;
        material->specularPercentage = 0.0f;
        std::copy(macbethSpectrum, macbethSpectrum + SBEConfiguration::numberOfSamples, material->spectrumSamples);
        
        return material;
    }
    
    static Material* createSpecularMacbethMaterial(const float macbethSpectrum[SBEConfiguration::numberOfSamples]) {
        
        MaterialSpectral* material = new MaterialSpectral();
        material->ambientPercentage = 0.2f;
        material->diffusePercentage = 0.65f;
        material->specularPercentage = 0.15f;
        material->sh = 76.8f;
        std::copy(macbethSpectrum, macbethSpectrum + SBEConfiguration::numberOfSamples, material->spectrumSamples);
        
        return material;
    }
};

#endif /* MaterialSpectral_hpp */
