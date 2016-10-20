//
//  Utils.cpp
//  SpectralBRDFExplorer
//
//  Created by Fabrizio Duroni on 17/09/16.
//  
//

#include "Utils.hpp"

std::string getFileContents(std::string filename) {
    
    std::FILE *fp = fopen(filename.c_str(), "rb");
    
    if (fp) {
        
        std::string contents;
        std::fseek(fp, 0, SEEK_END);
        contents.resize(std::ftell(fp));
        std::rewind(fp);
        std::fread(&contents[0], 1, contents.size(), fp);
        std::fclose(fp);
        return(contents);
    }

    return "";
}
