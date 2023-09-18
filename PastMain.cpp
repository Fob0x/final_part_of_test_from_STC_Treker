//#include <iostream>
//#include <cpprest/http_listener.h>
//#include <cpprest/json.h>
//#include <libpq-fe.h>
//
//using namespace web;
//using namespace web::http;
//using namespace web::http::experimental::listener;
//
//// ������� ������ ����������� � �� PostgreSQL.
//const char* db_conn_str = "dbname = TaskTracker user = postgres password = 123456 hostaddr=127.0.0.1";
//// !����� ������� ��� ������. ���� ������ ����������� �� ����� �� - �������� ��� ����
//
//// ���������� ������� ��� ��������� HTTP-��������.
//void handle_request(http_request request)
//{
//    // ������������� ��������� Content-Type ��� ������.
//    request.headers().set_content_type(U("text/html; charset=utf-8"));
//
//    // ������������� PostgreSQL ����������.
//    PGconn* conn = PQconnectdb(db_conn_str);
//    if (PQstatus(conn) != CONNECTION_OK)
//    {
//        // ���� ���������� � PostgreSQL �� �������, ���������� ������ HTTP.
//        request.reply(status_codes::InternalError, U("PostgreSQL connection failed"));
//        PQfinish(conn);
//        return;
//    }
//
//    // ������������� ��������� UTF-8.
//    if (PQsetClientEncoding(conn, "UTF8") != 0)
//    {
//        // ���� ��������� ��������� �� �������, ���������� ������ HTTP.
//        request.reply(status_codes::InternalError, U("Failed to set client encoding"));
//        PQfinish(conn);
//        return;
//    }
//
//    // ���������� ��� �������� ���������� � ������.
//    PGresult* res = nullptr;
//    std::wstring response_body;
//
//    // ��������� ���� ������� ��� ����������� ����������� �������.
//    utility::string_t path = request.request_uri().to_string();
//    if (path == U("/expired_tasks"))
//    {
//        // �������� ������� ���� � ����� � ������� PostgreSQL.
//        PGresult* current_time_res = PQexec(conn, "SELECT NOW()");
//        std::string current_time = PQgetvalue(current_time_res, 0, 0);
//        PQclear(current_time_res);
//
//        // ���������� SQL-������� � ������� ��� ����������� � ��������� ������� �����.
//        std::string sql_query = "SELECT * FROM employees WHERE employee_id IN (SELECT executor_id FROM tasks WHERE due_date < '" + current_time + "')";
//        res = PQexec(conn, sql_query.c_str());
//
//        if (PQresultStatus(res) != PGRES_TUPLES_OK)
//        {
//            // ���� SQL-������ �� �������� �������, �� ���������� ������ HTTP.
//            std::wstring error_message = L"SQL query error: " + utility::conversions::utf8_to_utf16(PQerrorMessage(conn));
//            request.reply(status_codes::InternalError, error_message);
//            PQclear(res);
//            PQfinish(conn);
//            return;
//        }
//
//        // ��������� HTML-�������� � ������� � ����������� � ��������� ������� �����.
//        response_body = L"<html><head><title>���������� � ��������� ������� ���������� �����</title></head><body><h1>���������� � ��������� ������� ���������� �����:</h1><ul>";
//    }
//    else if (path == U("/edit_tasks"))
//    {
//        if (request.method() == methods::GET)
//        {
//            // ���������� HTML-����� ��� ���������� ������.
//            utility::string_t html_forms = U("<html><head><title>������������� ������</title></head><body>")
//                U("<h1>������������� ������</h1>")
//                //U("<form method=\"POST\" action=\"/edit_tasks\" enctype=\"application/json\">") // �������� Content-Type
//                U("<form method=\"POST\" action=\"/edit_tasks\" enctype=\"application/json;charset=UTF-8\">") // ��������� ���������� Content-Type (�� �������)
//                U("   <label for=\"task_name\">�������� ������:</label><br>")
//                U("   <input type=\"text\" id=\"task_name\" name=\"task_name\"><br><br>")
//                U("   <label for=\"executor_id\">ID �����������:</label><br>")
//                U("   <input type=\"number\" id=\"executor_id\" name=\"executor_id\"><br><br>")
//                U("   <label for=\"due_date\">���� ����������:</label><br>")
//                U("   <input type=\"text\" id=\"due_date\" name=\"due_date\"><br><br>")
//                U("   <label for=\"status\">������:</label><br>")
//                U("   <input type=\"text\" id=\"status\" name=\"status\"><br><br>")
//                U("   <input type=\"submit\" value=\"�������� ������\">")
//                U("</form></body></html>");
//
//            // ����� ��� �������� ������.
//            utility::string_t delete_task_form = U("<h2>������� ������:</h2>")
//                U("<form method=\"POST\" action=\"/edit_tasks\" enctype=\"application/json\">")
//                U("   <label for=\"delete_task_id\">ID ������:</label><br>")
//                U("   <input type=\"number\" id=\"delete_task_id\" name=\"delete_task_id\"><br><br>")
//                U("   <input type=\"submit\" value=\"������� ������\">")
//                U("</form>");
//
//            // ����������� ��� �����.
//            utility::string_t html_form = html_forms + delete_task_form;
//
//            request.reply(status_codes::OK, html_form, U("text/html"));
//        }
//    }
//    else
//    {
//        // ��������� HTML-�������� � ������� ��� �������� �� ������� � ��������� ������� ���������� �����.
//        response_body = L"<html><head><title>������ �����������</title></head><body><h1>������ �����������:</h1>"
//            L"<a href='/expired_tasks'>�������� ����������� � ��������� ������� ���������� �����</a>"
//            L"<a href='/edit_tasks'>������������� ������</a>"
//            L"<ul>";
//
//        // ��������� ��� ��� ������ �����������
//        std::string sql_query = "SELECT * FROM employees";
//        res = PQexec(conn, sql_query.c_str());
//    }
//
//    // ��������� ��� ��� ������ �����������
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
//        // ��������� ���������� � ����������� � HTML-��������.
//        response_body += L"<li>Employee ID: " + w_id + L", Full Name: " + w_name + L", Position: " + w_position + L"</li>";
//    }
//
//    response_body += L"</ul></body></html>";
//
//    PQclear(res);
//    PQfinish(conn);
//
//    // ���������� HTML-�������� ��� �����.
//    request.reply(status_codes::OK, response_body, U("text/html"));
//}
//
//// �������� �������.
//int main()
//{
//    // ������������� ������ ��� ������ � ��������� UTF-8 (������, ����� ��� �� �������, ���� ����� ������ [��������� �����])
//    setlocale(LC_CTYPE, "en_US.UTF-8");
//
//    // ������� HTTP-��������� �� ��������� ������ � �����.
//    http_listener listener(U("http://localhost:8000"));
//
//    /*����������� ��������� ������ GET - ��������(��� ������ ����������� �� ���������� :/, �� ��� ������ ������ �������...)
//    � ������� ������ HandleRequest �� ������� requestHandler.*/
//    listener.support(methods::GET, handle_request);
//    listener.support(methods::POST, handle_request);
//    listener.support(methods::DEL, handle_request);
//
//    try
//    {
//        // ��������� ��������� � ���� ����������.
//        listener.open().wait();
//        std::wcout << L"Listening on address: " << listener.uri().to_string() << std::endl;
//
//        // ����� ������ �� ���������� ����� ��, � ������� �����-�� �����
//        std::this_thread::sleep_for(std::chrono::seconds(60000));
//
//        // ��������� ���������.
//        listener.close().wait();
//    }
//    catch (const std::exception& e)
//    {
//        // ��������� � ����� ������, ���� ���-�� ��� �� ���.
//        std::wcerr << e.what() << std::endl;
//    }
//
//    return 0;
//}
//
///*����������:
//������������, ��������� ��, ��� ���������� �� ������ ������, ����� �� �� �������� ����� �� � ����� � ��� � ������ :)
//
//1) ����������������� ����:
//��� ��������, ������, �� � �� ���� ��� �� ������ �������� ����� � ��, ����� �� ����� ��������� ��� ����������. (���������� �������� �� x86)
//�������� ���� ��������� ���, ��������� �������� ������ � �������� ��:
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
//-- ��������� ������ � �����������
//INSERT INTO employees (full_name, position)
//VALUES
//    ('��������� ���������', '�����������'),
//    ('�������� ������', '�����������'),
//    ('������ �����', '��������'),
//    ('�������� ��������', '��������'),
//    ('������ ������', '�����������'),
//    ('��������� ���������', '�����������'),
//    ('������ ����������', '�������� �������'),
//    ('������ �����������', '�����������'),
//    ('������ �����������', '��������'),
//    ('�������� ������', '�����������');
//
//-- ��������� ������ � ��������
//INSERT INTO tasks (task_name, executor_id, due_date, status)
//VALUES
//    ('���������� ������ ���-����������', 2, '2023-10-15', '� ��������'),
//    ('������������ ����������', 7, '2023-09-20', '���������'),
//    ('������ ����� ������������', 5, '2023-09-30', '�� ������'),
//    ('�������� ������� ��� �������', 4, '2023-10-10', '� ��������'),
//    ('������ ������������������ ��������', 6, '2023-09-25', '� ��������'),
//    ('���������� �������', 2, '2023-10-05', '�� ������'),
//    ('������������ �������', 8, '2023-09-15', '���������'),
//    ('����������� ���� ������', 10, '2023-10-08', '�� ������'),
//    ('������ ���������� ���������', 9, '2023-09-10', '� ��������'),
//    ('������������ �����������', 7, '2023-09-09', '�� ������');
//
//2) ���������� ����:
//� ������� ��������� �� ��� ���� � ��� �������� ������� �� �������:
//1.1. ����������� �� - PostgreSQL
//1.2. ������������ ����� ������ ����� � ���������:
//1.2.1. ������� �����������
//1.2.2. ������� �����
//
//2.1 �������� ������ �����������
//2.2. ������� � ������������� "������������" ����� (� ������ ��������� ��������, ��������� - ��������
//2.3. ����������� �������������� ����� (��������� ��������). ��������� ��������. ��������� �������, �� ���������� �� ������.
//���� ���������, �� ����� ��� �� ������������.
//4. ��������� ����� ������������� RestAPI
//
//3) ����������� ����:
//��� �� ������������� �����, ������ � ����������� �������� ���� ���, �� ��� ���. ���������� ����� ��� � ����������� ������,
//���� ��� ����� ����� ���������� (�� ������ ����������)*/