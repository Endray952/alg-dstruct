#include "stdio.h"
#include "list.h"


void readFile(List* list) {
	FILE* file = fopen("data.txt", "r");
	Date date;
	int temperature ;
	while (fscanf(file, "%i.%i.%i;%i", &date.day, &date.month, &date.year, &temperature ) == 4) {
		addNodeSorting(list, date, temperature);
	}

	fclose(file);
}

int main() {
	List* list_of_temperatures = listInitialize();
	readFile(list_of_temperatures);
	printList(list_of_temperatures);
	puts("");
	printListLT0(list_of_temperatures);
	puts("");
	int find_temperature;
	scanf("%i", &find_temperature);
	checkTemperature(list_of_temperatures, find_temperature);
	return 0;
}