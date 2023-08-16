#ifndef FILEHELPER_H
#define FILEHELPER_H

#include <iostream>
#include <fstream>
#include <filesystem>
#include "zip.h"

class filehelper
{
public:
   static std::filesystem::path getBaseDir(char *arvg);
   //static std::wstring ConvertToWStringFromUTF8(const std::string &str);
   //static std::string ConvertToStringFromWString(const std::wstring &wstr);
   static void TraverseAndCompress(zipFile zipArchive, const std::string &folderPath, const std::string &relativePath);
   static void AddFileToZip(zipFile zipArchive, const std::string &filePath, const std::string &entryName);
   static std::vector<uint8_t> CompressFolder(const std::string &folderPath);
   static std::vector<uint8_t> DecompressFolder(const std::vector<uint8_t> &compressedData, const std::string &outputFolderPath);

   static std::filesystem::path rootDir;
};

#endif // FILEHELPER_H