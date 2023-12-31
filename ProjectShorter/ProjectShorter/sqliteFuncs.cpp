#include <iostream>
#include <string>
#include "sqlite/sqlite3.h"
#include "sqliteFuncs.h"

// Функция для создания базы данных SQLite и таблицы "data"
void sqliteFuncs::createDatabase(const char* dbName) {
	sqlite3* db;
	int result = sqlite3_open(dbName, &db);
	if (result != SQLITE_OK) {
		std::cerr << "Ошибка при создании базы данных: " << sqlite3_errmsg(db) << std::endl;
		sqlite3_close(db);
		return;
	}
	std::string query = "CREATE TABLE IF NOT EXISTS data (key TEXT PRIMARY KEY, data TEXT)";
	result = sqlite3_exec(db, query.c_str(), nullptr, nullptr, nullptr);
	if (result != SQLITE_OK) {
		std::cerr << "Ошибка при создании таблицы: " << sqlite3_errmsg(db) << std::endl;
	}
	else {
		std::cout << "Таблица успешно создана." << std::endl;
	}
	sqlite3_close(db);
}

// Функция для добавления данных в базу данных по ключу
void sqliteFuncs::addData(const char* dbName, const char* key, const char* data) {
	sqlite3* db;
	int result = sqlite3_open(dbName, &db);
	if (result != SQLITE_OK) {
		std::cerr << "Ошибка при открытии базы данных: " << sqlite3_errmsg(db) << std::endl;
		sqlite3_close(db);
		return;
	}
	std::string query = "INSERT INTO data (key, data) VALUES ('" + std::string(key) + "', '" + std::string(data) + "')";
	result = sqlite3_exec(db, query.c_str(), nullptr, nullptr, nullptr);
	if (result != SQLITE_OK) {
		std::cerr << "Ошибка при добавлении данных: " << sqlite3_errmsg(db) << std::endl;
	}
	else {
		std::cout << "Данные успешно добавлены." << std::endl;
	}
	sqlite3_close(db);
}

// Функция для удаления данных из базы данных по ключу
void sqliteFuncs::deleteData(const char* dbName, const char* key) {
	sqlite3* db;
	int result = sqlite3_open(dbName, &db);
	if (result != SQLITE_OK) {
		std::cerr << "Ошибка при открытии базы данных: " << sqlite3_errmsg(db) << std::endl;
		sqlite3_close(db);
		return;
	}
	std::string query = "DELETE FROM data WHERE key = '" + std::string(key) + "'";
	result = sqlite3_exec(db, query.c_str(), nullptr, nullptr, nullptr);
	if (result != SQLITE_OK) {
		std::cerr << "Ошибка при удалении данных: " << sqlite3_errmsg(db) << std::endl;
	}
	else {
		std::cout << "Данные успешно удалены." << std::endl;
	}
	sqlite3_close(db);
}


bool sqliteFuncs::checkKeyExists(const char* key) {
	sqlite3* database;
	sqlite3_open("database.db", &database); // Открываем базу данных SQLite в памяти

	std::string query = "SELECT EXISTS(SELECT 1 FROM data WHERE key = '" + std::string(key) + "')";
	sqlite3_stmt* statement;
	if (sqlite3_prepare_v2(database, query.c_str(), -1, &statement, nullptr) != SQLITE_OK) {
		std::cerr << "Ошибка при подготовке запроса: " << sqlite3_errmsg(database) << std::endl;
		sqlite3_close(database);
		return false;
	}

	int result = 0;
	if (sqlite3_step(statement) == SQLITE_ROW) {
		result = sqlite3_column_int(statement, 0);
	}

	sqlite3_finalize(statement);
	sqlite3_close(database); // Закрываем базу данных SQLite

	return (result == 1);
}

extern "C" {
	// Функция для выполнения запроса и получения результата
	static int callback(void* data, int argc, char** argv, char** azColName) {
		std::string* result = static_cast<std::string*>(data);
		for (int i = 0; i < argc; i++) {
			*result = argv[i];
		}
		return 0;
	}

	// Функция для получения данных по ключу из базы данных SQLite
	__declspec(dllexport) const char* getDataFromDatabase(const char* key) {
		sqlite3* db;
		char* zErrMsg = 0;
		int rc;
		std::string result;

		// Открываем соединение с базой данных
		rc = sqlite3_open("database.db", &db);
		if (rc) {
			std::cerr << "Ошибка при открытии базы данных: " << sqlite3_errmsg(db) << std::endl;
			return nullptr;
		}

		// Формируем SQL-запрос для получения данных по ключу
		std::string sqlQuery = "SELECT data FROM data WHERE key = '" + std::string(key) + "';";

		// Выполняем SQL-запрос
		rc = sqlite3_exec(db, sqlQuery.c_str(), callback, &result, &zErrMsg);
		if (rc != SQLITE_OK) {
			std::cerr << "Ошибка при выполнении запроса: " << zErrMsg << std::endl;
			sqlite3_free(zErrMsg);
		}
		else {
			std::cerr << "Данные успешно найдены." << std::endl;
		}

		// Закрываем соединение с базой данных
		sqlite3_close(db);

		const char* Readyresult = const_cast<char*>(result.c_str());
		size_t lenС = std::strlen(Readyresult);
		char* resultmem = new char[lenС + 1];
		strcpy_s(resultmem, lenС + 1, Readyresult);

		// Возвращаем результат в типе const char*

		return resultmem;
	}
}
