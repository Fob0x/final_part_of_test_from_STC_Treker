#include "PostgreSQLdb.h"

// ����������� ������. �������������� ��������� �� ����������
PostgreSQLdatabase::PostgreSQLdatabase(const std::string& dbConnectionString) : connection(nullptr), dbConnectionString(dbConnectionString) {}

// ���������� ������. ����������� ������� � ��������� ���������� � ��.
PostgreSQLdatabase::~PostgreSQLdatabase() 
{
	Disconnect();
}

bool PostgreSQLdatabase::Connect()
{
	/* ������� ���������� ���������� � ��,
	��������� ���������� ������ �����������*/
	connection = PQconnectdb(dbConnectionString.c_str());

	// ���������� true, ���� ���������� ����������� �������, false - � ��������� ������.
	return PQstatus(connection) == CONNECTION_OK;
}

void PostgreSQLdatabase::Disconnect()
{
	// ��������� ���������� � ��, ���������� �������.
	if (connection)
	{
		PQfinish(connection);
		connection = nullptr;
	}
}

bool PostgreSQLdatabase::SetClientEncoding(const std::string& encoding)
{
	// ������������� ��������� ������� PostgreSQL. ���������� true, ���� ��������� ��������� ������ �������, � false � ��������� ������.
	return PQsetClientEncoding(connection, encoding.c_str()) == 0;
}

PGconn* PostgreSQLdatabase::GetConnection() const
{
	return connection;
}

PGresult* PostgreSQLdatabase::ExecuteQuery(const std::string& query)
{
	/* ��������� SQL-������ � ��.
	��������� ������ � ���� ������.
	���������� ��������� ���������� ������� � ���� PGresult* */
	return PQexec(connection, query.c_str());
}
