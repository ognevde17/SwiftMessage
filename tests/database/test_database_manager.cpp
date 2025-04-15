#include <iostream>
#include <string>
#include <memory>
#include <cassert>
#include <ctime>
#include "../../include/server/database_manager.hpp"
#include "../../include/common/user.hpp"
#include "../../include/common/chat.hpp"
#include "../../include/common/message.hpp"

// Вспомогательная функция для вывода сообщений
void printTestResult(const std::string& testName, bool success) {
    std::cout << (success ? "[УСПЕХ] " : "[ОШИБКА] ") << testName << std::endl;
}

// Тест создания и аутентификации пользователя
bool testUserCreationAndAuthentication(DatabaseManager& db) {
    std::string testUsername = "test_user_" + std::to_string(rand() % 1000);
    std::string testPassword = "test_password";
    
    // Создаем пользователя
    bool userCreated = db.CreateUser(testUsername, testPassword);
    if (!userCreated) {
        std::cout << "Не удалось создать пользователя" << std::endl;
        return false;
    }
    
    // Проверяем аутентификацию
    bool authenticated = db.AuthenticateUser(testUsername, testPassword);
    if (!authenticated) {
        std::cout << "Не удалось аутентифицировать пользователя" << std::endl;
        return false;
    }
    
    // Проверяем получение ID пользователя
    int userId = db.GetClientIdByLogin(testUsername);
    if (userId <= 0) {
        std::cout << "Не удалось получить ID пользователя" << std::endl;
        return false;
    }
    
    std::cout << "Создан тестовый пользователь: " << testUsername << " с ID: " << userId << std::endl;
    return true;
}

// Тест создания чата и сообщений
bool testChatAndMessages(DatabaseManager& db) {
    // Создаем двух тестовых пользователей
    std::string user1 = "chat_test_user1_" + std::to_string(rand() % 1000);
    std::string user2 = "chat_test_user2_" + std::to_string(rand() % 1000);
    std::string password = "test_password";
    
    db.CreateUser(user1, password);
    db.CreateUser(user2, password);
    
    int userId1 = db.GetClientIdByLogin(user1);
    int userId2 = db.GetClientIdByLogin(user2);
    
    if (userId1 <= 0 || userId2 <= 0) {
        std::cout << "Не удалось получить ID пользователей" << std::endl;
        return false;
    }
    
    // Создаем чат между пользователями
    bool chatCreated = db.CreateChat(userId1, userId2);
    if (!chatCreated) {
        std::cout << "Не удалось создать чат" << std::endl;
        return false;
    }
    
    // Получаем чаты пользователя
    std::vector<Chat> userChats = db.GetUserChats(userId1);
    if (userChats.empty()) {
        std::cout << "Не удалось получить чаты пользователя" << std::endl;
        return false;
    }
    
    int chatId = userChats[0].id;
    
    // Создаем сообщение
    Message msg;
    msg.sender_id = userId1;
    msg.receiver_id = userId2;
    msg.chat_id = chatId;
    msg.content = "Тестовое сообщение";
    
    bool msgSaved = db.SaveMessage(msg);
    if (!msgSaved) {
        std::cout << "Не удалось сохранить сообщение" << std::endl;
        return false;
    }
    
    // Получаем сообщения чата
    std::vector<Message> chatMessages = db.GetChatMessages(chatId);
    if (chatMessages.empty()) {
        std::cout << "Не удалось получить сообщения чата" << std::endl;
        return false;
    }
    
    std::cout << "Создан чат с ID: " << chatId << " между пользователями " 
              << userId1 << " и " << userId2 << std::endl;
    std::cout << "Сообщение в чате: " << chatMessages[0].content << std::endl;
    
    return true;
}

// Тест получения пользователей
bool testGetUsers(DatabaseManager& db) {
    std::vector<User> users = db.GetUsers();
    if (users.empty()) {
        std::cout << "Не удалось получить список пользователей или список пуст" << std::endl;
        return false;
    }
    
    std::cout << "Получен список пользователей. Количество: " << users.size() << std::endl;
    std::cout << "Первый пользователь: " << users[0].username << " (ID: " << users[0].id << ")" << std::endl;
    
    return true;
}

int main() {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    
    try {
        // Строка подключения к БД (измените в соответствии с вашими настройками)
        std::string connection_string = "dbname=swiftmessagedatabase user=mikhailnarizhnyy password=mikhail2004";
        
        // Создаем экземпляр DatabaseManager
        DatabaseManager db(connection_string);
        
        std::cout << "=== ТЕСТЫ БАЗЫ ДАННЫХ ===" << std::endl;
        
        // Запускаем тесты
        bool test1 = testUserCreationAndAuthentication(db);
        printTestResult("Тест создания и аутентификации пользователя", test1);
        
        bool test2 = testChatAndMessages(db);
        printTestResult("Тест создания чата и сообщений", test2);
        
        bool test3 = testGetUsers(db);
        printTestResult("Тест получения пользователей", test3);
        
        std::cout << "=== ТЕСТЫ ЗАВЕРШЕНЫ ===" << std::endl;
        
        // Общий результат тестов
        if (test1 && test2 && test3) {
            std::cout << "\nВСЕ ТЕСТЫ УСПЕШНО ПРОЙДЕНЫ!" << std::endl;
            return 0;
        } else {
            std::cout << "\nНЕКОТОРЫЕ ТЕСТЫ НЕ ПРОЙДЕНЫ!" << std::endl;
            return 1;
        }
    }
    catch (const std::exception &e) {
        std::cerr << "ОШИБКА: " << e.what() << std::endl;
        return 1;
    }
} 