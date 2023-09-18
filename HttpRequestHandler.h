#pragma once

#include <cpprest/http_listener.h>

class HttpRequestHandler
{
public:
	/* Конструктор класса, инициализирующий объект httprequestHandler 
	с указанием строки подключения к бд */
	HttpRequestHandler(const std::string& dbConnectionString);

	/* Метод для обработки HTTP-запросов.
	Принимает объект web::http::http_request request, представляющий текущий HTTP-запрос */
	void HandleRequest(web::http::http_request request);

private:
	// Строка подключения к БД
	std::string dbConnectionString;
};