# #!/bin/bash

# # Путь к директории сборки
# BUILD_DIR="../build"
# DB_NAME="swift_message"

# # Проверка существования базы данных
# echo "Проверка существования базы данных $DB_NAME..."
# if ! psql -lqt | cut -d \| -f 1 | grep -qw $DB_NAME; then
#     echo "База данных $DB_NAME не существует. Создаем..."
#     createdb $DB_NAME
# else
#     echo "База данных $DB_NAME уже существует."
# fi

# # Инициализация базы данных
# echo "Инициализация структуры базы данных..."
# psql -d $DB_NAME -f init_db.sql

# # Перекомпиляция тестов
# echo "Перекомпиляция тестов..."
# cd $BUILD_DIR && make SwiftMessage_DbTest && cd ..

# # Запуск тестов
# echo "Запуск тестов базы данных..."
# cd $BUILD_DIR && ./SwiftMessage_DbTest

# # Вывод результата
# if [ $? -eq 0 ]; then
#     echo "Тесты успешно выполнены!"
# else
#     echo "Тесты не пройдены!"
# fi 