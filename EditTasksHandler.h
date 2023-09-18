#pragma once

#include "PostgreSQLdb.h"
#include <cpprest/http_listener.h>

class EditTasksHandler
{
public:
	EditTasksHandler(const std::string& dbConnectionString);

	void HandleRequest(web::http::http_request request);

private:
	std::string dbConnectionString;
};