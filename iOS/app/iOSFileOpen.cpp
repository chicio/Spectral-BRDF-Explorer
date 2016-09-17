//
//  iOSFileOpen.cpp
//  SpectralBRDFExplorer
//
//  Created by Fabrizio Duroni on 17/09/16.
//  Copyright © 2016 Fabrizio Duroni. All rights reserved.
//

#include <stdio.h>
#include "iOSFileOpen.hpp"

std::string fileExtension(std::string file){
    
    std::size_t found = file.find_last_of(".");
    return file.substr(found + 1);
}

std::string fileNameWithoutExtension(std::string file){
    
    std::size_t found = file.find_last_of(".");
    return file.substr(0, found);
}

FILE* ios_fopen(const char* fileName, const char* mode) {
    
    std::string stringFileName(fileName);
    
    //Search in bundle (in default scenes and files).
    //Get file path using Core Foundation (opaque data type and pointer...).
    std::string name = fileNameWithoutExtension(stringFileName);
    std::string extension = fileExtension(stringFileName);
    
    CFStringRef filenameCFString = CFStringCreateWithCString(NULL, name.c_str(), kCFStringEncodingUTF8);
    CFStringRef extensionCFString = CFStringCreateWithCString(NULL, extension.c_str(), kCFStringEncodingUTF8);
    
    //Get a reference to the main bundle.
    CFBundleRef mainBundle = CFBundleGetMainBundle();
    
    //Get a reference to the file's URL.
    CFURLRef fileURL = CFBundleCopyResourceURL(mainBundle, filenameCFString, extensionCFString, NULL);
    
    //Convert the URL reference into a string reference.
    CFStringRef cfPath = CFURLCopyFileSystemPath(fileURL, kCFURLPOSIXPathStyle);
    
    //Get the system encoding method.
    CFStringEncoding encodingMethod = CFStringGetSystemEncoding();
    
    //Convert the string reference into a C string.
    const char* path = CFStringGetCStringPtr(cfPath, encodingMethod);
    
    std::string filePath(path);
    
    std::cout << "Bundle file: " << filePath << "\n";

    //Remove fopen here to avoid hijack where we don't need it.
#undef fopen
    
    return std::fopen(filePath.c_str(), mode);
}
