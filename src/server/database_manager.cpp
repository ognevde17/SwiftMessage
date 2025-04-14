#include "../../include/server/database_manager.hpp"
#include "../../include/server/user.hpp"

DatabaseManager::DatabaseManager(const std::string& connection_string) {
    try {
        db_connection = std::make_unique<pqxx::connection>(connection_string);
    } catch (const std::exception& e) {
        throw std::runtime_error("Failed to connect to database: " + std::string(e.what()));
    }
}

DatabaseManager::~DatabaseManager() {
    if (db_connection && db_connection->is_open()) {
        db_connection->close();
    }
}

bool DatabaseManager::IsClientCorrectLoginAndPassword(
    const std::string& login, const std::string& password) {
    try {
        pqxx::work txn(*db_connection);
        auto result = txn.exec_params(
            "SELECT COUNT(*) FROM \"User\" WHERE username = $1 AND password = $2",
            login,
            password
        );
        txn.commit();
        return result[0][0].as<int>() > 0;
    } catch (const std::exception& e) {
        return false;
    }
}

bool DatabaseManager::CreateUser(const std::string& login, const std::string& password) {
    try {
        pqxx::work txn(*db_connection);
        
        // Проверяем, существует ли пользователь
        auto check = txn.exec_params(
            "SELECT COUNT(*) FROM \"User\" WHERE username = $1",
            login
        );
        
        if (check[0][0].as<int>() > 0) {
            return false; // Пользователь уже существует
        }

        // Создаем нового пользователя
        txn.exec_params(
            "INSERT INTO \"User\" (username, password) VALUES ($1, $2)",
            login,
            password
        );

        txn.commit();
        return true;
    } catch (const std::exception& e) {
        return false;
    }
}

bool DatabaseManager::AuthenticateUser(const std::string& login, const std::string& password) {
    try {
        pqxx::work txn(*db_connection);
        
        auto result = txn.exec_params(
            "SELECT COUNT(*) FROM \"User\" WHERE username = $1 AND password = $2",
            login,
            password
        );

        txn.commit();
        return result[0][0].as<int>() > 0;
    } catch (const std::exception& e) {
        return false;
    }
}

std::vector<User> DatabaseManager::GetUsers() {
    std::vector<User> users;
    try {
        pqxx::work txn(*db_connection);
        
        auto result = txn.exec("SELECT used_id, username, email FROM \"User\"");
        
        for (const auto& row : result) {
            User user;
            user.id = row["used_id"].as<int>();
            user.username = row["username"].as<std::string>();
            if (!row["email"].is_null()) {
                user.email = row["email"].as<std::string>();
            }
            users.push_back(user);
        }

        txn.commit();
    } catch (const std::exception& e) {
        // Логирование ошибки
    }
    return users;
}

User DatabaseManager::GetUserById(int user_id) {
    try {
        pqxx::work txn(*db_connection);
        
        auto result = txn.exec_params(
            "SELECT used_id, username, email FROM \"User\" WHERE used_id = $1",
            user_id
        );

        if (result.empty()) {
            throw std::runtime_error("User not found");
        }

        User user;
        user.id = result[0]["used_id"].as<int>();
        user.username = result[0]["username"].as<std::string>();
        if (!result[0]["email"].is_null()) {
            user.email = result[0]["email"].as<std::string>();
        }

        txn.commit();
        return user;
    } catch (const std::exception& e) {
        throw std::runtime_error("Failed to get user: " + std::string(e.what()));
    }
}

bool DatabaseManager::CreateChat(int user_id1, int user_id2) {
    try {
        pqxx::work txn(*db_connection);
        
        // Создаем новый чат
        auto result = txn.exec("INSERT INTO \"Chat\" (chat_type) VALUES ('private') RETURNING chat_id");
        
        int chat_id = result[0][0].as<int>();

        // Добавляем участников чата
        txn.exec_params(
            "INSERT INTO \"ChatParticipant\" (user_id, chat_id) VALUES ($1, $2), ($3, $2)",
            user_id1,
            chat_id,
            user_id2
        );

        txn.commit();
        return true;
    } catch (const std::exception& e) {
        return false;
    }
}

std::vector<Chat> DatabaseManager::GetUserChats(int user_id) {
    std::vector<Chat> chats;
    try {
        pqxx::work txn(*db_connection);
        
        auto result = txn.exec_params(
            "SELECT c.chat_id, c.chat_name, c.chat_type "
            "FROM \"Chat\" c "
            "JOIN \"ChatParticipant\" cp ON c.chat_id = cp.chat_id "
            "WHERE cp.user_id = $1",
            user_id
        );
        
        for (const auto& row : result) {
            Chat chat;
            chat.id = row["chat_id"].as<int>();
            if (!row["chat_name"].is_null()) {
                chat.name = row["chat_name"].as<std::string>();
            }
            chat.type = row["chat_type"].as<std::string>();
            chats.push_back(chat);
        }

        txn.commit();
    } catch (const std::exception& e) {
        // Логирование ошибки
    }
    return chats;
}

bool DatabaseManager::SaveMessage(const Message& message) {
    try {
        pqxx::work txn(*db_connection);
        
        txn.exec_params(
            "INSERT INTO \"Message\" (sender_id, receiver_id, chat_id, content, is_read, is_edited, sent_at) "
            "VALUES ($1, $2, $3, $4, $5, $6, $7)",
            message.sender_id,
            message.receiver_id,
            message.chat_id,
            message.content,
            message.is_read,
            message.is_edited,
            message.sent_at
        );

        txn.commit();
        return true;
    } catch (const std::exception& e) {
        return false;
    }
}

std::vector<Message> DatabaseManager::GetChatMessages(int chat_id) {
    std::vector<Message> messages;
    try {
        pqxx::work txn(*db_connection);
        
        auto result = txn.exec_params(
            "SELECT message_id, sender_id, receiver_id, content, is_read, is_edited, sent_at "
            "FROM \"Message\" "
            "WHERE chat_id = $1 "
            "ORDER BY sent_at ASC",
            chat_id
        );
        
        for (const auto& row : result) {
            Message msg;
            msg.id = row["message_id"].as<int>();
            msg.sender_id = row["sender_id"].as<int>();
            msg.receiver_id = row["receiver_id"].as<int>();
            msg.chat_id = chat_id;
            msg.content = row["content"].as<std::string>();
            msg.is_read = row["is_read"].as<bool>();
            msg.is_edited = row["is_edited"].as<bool>();
            msg.sent_at = row["sent_at"].as<std::string>();
            messages.push_back(msg);
        }

        txn.commit();
    } catch (const std::exception& e) {
        // Логирование ошибки
    }
    return messages;
}

int DatabaseManager::GetClientIdByLogin(const std::string& login) {
    try {
        pqxx::work txn(*db_connection);
        auto result = txn.exec_params(
            "SELECT used_id FROM \"User\" WHERE username = $1",
            login
        );
        
        if (result.empty()) {
            throw std::runtime_error("User not found");
        }

        txn.commit();
        return result[0]["used_id"].as<int>();
    } catch (const std::exception& e) {
        throw std::runtime_error("Failed to get user ID: " + std::string(e.what()));
    }
}