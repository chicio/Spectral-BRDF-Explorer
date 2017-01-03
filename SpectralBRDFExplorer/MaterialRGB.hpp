//
//  MaterialRGB.hpp
//  SpectralBRDFExplorer
//
//  Created by Fabrizio Duroni on 03/01/2017.
//  
//

#ifndef MaterialRGB_hpp
#define MaterialRGB_hpp

#include "Material.hpp"

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

class MaterialRGB : public Material {
public:
    
    /// Ambient color.
    RGBColor ambientColor;
    /// Diffuse color.
    RGBColor diffuseColor;
    /// Specular color.
    RGBColor specularColor;
    
    static Material* createRubyMaterial() {
        
        MaterialRGB* material = new MaterialRGB();
        material->ambientColor = RGBColor(0.1745f, 0.01175f, 0.01175f, 1.0);
        material->diffuseColor = RGBColor(0.61424f, 0.04136f, 0.04136f, 1.0f);
        material->specularColor = RGBColor(0.727811f, 0.626959f, 0.626959f, 1.0f);
        material->sh = 76.8f;
        
        return material;
    };
    
    static Material* createBronzeMaterial() {
        
        MaterialRGB* material = new MaterialRGB();
        material->ambientColor = RGBColor(0.2125f, 0.1275f, 0.054f, 1.0f);
        material->diffuseColor = RGBColor(0.714f, 0.4284f, 0.18144f, 1.0f);
        material->specularColor = RGBColor(0.393548, 0.271906, 0.166721, 1.0f);
        material->sh = 25.6f;
        
        return material;
    }
    
    static Material* createOrangeMaterial() {
        
        MaterialRGB* material = new MaterialRGB();
        material->ambientColor = RGBColor(0.0f, 0.0f, 0.0f, 1.0f);
        material->diffuseColor = RGBColor(0.992157f, 0.513726f, 0.18144f, 1.0f);
        material->specularColor = RGBColor(0.0225f, 0.0225f, 0.0225f, 1.0f);
        material->sh = 12.8f;
        
        return material;
    }
    
    static Material* createMatteMaterial() {
        
        MaterialRGB* material = new MaterialRGB();
        material->ambientColor = RGBColor(0.2f, 0.2f, 0.2f, 1.0f);
        material->diffuseColor = RGBColor(0.8f, 0.8f, 0.8f, 1.0f);
        material->specularColor = RGBColor(0.0f, 0.0f, 0.0f, 1.0f);
        material->sh = 12.8f;
        
        return material;
    }
    
    static Material* createJadeMaterial() {
        
        MaterialRGB* material = new MaterialRGB();
        material->ambientColor = RGBColor(0.135f, 0.2225f, 0.1575f, 1.0f);
        material->diffuseColor = RGBColor(0.54f, 0.89f, 0.63f, 1.0f);
        material->specularColor = RGBColor(0.316228f, 0.316228f, 0.316228f, 1.0f);
        material->sh = 99.0f;
        
        return material;
    }
};

#endif /* MaterialRGB_hpp */
