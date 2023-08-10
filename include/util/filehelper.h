#ifndef FILEHELPER_H
#define FILEHELPER_H

#include <iostream>
#include <fstream>
#include <filesystem> 
#include "zip.h"


class filehelper
{
    public:
       static  std::filesystem::path getBaseDir(char * arvg);
      // static void AddFileToZip(zipFile zipArchive, const std::string& filePath, const std::string& fileNameInZip);

       static void TraverseAndCompress(zipFile zipArchive, const std::string& folderPath, const std::string& relativePath);

    static void AddFileToZip(zipFile zipArchive, const std::string& filePath, const std::string& entryName);
       static std::vector<uint8_t> CompressFolder(const std::string& folderPath);
       static std::vector<uint8_t> DecompressFolder(const std::vector<uint8_t>& compressedData, const std::string& outputFolderPath);

       // static bool compressFile(const std::string &inputFilePath, std::vector<unsigned char> &outputData);
        //static bool compressFolder(const std::string &inputFolderPath, std::vector<unsigned char> &outputData);
};

#endif // FILEHELPER_H