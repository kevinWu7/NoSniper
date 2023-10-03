#include <iostream>
#include <sstream>
#include <iomanip>
#include "asio.hpp"
#include "zlib.h"
#include "zip.h"
#include "filehelper.h"
#include "baseinfo.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

void handle_client(std::shared_ptr<asio::ip::tcp::socket> socket_ptr)
{
  try
  {
    asio::ip::tcp::socket &socket = *socket_ptr;
    while (true)
    {
      // Read data sent by the client
      asio::streambuf buffer;
      asio::read_until(socket, buffer, '\n'); // 会阻塞

      // Convert the data to a string and output it
      std::istream is(&buffer);
      std::string message;
      std::getline(is, message);
      std::cout << "接收到消息：" << message << std::endl;

  
      // Send a response to the client
      std::time_t now = std::time(nullptr);
      std::tm localTime = *std::localtime(&now);
      // Create the desired format
      std::ostringstream oss;
      oss << std::put_time(&localTime, "%Y%m%d %H:%M:%S");
      std::string formattedTime = oss.str();
      
      if (message.find("GET_ALL_FILE") != std::string::npos)
      {
        std::cout << "GET_ALL_FILE" << std::endl;
        std::filesystem::path baseDir = filehelper::rootDir;
        auto targetDir= baseDir.append("dataFile");
        std::vector<uint8_t> compressedData = filehelper::CompressFolder(targetDir.string());
        uint64_t datalength=compressedData.size();

        std::cout << "compressedData's length : "+ std::to_string(compressedData.size()) << std::endl;
        // 将整数值以大端序填充到中间8个字节
     // 将uint64_t值以大端序填充到中间8个字节（第5到第12个字节）
    for (int i = 0; i < 8; ++i) {
        SniperHead[4 + i] = (datalength >> (56 - i * 8)) & 0xFF;
    }
      compressedData.insert(compressedData.begin(), SniperHead, SniperHead + sn_totalHeadLength);

        asio::write(socket, asio::buffer(compressedData));
      }
      else
      {
        std::cout << "no type" << std::endl;
        std::string res_message = "server response: " + formattedTime + "\n";
        asio::write(socket, asio::buffer(res_message));
        std::cout << "Send message: " << res_message << std::endl;
      }
    }
  }
  catch (std::exception &e)
  {
    std::cerr << "Exception: " << e.what() << std::endl;
  }
}

void start_server()
{
  try
  {
    // Create an io_context object
    asio::io_context io_context;

    // Create a TCP acceptor object and bind it to the local port
    asio::ip::tcp::acceptor acceptor(io_context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), 18889));

    while (true)
    {
      // Wait for client connection
      std::shared_ptr<asio::ip::tcp::socket> socket_ptr = std::make_shared<asio::ip::tcp::socket>(io_context);
      acceptor.accept(*socket_ptr); // 这句代码会阻塞当前while true。新客户端建立连接后，会继续往下走

      // Create a new thread to handle the client connection
      std::thread t([socket_ptr](){
        handle_client(socket_ptr);
        if(socket_ptr)
        {
           std::cout << "handle_client结束" <<std::endl;
        } });
      t.detach(); // Detach the thread to let it run in the background
    }
  }
  catch (std::exception &e)
  {
    std::cerr << "Exception: " << e.what() << std::endl;
  }
}

void maycompress()
{
  const unsigned char pData[] = {
      123, 34, 37, 83, 24, 2, 98, 178, 57, 220,
      123, 34, 37, 83, 24, 2, 98, 178, 57, 220,
      123, 34, 37, 83, 24, 2, 98, 178, 57, 220,
      123, 34, 37, 83, 24, 2, 98, 178, 57, 220,
      123, 34, 37, 83, 24, 2, 98, 178, 57, 220,
      123, 34, 37, 83, 24, 2, 98, 178, 57, 220,
      123, 34, 37, 83, 24, 2, 98, 178, 57, 220,
      123, 34, 37, 83, 24, 2, 98, 178, 57, 220,
      123, 34, 37, 83, 24, 2, 98, 178, 57, 220,
      123, 34, 37, 83, 24, 2, 98, 178, 57, 220};
  unsigned long nDataSize = 100;

  // printf("Initial size: %d\n", nDataSize);

  unsigned long nCompressedDataSize = nDataSize;
  unsigned char *pCompressedData = new unsigned char[nCompressedDataSize];

  int nResult = compress2(pCompressedData, &nCompressedDataSize, pData, nDataSize, 9);

  if (nResult == Z_OK)
  {
    // printf("Compressed size: %d\n", nCompressedDataSize);

    unsigned char *pUncompressedData = new unsigned char[nDataSize];
    nResult = uncompress(pUncompressedData, &nDataSize, pCompressedData, nCompressedDataSize);
    if (nResult == Z_OK)
    {
      // printf("Uncompressed size: %d\n", nDataSize);
      if (memcmp(pUncompressedData, pData, nDataSize) == 0)
        printf("Great Success\n");
    }
    delete[] pUncompressedData;
  }

  delete[] pCompressedData;
}

int main(int argc, char *argv[])
{
  if (argc < 1)
  {
    std::cerr << "No command-line arguments provided." << std::endl;
    return 1;
  }
  filehelper::rootDir = filehelper::getBaseDir(argv[0]);

  std::cout << "argv:" + std::string(argv[0]) << std::endl;
  std::cout << "current Path:" + filehelper::rootDir.string() << std::endl;

  /* if (filehelper::compressFolder(baseDir, compressedData))
  {
      // 'compressedData' now contains the compressed data of the entire folder
      // You can save it to a file or use it as needed
  }
  else
  {
      std::cerr << "Compression failed." << std::endl;
  }*/

  maycompress();

  /*std::filesystem::path targetDir = filehelper::rootDir;
  std::vector<uint8_t> compressedData = filehelper::CompressFolder(targetDir.append("dataFile"));
  std::cout << "compressedData length:" + std::to_string(compressedData.size()) << std::endl;

  std::filesystem::path targetDir2 = filehelper::rootDir;
  filehelper::DecompressFolder(compressedData, targetDir2.append("realdata"));
  std::cout << "Decompression failed." << std::endl;*/

  std::cout << "start server" << std::endl;
  start_server();
  return 0;
}
