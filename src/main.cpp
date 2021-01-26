#include <memory>
#include <string>

#include "Hive.h"

int main() {
	const char filepath[] = "G:\\Coding Projects\\reg_parse_v2\\SYSTEM.hive";
	Hive* hive = new Hive(filepath);

	// must end with a forward slash
	const char* key = "CsiTool-CreateHive-{00000000-0000-0000-0000-000000000000}/ControlSet001/Control/";
	size_t key_size = 82;

	std::cout << "The subkeys of " << key << " are:" << std::endl;
	std::vector<NK*> keys = hive->GetSubkeys(Helper::CharToWstring(key, key_size));
	for (int i = 0; i < keys.size(); i++)
	{
		std::wcout << keys[i]->GetName() << std::endl;
	}

	std::cout << std::endl;


	std::cout << "The value of " << key << "::LastBootSucceeded is:" << std::endl;
	VK* value = hive->GetValue(Helper::CharToWstring(key, key_size), L"LastBootSucceeded");

	std::wcout << value->GetName() << std::endl;

	std::cout << std::endl;


	std::cout << "The values of " << key << " are:" << std::endl;
	std::vector<VK*> values = hive->GetValues(Helper::CharToWstring(key, key_size));
	for (int i = 0; i < values.size(); i++)
	{
		std::wcout << values[i]->GetName() << L" - Type=" << int(values[i]->GetType()) << L" - Resident=" << values[i]->GetResidence() << std::endl;
	}

	std::cin.get();

	delete hive;
}