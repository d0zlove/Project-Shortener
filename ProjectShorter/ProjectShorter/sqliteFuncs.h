class sqliteFuncs {
public:
	void createDatabase(const char* dbName);
	void addData(const char* dbName, const char* key, const char* data);
	void deleteData(const char* dbName, const char* key);
	bool checkKeyExists(const char* key);
};
