#include <iostream>
#include <string>
#include <map>
#include "sqlite/sqlite3.h"


extern "C" {

    // Функция для создания базы данных SQLite и таблицы "data"
    void createDatabase(const char* dbName) {
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
    void addData(const char* dbName, const char* key, const char* data) {
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

    // Функция для получения данных из базы данных по ключу
    const char* getData(const char* dbName, const char* key) {
        sqlite3* db;
        int result = sqlite3_open(dbName, &db);
        if (result != SQLITE_OK) {
            std::cerr << "Ошибка при открытии базы данных: " << sqlite3_errmsg(db) << std::endl;
            sqlite3_close(db);
            return nullptr;
        }
        std::string query = "SELECT data FROM data WHERE key = '" + std::string(key) + "'";
        sqlite3_stmt* stmt;
        result = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
        if (result != SQLITE_OK) {
            std::cerr << "Ошибка при выполнении запроса: " << sqlite3_errmsg(db) << std::endl;
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            return nullptr;
        }
        const char* data = nullptr;
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            data = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        }
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return data;
    }

    // Функция для удаления данных из базы данных по ключу
    void deleteData(const char* dbName, const char* key) {
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


	__declspec(dllexport) const char* shorten_url(int URLOrID, const char* longURl) {

		std::string characters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

		std::string short_id = "";
		srand(time(NULL));

		for (int i = 0; i < 6; i++) {
			int random_index = rand() % characters.length();
			short_id += characters[random_index];
		}

		const char* link = const_cast<char*>("http://127.0.0.1:5000/");//основная строка
		const char* ID = const_cast<char*>(short_id.c_str()); //дополнительная строка

		size_t lenA = std::strlen(link);// длина строки с ссылкой 
		size_t lenB = std::strlen(ID);// длина строки с айди


		char* shortURL = new char[lenA + lenB + 1]; // память под новую строку
		strcpy_s(shortURL, lenA + 1, link); // копируем первую строку
		strcpy_s(shortURL + lenA, lenB + 1, ID); // добавляем вторую строку без пробела в конец

		char* IDresult = new char[lenB + 1]; // память под ID
		strcpy_s(IDresult, lenB + 1, ID);// копиируем ID

		const char* dbName = "database.db";
		createDatabase(dbName);
		addData(dbName, shortURL, longURl);

        /*
		const char* data = getData(dbName, "key1");
		if (data != nullptr) {
			std::cout << "Полученные данные: " << data << std::endl;
		}
      
		deleteData(dbName, "key2");*/

		if (URLOrID == 1)
			return shortURL;
		else if (URLOrID == 0)
			return IDresult;

		return 0;
	}
}