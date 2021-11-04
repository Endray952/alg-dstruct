#pragma once
//#define HandleError(error) _handle_error(error); return 0

//typedef enum {
//	NO_ERROR,
//	MALLOC_ERROR,
//	SCANF_ERROR,
//	EMPTY_LIST
//}error_t;
error_t ReadAdjacencyList();
void _handle_error(error_t error);