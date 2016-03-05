#include <windows.h>
#include <stdio.h>
#include "Row.h"

using namespace std;

DWORD WINAPI min_max(LPVOID row) {
	Row* tRow = (Row*)row;	
	
	for (int i = 0; i < tRow->length; ++i) {
		if(tRow->r[i] < tRow->min) 
			tRow->min = tRow->r[i];
		if(tRow->r[i] > tRow->max) 
			tRow->max = tRow->r[i];
	}

	Sleep(7);

	printf("row %d : min = %d, max = %d\n", tRow->idx, tRow->min, tRow->max);

	return 0;
}