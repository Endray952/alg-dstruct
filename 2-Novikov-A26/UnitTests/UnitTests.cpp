#include "gtest/gtest.h"
extern "C" {
#include "list.h"
}
#define ADD_NODE_BEFORE_TWO_NODES \
node_t nodeInList_1;\
node_t nodeInList_2;\
list_t list;\
list.head = &nodeInList_1;\
nodeInList_1.next = &nodeInList_2;\
nodeInList_1.prev = NULL;\
nodeInList_2.next = NULL;\
nodeInList_2.prev = &nodeInList_1;\
node_t insertingNode;

#define ADD_NODE_AFTER_TWO_NODES \
node_t nodeInList_1;\
node_t nodeInList_2;\
list_t list;\
list.head = &nodeInList_1;\
nodeInList_1.next = &nodeInList_2;\
nodeInList_1.prev = NULL;\
nodeInList_2.next = NULL;\
nodeInList_2.prev = &nodeInList_1;\
node_t insertingNode;
TEST(ListInitialize_Test, check_if_not_return_null) {
	list_t* list = ListInitialize();
	EXPECT_TRUE(list != NULL);
	EXPECT_TRUE(list->head == NULL);
}
TEST(AddNodeInEmptyList_Test, check_if_list_is_not_empty_after_adding_node) {
	list_t list;
	list.head = NULL;
	node_t node;
	AddNodeInEmptyList(&list, &node);
	EXPECT_TRUE(list.head != NULL);
	EXPECT_TRUE(node.next == NULL);
	EXPECT_TRUE(node.prev == NULL);
}
TEST(AddNodeBefore_Test, list_has_one_node) {
	node_t nodeInList;
	list_t list;
	list.head = &nodeInList;
	nodeInList.next = NULL;
	nodeInList.prev = NULL;
	node_t insertingNode;

	AddNodeBefore(&list, &nodeInList, &insertingNode);

	EXPECT_TRUE(list.head == &insertingNode);
	EXPECT_TRUE(insertingNode.next == &nodeInList);
	EXPECT_TRUE(insertingNode.prev == NULL);
	EXPECT_TRUE(nodeInList.next == NULL);
	EXPECT_TRUE(nodeInList.prev == &insertingNode);
}
TEST(AddNodeBefore_Test, list_has_two_nodes_insert_before_first_element) {
	ADD_NODE_BEFORE_TWO_NODES
	AddNodeBefore(&list, &nodeInList_1, &insertingNode);

	EXPECT_TRUE(list.head == &insertingNode);
	EXPECT_TRUE(insertingNode.next == &nodeInList_1);
	EXPECT_TRUE(insertingNode.prev == NULL);
	EXPECT_TRUE(nodeInList_1.next == &nodeInList_2);
	EXPECT_TRUE(nodeInList_1.prev == &insertingNode);
	EXPECT_TRUE(nodeInList_2.next == NULL);
	EXPECT_TRUE(nodeInList_2.prev == &nodeInList_1);
}
TEST(AddNodeBefore_Test, list_has_two_nodes_insert_before_second_element) {
	ADD_NODE_BEFORE_TWO_NODES
	AddNodeBefore(&list, &nodeInList_2, &insertingNode);

	EXPECT_TRUE(list.head == &nodeInList_1);
	EXPECT_TRUE(insertingNode.next == &nodeInList_2);
	EXPECT_TRUE(insertingNode.prev == &nodeInList_1);
	EXPECT_TRUE(nodeInList_1.next == &insertingNode);
	EXPECT_TRUE(nodeInList_1.prev == NULL);
	EXPECT_TRUE(nodeInList_2.next == NULL);
	EXPECT_TRUE(nodeInList_2.prev == &insertingNode);
}
TEST(AddNodeBefore_Test, list_has_three_nodes_insert_before_second_element) {
	node_t nodeInList_1;
	node_t nodeInList_2;
	node_t nodeInList_3;
	list_t list;
	list.head = &nodeInList_1;

	nodeInList_1.next = &nodeInList_2;
	nodeInList_1.prev = NULL;

	nodeInList_2.next = &nodeInList_3;
	nodeInList_2.prev = &nodeInList_1;

	nodeInList_3.next = NULL;
	nodeInList_3.prev = &nodeInList_2;

	node_t insertingNode;

	AddNodeBefore(&list, &nodeInList_2, &insertingNode);

	EXPECT_TRUE(list.head == &nodeInList_1);

	EXPECT_TRUE(insertingNode.next == &nodeInList_2);
	EXPECT_TRUE(insertingNode.prev == &nodeInList_1);

	EXPECT_TRUE(nodeInList_1.next == &insertingNode);
	EXPECT_TRUE(nodeInList_1.prev == NULL);

	EXPECT_TRUE(nodeInList_2.next == &nodeInList_3);
	EXPECT_TRUE(nodeInList_2.prev == &insertingNode);

	EXPECT_TRUE(nodeInList_3.next == NULL);
	EXPECT_TRUE(nodeInList_3.prev == &nodeInList_2);
}
TEST(AddNodeAfter_Test, list_has_one_node) {
	node_t nodeInList;
	list_t list;
	list.head = &nodeInList;
	nodeInList.next = NULL;
	nodeInList.prev = NULL;
	node_t insertingNode;

	AddNodeAfter(&list, &nodeInList, &insertingNode);

	EXPECT_TRUE(list.head == &nodeInList);

	EXPECT_TRUE(nodeInList.prev == NULL);
	EXPECT_TRUE(nodeInList.next == &insertingNode);

	EXPECT_TRUE(insertingNode.next == NULL);
	EXPECT_TRUE(insertingNode.prev == &nodeInList);
}
TEST(AddNodeAfter_Test, list_has_two_nodes_inset_after_first) {
	ADD_NODE_AFTER_TWO_NODES
	AddNodeAfter(&list, &nodeInList_1, &insertingNode);

	EXPECT_TRUE(list.head == &nodeInList_1);

	EXPECT_TRUE(nodeInList_1.prev == NULL);
	EXPECT_TRUE(nodeInList_1.next == &insertingNode);

	EXPECT_TRUE(insertingNode.next == &nodeInList_2);
	EXPECT_TRUE(insertingNode.prev == &nodeInList_1);

	EXPECT_TRUE(nodeInList_2.prev == &insertingNode);
	EXPECT_TRUE(nodeInList_2.next == NULL);
}
TEST(AddNodeAfter_Test, list_has_two_nodes_inset_after_second) {
	ADD_NODE_AFTER_TWO_NODES
		AddNodeAfter(&list, &nodeInList_2, &insertingNode);

	EXPECT_TRUE(list.head == &nodeInList_1);

	EXPECT_TRUE(nodeInList_1.prev == NULL);
	EXPECT_TRUE(nodeInList_1.next == &nodeInList_2);

	EXPECT_TRUE(nodeInList_2.prev == &nodeInList_1);
	EXPECT_TRUE(nodeInList_2.next == &insertingNode);

	EXPECT_TRUE(insertingNode.next == NULL);
	EXPECT_TRUE(insertingNode.prev == &nodeInList_2);
}
TEST(AddNodeAfter_Test, list_has_three_nodes_inset_after_second) {
	node_t nodeInList_1;
	node_t nodeInList_2;
	node_t nodeInList_3;
	list_t list;
	list.head = &nodeInList_1;
	nodeInList_1.next = &nodeInList_2;
	nodeInList_1.prev = NULL;

	nodeInList_2.next = &nodeInList_3;
	nodeInList_2.prev = &nodeInList_1;

	nodeInList_3.next = NULL;
	nodeInList_3.prev = &nodeInList_2;

	node_t insertingNode;

	AddNodeAfter(&list, &nodeInList_2, &insertingNode);

	EXPECT_TRUE(list.head == &nodeInList_1);

	EXPECT_TRUE(nodeInList_1.prev == NULL);
	EXPECT_TRUE(nodeInList_1.next == &nodeInList_2);

	EXPECT_TRUE(nodeInList_2.prev == &nodeInList_1);
	EXPECT_TRUE(nodeInList_2.next == &insertingNode);

	EXPECT_TRUE(insertingNode.next == &nodeInList_3);
	EXPECT_TRUE(insertingNode.prev == &nodeInList_2);

	EXPECT_TRUE(nodeInList_3.prev == &insertingNode);
	EXPECT_TRUE(nodeInList_3.next == NULL);
}
TEST(AddNodeSorting_Test, empty_list) {
	date_t date = { 1,1,2000 };
	int temperature = 1;
	list_t list;
	list.head = NULL;

	AddNodeSorting(&list, &date, temperature);

	EXPECT_TRUE(list.head != NULL);

	EXPECT_EQ(list.head->temperature, temperature);
	EXPECT_EQ(list.head->date.day, date.day);
	EXPECT_EQ(list.head->date.month, date.month);
	EXPECT_EQ(list.head->date.year, date.year);

	EXPECT_TRUE(list.head->next == NULL);
	EXPECT_TRUE(list.head->prev == NULL);
}
#define ADD_NODE_SORTING_ONE_NODE(day, month, year, temp)\
date_t date = {1,1,2000 };\
int temperature = 1;\
node_t nodeInList;\
nodeInList.date = date;\
nodeInList.temperature = temperature;\
list_t list;\
list.head = &nodeInList;\
nodeInList.next = NULL;\
nodeInList.prev = NULL;\
date_t dateAdd = { day,month,year};\
int temperatureAdd = temp;\
AddNodeSorting(&list, &dateAdd, temperatureAdd);\
node_t* iterNode = list.head;\
int n = 0;\
while (iterNode != NULL) {\
    iterNode = iterNode->next;\
    n++;\
}
TEST(AddNodeSorting_Test, list_with_one_node_add_new_node_with_higher_temperature_after) {
	ADD_NODE_SORTING_ONE_NODE(1, 1, 2021, 10)

	EXPECT_EQ(n, 2);

	EXPECT_TRUE(list.head != NULL);
	EXPECT_EQ(list.head, &nodeInList);
	EXPECT_TRUE(nodeInList.next != NULL);
	EXPECT_TRUE(nodeInList.prev == NULL);
	EXPECT_TRUE(nodeInList.next->next == NULL);
	EXPECT_EQ(nodeInList.next->prev, &nodeInList);

	EXPECT_EQ(nodeInList.next->temperature, temperatureAdd);
	EXPECT_EQ(nodeInList.next->date.day, dateAdd.day);
	EXPECT_EQ(nodeInList.next->date.month, dateAdd.month);
	EXPECT_EQ(nodeInList.next->date.year, dateAdd.year);

}
TEST(AddNodeSorting_Test, list_with_one_node_add_new_node_with_lower_temperature_before) {
	ADD_NODE_SORTING_ONE_NODE(1, 1, 2021, -10)

	EXPECT_EQ(n, 2);

	EXPECT_TRUE(list.head != NULL);
	EXPECT_TRUE(nodeInList.next == NULL);
	EXPECT_TRUE(nodeInList.prev != NULL);
	EXPECT_EQ(list.head, nodeInList.prev);
	EXPECT_TRUE(nodeInList.prev->prev == NULL);
	EXPECT_EQ(nodeInList.prev->next, &nodeInList);

	EXPECT_EQ(nodeInList.prev->temperature, temperatureAdd);
	EXPECT_EQ(nodeInList.prev->date.day, dateAdd.day);
	EXPECT_EQ(nodeInList.prev->date.month, dateAdd.month);
	EXPECT_EQ(nodeInList.prev->date.year, dateAdd.year);
}
TEST(AddNodeSorting_Test, list_with_one_node_add_new_node_with_equal_temperature_but_later_after) {
	ADD_NODE_SORTING_ONE_NODE(1, 1, 2021, 1)

	EXPECT_EQ(n, 2);

	EXPECT_TRUE(list.head != NULL);
	EXPECT_EQ(list.head, &nodeInList);
	EXPECT_TRUE(nodeInList.next != NULL);
	EXPECT_TRUE(nodeInList.prev == NULL);
	EXPECT_TRUE(nodeInList.next->next == NULL);
	EXPECT_EQ(nodeInList.next->prev, &nodeInList);

	EXPECT_EQ(nodeInList.next->temperature, temperatureAdd);
	EXPECT_EQ(nodeInList.next->date.day, dateAdd.day);
	EXPECT_EQ(nodeInList.next->date.month, dateAdd.month);
	EXPECT_EQ(nodeInList.next->date.year, dateAdd.year);

}
TEST(AddNodeSorting_Test, list_with_one_node_add_new_node_with_equal_temperature_but_earlier_before) {
	ADD_NODE_SORTING_ONE_NODE(1, 1, 1970, 1)

	EXPECT_EQ(n, 2);

	EXPECT_TRUE(list.head != NULL);
	EXPECT_TRUE(nodeInList.next == NULL);
	EXPECT_TRUE(nodeInList.prev != NULL);
	EXPECT_EQ(list.head, nodeInList.prev);
	EXPECT_TRUE(nodeInList.prev->prev == NULL);
	EXPECT_EQ(nodeInList.prev->next, &nodeInList);

	EXPECT_EQ(nodeInList.prev->temperature, temperatureAdd);
	EXPECT_EQ(nodeInList.prev->date.day, dateAdd.day);
	EXPECT_EQ(nodeInList.prev->date.month, dateAdd.month);
	EXPECT_EQ(nodeInList.prev->date.year, dateAdd.year);
}
