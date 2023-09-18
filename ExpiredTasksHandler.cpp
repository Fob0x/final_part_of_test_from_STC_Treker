#include "ExpiredTasksHandler.h"

// ����������� ������, ���������������� ������ ExpiredTasksHandler c ��������� ������ ����������� � ��
ExpiredTasksHandler::ExpiredTasksHandler(const std::string& dbConnectionString) : dbConnectionString(dbConnectionString) {}

// ����� ��� ��������� HTTP-��������
void ExpiredTasksHandler::HandleRequest(web::http::http_request request)
{
	PostgreSQLdatabase db(dbConnectionString);

	if (!db.Connect())
	{
		request.reply(web::http::status_codes::InternalError, U("PostgreSQL connection failed"));
		return;
	}

	// ���������� ��� �������� ���������� � ������
	PGresult* res = nullptr;
	std::wstring response_body;

	// �������� ������� ���� � ����� � ������� PostgreSQL
	PGresult* current_time_res = db.ExecuteQuery("SELECT NOW()");
	std::string current_time = PQgetvalue(current_time_res, 0, 0);
	PQclear(current_time_res);

	// ��������� SQL-������ � �� ��� ����������� � �������� ������� �����.
	std::string sql_query = "SELECT * FROM employees e WHERE e.employee_id IN (SELECT s.executor_id FROM tasks s WHERE due_date < '" + current_time + "')";
	res = db.ExecuteQuery(sql_query);

	if (PQresultStatus(res) != PGRES_TUPLES_OK)
	{
		// ���� SQL-������ ���������� ��������, �� ���������� HTTP-����� � ����� ������ � ���������� � ���.
		std::wstring error_message = L"SQL query error: " + utility::conversions::utf8_to_utf16(PQerrorMessage(db.GetConnection()));
		// L - ������ (�������) � ������� ������� �������� (���� �������, �� ��� ������ � wstring)
		request.reply(web::http::status_codes::InternalError, error_message);
		PQclear(res);
		return;
	}

	// ��������� HTML-�������� � ������� � ����������� � �������� ������� �����.
	response_body = L"<html><head><title>���������� � �������� ������� ���������� �����</title></head><body><h1>���������� � �������� ������� ���������� �����:</h1><ul>";

	int rows = PQntuples(res);
	for (int i = 0; i < rows; ++i)
	{
		std::string id = PQgetvalue(res, i, 0);
		std::wstring w_id = utility::conversions::utf8_to_utf16(id);

		std::string name = PQgetvalue(res, i, 1);
		std::wstring w_name = utility::conversions::utf8_to_utf16(name);

		std::string position = PQgetvalue(res, i, 2);
		std::wstring w_position = utility::conversions::utf8_to_utf16(position);

		// ��������� ���������� � ����������� � HTML-��������
		response_body += L"<li>Employee ID: " + w_id + L", Full Name: " + w_name + L", Position: " + w_position + L"</li>";
	}

	response_body += L"</ul></body></html>";

	PQclear(res);

	// ���������� HTML-�������� ��� �����
	request.reply(web::http::status_codes::OK, response_body, U("text/html"));
}



