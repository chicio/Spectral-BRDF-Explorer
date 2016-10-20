//
//  iOS.hpp
//  SpectralBRDFExplorer
//
//  Created by Fabrizio Duroni on 16/09/16.
//  
//

#ifndef PathReader_hpp
#define PathReader_hpp

#import <Foundation/Foundation.h>

#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>

FILE* ios_fopen(const char* fileName, const char* mode);

#define fopen(name, mode) ios_fopen(name, mode)

#endif /* PathReader_hpp */
