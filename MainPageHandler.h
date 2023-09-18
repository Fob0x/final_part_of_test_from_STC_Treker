#pragma once

#include <cpprest/http_listener.h>
#include "PostgreSQLdb.h"

class MainPageHandler
{
public:
	// Конструктор класса, принимаюбщий строку подключения к PostgreSQL
	MainPageHandler(const std::string& dbConnectionString);

	/* Метод для обработки HTTP-запросов.
	Принимает объект web::http::http_request, представляющий текущий HTTP-запрос*/
	void HandleRequest(web::http::http_request request);

private:
	std::string dbConnectionString;
};