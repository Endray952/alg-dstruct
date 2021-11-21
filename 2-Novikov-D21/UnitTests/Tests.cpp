#define _CRT_SECURE_NO_WARNINGS
#include "gtest/gtest.h"
extern "C" {
#include "HamiltonianPath.h"
}
TEST(FindHamiltonianPath_Test, give_hamiltonian_path_expect_order_of_visits) {
	FILE* input = fopen("input.txt", "w");
	char adjList[30] = "3\n1 2 3\n2\n3\n\0";
	fprintf(input, "%s", adjList);
	fclose(input);
	FindHamiltonianPath();
	FILE* output = fopen("output.txt", "r");
	int result[3];
	for (int i = 0; i < 3; i++){
		fscanf(output, "%i ", &result[i]);
	}
	fclose(output);
	EXPECT_TRUE((result[0] == 2 && result[1] == 1 && result[2] == 3) || (result[0] == 3 && result[1] == 1 && result[2] == 2));
}
TEST(FindHamiltonianPath_Test, give_non_hamiltonian_path_expect_zero) {
	FILE* input = fopen("input.txt", "w");
	char adjList[30] = "4\n1 2 3 4\n2\n3\n4\n\0";
	fprintf(input, "%s", adjList);
	fclose(input);
	FindHamiltonianPath();
	FILE* output = fopen("output.txt", "r");
	int result = 0;
	char c;
	fscanf(output, "%i", &result);
	fclose(output);
	EXPECT_TRUE(result == 0);
}
TEST(FindHamiltonianPath_Test, give_non_connected_graph_expect_zero) {
	FILE* input = fopen("input.txt", "w");
	char adjList[30] = "4\n1\n2 3 4\n3 4\n4\n\0";
	fprintf(input, "%s", adjList);
	fclose(input);
	FindHamiltonianPath();
	FILE* output = fopen("output.txt", "r");
	int result = 0;
	char c;
	fscanf(output, "%i", &result);
	fclose(output);
	EXPECT_TRUE(result == 0);
}
