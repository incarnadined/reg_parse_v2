#include <string>

#include "Hive.h"

int main() {
	const char filepath[] = "F:\\C\\Windows\\System32\\config\\SAM";

	Hive* hive = new Hive(filepath);

	std::string key = "CsiTool-CreateHive-{00000000-0000-0000-0000-000000000000}/SAM";
	hive->ListSubkeys(key);

	return 0;
}