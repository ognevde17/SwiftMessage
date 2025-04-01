CREATE TABLE IF NOT EXISTS "MediaFile" (file_id SERIAL PRIMARY KEY, message_id INT REFERENCES "Message"(message_id), file_url TEXT, file_type VARCHAR(50));
