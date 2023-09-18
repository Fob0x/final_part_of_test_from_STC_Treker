#pragma once

#include <cpprest/http_listener.h>

class HttpRequestHandler
{
public:
	/* ����������� ������, ���������������� ������ httprequestHandler 
	� ��������� ������ ����������� � �� */
	HttpRequestHandler(const std::string& dbConnectionString);

	/* ����� ��� ��������� HTTP-��������.
	��������� ������ web::http::http_request request, �������������� ������� HTTP-������ */
	void HandleRequest(web::http::http_request request);

private:
	// ������ ����������� � ��
	std::string dbConnectionString;
};