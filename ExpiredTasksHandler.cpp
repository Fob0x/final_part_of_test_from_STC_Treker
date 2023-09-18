#include "ExpiredTasksHandler.h"

// Конструктор класса, инициализирующий объект ExpiredTasksHandler c указанием строки подключения к БД
ExpiredTasksHandler::ExpiredTasksHandler(const std::string& dbConnectionString) : dbConnectionString(dbConnectionString) {}

// Метод для обработки HTTP-запросов
void ExpiredTasksHandler::HandleRequest(web::http::http_request request)
{
	PostgreSQLdatabase db(dbConnectionString);

	if (!db.Connect())
	{
		request.reply(web::http::status_codes::InternalError, U("PostgreSQL connection failed"));
		return;
	}

	// Переменные для хранения результата и ответа
	PGresult* res = nullptr;
	std::wstring response_body;

	// Получаем текущую дату и время в формате PostgreSQL
	PGresult* current_time_res = db.ExecuteQuery("SELECT NOW()");
	std::string current_time = PQgetvalue(current_time_res, 0, 0);
	PQclear(current_time_res);

	// Выполняем SQL-запрос к БД для сотрудников с истёкшими сроками задач.
	std::string sql_query = "SELECT * FROM employees e WHERE e.employee_id IN (SELECT s.executor_id FROM tasks s WHERE due_date < '" + current_time + "')";
	res = db.ExecuteQuery(sql_query);

	if (PQresultStatus(res) != PGRES_TUPLES_OK)
	{
		// Если SQL-запрос завершился неудачно, то отправляем HTTP-ответ с кодом ошибки и сообщением о ней.
		std::wstring error_message = L"SQL query error: " + utility::conversions::utf8_to_utf16(PQerrorMessage(db.GetConnection()));
		// L - строка (литерал) в формате широких символов (если коротко, то для работы с wstring)
		request.reply(web::http::status_codes::InternalError, error_message);
		PQclear(res);
		return;
	}

	// Формируем HTML-страницу с данными о сотрудниках с истёкшими сроками задач.
	response_body = L"<html><head><title>Сотрудники с истёкшими сроками выполнения задач</title></head><body><h1>Сотрудники с истёкшими сроками выполнения задач:</h1><ul>";

	int rows = PQntuples(res);
	for (int i = 0; i < rows; ++i)
	{
		std::string id = PQgetvalue(res, i, 0);
		std::wstring w_id = utility::conversions::utf8_to_utf16(id);

		std::string name = PQgetvalue(res, i, 1);
		std::wstring w_name = utility::conversions::utf8_to_utf16(name);

		std::string position = PQgetvalue(res, i, 2);
		std::wstring w_position = utility::conversions::utf8_to_utf16(position);

		// Добавляем информация о сотрудниках к HTML-странице
		response_body += L"<li>Employee ID: " + w_id + L", Full Name: " + w_name + L", Position: " + w_position + L"</li>";
	}

	response_body += L"</ul></body></html>";

	PQclear(res);

	// Отправляем HTML-страницу как ответ
	request.reply(web::http::status_codes::OK, response_body, U("text/html"));
}



