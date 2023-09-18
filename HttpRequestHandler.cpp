#include "HttpRequestHandler.h"
#include "MainPageHandler.h"
#include "ExpiredTasksHandler.h"
#include "EditTasksHandler.h"

// Конструктор класса, инициализирует объект HttpRequestHandler с указанием строки подключения к бд.
HttpRequestHandler::HttpRequestHandler(const std::string& dbConnectionString) : dbConnectionString(dbConnectionString) {}

void HttpRequestHandler::HandleRequest(web::http::http_request request)
{
	// Получаем строка запроса из URI
	utility::string_t path = request.request_uri().to_string();

	// Проверяем путь запроса для определения требуемой (запрошенной) вкладки
	if (path == U("/expired_tasks"))
	{
		/* Если запрос для вкладки "Показать сотрудников с истёкшими сроками выполнения задач", 
		то создаём объект ExpiredTasksHandler и вызываем его метод обработки запроса */
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
		// Если запрос для главной страницы, создаем объект MainPageHandler и вызываем его метод обработки запроса.
		MainPageHandler mainPageHandler(dbConnectionString);
		mainPageHandler.HandleRequest(request);
	}
}
