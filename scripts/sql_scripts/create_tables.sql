-- Создание таблицы пользователей
CREATE TABLE IF NOT EXISTS "User" (
    user_id INT AUTO_INCREMENT PRIMARY KEY,
    username TEXT NOT NULL UNIQUE,
    password TEXT NOT NULL,
    email TEXT,
    last_seen_at TEXT
);

-- Создание таблицы чатов
CREATE TABLE IF NOT EXISTS "Chat" (
    chat_id INT AUTO_INCREMENT PRIMARY KEY,
    chat_name TEXT,
    chat_type TEXT NOT NULL
);

-- Создание таблицы участников чата
CREATE TABLE IF NOT EXISTS "ChatParticipant" (
    user_id INTEGER REFERENCES "User"(user_id),
    chat_id INTEGER REFERENCES "Chat"(chat_id),
    PRIMARY KEY (user_id, chat_id)
);

-- Создание таблицы сообщений
CREATE TABLE IF NOT EXISTS "Message" (
    message_id INT AUTO_INCREMENT PRIMARY KEY,
    sender_id INTEGER REFERENCES "User"(user_id),
    receiver_id INTEGER REFERENCES "User"(user_id),
    chat_id INTEGER REFERENCES "Chat"(chat_id),
    content TEXT,
    is_read BOOLEAN DEFAULT FALSE,
    is_edited BOOLEAN DEFAULT FALSE,
    sent_at TEXT
); 