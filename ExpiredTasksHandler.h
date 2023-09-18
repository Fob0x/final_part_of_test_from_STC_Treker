#pragma once

#include "PostgreSQLdb.h"
#include <cpprest/http_listener.h>

class ExpiredTasksHandler
{
public:
	// Конструктор класса, принимающий строку подключения к БД
	ExpiredTasksHandler(const std::string& dbConnectionString);

	/* Метод для обработки HTTP-запросов.
	Принимает объект web::http::http_request, представляющий текущий HTTP-запрос */
	void HandleRequest(web::http::http_request request);

private:
	// Строка для подключения к БД
	std::string dbConnectionString;
};