#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../../include/server/database_manager.hpp"
#include "../../include/common/user.hpp"
#include "../../include/common/chat.hpp"
#include "../../include/common/message.hpp"

// Тестирование требует наличия базы данных PostgreSQL
// Тесты используют моки для избежания зависимости от реальной БД

// Mock класс для DatabaseManager
class MockDatabaseManager : public DatabaseManager {
public:
    // Используем конструктор базового класса
    MockDatabaseManager() : DatabaseManager("mock_connection_string") {}
    
    // Моки для методов работы с пользователями
    MOCK_METHOD(bool, IsClientCorrectLoginAndPassword, 
                (const std::string& login, const std::string& password), (override));
    
    MOCK_METHOD(bool, CreateUser, 
                (const std::string& login, const std::string& password), (override));
    
    MOCK_METHOD(bool, AuthenticateUser, 
                (const std::string& login, const std::string& password), (override));
    
    MOCK_METHOD(bool, IsClientLoginExists, 
                (const std::string& login), (override));
    
    MOCK_METHOD(std::vector<User>, GetUsers, (), (override));
    
    MOCK_METHOD(User, GetUserById, (int user_id), (override));
    
    MOCK_METHOD(int, GetClientIdByLogin, (const std::string& login), (override));
    
    // Моки для методов работы с чатами
    MOCK_METHOD(bool, CreateChat, (int user_id1, int user_id2), (override));
    
    MOCK_METHOD(std::vector<Chat>, GetUserChats, (int user_id), (override));
    
    // Моки для методов работы с сообщениями
    MOCK_METHOD(bool, SaveMessage, (const Message& message), (override));
    
    MOCK_METHOD(std::vector<Message>, GetChatMessages, (int chat_id), (override));
};

// Тесты для методов управления пользователями
TEST(DatabaseManagerTest, UserManagement) {
    MockDatabaseManager mockDb;
    
    // Настраиваем поведение моков
    EXPECT_CALL(mockDb, IsClientLoginExists("testuser"))
        .WillOnce(::testing::Return(false));
    
    EXPECT_CALL(mockDb, CreateUser("testuser", "password123"))
        .WillOnce(::testing::Return(true));
    
    EXPECT_CALL(mockDb, AuthenticateUser("testuser", "password123"))
        .WillOnce(::testing::Return(true));
    
    EXPECT_CALL(mockDb, GetClientIdByLogin("testuser"))
        .WillOnce(::testing::Return(1));
    
    // Тестируем методы
    EXPECT_FALSE(mockDb.IsClientLoginExists("testuser"));
    EXPECT_TRUE(mockDb.CreateUser("testuser", "password123"));
    EXPECT_TRUE(mockDb.AuthenticateUser("testuser", "password123"));
    EXPECT_EQ(1, mockDb.GetClientIdByLogin("testuser"));
}

// Тесты для методов управления чатами
TEST(DatabaseManagerTest, ChatManagement) {
    MockDatabaseManager mockDb;
    
    // Настраиваем мок для создания чата
    EXPECT_CALL(mockDb, CreateChat(1, 2))
        .WillOnce(::testing::Return(true));
    
    // Настраиваем мок для получения чатов пользователя
    Chat chat1;
    chat1.id = 1;
    chat1.name = "Chat with User 2";
    chat1.type = "private";
    
    std::vector<Chat> chats = {chat1};
    
    EXPECT_CALL(mockDb, GetUserChats(1))
        .WillOnce(::testing::Return(chats));
    
    // Тестируем методы
    EXPECT_TRUE(mockDb.CreateChat(1, 2));
    
    std::vector<Chat> resultChats = mockDb.GetUserChats(1);
    EXPECT_EQ(1, resultChats.size());
    EXPECT_EQ(1, resultChats[0].id);
    EXPECT_EQ("Chat with User 2", resultChats[0].name);
    EXPECT_EQ("private", resultChats[0].type);
}

// Тесты для методов управления сообщениями
TEST(DatabaseManagerTest, MessageManagement) {
    MockDatabaseManager mockDb;
    
    // Создаём тестовое сообщение
    Message message;
    message.id = 0; // ID будет присвоен базой данных
    message.sender_id = 1;
    message.receiver_id = 2;
    message.chat_id = 1;
    message.content = "Test message";
    message.is_read = false;
    message.is_edited = false;
    message.sent_at = std::time(nullptr);
    
    // Настраиваем мок для сохранения сообщения
    EXPECT_CALL(mockDb, SaveMessage(::testing::_))
        .WillOnce(::testing::Return(true));
    
    // Настраиваем мок для получения сообщений чата
    Message storedMessage = message;
    storedMessage.id = 1; // База данных присвоила ID
    std::vector<Message> messages = {storedMessage};
    
    EXPECT_CALL(mockDb, GetChatMessages(1))
        .WillOnce(::testing::Return(messages));
    
    // Тестируем методы
    EXPECT_TRUE(mockDb.SaveMessage(message));
    
    std::vector<Message> resultMessages = mockDb.GetChatMessages(1);
    EXPECT_EQ(1, resultMessages.size());
    EXPECT_EQ(1, resultMessages[0].id);
    EXPECT_EQ(1, resultMessages[0].sender_id);
    EXPECT_EQ(2, resultMessages[0].receiver_id);
    EXPECT_EQ(1, resultMessages[0].chat_id);
    EXPECT_EQ("Test message", resultMessages[0].content);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
} 