#include <vector>
#include <stdexcept>
#include <fstream>
#include "FileResponse.h"


std::vector<unsigned char> FileResponse::GetAllFile(std::string path)
{
    std::ifstream file(path, std::ios::binary); // 打开二进制文件
    if (!file)
    {
        throw std::runtime_error("无法打开文件"); // 文件打开失败，抛出异常
    }

    // 获取文件大小
    file.seekg(0, std::ios::end);
    std::streampos fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    // 创建字节流缓冲区并读取文件内容
    std::vector<unsigned char> buffer(fileSize);
    file.read(reinterpret_cast<char*>(buffer.data()), fileSize);

    if (!file)
    {
        throw std::runtime_error("读取文件内容时发生错误"); // 读取文件内容失败，抛出异常
    }

    return buffer;
}
