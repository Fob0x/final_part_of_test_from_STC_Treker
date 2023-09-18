#include "HttpRequestHandler.h"
#include "MainPageHandler.h"
#include "ExpiredTasksHandler.h"
#include "EditTasksHandler.h"

// ����������� ������, �������������� ������ HttpRequestHandler � ��������� ������ ����������� � ��.
HttpRequestHandler::HttpRequestHandler(const std::string& dbConnectionString) : dbConnectionString(dbConnectionString) {}

void HttpRequestHandler::HandleRequest(web::http::http_request request)
{
	// �������� ������ ������� �� URI
	utility::string_t path = request.request_uri().to_string();

	// ��������� ���� ������� ��� ����������� ��������� (�����������) �������
	if (path == U("/expired_tasks"))
	{
		/* ���� ������ ��� ������� "�������� ����������� � �������� ������� ���������� �����", 
		�� ������ ������ ExpiredTasksHandler � �������� ��� ����� ��������� ������� */
		/*ExpiredTasksHandler*/
		ExpiredTasksHandler expiredTasksHandler(dbConnectionString);
		expiredTasksHandler.HandleRequest(request);
	}
	else if (path == U("/edit_tasks"))
	{
		EditTasksHandler editTasksHandler(dbConnectionString);
		editTasksHandler.HandleRequest(request);
	}
	else
	{
		// ���� ������ ��� ������� ��������, ������� ������ MainPageHandler � �������� ��� ����� ��������� �������.
		MainPageHandler mainPageHandler(dbConnectionString);
		mainPageHandler.HandleRequest(request);
	}
}
