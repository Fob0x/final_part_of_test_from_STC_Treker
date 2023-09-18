//#include <iostream>
//#include <cpprest/http_listener.h>
//#include <cpprest/json.h>
//#include <libpq-fe.h>
//
//using namespace web;
//using namespace web::http;
//using namespace web::http::experimental::listener;
//
//// Создаем строку подключения к бд PostgreSQL.
//const char* db_conn_str = "dbname = TaskTracker user = postgres password = 123456 hostaddr=127.0.0.1";
//// !Здесь введены мои данные. Если будете тестировать на своей бд - заменить под себя
//
//// Объявление функции для обработки HTTP-запросов.
//void handle_request(http_request request)
//{
//    // Устанавливаем заголовок Content-Type для ответа.
//    request.headers().set_content_type(U("text/html; charset=utf-8"));
//
//    // Инициализация PostgreSQL соединения.
//    PGconn* conn = PQconnectdb(db_conn_str);
//    if (PQstatus(conn) != CONNECTION_OK)
//    {
//        // Если соединение с PostgreSQL не удалось, отправляем ошибку HTTP.
//        request.reply(status_codes::InternalError, U("PostgreSQL connection failed"));
//        PQfinish(conn);
//        return;
//    }
//
//    // Устанавливаем кодировку UTF-8.
//    if (PQsetClientEncoding(conn, "UTF8") != 0)
//    {
//        // Если установка кодировки не удалась, отправляем ошибку HTTP.
//        request.reply(status_codes::InternalError, U("Failed to set client encoding"));
//        PQfinish(conn);
//        return;
//    }
//
//    // Переменные для хранения результата и ответа.
//    PGresult* res = nullptr;
//    std::wstring response_body;
//
//    // Проверяем путь запроса для определения запрошенной вкладки.
//    utility::string_t path = request.request_uri().to_string();
//    if (path == U("/expired_tasks"))
//    {
//        // Получаем текущую дату и время в формате PostgreSQL.
//        PGresult* current_time_res = PQexec(conn, "SELECT NOW()");
//        std::string current_time = PQgetvalue(current_time_res, 0, 0);
//        PQclear(current_time_res);
//
//        // Выполнение SQL-запроса к бдшечке для сотрудников с истекшими сроками задач.
//        std::string sql_query = "SELECT * FROM employees WHERE employee_id IN (SELECT executor_id FROM tasks WHERE due_date < '" + current_time + "')";
//        res = PQexec(conn, sql_query.c_str());
//
//        if (PQresultStatus(res) != PGRES_TUPLES_OK)
//        {
//            // Если SQL-запрос не выполнен успешно, то отправляем ошибку HTTP.
//            std::wstring error_message = L"SQL query error: " + utility::conversions::utf8_to_utf16(PQerrorMessage(conn));
//            request.reply(status_codes::InternalError, error_message);
//            PQclear(res);
//            PQfinish(conn);
//            return;
//        }
//
//        // Формируем HTML-страницу с данными о сотрудниках с истекшими сроками задач.
//        response_body = L"<html><head><title>Сотрудники с истекшими сроками выполнения задач</title></head><body><h1>Сотрудники с истекшими сроками выполнения задач:</h1><ul>";
//    }
//    else if (path == U("/edit_tasks"))
//    {
//        if (request.method() == methods::GET)
//        {
//            // Отправляем HTML-форму для добавления задачи.
//            utility::string_t html_forms = U("<html><head><title>Редактировать задачи</title></head><body>")
//                U("<h1>Редактировать задачи</h1>")
//                //U("<form method=\"POST\" action=\"/edit_tasks\" enctype=\"application/json\">") // Изменили Content-Type
//                U("<form method=\"POST\" action=\"/edit_tasks\" enctype=\"application/json;charset=UTF-8\">") // Указываем правильный Content-Type (не помогло)
//                U("   <label for=\"task_name\">Название задачи:</label><br>")
//                U("   <input type=\"text\" id=\"task_name\" name=\"task_name\"><br><br>")
//                U("   <label for=\"executor_id\">ID исполнителя:</label><br>")
//                U("   <input type=\"number\" id=\"executor_id\" name=\"executor_id\"><br><br>")
//                U("   <label for=\"due_date\">Срок выполнения:</label><br>")
//                U("   <input type=\"text\" id=\"due_date\" name=\"due_date\"><br><br>")
//                U("   <label for=\"status\">Статус:</label><br>")
//                U("   <input type=\"text\" id=\"status\" name=\"status\"><br><br>")
//                U("   <input type=\"submit\" value=\"Добавить задачу\">")
//                U("</form></body></html>");
//
//            // Форма для удаления задачи.
//            utility::string_t delete_task_form = U("<h2>Удалить задачу:</h2>")
//                U("<form method=\"POST\" action=\"/edit_tasks\" enctype=\"application/json\">")
//                U("   <label for=\"delete_task_id\">ID задачи:</label><br>")
//                U("   <input type=\"number\" id=\"delete_task_id\" name=\"delete_task_id\"><br><br>")
//                U("   <input type=\"submit\" value=\"Удалить задачу\">")
//                U("</form>");
//
//            // Комбинируем обе формы.
//            utility::string_t html_form = html_forms + delete_task_form;
//
//            request.reply(status_codes::OK, html_form, U("text/html"));
//        }
//    }
//    else
//    {
//        // Формируем HTML-страницу с кнопкой для перехода на вкладку с истекшими сроками выполнения задач.
//        response_body = L"<html><head><title>Список сотрудников</title></head><body><h1>Список сотрудников:</h1>"
//            L"<a href='/expired_tasks'>Показать сотрудников с истекшими сроками выполнения задач</a>"
//            L"<a href='/edit_tasks'>Редактировать задачи</a>"
//            L"<ul>";
//
//        // Остальной код для вывода сотрудников
//        std::string sql_query = "SELECT * FROM employees";
//        res = PQexec(conn, sql_query.c_str());
//    }
//
//    // Остальной код для вывода сотрудников
//    int rows = PQntuples(res);
//    for (int i = 0; i < rows; ++i)
//    {
//        std::string id = PQgetvalue(res, i, 0);
//        std::wstring w_id = utility::conversions::utf8_to_utf16(id);
//
//        std::string name = PQgetvalue(res, i, 1);
//        std::wstring w_name = utility::conversions::utf8_to_utf16(name);
//
//        std::string position = PQgetvalue(res, i, 2);
//        std::wstring w_position = utility::conversions::utf8_to_utf16(position);
//
//        // Добавляем информацию о сотрудниках к HTML-странице.
//        response_body += L"<li>Employee ID: " + w_id + L", Full Name: " + w_name + L", Position: " + w_position + L"</li>";
//    }
//
//    response_body += L"</ul></body></html>";
//
//    PQclear(res);
//    PQfinish(conn);
//
//    // Отправляем HTML-страницу как ответ.
//    request.reply(status_codes::OK, response_body, U("text/html"));
//}
//
//// Основная функция.
//int main()
//{
//    // Устанавливаем локаль для работы с символами UTF-8 (кстати, вроде как не помогла, быть может убрать [проверить позже])
//    setlocale(LC_CTYPE, "en_US.UTF-8");
//
//    // Создаем HTTP-слушатель на указанном адресе и порту.
//    http_listener listener(U("http://localhost:8000"));
//
//    /*Настраиваем обработку только GET - запросов(ибо другие реализовать не получилось :/, но это совсем другая история...)
//    с помощью метода HandleRequest из объекта requestHandler.*/
//    listener.support(methods::GET, handle_request);
//    listener.support(methods::POST, handle_request);
//    listener.support(methods::DEL, handle_request);
//
//    try
//    {
//        // Открываем слушатель и ждем соединений.
//        listener.open().wait();
//        std::wcout << L"Listening on address: " << listener.uri().to_string() << std::endl;
//
//        // Чтобы сервер не отключался сразу же, а работал какое-то время
//        std::this_thread::sleep_for(std::chrono::seconds(60000));
//
//        // Закрываем слушатель.
//        listener.close().wait();
//    }
//    catch (const std::exception& e)
//    {
//        // Обработка и вывод ошибок, если что-то идёт не так.
//        std::wcerr << e.what() << std::endl;
//    }
//
//    return 0;
//}
//
///*ПРИМЕЧАНИЕ:
//Здравствуйте, отправляю то, что получилось на данный момент, чтобы вы не подумали будто бы я забыл о вас и пропал :)
//
//1) Работоспособность кода:
//Код работает, правда, но я не знаю как на гитхаб добавить файлы с БД, чтобы вы могли проверить его полноценно. (библиотеки работают на x86)
//Максимум могу приложить код, благодаря которому создал и заполнил БД:
//
//CREATE TABLE employees
//(
//    employee_id SERIAL PRIMARY KEY,
//    full_name VARCHAR(255) NOT NULL,
//    position VARCHAR(100)
//);
//
//CREATE TABLE tasks
//(
//    task_id SERIAL PRIMARY KEY,
//    task_name VARCHAR(255) NOT NULL,
//    executor_id INT REFERENCES employees(employee_id),
//    due_date DATE,
//    status VARCHAR(50)
//);
//
//-- Вставляем данные о сотрудниках
//INSERT INTO employees (full_name, position)
//VALUES
//    ('Елизавета Колганова', 'Разработчик'),
//    ('Вячеслав Травин', 'Тестировщик'),
//    ('Ковтун Дарья', 'Аналитик'),
//    ('Солопова Алевтина', 'Дизайнер'),
//    ('Андрей Евсеев', 'Разработчик'),
//    ('Елизавета Колганова', 'Разработчик'),
//    ('Михаил Омельченко', 'Менеджер проекта'),
//    ('Андрей Абдурахимов', 'Разработчик'),
//    ('Михаил Машталлеров', 'Аналитик'),
//    ('Вячеслав Травин', 'Тестировщик');
//
//-- Вставляем данные о заданиях
//INSERT INTO tasks (task_name, executor_id, due_date, status)
//VALUES
//    ('Разработка нового веб-приложения', 2, '2023-10-15', 'В процессе'),
//    ('Тестирование интерфейса', 7, '2023-09-20', 'Завершено'),
//    ('Анализ рынка исследований', 5, '2023-09-30', 'Не начато'),
//    ('Создание макетов для клиента', 4, '2023-10-10', 'В процессе'),
//    ('Оценка производительности серверов', 6, '2023-09-25', 'В процессе'),
//    ('Разработка бэкенда', 2, '2023-10-05', 'Не начато'),
//    ('Планирование проекта', 8, '2023-09-15', 'Завершено'),
//    ('Оптимизация базы данных', 10, '2023-10-08', 'Не начато'),
//    ('Анализ требований заказчика', 9, '2023-09-10', 'В процессе'),
//    ('Тестирование функционала', 7, '2023-09-09', 'Не начато');
//
//2) Функционал кода:
//Я пытался выполнить всё что было в том тестовом задании по пунктам:
//1.1. Реляционная БД - PostgreSQL
//1.2. Представляет собой трекер задач с таблицами:
//1.2.1. Таблица сотрудников
//1.2.2. Таблица задач
//
//2.1 Просмотр списка сотрудников
//2.2. Колонка с перечислением "просроченных" задач (я сделал отдельной вкладкой, проверено - работает
//2.3. Возможность редактирования задач (отдельной вкладкой). Отдельный разговор. Интерфейс накидал, но функционал не осилил.
//Есть наработки, но здесь они не представлены.
//4. Серверная часть реализовывает RestAPI
//
//3) Последующие шаги:
//Это не окончательная часть, сейчас я дорабатываю отдельно этот код, но под ООП. Постараюсь сдать его к договорённым срокам,
//если это нужно будет доработать (по вашему усмотрению)*/