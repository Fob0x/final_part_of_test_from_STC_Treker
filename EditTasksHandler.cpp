#include "EditTasksHandler.h"
#include <cpprest/json.h>

EditTasksHandler::EditTasksHandler(const std::string& dbConnectionString) : dbConnectionString(dbConnectionString) {}

void EditTasksHandler::HandleRequest(web::http::http_request request)
{
    PostgreSQLdatabase db(dbConnectionString);
    if (request.method() == web::http::methods::GET) 
    {
        // Отправляем HTML-форму для добавления задачи.
        utility::string_t html_forms = U("<html><head><title>Редактировать задачи</title></head><body>")
                                       U("<h1>Редактировать задачи</h1>")
                                       U("<form method=\"POST\" action=\"/edit_tasks\" enctype=\"application/json;charset=UTF-8\">")
                                       U("<label for=\"task_name\">Название задачи:</label><br>")
                                       U("<input type=\"text\" id=\"task_name\" name=\"task_name\"><br><br>")
                                       U("<label for=\"executor_id\">ID исполнителя:</label><br>")
                                       U("<input type=\"number\" id=\"executor_id\" name=\"executor_id\"><br><br>")
                                       U("<label for=\"due_date\">Срок выполнения:</label><br>")
                                       U("<input type=\"text\" id=\"due_date\" name=\"due_date\"><br><br>")
                                       U("<label for=\"status\">Статус:</label><br>")
                                       U("<input type=\"text\" id=\"status\" name=\"status\"><br><br>")
                                       U("<input type=\"submit\" value=\"Добавить задачу\">")
                                       U("</form></body></html>");

        // Форма для удаления задачи.
        utility::string_t delete_task_form = U("<h2>Удалить задачу:</h2>")
                                             U("<form method=\"POST\" action=\"/edit_tasks\" enctype=\"application/json\">")
                                             U("<label for=\"delete_task_id\">ID задачи:</label><br>")
                                             U("<input type=\"number\" id=\"delete_task_id\" name=\"delete_task_id\"><br><br>")
                                             U("<input type=\"submit\" value=\"Удалить задачу\">")
                                             U("</form>");

        // Комбинируем обе формы
        utility::string_t full_html_form = html_forms + delete_task_form;

        request.reply(web::http::status_codes::OK, full_html_form, U("text/html"));
    }
    if (request.method() == web::http::methods::POST) 
    {
        
    }
    if (request.method() == web::http::methods::DEL) // Нужно ли использовать метод DEL, если удаление можно произвести через POST..?
    {

    }
    else 
    {
        // Обработка других методов запросов.
        request.reply(web::http::status_codes::MethodNotAllowed, U("Method not allowed"));
    }
}
