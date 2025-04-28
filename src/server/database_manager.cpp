#include "../../include/server/database_manager.hpp"
#include "../../include/common/user.hpp"
#include "../../include/common/chat_participant.hpp"
#include "../../include/common/chat_message.hpp"

DatabaseManager::DatabaseManager(const std::string& connection_string) {
    try {
        db_connection = std::make_unique<pqxx::connection>(connection_string);
    } catch (const std::exception& e) {
        throw std::runtime_error("Failed to connect to database: " + std::string(e.what()));
    }
}

DatabaseManager::~DatabaseManager() {
    // if (db_connection && db_connection->is_open()) {
    //     db_connection->close();
    // }
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

bool DatabaseManager::IsClientLoginExists(const std::string& login) {
    try {
        pqxx::work txn(*db_connection);
        
        auto result = txn.exec_params(
            "SELECT COUNT(*) FROM \"User\" WHERE username = $1",
            login
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
        
        auto result = txn.exec("SELECT user_id, username, email FROM \"User\"");
        
        for (const auto& row : result) {
            User user;
            user.id = row["user_id"].as<int>();
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
            "SELECT user_id, username, email FROM \"User\" WHERE user_id = $1",
            user_id
        );

        if (result.empty()) {
            throw std::runtime_error("User not found");
        }

        User user;
        user.id = result[0]["user_id"].as<int>();
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

bool DatabaseManager::AddUserToChat(int user_id, int chat_id) {
    try {
        pqxx::work txn(*db_connection);
        
        txn.exec_params(
            "INSERT INTO \"ChatParticipant\" (user_id, chat_id) VALUES ($1, $2)",
            user_id,
            chat_id
        );

        txn.commit();
        return true;
    } catch (const std::exception& e) {
        return false;
    }
}

std::vector<Chat> DatabaseManager::GetChatsByUserLogin(const std::string& login) {
    std::vector<Chat> chats;
    try {
        pqxx::work txn(*db_connection);
        
        // Сначала получаем ID пользователя по логину
        auto idResult = txn.exec_params(
            "SELECT user_id FROM \"User\" WHERE username = $1",
            login
        );
        
        if (idResult.empty()) {
            txn.commit();
            return chats; // Пользователь не найден
        }
        
        int user_id = idResult[0][0].as<int>();
        
        // Затем получаем все чаты, в которых пользователь участвует
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

bool DatabaseManager::LinkMessageToChat(int chat_id, int message_id) {
    try {
        pqxx::work txn(*db_connection);
        
        txn.exec_params(
            "INSERT INTO \"ChatMessage\" (chat_id, message_id) VALUES ($1, $2)",
            chat_id,
            message_id
        );

        txn.commit();
        return true;
    } catch (const std::exception& e) {
        return false;
    }
}

std::vector<Message> DatabaseManager::GetMessagesByChatId(int chat_id) {
    std::vector<Message> messages;
    try {
        pqxx::work txn(*db_connection);
        
        auto result = txn.exec_params(
            "SELECT m.message_id, m.sender_id, m.receiver_id, m.content, m.is_read, m.is_edited, m.sent_at "
            "FROM \"Message\" m "
            "JOIN \"ChatMessage\" cm ON m.message_id = cm.message_id "
            "WHERE cm.chat_id = $1 "
            "ORDER BY m.sent_at ASC",
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

bool DatabaseManager::SaveMessage(const Message& message) {
    try {
        pqxx::work txn(*db_connection);
        
        // Создаем чат, если его не существует (сообщение отправляется в еще не созданном чате)
        int chat_id = message.chat_id;
        int message_id = 0;
        
        if (chat_id == 0) {
            auto chatResult = txn.exec("INSERT INTO \"Chat\" (chat_type) VALUES ('private') RETURNING chat_id");
            chat_id = chatResult[0][0].as<int>();
            
            // Добавляем участников в чат (создаем записи в ChatParticipant)
            txn.exec_params(
                "INSERT INTO \"ChatParticipant\" (user_id, chat_id) VALUES ($1, $2), ($3, $2)",
                message.sender_id,
                chat_id,
                message.receiver_id
            );
            
            // Вставляем сообщение с новым chat_id
            auto messageResult = txn.exec_params(
                "INSERT INTO \"Message\" (sender_id, receiver_id, chat_id, content, is_read, is_edited, sent_at) "
                "VALUES ($1, $2, $3, $4, $5, $6, $7) RETURNING message_id",
                message.sender_id,
                message.receiver_id,
                chat_id,
                message.content,
                message.is_read,
                message.is_edited,
                message.sent_at
            );
            
            message_id = messageResult[0][0].as<int>();
        } else {
            // Проверяем, есть ли уже участники в чате
            auto participant1 = txn.exec_params(
                "SELECT COUNT(*) FROM \"ChatParticipant\" WHERE user_id = $1 AND chat_id = $2",
                message.sender_id,
                chat_id
            );
            
            // Если отправитель еще не участник чата, добавляем его
            if (participant1[0][0].as<int>() == 0) {
                txn.exec_params(
                    "INSERT INTO \"ChatParticipant\" (user_id, chat_id) VALUES ($1, $2)",
                    message.sender_id,
                    chat_id
                );
            }
            
            auto participant2 = txn.exec_params(
                "SELECT COUNT(*) FROM \"ChatParticipant\" WHERE user_id = $1 AND chat_id = $2",
                message.receiver_id,
                chat_id
            );
            
            // Если получатель еще не участник чата, добавляем его
            if (participant2[0][0].as<int>() == 0) {
                txn.exec_params(
                    "INSERT INTO \"ChatParticipant\" (user_id, chat_id) VALUES ($1, $2)",
                    message.receiver_id,
                    chat_id
                );
            }
            
            // Вставляем сообщение в существующий чат
            auto result = txn.exec_params(
                "INSERT INTO \"Message\" (sender_id, receiver_id, chat_id, content, is_read, is_edited, sent_at) "
                "VALUES ($1, $2, $3, $4, $5, $6, $7) RETURNING message_id",
                message.sender_id,
                message.receiver_id,
                chat_id,
                message.content,
                message.is_read,
                message.is_edited,
                message.sent_at
            );
            
            message_id = result[0][0].as<int>();
        }
        
        // Создаем связь сообщения с чатом в таблице ChatMessage
        txn.exec_params(
            "INSERT INTO \"ChatMessage\" (chat_id, message_id) VALUES ($1, $2)",
            chat_id,
            message_id
        );

        txn.commit();
        return true;
    } catch (const std::exception& e) {
        return false;
    }
}

int DatabaseManager::GetClientIdByLogin(const std::string& login) {
    try {
        pqxx::work txn(*db_connection);
        auto result = txn.exec_params(
            "SELECT user_id FROM \"User\" WHERE username = $1",
            login
        );

        if (result.empty()) {
            return -1; // Пользователь не найден
        }

        int user_id = result[0][0].as<int>();
        txn.commit();
        return user_id;
    } catch (const std::exception& e) {
        return -1;
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
