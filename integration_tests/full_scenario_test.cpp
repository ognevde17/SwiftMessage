#include <gtest/gtest.h>
#include <thread>
#include <chrono>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <cstdlib>
#include <ctime>
#include <iostream>

#include "../include/server/server.hpp"
#include "../include/client/client.hpp"
#include "../include/common/messages.hpp"

// Класс для управления сервером в отдельном потоке
class ServerThread {
public:
    ServerThread(int port) : port_(port), running_(false) {}

    void Start() {
        if (running_) return;
        
        running_ = true;
        server_thread_ = std::thread([this]() {
            try {
                std::string connection_string = "dbname=swiftmessage user=postgres password=postgres";
                server_ = std::make_unique<Server>(port_, connection_string);
                server_->Run();
            }
            catch (const std::exception& e) {
                std::cerr << "Server error: " << e.what() << std::endl;
                running_ = false;
            }
        });
        
        // Даем серверу время для запуска
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    void Stop() {
        if (!running_) return;
        
        running_ = false;
        if (server_) {
            server_->Stop();
        }
        
        if (server_thread_.joinable()) {
            server_thread_.join();
        }
    }

    ~ServerThread() {
        Stop();
    }

private:
    int port_;
    std::atomic<bool> running_;
    std::unique_ptr<Server> server_;
    std::thread server_thread_;
};

// Класс для управления клиентом в отдельном потоке
class ClientThread {
public:
    ClientThread(const std::string& name, const std::string& password, 
                 const std::string& host, int port)
        : name_(name), password_(password), host_(host), port_(port), 
          running_(false), received_message_(false) {}

    void Start() {
        if (running_) return;
        
        running_ = true;
        client_thread_ = std::thread([this]() {
            try {
                client_ = std::make_unique<Client>(host_, port_);
                
                // Регистрация пользователя
                bool registered = client_->RegisterUser(name_, password_);
                {
                    std::lock_guard<std::mutex> lock(mutex_);
                    registered_ = registered;
                    cv_.notify_one();
                }
                
                if (!registered) {
                    std::cerr << "Failed to register user: " << name_ << std::endl;
                    running_ = false;
                    return;
                }
                
                // Авторизация пользователя
                bool authenticated = client_->Login(name_, password_);
                {
                    std::lock_guard<std::mutex> lock(mutex_);
                    authenticated_ = authenticated;
                    cv_.notify_one();
                }
                
                if (!authenticated) {
                    std::cerr << "Failed to authenticate user: " << name_ << std::endl;
                    running_ = false;
                    return;
                }
                
                // Ожидаем сообщений
                while (running_) {
                    std::string message = client_->ReceiveMessage();
                    if (!message.empty()) {
                        std::lock_guard<std::mutex> lock(mutex_);
                        received_messages_.push_back(message);
                        received_message_ = true;
                        cv_.notify_one();
                    }
                }
            }
            catch (const std::exception& e) {
                std::cerr << "Client error: " << e.what() << std::endl;
                running_ = false;
            }
        });
        
        // Даем клиенту время для подключения
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    void Stop() {
        if (!running_) return;
        
        running_ = false;
        if (client_thread_.joinable()) {
            client_thread_.join();
        }
    }

    bool SendMessage(const std::string& recipient, const std::string& text) {
        if (!client_ || !running_) return false;
        return client_->SendMessage(recipient, text);
    }

    bool WaitForRegistration(std::chrono::seconds timeout = std::chrono::seconds(5)) {
        std::unique_lock<std::mutex> lock(mutex_);
        return cv_.wait_for(lock, timeout, [this]() { return registered_; });
    }

    bool WaitForAuthentication(std::chrono::seconds timeout = std::chrono::seconds(5)) {
        std::unique_lock<std::mutex> lock(mutex_);
        return cv_.wait_for(lock, timeout, [this]() { return authenticated_; });
    }

    bool WaitForMessage(std::chrono::seconds timeout = std::chrono::seconds(5)) {
        std::unique_lock<std::mutex> lock(mutex_);
        return cv_.wait_for(lock, timeout, [this]() { return received_message_; });
    }

    std::vector<std::string> GetReceivedMessages() {
        std::lock_guard<std::mutex> lock(mutex_);
        return received_messages_;
    }

    ~ClientThread() {
        Stop();
    }

private:
    std::string name_;
    std::string password_;
    std::string host_;
    int port_;
    std::atomic<bool> running_;
    std::unique_ptr<Client> client_;
    std::thread client_thread_;
    
    std::mutex mutex_;
    std::condition_variable cv_;
    bool registered_ = false;
    bool authenticated_ = false;
    bool received_message_ = false;
    std::vector<std::string> received_messages_;
};

class IntegrationTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Генерируем случайный порт в диапазоне 10000-60000
        std::srand(std::time(nullptr));
        port_ = 10000 + std::rand() % 50000;
        
        // Запускаем сервер на случайном порту
        server_ = std::make_unique<ServerThread>(port_);
        server_->Start();
        
        // Даем время для инициализации сервера
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }
    
    void TearDown() override {
        // Останавливаем все клиентские потоки
        for (auto& client : clients_) {
            client->Stop();
        }
        clients_.clear();
        
        // Останавливаем сервер
        if (server_) {
            server_->Stop();
        }
    }
    
    // Создаем и запускаем нового клиента
    std::shared_ptr<ClientThread> CreateClient(const std::string& name, const std::string& password) {
        auto client = std::make_shared<ClientThread>(name, password, "localhost", port_);
        client->Start();
        clients_.push_back(client);
        return client;
    }
    
    int port_;
    std::unique_ptr<ServerThread> server_;
    std::vector<std::shared_ptr<ClientThread>> clients_;
};

// Тест полного сценария: регистрация, авторизация, отправка сообщения
TEST_F(IntegrationTest, FullScenario) {
    // Создаем двух клиентов
    auto client1 = CreateClient("user1", "password1");
    auto client2 = CreateClient("user2", "password2");
    
    // Ожидаем успешной регистрации обоих клиентов
    EXPECT_TRUE(client1->WaitForRegistration());
    EXPECT_TRUE(client2->WaitForRegistration());
    
    // Ожидаем успешной авторизации обоих клиентов
    EXPECT_TRUE(client1->WaitForAuthentication());
    EXPECT_TRUE(client2->WaitForAuthentication());
    
    // Отправляем сообщение от первого клиента ко второму
    std::string test_message = "Hello from user1 to user2!";
    EXPECT_TRUE(client1->SendMessage("user2", test_message));
    
    // Проверяем, что второй клиент получил сообщение
    EXPECT_TRUE(client2->WaitForMessage());
    
    // Получаем все полученные сообщения
    auto messages = client2->GetReceivedMessages();
    
    // Проверяем, что хотя бы одно сообщение получено
    EXPECT_FALSE(messages.empty());
    
    // Проверяем содержимое сообщения
    // Обратите внимание, что мы должны десериализовать сообщение из формата Boost
    // и проверить его содержимое
    if (!messages.empty()) {
        SendMessageRequest request = SendMessageRequest::from_string(messages[0]);
        EXPECT_EQ("user1", request.sender_login);
        EXPECT_EQ("user2", request.recipient_login);
        EXPECT_EQ(test_message, request.message_text);
    }
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
} 