#include "MainPageHandler.h"

MainPageHandler::MainPageHandler(const std::string& dbConnectionString) : dbConnectionString(dbConnectionString) {}

void MainPageHandler::HandleRequest(web::http::http_request request)
{
	// Инициализация PostgreSQL соедниениея
	PostgreSQLdatabase db(dbConnectionString);

	if (!db.Connect())
	{
		request.reply(web::http::status_codes::InternalError, U("PostgreSQL connection failed"));
		return;
	}

	// Переменные для хранения результата и ответа.
	PGresult* res = nullptr;
	std::wstring response_body;

	// Формируем HTML-страницу для главной страницы
	response_body = L"<html><head><title>Список сотрудников</title></head><body><h1>Список сотрудников:</h1>"
					L"<a href='/expired_tasks'>Показать сотрудников с истекшими сроками выполнения задач</a>"
					L"<a href='/edit_tasks'>Редактировать задачи</a>"
					L"<ul>";

	// Запрос с последующим выводов сотрудников на главной странице
	std::string sql_squery = "SELECT * FROM employees";
	res = db.ExecuteQuery(sql_squery);

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

	// Отправляем HTML-страницу
	request.reply(web::http::status_codes::OK, response_body, U("text/html"));
}
