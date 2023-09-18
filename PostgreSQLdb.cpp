#include "PostgreSQLdb.h"

// Конструктор класса. Инициализирует указатель на соединение
PostgreSQLdatabase::PostgreSQLdatabase(const std::string& dbConnectionString) : connection(nullptr), dbConnectionString(dbConnectionString) {}

// Деструктор класса. Освобождает ресурсы и закрывает соединение с БД.
PostgreSQLdatabase::~PostgreSQLdatabase() 
{
	Disconnect();
}

bool PostgreSQLdatabase::Connect()
{
	/* Попытка установить соединение с БД,
	используя сохранённую строку подключения*/
	connection = PQconnectdb(dbConnectionString.c_str());

	// Возвращает true, если соединение установлено успешно, false - в противном случае.
	return PQstatus(connection) == CONNECTION_OK;
}

void PostgreSQLdatabase::Disconnect()
{
	// Отключаем соединение с БД, освобождая ресурсы.
	if (connection)
	{
		PQfinish(connection);
		connection = nullptr;
	}
}

bool PostgreSQLdatabase::SetClientEncoding(const std::string& encoding)
{
	// Устанавливает кодировку клиента PostgreSQL. Возвращает true, если установка кодировки прошла успешно, и false в противном случае.
	return PQsetClientEncoding(connection, encoding.c_str()) == 0;
}

PGconn* PostgreSQLdatabase::GetConnection() const
{
	return connection;
}

PGresult* PostgreSQLdatabase::ExecuteQuery(const std::string& query)
{
	/* Выполняет SQL-запрос к БД.
	Принимает запрос в виде строки.
	Возвращает результат выполнения запроса в виде PGresult* */
	return PQexec(connection, query.c_str());
}
