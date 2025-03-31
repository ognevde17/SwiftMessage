#pragma once

#include <string>
#include <sstream>
#include <boost/serialization/string.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

// ------------------------- BaseMessage START -------------------------

struct BaseMessage {
  std::string request_type;

  BaseMessage() = default;

  BaseMessage(const std::string& request_type) : request_type(request_type) {}

  template <class Archive>
  void serialize(Archive& ar, const unsigned int version) {
    ar & request_type;
  }

  static BaseMessage GetRequestType(const std::string& raw_data) {
    std::istringstream iss(raw_data);
    boost::archive::text_iarchive ia(iss);
    BaseMessage base;
    ia >> base;
    return base;
  }
};

// ------------------------- BaseMessage END -------------------------

// ------------------------- ServerResponse START -------------------------

struct ServerResponse : BaseMessage {
  std::string response_text;

  ServerResponse() : BaseMessage("SERVER_RESPONSE") {}
  ServerResponse(const std::string& response_text) : BaseMessage("SERVER_RESPONSE"), response_text(response_text) {}
  
  template <class Archive>
  void serialize(Archive& ar, const unsigned int version) {
    ar & request_type;
    ar & response_text;
  }

  std::string to_string() const {
    std::ostringstream oss;
    boost::archive::text_oarchive oa(oss);
    oa << *this;
    return oss.str();
  }
  static ServerResponse from_string(const std::string& data) {
    std::istringstream iss(data);
    boost::archive::text_iarchive ia(iss);
    ServerResponse result;
    ia >> result;
    return result;
  }
};

// ------------------------- ServerResponse END -------------------------

// ------------------------- SendMessageRequest START -------------------------

struct SendMessageRequest : BaseMessage {
  std::string sender_login;
  std::string recipient_login;
  std::string message_text;

  SendMessageRequest() : BaseMessage("SEND_MESSAGE") {}

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

// ------------------------- SendMessageRequest END -------------------------

// ------------------------- AuthRequest START -------------------------

struct AuthRequest : BaseMessage {
  std::string login;
  std::string password;

  AuthRequest() : BaseMessage("AUTH_REQUEST") {}

  // Сериализация с использованием Boost.Serialization
  template <class Archive>
  void serialize(Archive& ar, const unsigned int version) {
    ar & request_type;
    ar & login;
    ar & password;
  }

  // Сериализует объект в строку
  std::string to_string() const {
    std::ostringstream oss;
    boost::archive::text_oarchive oa(oss);
    oa << *this;
    return oss.str();
  }

  // Десериализует объект из строки
  static AuthRequest from_string(const std::string& data) {
    AuthRequest result;
    std::istringstream iss(data);
    boost::archive::text_iarchive ia(iss);
    ia >> result;
    return result;
  }
};

// ------------------------- AuthRequest END -------------------------
