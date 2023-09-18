#pragma once

#include "PostgreSQLdb.h"
#include <cpprest/http_listener.h>

class ExpiredTasksHandler
{
public:
	// ����������� ������, ����������� ������ ����������� � ��
	ExpiredTasksHandler(const std::string& dbConnectionString);

	/* ����� ��� ��������� HTTP-��������.
	��������� ������ web::http::http_request, �������������� ������� HTTP-������ */
	void HandleRequest(web::http::http_request request);

private:
	// ������ ��� ����������� � ��
	std::string dbConnectionString;
};