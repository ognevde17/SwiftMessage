#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <memory>
#include "../../include/server/request_manager.hpp"
#include "../../include/server/database_manager.hpp"
#include "../../include/common/messages.hpp"

// Мок для соединений, чтобы не зависеть от реальной сети
class MockConnectionManager {
public:
    static bool SendData(int connection_id, const std::string& data) {
        // Статический счетчик вызовов
        static int call_count = 0;
        call_count++;
        last_connection_id = connection_id;
        last_data = data;
        return true;
    }
    
    // Статические переменные для проверки аргументов последнего вызова
    static int last_connection_id;
    static std::string last_data;
    
    // Сброс статических переменных
    static void Reset() {
        last_connection_id = -1;
        last_data = "";
    }
};

// Инициализация статических переменных
int MockConnectionManager::last_connection_id = -1;
std::string MockConnectionManager::last_data = "";

// Мок класс для DatabaseManager
class MockDatabaseManager : public DatabaseManager {
public:
    MockDatabaseManager() : DatabaseManager("mock_connection_string") {}
    
    MOCK_METHOD(bool, IsClientLoginExists, (const std::string& login), (override));
    MOCK_METHOD(bool, IsClientCorrectLoginAndPassword, 
                (const std::string& login, const std::string& password), (override));
    MOCK_METHOD(bool, CreateUser, 
                (const std::string& login, const std::string& password), (override));
    MOCK_METHOD(int, GetClientIdByLogin, (const std::string& login), (override));
};

// Тестовый класс для RequestManager
class TestRequestManager : public ::testing::Test {
protected:
    void SetUp() override {
        // Создаем мок-объект для DatabaseManager
        mock_db = std::make_shared<::testing::NiceMock<MockDatabaseManager>>();
        
        // Сбрасываем состояние MockConnectionManager
        MockConnectionManager::Reset();
        
        // Заменяем ConnectionManager::SendData на нашу мок-функцию
        ConnectionManager::SendData = MockConnectionManager::SendData;
    }
    
    std::shared_ptr<MockDatabaseManager> mock_db;
    RequestManager request_manager;
};

// Тест для обработки запроса регистрации
TEST_F(TestRequestManager, HandleRegisterRequest) {
    // Настраиваем поведение мока DatabaseManager
    EXPECT_CALL(*mock_db, IsClientLoginExists("newuser"))
        .WillOnce(::testing::Return(false));
        
    EXPECT_CALL(*mock_db, CreateUser("newuser", "password123"))
        .WillOnce(::testing::Return(true));
    
    // Создаем запрос регистрации
    RegisterRequest request;
    request.login = "newuser";
    request.password = "password123";
    
    // Вызываем тестируемый метод
    request_manager.HandleRegisterRequest(*mock_db, request.to_string(), 1);
    
    // Проверяем, что был отправлен ответ с успешной регистрацией
    EXPECT_EQ(1, MockConnectionManager::last_connection_id);
    
    // Преобразуем отправленные данные обратно в ServerResponse
    ServerResponse response = ServerResponse::from_string(MockConnectionManager::last_data);
    EXPECT_EQ("You have successfully registered", response.response_text);
}

// Тест для обработки запроса регистрации с существующим логином
TEST_F(TestRequestManager, HandleRegisterRequestWithExistingLogin) {
    // Настраиваем поведение мока DatabaseManager
    EXPECT_CALL(*mock_db, IsClientLoginExists("existinguser"))
        .WillOnce(::testing::Return(true));
    
    // Создаем запрос регистрации
    RegisterRequest request;
    request.login = "existinguser";
    request.password = "password123";
    
    // Вызываем тестируемый метод
    request_manager.HandleRegisterRequest(*mock_db, request.to_string(), 1);
    
    // Проверяем, что был отправлен ответ с ошибкой
    EXPECT_EQ(1, MockConnectionManager::last_connection_id);
    
    // Преобразуем отправленные данные обратно в ServerResponse
    ServerResponse response = ServerResponse::from_string(MockConnectionManager::last_data);
    EXPECT_EQ("This login already exists", response.response_text);
}

// Тест для обработки запроса аутентификации
TEST_F(TestRequestManager, HandleAuthRequest) {
    // Настраиваем поведение мока DatabaseManager
    EXPECT_CALL(*mock_db, IsClientCorrectLoginAndPassword("testuser", "password123"))
        .WillOnce(::testing::Return(true));
        
    EXPECT_CALL(*mock_db, GetClientIdByLogin("testuser"))
        .WillOnce(::testing::Return(1));
    
    // Создаем запрос аутентификации
    AuthRequest request;
    request.login = "testuser";
    request.password = "password123";
    
    // Вызываем тестируемый метод
    request_manager.HandleAuthRequest(*mock_db, request.to_string(), 1);
    
    // Проверяем, что был отправлен ответ с успешной аутентификацией
    EXPECT_EQ(1, MockConnectionManager::last_connection_id);
    
    // Преобразуем отправленные данные обратно в ServerResponse
    ServerResponse response = ServerResponse::from_string(MockConnectionManager::last_data);
    EXPECT_EQ("AUTH_SUCCESS", response.response_text);
}

// Тест для обработки запроса аутентификации с неверными данными
TEST_F(TestRequestManager, HandleAuthRequestWithIncorrectCredentials) {
    // Настраиваем поведение мока DatabaseManager
    EXPECT_CALL(*mock_db, IsClientCorrectLoginAndPassword("wronguser", "wrongpass"))
        .WillOnce(::testing::Return(false));
    
    // Создаем запрос аутентификации
    AuthRequest request;
    request.login = "wronguser";
    request.password = "wrongpass";
    
    // Вызываем тестируемый метод
    request_manager.HandleAuthRequest(*mock_db, request.to_string(), 1);
    
    // Проверяем, что был отправлен ответ с ошибкой
    EXPECT_EQ(1, MockConnectionManager::last_connection_id);
    
    // Преобразуем отправленные данные обратно в ServerResponse
    ServerResponse response = ServerResponse::from_string(MockConnectionManager::last_data);
    EXPECT_EQ("This login or password is incorrect", response.response_text);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
} 