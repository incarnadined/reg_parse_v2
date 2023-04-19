#include <memory>
#include <string>

#include "Hive.h"

int main() {
	const char filepath[] = "hives/SYSTEM";
	Hive* hive = new Hive(filepath);


	std::vector<NK*> k = hive->GetSubkeys(L"/");


	NK* Key = hive->GetKey(L"ControlSet001/Enum/USBSTOR/Disk&Ven_Lexar&Prod_USB_Flash_Drive&Rev_1100/AAZ62W7KENRSJLHY&0/Properties/{83da6326-97a6-4088-9453-a1923f573b29}/0064");
	std::vector<VK*> data = Key->GetValues();
	for (int i = 0; i < data.size(); i++)
	{
		std::wcout << data[i]->GetName() << " " << data[i]->GetData() << std::endl;
	}

	std::cout << std::endl;

	// must end with a forward slash
	const char* key = "ControlSet001/Control";
	size_t key_size = 82;

	std::cout << "The subkeys of " << key << " are:" << std::endl;
	std::vector<NK*> keys = hive->GetSubkeys(Helper::CharToWstring(key, key_size));
	for (int i = 0; i < keys.size(); i++)
	{
		std::wcout << keys[i]->GetName() << std::endl;
	}

	std::cout << std::endl;


	std::cout << "The value of " << key << "::LastBootSucceeded is: ";
	VK* value = hive->GetValue(Helper::CharToWstring(key, key_size), L"LastBootSucceeded");

	std::wcout << value->GetData() << std::endl;

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