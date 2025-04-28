-- Создание таблицы ChatParticipant для связи пользователя и чата
CREATE TABLE IF NOT EXISTS "ChatParticipant" (
    participant_id SERIAL PRIMARY KEY,
    user_id INT NOT NULL REFERENCES "User"(user_id) ON DELETE CASCADE,
    chat_id INT NOT NULL REFERENCES "Chat"(chat_id) ON DELETE CASCADE,
    joined_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    UNIQUE(user_id, chat_id)
);

-- Создание таблицы ChatMessage для связи сообщения и чата
CREATE TABLE IF NOT EXISTS "ChatMessage" (
    chat_message_id SERIAL PRIMARY KEY,
    chat_id INT NOT NULL REFERENCES "Chat"(chat_id) ON DELETE CASCADE,
    message_id INT NOT NULL REFERENCES "Message"(message_id) ON DELETE CASCADE,
    UNIQUE(message_id)
);

-- Создание индексов для оптимизации запросов
CREATE INDEX IF NOT EXISTS idx_chatparticipant_user_id ON "ChatParticipant" (user_id);
CREATE INDEX IF NOT EXISTS idx_chatparticipant_chat_id ON "ChatParticipant" (chat_id);
CREATE INDEX IF NOT EXISTS idx_chatmessage_chat_id ON "ChatMessage" (chat_id);
CREATE INDEX IF NOT EXISTS idx_chatmessage_message_id ON "ChatMessage" (message_id); 