#include "Hive.h"

int main() {
	const char filepath[] = "F:\\C\\Windows\\System32\\config\\SOFTWARE";

	Hive* hive = new Hive(filepath);

	return 0;
}