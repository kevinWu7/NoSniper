#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>
#include <cstdint>
#include <string>
#include <cstdio>
#include "filehelper.h"
#include "zip.h"
#include "zlib.h"
#include "unzip.h"
#include "baseinfo.h"


std::filesystem::path filehelper::rootDir="";

std::filesystem::path filehelper::getBaseDir(char *argv)
{
    // 获取当前可执行程序的路径
    std::filesystem::path exePath = std::filesystem::canonical(std::filesystem::path(std::string(argv)));
    // 获取当前可执行程序的父目录（上一级目录）
    std::filesystem::path parentPath = exePath.parent_path().parent_path();
    return parentPath;
}

void filehelper::TraverseAndCompress(zipFile zipArchive, const std::string &folderPath, const std::string &relativePath)
{
    for (const auto &entry : std::filesystem::directory_iterator(folderPath))
    {
        const std::string entryPath = entry.path().string();
        const std::string entryName = relativePath.empty() ? entry.path().filename().string() : relativePath +  std::filesystem::path::preferred_separator + entry.path().filename().string();
        std::cout << "entryPath:" << entryPath << std::endl;
        std::cout << "entryName:" << entryName << std::endl;
        if (entry.is_regular_file())
        {
            AddFileToZip(zipArchive, entryPath, entryName);
        }
        else if (entry.is_directory())
        {
            TraverseAndCompress(zipArchive, entryPath, entryName);
        }
    }
}
void filehelper::AddFileToZip(zipFile zipArchive, const std::string &filePath, const std::string &entryName)
{
    std::ifstream sourceFile(filePath, std::ios::binary);
    
    if (sourceFile)
    {
        zip_fileinfo fileInfo = {0};
        zipOpenNewFileInZip4_64(zipArchive, entryName.c_str(), &fileInfo, nullptr, 0, nullptr, 0, nullptr, Z_DEFLATED, Z_DEFAULT_COMPRESSION, 0, -MAX_WBITS, DEF_MEM_LEVEL, Z_DEFAULT_STRATEGY, nullptr, 0, 45, 0, 0);
        
        const size_t bufferSize = 1024;
        uint8_t buffer[bufferSize];
        size_t bytesRead;
        
        do
        {
            sourceFile.read(reinterpret_cast<char*>(buffer), bufferSize);
            bytesRead = sourceFile.gcount();
            zipWriteInFileInZip(zipArchive, buffer, static_cast<uint32_t>(bytesRead));
        } while (bytesRead > 0);
        
        zipCloseFileInZip(zipArchive);
    }
}

std::vector<uint8_t> filehelper::CompressFolder(const std::string &folderPath)
{
    std::vector<uint8_t> compressedData;

    zipFile zipArchive = zipOpen64("compressed.zip", APPEND_STATUS_CREATE);
    if (!zipArchive)
    {
        return compressedData;
    }

    TraverseAndCompress(zipArchive, folderPath, "");

    zipClose(zipArchive, nullptr);

    // Read the compressed data from the zip file and store it in the vector
    std::ifstream zipFile("compressed.zip", std::ios::binary);
    if (zipFile)
    {
        zipFile.seekg(0, std::ios::end);
        compressedData.resize(zipFile.tellg());
        zipFile.seekg(0, std::ios::beg);
        zipFile.read(reinterpret_cast<char*>(&compressedData[0]), compressedData.size());
    }

    // Remove the temporary zip file
    std::remove("compressed.zip");

    return compressedData;
}

std::vector<uint8_t> filehelper::DecompressFolder(const std::vector<uint8_t> &compressedData, const std::string &outputFolderPath)
{
    std::vector<uint8_t> decompressedData;
    // Write the compressed data to a temporary zip file
    std::ofstream zipFile("compressed.zip", std::ios::binary);
    if (zipFile.is_open())
    {
        zipFile.write(reinterpret_cast<const char *>(&compressedData[0]), compressedData.size());
        zipFile.close();
    }
    else
    {
        return decompressedData;
    }

    // Open the zip archive for reading
    unzFile unzipArchive = unzOpen64("compressed.zip");
    if (!unzipArchive)
    {
        return decompressedData;
    }

    // Create the output folder if it doesn't exist
    std::filesystem::create_directories(outputFolderPath);

    unz_global_info64 globalInfo;
    if (unzGetGlobalInfo64(unzipArchive, &globalInfo) != UNZ_OK)
    {
        unzClose(unzipArchive);
        return decompressedData;
    }

    for (uLong i = 0; i < globalInfo.number_entry; ++i)
    {
        unz_file_info64 fileInfo;
        char filename[256];
        if (unzGetCurrentFileInfo64(unzipArchive, &fileInfo, filename, 256, nullptr, 0, nullptr, 0) != UNZ_OK)
        {
            unzClose(unzipArchive);
            return decompressedData;
        }

        std::string fullFilename = filename;
        std::string outputPath = outputFolderPath + std::filesystem::path::preferred_separator + fullFilename;
        if (unzOpenCurrentFile(unzipArchive) != UNZ_OK)
        {
            unzClose(unzipArchive);
            return decompressedData;
        }

        // Find last occurrence of path separator
        size_t lastSeparator = fullFilename.find_last_of( std::filesystem::path::preferred_separator);
        if (lastSeparator != std::string::npos)
        {
            std::string folderPath = outputFolderPath +  std::filesystem::path::preferred_separator + fullFilename.substr(0, lastSeparator);
            std::filesystem::create_directories(folderPath);
        }

        std::ofstream outFile(outputPath, std::ios::binary);

        if (outFile.is_open())
        {
            std::vector<uint8_t> buffer(fileInfo.uncompressed_size);
            unzReadCurrentFile(unzipArchive, &buffer[0], fileInfo.uncompressed_size);
            outFile.write(reinterpret_cast<const char *>(&buffer[0]), buffer.size());
            outFile.close();
        }
        else
        {
            unzCloseCurrentFile(unzipArchive);
            unzClose(unzipArchive);
            return decompressedData;
        }

        // 将unzipArchive偏移到下个文件
        unzCloseCurrentFile(unzipArchive);
        if (i < globalInfo.number_entry - 1)
        {
            if (unzGoToNextFile(unzipArchive) != UNZ_OK)
            {
                unzClose(unzipArchive);
                return decompressedData;
            }
        }
    }
    unzClose(unzipArchive);
    return decompressedData;
}


