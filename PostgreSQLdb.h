#pragma once

#include <libpq-fe.h>
#include <string>

class PostgreSQLdatabase
{
public:
	// Конструктор класса (принимает строку для подключения к PostrgreSQL)
	PostgreSQLdatabase(const std::string& dbConnectionString);

	// Деструктор (освобождает ресурсы и закрывает соединение с БД)
	~PostgreSQLdatabase();

	/* Пытаемся установить соединение с БД.
	Возвращает true в случае успешного подключения, false - в противном */
	bool Connect();

	// Отключить от БД, освободив ресурсы.
	void Disconnect();

	/*Установливаем кодировку клиента PostgreSQL.
    Принимаем строку с именем кодировки.
    Возвращает true, если установка кодировки прошла успешно, и false - в ином.*/
    bool SetClientEncoding(const std::string& encoding);

	// Получаем указатель на соединение с БД
	PGconn* GetConnection() const;

	/* Выполнить SQL-запрос к БД.
	Принимает запрос в виде строки.
	Возвращает результат выполнения запроса в виде PGresult* */
	PGresult* ExecuteQuery(const std::string& query);

private:
	// Указатель на соединение с БД
	PGconn* connection;
	std::string dbConnectionString;
};