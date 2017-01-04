# Spectral BRDF Explorer

[![Build Status](https://www.bitrise.io/app/6e58fb34f951eea4.svg?token=FoOVtEqyeSqs499m0WPWfA&branch=master)](https://www.bitrise.io/app/6e58fb34f951eea4)
[![Build Status](https://www.bitrise.io/app/0f216f14818a0da5.svg?token=E_AXUmBiaFKXBgCdOC32dg&branch=master)](https://www.bitrise.io/app/0f216f14818a0da5)
[![GitHub license](https://img.shields.io/badge/license-MIT-blue.svg)](https://raw.githubusercontent.com/chicio/Spectral-BRDF-Explorer/master/LICENSE.md)
![Supported platform](https://img.shields.io/badge/platforms-iOS%20%7C%20Android-orange.svg)


OpenGL ES application inspired by [Walt Animation Disney Studios BRDF Viewer](https://github.com/wdas/brdf). A simple application thats shows some of the most famous lighting model used in computer graphics and that supports color calculation using RGB and spectral data of lights and object surfaces.

***

### Introduction

In computer graphics usually color calculation is usually done using RGB colors. As a consequence of the fact that light is a subset of the entire electromagnetic spectrum, color in rendering can be computed using spectral distribution data, the tristimulus values and the correlated CIE XYZ color space (Kang, 2007 [1]). This color space was created by the main authority on color and illumination: International Commission on Illumination (CIE). The formulas that define the tristimulus values are (Kang, 2007 [1]):

<p align="center">
<img src="https://github.com/chicio/Spectral-BRDF-Explorer/blob/master/Screenshots/tristimulus-x.png"/>
</p>
<p align="center">
<img src="https://github.com/chicio/Spectral-BRDF-Explorer/blob/master/Screenshots/tristimulus-y.png"/>
</p>
<p align="center">
<img src="https://github.com/chicio/Spectral-BRDF-Explorer/blob/master/Screenshots/tristimulus-z.png"/>
</p>

In this way it is possible to render scenes with high color fidelity, that could show physical phenomenons like metamerism, impossible to obtain with standard RGB data.

***

### Overview

Spectral BRDF Explorer is an OpenGL ES application for iOS and Android devices that support multiple lighting models and color calculation using spectral data of lights and object.
It supports 5 lighting models:

- Phong [2]
- Blinn-phong [3]
- Lambertian 
- Oren-Nayar [4]
- Cook-torrance [5]

All the lighting model above are implemented in specific fragment shader in two version: a RGB version and a spectral one. To show this lighting models I created two scenes, each one with support for RGB or Spectral colors.  
The scenes support also other computer graphics technique (not strictly related to the spectral data calculation):

- shadow mapping
- skybox (cube map texture)

***

### Scenes

**Scene 1**
- RGB data
- all lighting models
- Skybox and shadow mapping

<p align="center">
<img src="https://github.com/chicio/Spectral-BRDF-Explorer/blob/master/Screenshots/rgb.gif"/>
</p>

**Scene 2**
- Spectral data
	- Illuminant D65 [] and Macbeth color checker [7] spectral data
- all lighting models
- Skybox and shadow mapping

<p align="center">
<img src="https://github.com/chicio/Sales-Taxes/blob/master/Screenshots/spectral.gif"/>
</p>

***

### Other projects

For other information about color calculation in computer graphics see my thesis project [Spectral Clara Lux Tracer: Physically based ray tracer with multiple shading models support and Color Rendering Index (CRI) evaluation](https://github.com/chicio/Spectral-Clara-Lux-Tracer).

***

### References

[1] H. R. Kang, "Tristimulus specification," in Computation color technology, Bellingham, Washington: SPIE Publications, 2006, ch. 1, pp. 1-16

[2] B. T. Phong, “Illumination of Computer-Generated Images,” Communications of the ACM, vol. 18, no. 6, pp. 311-317, 1975.

[3] J. Blinn, “Models of light reflection for computer synthesized pictures,” in SIGGRAPH '77, San Jose, 1977, pp. 192-198.

[4] M. Oren and K. S. Nayar, "Generalization of Lambert’s Reflectance Model," in SIGGRAPH '94, Orlando, 1994, pp. 239-246.

[5] R. Cook and K. Torrance, "A reflectance model for computer graphics" in SIGGRAPH '81, vol. 15, no. 3, pp. 301–316, 1981. 

[6] Wikipedia [Online]. Available:  https://en.wikipedia.org/wiki/Illuminant_D65. 

[7] D. Pascale, “RGB coordinates of the Macbeth ColorChecker,” The BabelColor Company, 1 June 2006. [Online]. Available: http://www.babelcolor.com.

