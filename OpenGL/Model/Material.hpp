//
//  Material.hpp
//  OpenGL
//
//  Created by Fabrizio Duroni on 02/06/16.
//  Copyright © 2016 Fabrizio Duroni. All rights reserved.
//

#ifndef Material_h
#define Material_h

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

struct Material {
    
    /// Ambient component.
    RGBColor ka;
    /// Diffuse component.
    RGBColor kd;
    /// Specular component.
    RGBColor ks;
    /// Shiness.
    float sh;
    
    static Material createRubyMaterial() {
        
        Material material;
        material.ka = RGBColor(0.1745f, 0.01175f, 0.01175f, 1.0);
        material.kd = RGBColor(0.61424f, 0.04136f, 0.04136f, 1.0f);
        material.ks = RGBColor(0.727811f, 0.626959f, 0.626959f, 1.0f);
        material.sh = 76.8f;
        
        return material;
    };
};

#endif /* Material_h */
