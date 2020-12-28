#pragma once

class Hive
{
public:
	Hive(const char* filepath);
	~Hive();


	// auto here should be replaced with a data type that contains the value data (struct)
	auto GetValue(char* keypath, char* valuename);
	auto GetValues(char* keypath);

private:

};

