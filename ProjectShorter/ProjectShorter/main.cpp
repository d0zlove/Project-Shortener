#include <iostream>
#include <string>
#include "sqlite/sqlite3.h"
#include "sqliteFuncs.h"

#define ADRESS "http://127.0.0.1:8000/"
#define CHARACTERS "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"

const char* createSURL(int numvalue) {
	std::string characters = CHARACTERS;

	std::string short_id = "";
	srand(static_cast<unsigned int>(time(NULL)));

	for (int i = 0; i < numvalue; i++) {
		int random_index = rand() % characters.length();
		short_id += characters[random_index];
	}

	const char* link = const_cast<char*>(ADRESS);//основная строка
	const char* ID = const_cast<char*>(short_id.c_str()); //дополнительная строка

	size_t URLlen = std::strlen(link);// длина строки с ссылкой 
	size_t IDlen = std::strlen(ID);// длина строки с айди


	char* shortURL = new char[URLlen + IDlen + 1]; // память под новую строку
	strcpy_s(shortURL, URLlen + 1, link); // копируем первую строку
	strcpy_s(shortURL + URLlen, IDlen + 1, ID); // добавляем вторую строку без пробела в конец

	return shortURL;

}


extern "C" {
	__declspec(dllexport) const char* shorten_url(const char* longURl) {
		sqliteFuncs sf;

		const char* shortURL = createSURL(3);

		if (sf.checkKeyExists(shortURL)) {
			do
			{
				for (int i = 0; i < 5; i++) {
					std::cerr << "Ключ уже сущeствует." << std::endl;
					createSURL(3 + i);
					std::cerr << "Создан новый." << std::endl;
					if (!sf.checkKeyExists(shortURL))
						break;
				}
			} while (sf.checkKeyExists(shortURL));
		}
		else
		{
			std::cerr << "Ключ еще не сущeствует." << std::endl;
		}

		const char* dbName = "database.db";
		sf.createDatabase(dbName);//закоментировать перед продом
		sf.addData(dbName, shortURL, longURl);

		/*
		const char* data = getData(dbName, "key1");
		if (data != nullptr) {
			std::cout << "Полученные данные: " << data << std::endl;
		}

		deleteData(dbName, "key2");*/


		return shortURL;
	}
}

