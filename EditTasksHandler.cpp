#include "EditTasksHandler.h"
#include <cpprest/json.h>

EditTasksHandler::EditTasksHandler(const std::string& dbConnectionString) : dbConnectionString(dbConnectionString) {}

void EditTasksHandler::HandleRequest(web::http::http_request request)
{
    PostgreSQLdatabase db(dbConnectionString);
    if (request.method() == web::http::methods::GET) 
    {
        // ���������� HTML-����� ��� ���������� ������.
        utility::string_t html_forms = U("<html><head><title>������������� ������</title></head><body>")
                                       U("<h1>������������� ������</h1>")
                                       U("<form method=\"POST\" action=\"/edit_tasks\" enctype=\"application/json;charset=UTF-8\">")
                                       U("<label for=\"task_name\">�������� ������:</label><br>")
                                       U("<input type=\"text\" id=\"task_name\" name=\"task_name\"><br><br>")
                                       U("<label for=\"executor_id\">ID �����������:</label><br>")
                                       U("<input type=\"number\" id=\"executor_id\" name=\"executor_id\"><br><br>")
                                       U("<label for=\"due_date\">���� ����������:</label><br>")
                                       U("<input type=\"text\" id=\"due_date\" name=\"due_date\"><br><br>")
                                       U("<label for=\"status\">������:</label><br>")
                                       U("<input type=\"text\" id=\"status\" name=\"status\"><br><br>")
                                       U("<input type=\"submit\" value=\"�������� ������\">")
                                       U("</form></body></html>");

        // ����� ��� �������� ������.
        utility::string_t delete_task_form = U("<h2>������� ������:</h2>")
                                             U("<form method=\"POST\" action=\"/edit_tasks\" enctype=\"application/json\">")
                                             U("<label for=\"delete_task_id\">ID ������:</label><br>")
                                             U("<input type=\"number\" id=\"delete_task_id\" name=\"delete_task_id\"><br><br>")
                                             U("<input type=\"submit\" value=\"������� ������\">")
                                             U("</form>");

        // ����������� ��� �����
        utility::string_t full_html_form = html_forms + delete_task_form;

        request.reply(web::http::status_codes::OK, full_html_form, U("text/html"));
    }
    if (request.method() == web::http::methods::POST) 
    {
        
    }
    if (request.method() == web::http::methods::DEL) // ����� �� ������������ ����� DEL, ���� �������� ����� ���������� ����� POST..?
    {

    }
    else 
    {
        // ��������� ������ ������� ��������.
        request.reply(web::http::status_codes::MethodNotAllowed, U("Method not allowed"));
    }
}
