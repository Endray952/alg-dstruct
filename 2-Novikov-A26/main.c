#include "stdio.h"
#include "list.h"

int ReadFile(list_t* list) {
	FILE* file = fopen("data.txt", "r");
	if (file == NULL) {
		puts("open file error");
		return -1;
	}
	date_t date;
	int temperature;
	while (fscanf(file, "%i.%i.%i;%i", &date.day, &date.month, &date.year, &temperature) == 4) {
		if (AddNodeSorting(list, &date, temperature) < 0) {
			return -1;
		}
	}
	fclose(file);
	return 0;
}

int main() {
	list_t* listTemperatures = ListInitialize();
	if (listTemperatures == NULL) {
		puts("error in list initialization");
		return 0;
	}
	if (ReadFile(listTemperatures) < 0) {
		return 0;
	}
	PrintListTempLower0(listTemperatures);
	int inputTemperature;
	scanf("%i", &inputTemperature);
	CheckTemperature(listTemperatures, inputTemperature);
	return 0;
}