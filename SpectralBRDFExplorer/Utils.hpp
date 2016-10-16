//
//  Utils.hpp
//  SpectralBRDFExplorer
//
//  Created by Fabrizio Duroni on 17/09/16.
//  Copyright © 2016 Fabrizio Duroni. All rights reserved.
//

#ifndef Utils_hpp
#define Utils_hpp

#include <stdio.h>
#include <fstream>
#include <string>

#ifdef __APPLE__
#include "iOSFileOpen.hpp"
#else
#include "android_fopen.h"
#endif

std::string getFileContents(std::string filename);

#endif /* Utils_hpp */
