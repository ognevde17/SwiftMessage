#include <gtest/gtest.h>
#include "../include/common/messages.hpp"

// Тесты для BaseMessage
TEST(BaseMessageTest, GetRequestType) {
    // Создаем BaseMessage и сериализуем его
    BaseMessage baseMsg("TEST_REQUEST");
    std::ostringstream oss;
    boost::archive::text_oarchive oa(oss);
    oa << baseMsg;
    
    // Получаем запрос из строки
    BaseMessage result = BaseMessage::GetRequestType(oss.str());
    
    // Проверяем, что тип запроса правильно десериализовался
    EXPECT_EQ("TEST_REQUEST", result.request_type);
}

// Тесты для ServerResponse
TEST(ServerResponseTest, SerializationDeserialization) {
    // Создаем ServerResponse
    ServerResponse response("Test response message");
    
    // Сериализуем в строку
    std::string serialized = response.to_string();
    
    // Десериализуем из строки
    ServerResponse deserialized = ServerResponse::from_string(serialized);
    
    // Проверяем результат
    EXPECT_EQ("SERVER_RESPONSE", deserialized.request_type);
    EXPECT_EQ("Test response message", deserialized.response_text);
}

// Тесты для SendMessageRequest
TEST(SendMessageRequestTest, SerializationDeserialization) {
    // Создаем SendMessageRequest
    SendMessageRequest request;
    request.sender_login = "sender123";
    request.recipient_login = "recipient456";
    request.message_text = "Hello, this is a test message!";
    
    // Сериализуем в строку
    std::string serialized = request.to_string();
    
    // Десериализуем из строки
    SendMessageRequest deserialized = SendMessageRequest::from_string(serialized);
    
    // Проверяем результат
    EXPECT_EQ("SEND_MESSAGE", deserialized.request_type);
    EXPECT_EQ("sender123", deserialized.sender_login);
    EXPECT_EQ("recipient456", deserialized.recipient_login);
    EXPECT_EQ("Hello, this is a test message!", deserialized.message_text);
}

// Тесты для AuthRequest
TEST(AuthRequestTest, SerializationDeserialization) {
    // Создаем AuthRequest
    AuthRequest request;
    request.login = "testuser";
    request.password = "password123";
    
    // Сериализуем в строку
    std::string serialized = request.to_string();
    
    // Десериализуем из строки
    AuthRequest deserialized = AuthRequest::from_string(serialized);
    
    // Проверяем результат
    EXPECT_EQ("AUTH_REQUEST", deserialized.request_type);
    EXPECT_EQ("testuser", deserialized.login);
    EXPECT_EQ("password123", deserialized.password);
}

// Тесты для RegisterRequest
TEST(RegisterRequestTest, SerializationDeserialization) {
    // Создаем RegisterRequest
    RegisterRequest request;
    request.login = "newuser";
    request.password = "newpass123";
    
    // Сериализуем в строку
    std::string serialized = request.to_string();
    
    // Десериализуем из строки
    RegisterRequest deserialized = RegisterRequest::from_string(serialized);
    
    // Проверяем результат
    EXPECT_EQ("REGISTER_REQUEST", deserialized.request_type);
    EXPECT_EQ("newuser", deserialized.login);
    EXPECT_EQ("newpass123", deserialized.password);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
} 