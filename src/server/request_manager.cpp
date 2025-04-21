#include "../../include/server/request_manager.hpp"
#include <iostream>

// RequestManager::RequestManager()
//     : user_id_to_connection_id_mutex_(), user_id_to_connection_id_() {}

RequestManager::RequestManager()
    : user_id_to_connection_id_mutex_(), user_id_to_connection_id_() {}

void RequestManager::AssociateUserIdWithConnectionId(const int user_id,
                                                     const int connection_id) {
  std::lock_guard<std::mutex> lock(user_id_to_connection_id_mutex_);
  std::cout << "Associating user_id: " << user_id << " with connection_id: " << connection_id << std::endl;
  user_id_to_connection_id_.emplace(user_id, connection_id);
}

int RequestManager::GetConnectionIdByUserId(const int user_id) {
  std::lock_guard<std::mutex> lock(user_id_to_connection_id_mutex_);
  return user_id_to_connection_id_.at(user_id);
}

void RequestManager::HandleRegisterRequest(DatabaseManager& database_manager,
                                           const std::string& request,
                                           const int connection_id) {
  RegisterRequest register_request = RegisterRequest::from_string(request);

  if (database_manager.IsClientLoginExists(register_request.login)) {
    ConnectionManager::SendData(
        connection_id, ServerResponse("This login already exists").to_string());
    return;
  }

  database_manager.CreateUser(register_request.login,
                              register_request.password);

  ConnectionManager::SendData(
      connection_id,
      ServerResponse("You have successfully registered").to_string());
}

void RequestManager::HandleAuthRequest(DatabaseManager& database_manager,
                                       const std::string& request,
                                       const int connection_id) {
  AuthRequest auth_request = AuthRequest::from_string(request);

  if (!database_manager.IsClientCorrectLoginAndPassword(
          auth_request.login, auth_request.password)) {
    std::cout << "AUTH_REQUEST_ERROR: " << auth_request.login << " " << auth_request.password << std::endl;
    ConnectionManager::SendData(
        connection_id,
        ServerResponse("This login or password is incorrect").to_string());
    return;
  }

  int user_id = database_manager.GetClientIdByLogin(auth_request.login);
  AssociateUserIdWithConnectionId(user_id, connection_id);

  ConnectionManager::SendData(connection_id,
                              ServerResponse("AUTH_SUCCESS").to_string());
}

void RequestManager::HandleSendMessageRequest(DatabaseManager& database_manager,
                                              const std::string& request,
                                              const int connection_id) {
  SendMessageRequest send_message_request =
      SendMessageRequest::from_string(request);

  if (!database_manager.IsClientLoginExists(
          send_message_request.recipient_login)) {
    ConnectionManager::SendData(
        connection_id, ServerResponse("This user does not exist").to_string());
    return;
  }

  std::cout << "Message: " << send_message_request.message_text << std::endl;
  std::cout << "Recipient: " << send_message_request.recipient_login << std::endl;

  std::cout << "GetClientIdByLogin: " << database_manager.GetClientIdByLogin(send_message_request.recipient_login) << std::endl;

  int recipient_connection_id;
  {
    std::lock_guard<std::mutex> lock(user_id_to_connection_id_mutex_);
    recipient_connection_id = user_id_to_connection_id_.at(database_manager.GetClientIdByLogin(send_message_request.recipient_login));
  }
  ConnectionManager::SendData(
      recipient_connection_id,
      send_message_request.to_string());
}

void RequestManager::HandleRequest(DatabaseManager& database_manager,
                                   const std::string& request,
                                   const int connection_id) {
  BaseMessage base_request = BaseMessage::GetRequestType(request);

  if (base_request.request_type == "REGISTER_REQUEST") {
    std::cout << "REGISTER_REQUEST" << std::endl;
    HandleRegisterRequest(database_manager, request, connection_id);
    std::cout << "REGISTER_REQUEST_SUCCESS" << std::endl;
  } else if (base_request.request_type == "AUTH_REQUEST") {
    std::cout << "AUTH_REQUEST" << std::endl;
    HandleAuthRequest(database_manager, request, connection_id);
    std::cout << "AUTH_REQUEST_SUCCESS" << std::endl;
  } else if (base_request.request_type == "SEND_MESSAGE") {
    std::cout << "SEND_MESSAGE" << std::endl;
    HandleSendMessageRequest(database_manager, request, connection_id);
    std::cout << "SEND_MESSAGE_SUCCESS" << std::endl;
  }
}