#include <string>

#include "Hive.h"

int main() {
	const char filepath[] = "F:\\C\\Windows\\System32\\config\\SOFTWARE";

	Hive* hive = new Hive(filepath);

	// must end with a forward slash
	std::string key = "CsiTool-CreateHive-{00000000-0000-0000-0000-000000000000}/Apple Inc./Bonjour/DynDNS/Setup/";
	std::cout << "The subkeys of " << key << " are:" << std::endl;
	hive->ListSubkeys(key);

	std::cin.get();
	return 0;
}