#include <iostream>
#include <vector>
#include <string>
#include <pqxx/pqxx>

int main() {
  try {
    // Строка подключения к БД
    std::string connection_string = "dbname=mydb user=postgres password=yourpassword";
    
    // Установка соединения с БД
    pqxx::connection conn(connection_string);
    
    if (conn.is_open()) {
      std::cout << "Успешное подключение к базе данных!" << std::endl;
    } else {
      std::cout << "Ошибка подключения к базе данных!" << std::endl;
      return 1;
    }
    
    // Создание транзакции
    pqxx::work txn(conn);
    
    // Создаем тестовую таблицу
    txn.exec("CREATE TABLE IF NOT EXISTS test_table (id SERIAL PRIMARY KEY, name VARCHAR(100))");
    
    // Вставляем тестовые данные
    txn.exec("INSERT INTO test_table (name) VALUES ('Тест 1'), ('Тест 2')");
    
    // Проверяем данные
    pqxx::result result = txn.exec("SELECT * FROM test_table");
    
    std::cout << "\nСодержимое тестовой таблицы:" << std::endl;
    for (auto row : result) {
      std::cout << "ID: " << row[0].as<int>() 
                << ", Имя: " << row[1].as<std::string>() << std::endl;
    }
    
    // Фиксация транзакции
    txn.commit();
    
    std::cout << "\nТест успешно завершен!" << std::endl;
  }
  catch (const std::exception &e) {
    std::cerr << "Ошибка: " << e.what() << std::endl;
    return 1;
  }
  
  return 0;
}