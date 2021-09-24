#include "stdio.h"
#include "list.h"


void ReadFile(list_t* list) {
	FILE* file = fopen("data.txt", "r");
	date_t date;
	int temperature;
	while (fscanf(file, "%i.%i.%i;%i", &date.day, &date.month, &date.year, &temperature) == 4) {
		AddNodeSorting(list, &date, temperature);
	}

	fclose(file);
}

int main() {
	list_t* listTemperatures = ListInitialize();
	ReadFile(listTemperatures);
	PrintListTempLower0(listTemperatures);
	int inputTemperature;
	scanf("%i", &inputTemperature);
	CheckTemperature(listTemperatures, inputTemperature);
	node_t* node = malloc(sizeof(node_t));
	return 0;
}