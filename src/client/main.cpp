//
// blocking_tcp_echo_client.cpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2024 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <iostream>
#include <boost/asio.hpp>
#include "../screen_handler/interface.hpp"
#include "../screen_handler/chat_screen.hpp"

using boost::asio::ip::tcp;

int main(int argc, char* argv[]) {
  try {
    if (argc != 3) {
      std::cerr << "Usage: blocking_tcp_echo_client <host> <port>\n";
      return 1;
    }

    boost::asio::io_context io_context;
    tcp::socket s(io_context);
    tcp::resolver resolver(io_context);

    Interface interface;

    Interface::RenderGreeting();
    interface.RenderAR();  //TODO: большой иф с выходом, если просто Result::Exit
    auto user_data = interface.GetUserData();

    ChatScreen client_win(argv[1], argv[2]);
    client_win.update_status("Connecting...");
    boost::asio::connect(s, resolver.resolve(argv[1], argv[2]));
    client_win.update_status("Connected");
    client_win.add_message("Logged as: " + user_data.login, true);
    client_win.add_message("Type 'exit' to quit", true);
    while (true) {
      if (client_win.handle_input()) {
        std::string request = client_win.get_request();
        if (request == "exit") {
          client_win.add_message("Disconnecting...", true);
          break;
        }
        client_win.add_message(request);
        boost::asio::write(s, boost::asio::buffer(request));
        client_win.catch_reply(s);
      }
    }
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }
}