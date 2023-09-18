#pragma once

#include <libpq-fe.h>
#include <string>

class PostgreSQLdatabase
{
public:
	// ����������� ������ (��������� ������ ��� ����������� � PostrgreSQL)
	PostgreSQLdatabase(const std::string& dbConnectionString);

	// ���������� (����������� ������� � ��������� ���������� � ��)
	~PostgreSQLdatabase();

	/* �������� ���������� ���������� � ��.
	���������� true � ������ ��������� �����������, false - � ��������� */
	bool Connect();

	// ��������� �� ��, ��������� �������.
	void Disconnect();

	/*������������� ��������� ������� PostgreSQL.
    ��������� ������ � ������ ���������.
    ���������� true, ���� ��������� ��������� ������ �������, � false - � ����.*/
    bool SetClientEncoding(const std::string& encoding);

	// �������� ��������� �� ���������� � ��
	PGconn* GetConnection() const;

	/* ��������� SQL-������ � ��.
	��������� ������ � ���� ������.
	���������� ��������� ���������� ������� � ���� PGresult* */
	PGresult* ExecuteQuery(const std::string& query);

private:
	// ��������� �� ���������� � ��
	PGconn* connection;
	std::string dbConnectionString;
};