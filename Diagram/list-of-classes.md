# Архитектура

## 🖥️ **Клиентская часть**

### `Client::Interface` 
- **Основная задача**: Input/Output у юзера. Полями в этом классе могут быть, например, размеры терминала, в котором запущена программа, но по-сути этот класс - просто обёртка над множеством методов.
    - ### Поля:
        - `pair<int, int> terminal_dimensions`: длина и ширина терминала.
    - ### Методы:
        - #### Приветствие + Аутентификация
            - `void GreetingsMenu()`: просто выводит в терминал интро нашего мессенджера
            - `int RegistrationOrAuthMenu()`: отрисовывает меню, в котором человек выбирает, что он будет делать: регистрироваться или авторизироваться. Возвращается 0 - регистрируемся, 1 - авторизируемся
            - `pair<string, string> GetLoginPassword(bool flag)`: отрисовывает меню, в котором пользователь вводит свой логин и пароль (Эта функция принимает аргумент bool flag, который уточняет: человек выбрал регистрацию или авторизацию - это немного повлияет на вид меню). Возвращает пару {логин, пароль}
        - #### Отображение списков
            - `void RenderUserList(const vector<User>& users)`: отображение списка пользователей
            - `int SelectUserFromList(const vector<User>& users)`: выбор пользователя из списка
            - `void RenderChatList(const vector<Chat>& chats)`: отображение списка доступных чатов
            - `int SelectChatFromList(const vector<Chat>& chats)`: выбор чата из списка
        - #### Отображения чатов
            - `void RenderChat(const vector<Message>& messages, const vector<User>& participants)`: отображение истории сообщений в чате
            - `string GetMessageText()`: получение текста сообщения от пользователя
            - `void DisplayNewMessage(const Message& message, const User& sender)`: добавление нового сообщения в чат
            - `void ScrollChatHistory(int direction)`: прокрутка истории чата 

### `Client::ClientConnectionManager` 
- **Основная задача**: отправка данных на сервер, приём данных с сервера.
    - ### Поля:
        - `tcp::endpoint server_endpoint`: адрес и порт сервера
        - `tcp::socket socket`: сокет для связи с сервером
        - `bool is_connected`: статус подключения
    - ### Методы:
        - #### Соединение
            - `bool Connect(const string& server_address, int port)`: установка соединения с сервером
            - `void Disconnect()`: закрытие соединения с сервером
            - `bool IsConnected()`: проверка статуса соединения
        - #### Передача Данных
            - `bool Send(const string& data)`: отправка данных на сервер
            - `string Receive()`: получение данных от сервера
            - `bool SendAuthRequest(const string& login, const string& password)`: отправка запроса на аутентификацию
            - `bool SendRegistrationRequest(const string& login, const string& password)`: отправка запроса на регистрацию
            - `bool SendMessage(int chat_id, const string& message_text)`: отправка сообщения
            - `bool SendChatListRequest()`: запрос списка чатов
            - `bool SendChatHistoryRequest(int chat_id)`: запрос истории чата

### `Client` 
- **Основная задача**: логика взаимодействия между `Client::Interface` и `Client::ClientConnectionManager`.
    - ### Поля:
        - `Interface interface`: объект для взаимодействия с пользователем
        - `ClientConnectionManager connection_manager`: объект для связи с сервером
        - `bool is_running`: флаг работы клиента
        - `int current_user_id`: ID текущего пользователя
        - `int current_chat_id`: ID текущего открытого чата
        - `string current_user_login`: логин текущего пользователя
        - `vector<User> known_users`: список известных пользователей
        - `vector<Chat> user_chats`: список чатов пользователя
    - ### Методы:
        - #### Жизненный Цикл
            - `void Run()`: запуск основного цикла клиента
            - `void Shutdown()`: завершение работы клиента
        - #### Аутентификация/Регистрация
            - `bool RegisterUser()`: регистрация нового пользователя
            - `bool AuthenticateUser()`: аутентификация пользователя
            - `void Logout()`: выход из аккаунта
        - #### Работа С Чатами
            - `void LoadChatList()`: загрузка списка чатов
            - `void DisplayChatList()`: отображение списка чатов
        - #### Работа С Сообщениями
            - `void HandleMessageSend(int message_id)`: обработка отправки сообщения
        - #### Обработка Ответов Сервера
            - `void HandleAuthResponse(const string& response)`: обработка ответа на аутентификацию
            - `void HandleRegistrationResponse(const string& response)`: обработка ответа на регистрацию
            - `void HandleChatListResponse(const string& response)`: обработка ответа со списком чатов
            - `void HandleChatHistoryResponse(const string& response)`: обработка ответа с историей чата

## 🌐 **Серверная часть**

### `Server::ServerConnectionManager` 
- **Основная задача**: отправка данных пользователю, приём данных от пользователя.
    - ### Поля:
        - `tcp::acceptor acceptor`: акцептор для входящих соединений
        - `vector<tcp::socket> active_connections`: активные клиенты
    - ### Методы:
        - #### Инициализация
            - `void StartListening(int port)`: запуск прослушивания порта
            - `void StopListening()`: остановка прослушивания порта

        - #### Работа С Соединениями
            - `tcp::socket AcceptNewConnection()`: обработка нового входящего подключения
            - `void CloseConnection(tcp::socket& socket)`: закрыть подключение

        - #### Чтение/Запись Данных
            - `string ReceiveData(tcp::socket& socket)`: возвращает данные с сокета
            - `bool SendData(tcp::socket& socket, const std::string& data)`: отправляет данные на сокет
        
        - #### Управление Состояниями
            - `void RegisterUserConnection(int user_id, tcp::socket& socket)`: регистрирует соединение пользователя, связывая его ID с соответствующим сокетом
            - `bool IsUserConnected(int user_id)`: проверяет, подключен ли пользователь с указанным ID в данный момент
            - `tcp::socket* GetUserSocket(int user_id)`: возвращает указатель на сокет пользователя по его ID для отправки данных


### `Server::DatabaseManager` 
- **Основная задача**: отправка запросов базе данных, приём результатов запросов от базы данных.
    - ### Поля:
        - `Какой-то указатель на базу данных`
    - ### Методы:
        - #### Управление Пользователями
            - `bool CreateUser(const string& login, const string& password)`: создать пользователя
            - `bool AuthenticateUser(const string& login, const string& password)`: проверка на существование пользователя
            - `vector<User> GetUsers()`: получение списка всех пользователей
            - `User GetUserById(int user_id)`: получение информации о пользователе по его идентификатору
            
        - #### Управление Чатами
            - `bool CreateChat(int user_id1, int user_id2)`: создание нового чата между двумя пользователями
            - `vector<Chat> GetUserChats(int user_id)`: получение списка чатов пользователя
            
        - #### Управление Сообщениями
            - `bool SaveMessage(message)`: сохраняет сообщение в БД. Если этого не удалось сделать, то возвращает false.
            - `vector<Message> GetChatMessages(int chat_id)`: получение списка сообщений конкретного чата

### `Server` 
- **Основная задача**: логика взаимодействия между `Server::ServerConnectionManager` и  `Server::DatabaseManager`.
    - ### Поля:
        - `ServerConnectionManager connection_manager`
        - `DatabaseManager db_manager`
        - `bool is_running`
    - ### Методы:
        - #### Управление Жизненным Циклом
            - `void run()`: запустить основной цикл серверной части
            - `void stop()`: остановить работу сервера
        - #### Обработка Соединений
            - `void HandleNewConnection(tcp::socket& socket)`: обработка нового клиентского подключения
            - `void HandleClientDisconnect(tcp::socket& socket)`: обработка отключения клиента
        - #### Аутентификация/Регистрация
            - `void HandleUserRegistration(const string& login, const string& password, tcp::socket& client_socket)`: регистрация нового пользователя
            - `void HandleUserAuthentication(const string& login, const string& password, tcp::socket& client_socket)`: аутентификация пользователя
        - #### Обработка Сообщений
            - `void HandleMessageSend(int chat_id, int user_id, const string& message_text, tcp::socket& client_socket)`: обработка отправки сообщения
        - #### Управление Чатами
            - `void HandleChatCreation(int user_id1, int user_id2, tcp::socket& client_socket`: создание нового чата
            - `void HandleChatListRequest(int user_id, tcp::socket& client_socket)`: обработка запроса списка чатов
            - `void HandleChatHistoryRequest(int chat_id, int user_id, tcp::socket& client_socket)`: обработка запроса истории чата

## 🗄️ **База Данных**

### `Database` 
- **Основная задача**: база данных.