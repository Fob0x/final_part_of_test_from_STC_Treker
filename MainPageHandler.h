#pragma once

#include <cpprest/http_listener.h>
#include "PostgreSQLdb.h"

class MainPageHandler
{
public:
	// ����������� ������, ������������ ������ ����������� � PostgreSQL
	MainPageHandler(const std::string& dbConnectionString);

	/* ����� ��� ��������� HTTP-��������.
	��������� ������ web::http::http_request, �������������� ������� HTTP-������*/
	void HandleRequest(web::http::http_request request);

private:
	std::string dbConnectionString;
};