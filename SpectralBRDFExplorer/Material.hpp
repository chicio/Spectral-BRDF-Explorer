//
//  Material.hpp
//  SpectralBRDFExplorer
//
//  Created by Fabrizio Duroni on 02/06/16.
//  
//

#ifndef Material_h
#define Material_h

#include "MacbethColorChecker.hpp"

struct RGBColor {

    /// Red compoent.
    float red;
    /// Green component.
    float green;
    /// Blue component.
    float blue;
    /// Alpha
    float alpha;

    RGBColor() : red{0.0f}, green{0.0f}, blue{0.0f}, alpha{0.0f} {};
    
    /*!
     Create a color.
     
     @param r red component.
     @param g green component.
     @param b blue component.
     @param a alpha component.
     
     @returns rgb color.
     */
    RGBColor(float r, float g, float b, float a) : red{r}, green{g}, blue{b}, alpha{a} {};
};

class Material {
public:
    
    /// Shiness.
    float sh;
    
    virtual ~Material() {}
};

class RGBMaterial : public Material {
public:
    
    /// Ambient color.
    RGBColor ambientColor;
    /// Diffuse color.
    RGBColor diffuseColor;
    /// Specular color.
    RGBColor specularColor;
    
    static Material* createRubyMaterial() {
        
        RGBMaterial* material = new RGBMaterial();
        material->ambientColor = RGBColor(0.1745f, 0.01175f, 0.01175f, 1.0);
        material->diffuseColor = RGBColor(0.61424f, 0.04136f, 0.04136f, 1.0f);
        material->specularColor = RGBColor(0.727811f, 0.626959f, 0.626959f, 1.0f);
        material->sh = 76.8f;
        
        return material;
    };
    
    static Material* createBronzeMaterial() {

        RGBMaterial* material = new RGBMaterial();
        material->ambientColor = RGBColor(0.2125f, 0.1275f, 0.054f, 1.0f);
        material->diffuseColor = RGBColor(0.714f, 0.4284f, 0.18144f, 1.0f);
        material->specularColor = RGBColor(0.393548, 0.271906, 0.166721, 1.0f);
        material->sh = 25.6f;
        
        return material;
    }
    
    static Material* createOrangeMaterial() {
        
        RGBMaterial* material = new RGBMaterial();
        material->ambientColor = RGBColor(0.0f, 0.0f, 0.0f, 1.0f);
        material->diffuseColor = RGBColor(0.992157f, 0.513726f, 0.18144f, 1.0f);
        material->specularColor = RGBColor(0.0225f, 0.0225f, 0.0225f, 1.0f);
        material->sh = 12.8f;
        
        return material;
    }
    
    static Material* createMatteMaterial() {
        
        RGBMaterial* material = new RGBMaterial();
        material->ambientColor = RGBColor(0.2f, 0.2f, 0.2f, 1.0f);
        material->diffuseColor = RGBColor(0.8f, 0.8f, 0.8f, 1.0f);
        material->specularColor = RGBColor(0.0f, 0.0f, 0.0f, 1.0f);
        material->sh = 12.8f;
        
        return material;
    }
    
    static Material* createJadeMaterial() {
        
        RGBMaterial* material = new RGBMaterial();
        material->ambientColor = RGBColor(0.135f, 0.2225f, 0.1575f, 1.0f);
        material->diffuseColor = RGBColor(0.54f, 0.89f, 0.63f, 1.0f);
        material->specularColor = RGBColor(0.316228f, 0.316228f, 0.316228f, 1.0f);
        material->sh = 99.0f;
        
        return material;
    }
};

class SpectralMaterial : public Material {
public:
    
    /// Spectrum samples.
    float spectrumSamples[81];
    /// Ambient percentage.
    float ambientPercentage;
    /// Diffuse percentage.
    float diffusePercentage;
    /// Specular percentage.
    float specularPercentage;

    static Material* createRedDiffuseMackbethMaterial() {
        
        SpectralMaterial* material = new SpectralMaterial();
        material->ambientPercentage = 0.2f;
        material->diffusePercentage = 0.8f;
        material->specularPercentage = 0.0f;
        std::copy(std::begin(MacbethColorChecker::red),
                  std::end(MacbethColorChecker::red),
                  std::begin(material->spectrumSamples));
        
        return material;
    }
};

#endif /* Material_h */
