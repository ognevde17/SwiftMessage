//
// blocking_tcp_echo_server.cpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2024 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/asio.hpp>
#include <cstdlib>
#include <iostream>
#include <map>
#include <sstream>
#include <thread>
#include <utility>

using boost::asio::ip::tcp;

const int max_length = 1024;

namespace Constants {
const std::string SERVER_PORT_STRING = "1234";
const unsigned short SERVER_PORT_UNSIGNED_SHORT = 1234;
};  // namespace Constants

// Базовый класс для всех запросов
class BaseRequest {
 public:
  std::string request_type;
};

struct SendMessageRequest : BaseRequest {
  std::string sender_login;
  std::string recipient_login;
  std::string message_text;

  // Сериализация с использованием Boost.Serialization
  template <class Archive>
  void serialize(Archive& ar, const unsigned int version) {
    ar & request_type;
    ar & sender_login;
    ar & recipient_login;
    ar & message_text;
  }

  // Сериализует объект в строку
  std::string to_string() const {
    std::ostringstream oss;
    boost::archive::text_oarchive oa(oss);
    oa << *this;
    return oss.str();
  }

  // Десериализует объект из строки
  static SendMessageRequest from_string(const std::string& data) {
    SendMessageRequest result;
    std::istringstream iss(data);
    boost::archive::text_iarchive ia(iss);
    ia >> result;
    return result;
  }
};

class Server {
  class ConnectionManager {
   public:
    ConnectionManager()
        : acceptor_(io, tcp::endpoint(tcp::v4(),
                                      Constants::SERVER_PORT_UNSIGNED_SHORT)) {}

    tcp::socket AcceptConnection() { return acceptor_.accept(); }

    void CloseConnection(tcp::socket& socket) { socket.close(); }

    void AddClientSocket(int client_id, tcp::socket socket) {
      client_id_to_socket_[client_id] = std::move(socket);
    }

    tcp::socket& GetClientSocket(int client_id) {
      return client_id_to_socket_.at(client_id);
    }

    void RemoveClientSocket(int client_id) {
      client_id_to_socket_.erase(client_id);
    }

    SendMessageRequest ReceiveData(tcp::socket& socket) {
      char data[max_length];
      boost::system::error_code error;
      size_t length = socket.read_some(boost::asio::buffer(data), error);
      return SendMessageRequest::from_string(std::string(data, length));
    }

    bool SendData(int client_id, const SendMessageRequest& request) {
      return SendData(GetClientSocket(client_id), request);
    }

    bool SendData(tcp::socket& socket, const SendMessageRequest& request) {
      boost::system::error_code error;
      boost::asio::write(socket, boost::asio::buffer(request.to_string()),
                         error);
      return !error;
    }

   private:
    boost::asio::io_context io;
    tcp::acceptor acceptor_;

    std::vector<tcp::socket> active_connections_;

    std::map<int, tcp::socket> client_id_to_socket_;
  } connection_manager;

  class DatabaseManager {
   public:
    int GetClientIdByLogin(const std::string& login) {
      return client_login_to_id_[login];
    }

   private:
    std::map<int, std::string> client_id_to_login_;
    std::map<std::string, int> client_login_to_id_;
  };

  class RequestManager {
   public:
    RequestManager(DatabaseManager& db_manager, ConnectionManager& conn_manager)
        : database_manager_(db_manager), connection_manager_(conn_manager) {}

    void HandleRequest(const SendMessageRequest& request) {
      if (request.request_type == "SEND_MESSAGE") {
        int recipient_id =
            database_manager_.GetClientIdByLogin(request.recipient_login);
        connection_manager_.SendData(recipient_id, request);
      }
    }

   private:
    DatabaseManager& database_manager_;
    ConnectionManager& connection_manager_;
  };

  DatabaseManager database_manager;
  RequestManager request_manager{database_manager, connection_manager};

 private:
};

void session(tcp::socket sock) {
  try {
    for (;;) {
      char data[max_length];

      boost::system::error_code error;
      size_t length = sock.read_some(boost::asio::buffer(data), error);
      if (error == boost::asio::error::eof)
        break;  // Connection closed cleanly by peer.
      else if (error)
        throw boost::system::system_error(error);  // Some other error.

      boost::asio::write(sock, boost::asio::buffer(data, length));
    }
  } catch (std::exception& e) {
    std::cerr << "Exception in thread: " << e.what() << "\n";
  }
}

void server(boost::asio::io_context& io_context, unsigned short port) {
  tcp::acceptor a(io_context, tcp::endpoint(tcp::v4(), port));
  for (;;) {
    std::thread(session, a.accept()).detach();
  }
}

int main(int argc, char* argv[]) {
  try {
    if (argc != 2) {
      std::cerr << "Usage: blocking_tcp_echo_server <port>\n";
      return 1;
    }

    boost::asio::io_context io_context;

    server(io_context, std::atoi(argv[1]));
  } catch (std::exception& e) {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}