#include <iostream>
#include <string>
#include <map>
#include "sqlite/sqlite3.h"

// Функция для проверки результата выполнения SQL-запроса
int checkResult(int resultCode, char* errMsg) {
    if (resultCode != SQLITE_OK) {
        std::cerr << "Ошибка выполнения SQL: " << errMsg;
        return 1;
    }
    return 0;
}

// Функция для создания базы данных SQLite
int createDatabase(const char* dbName) {
    sqlite3* db;
    int resultCode = sqlite3_open(dbName, &db);
    if (resultCode != SQLITE_OK) {
        std::cerr << "Невозможно открыть базу данных: " << sqlite3_errmsg(db);
        return 1;
    }
    sqlite3_close(db);
    std::cout << "База данных успешно создана.\n";
    return 0;
}

// Функция для добавления данных по ключу в базу данных SQLite
int insertData(const char* dbName, const char* key, const char* data) {
    sqlite3* db;
    int resultCode = sqlite3_open(dbName, &db);
    if (resultCode != SQLITE_OK) {
        std::cerr << "Невозможно открыть базу данных: " << sqlite3_errmsg(db);
        return 1;
    }

    // Создаем SQL-запрос с использованием переданных данных
    std::string sql = "INSERT INTO table_name (key, data) VALUES ('" + std::string(key) + "', '" + std::string(data) + "')";

    char* errMsg;
    resultCode = sqlite3_exec(db, sql.c_str(), NULL, 0, &errMsg);
    if (checkResult(resultCode, errMsg)) {
        sqlite3_close(db);
        return 1;
    }

    sqlite3_close(db);
    std::cout << "Данные успешно добавлены.\n";
    return 0;
}

// Функция для удаления данных по ключу из базы данных SQLite
int deleteData(const char* dbName, const char* key) {
    sqlite3* db;
    int resultCode = sqlite3_open(dbName, &db);
    if (resultCode != SQLITE_OK) {
        std::cerr << "Невозможно открыть базу данных: " << sqlite3_errmsg(db);
        return 1;
    }

    // Создаем SQL-запрос с использованием переданного ключа
    std::string sql = "DELETE FROM table_name WHERE key = '" + std::string(key) + "'";

    char* errMsg;
    resultCode = sqlite3_exec(db, sql.c_str(), NULL, 0, &errMsg);
    if (checkResult(resultCode, errMsg)) {
        sqlite3_close(db);
        return 1;
    }

    sqlite3_close(db);
    std::cout << "Данные успешно удалены.\n";
    return 0;
}

// Функция для получения данных по ключу из базы данных SQLite
int getData(const char* dbName, const char* key) {
    sqlite3* db;
    int resultCode = sqlite3_open(dbName, &db);
    if (resultCode != SQLITE_OK) {
        std::cerr << "Невозможно открыть базу данных: " << sqlite3_errmsg(db);
        return 1;
    }

    // Создаем SQL-запрос с использованием переданного ключа
    std::string sql = "SELECT data FROM table_name WHERE key = '" + std::string(key) + "'";

    // Создаем callback-функцию для обработки полученных данных
    auto callback = [](void* data, int argc, char** argv, char** azColName) -> int {
        for (int i = 0; i < argc; i++) {
            std::cout << azColName[i] << " = " << argv[i] << std::endl;
        }
        return 0;
        };

    char* errMsg;
    resultCode = sqlite3_exec(db, sql.c_str(), callback, 0, &errMsg);
    if (checkResult(resultCode, errMsg)) {
        sqlite3_close(db);
        return 1;
    }

    sqlite3_close(db);
    return 0;
}

/*       
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
        */

int main() {
    const char* dbName = "database.db";
    const char* key = "some_key";
    const char* data = "some_data";

    createDatabase(dbName);
    insertData(dbName, key, data);
    getData(dbName, key);
    deleteData(dbName, key);

    return 0;
}