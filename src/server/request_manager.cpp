#include "../../include/server/request_manager.hpp"

void RequestManager::HandleRequest(const std::string& request) {
  // BaseMessage base_request = BaseMessage::GetRequestType(request);
  // if (base_request.request_type == "AUTH_REQUEST") {
  //   AuthRequest auth_request = AuthRequest::from_string(request);

  //   if (!DatabaseManager::IsClientCorrectLoginAndPassword(
  //           auth_request.login,
  //           auth_request.password)) {  // TODO: отправлять ошибку
  //     ConnectionManager::SendData(
  //         DatabaseManager::GetClientIdByLogin(auth_request.login),
  //         ServerResponse("AUTH_ERROR").to_string());
  //     return;
  //   }

  //   int client_id = DatabaseManager::GetClientIdByLogin(auth_request.login);
  //   ConnectionManager::SendData(client_id,
  //                               ServerResponse("AUTH_SUCCESS").to_string());

  // } else if (base_request.request_type == "SEND_MESSAGE") {
  //   SendMessageRequest send_message_request =
  //       SendMessageRequest::from_string(request);

  //   ConnectionManager::SendData(DatabaseManager::GetClientIdByLogin(
  //                                   send_message_request.recipient_login),
  //                               send_message_request.to_string());
  // }
}