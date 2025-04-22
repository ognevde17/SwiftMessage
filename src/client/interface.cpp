#include "../../include/client/interface.hpp"
#include "../../include/common/messages.hpp"
void Interface::ShowMessage(const std::string& message) {
    std::cout << "[INFO] " << message << std::endl;
}

void Interface::ShowError(const std::string& error) {
    std::cerr << "[ERROR] " << error << std::endl;
}

void Interface::DisplayMessage(const std::string& sender, const std::string& message) {
    std::cout << sender << "> " << message << std::endl;
}

std::string Interface::GetInput() {
    std::string input;
    std::getline(std::cin, input);
    return input;
}

AuthCredentials Interface::GetAuthCredentials() {
    AuthCredentials credentials;
    
    std::cout << "Введите логин: ";
    std::getline(std::cin, credentials.login);
    
    std::cout << "Введите пароль: ";
    std::getline(std::cin, credentials.password);
    
    return credentials;
}

AuthCredentials Interface::GetRegistrationCredentials() {
    return GetAuthCredentials(); // В данном случае логика аналогична
}

AuthChoice Interface::GetAuthChoice() {
    while (true) {
        std::cout << "Выберите действие:\n"
                  << "1. Вход\n"
                  << "2. Регистрация\n"
                  << "Ваш выбор: ";
        
        std::string choice;
        std::getline(std::cin, choice);
        
        if (choice == "1") return AuthChoice::Login;
        if (choice == "2") return AuthChoice::Register;
        
        ShowError("Некорректный выбор. Попробуйте снова.");
    }
}

std::string Interface::GetRecipientLogin() {
    std::cout << "Введите логин получателя: ";
    return GetInput();
}

void Interface::ClearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}