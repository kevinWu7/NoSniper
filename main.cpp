#include <iostream>
#include <asio.hpp>
#include "FileResponse.h"

void handle_client(std::shared_ptr<asio::ip::tcp::socket> socket_ptr) {
  try {
    asio::ip::tcp::socket& socket = *socket_ptr;

      // Read data sent by the client
      asio::streambuf buffer;
      asio::read_until(socket, buffer, '\n');

      // Convert the data to a string and output it
      std::istream is(&buffer);
      std::string message;
      std::getline(is, message);
      std::cout << "接收到消息：" << message <<  std::endl;

      // Send a response to the client
      std::string response = "Hello from server!\n";
      asio::write(socket, asio::buffer(response));

  } catch (std::exception& e) {
    std::cerr << "Exception: " << e.what() << std::endl;
  }
}

void start_server() {
  try {
    // Create an io_context object
    asio::io_context io_context;

    // Create a TCP acceptor object and bind it to the local port
    asio::ip::tcp::acceptor acceptor(io_context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), 18889));

    while (true) {
      // Wait for client connection
      std::shared_ptr<asio::ip::tcp::socket> socket_ptr = std::make_shared<asio::ip::tcp::socket>(io_context);
      acceptor.accept(*socket_ptr);

      // Create a new thread to handle the client connection
      std::thread t([socket_ptr]()
      {
        handle_client(socket_ptr);
        if(socket_ptr)
        {
           std::cout << "handle_client结束" <<std::endl;
        }
      });
      t.detach();  // Detach the thread to let it run in the background
    }
  } catch (std::exception& e) {
    std::cerr << "Exception: " << e.what() << std::endl;
  }
}

int main() {
  std::cout<<"start server"<<std::endl;
  start_server();
  return 0;
}

