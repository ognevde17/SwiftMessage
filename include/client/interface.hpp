
#include <string>
#include <utility>
#include <iostream>
enum class AuthChoice {
    Login,
    Register
};

struct AuthCredentials {
    std::string login;
    std::string password;
};

class Interface {
public:
    // Вывод информационного сообщения
    void ShowMessage(const std::string& message);
    
    // Вывод сообщения об ошибке
    void ShowError(const std::string& error);
    
    // Отображение входящего сообщения
    void DisplayMessage(const std::string& sender, const std::string& message);
    
    // Получение ввода от пользователя
    std::string GetInput();
    
    // Получение учетных данных для аутентификации
    AuthCredentials GetAuthCredentials();
    
    // Получение учетных данных для регистрации
    AuthCredentials GetRegistrationCredentials();
    
    // Выбор между аутентификацией и регистрацией
    AuthChoice GetAuthChoice();
    
    // Получение логина получателя сообщения
    std::string GetRecipientLogin();
    
    // Очистка экрана
    void ClearScreen();
};

