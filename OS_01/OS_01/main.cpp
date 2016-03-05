#include <windows.h>
#include <stdio.h>
#include "Row.h"

using namespace std;

DWORD WINAPI min_max(LPVOID row);

int main() {
	
	int n = 0;
	scanf("%d", &n);
	
	Row* row = (Row*)malloc(n * sizeof(Row));
	int** a = (int**)malloc(n * sizeof(int*));
	for (int i = 0; i < n; ++i) 
		a[i] = (int*)malloc(n * sizeof(int));

	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) 
			scanf("%d", &a[i][j]);
		row[i].idx = i;
		row[i].r = a[i];
		row[i].max = MININT32;
		row[i].min = MAXINT32;
		row[i].length = n;
	}

	HANDLE *hThreads = new HANDLE[n];
    DWORD  *IDThread = new DWORD[n];

	for (int i = 0; i < n; ++i) {
		hThreads[i] = CreateThread(NULL, 0, min_max, (void*)&row[i], 0, &IDThread[i]);
		if(hThreads[i] == NULL)
			return GetLastError();
	}

	if (WaitForMultipleObjects(n, hThreads, TRUE, INFINITE) == WAIT_FAILED) {
		printf("Wait for multiple objects failed.\n");
		printf("Press any key to exit.\n");
	}

	for(int i = 0; i < n; i++)
		CloseHandle(hThreads[i]);

	int gMax = MININT32;
	int gMin = MAXINT32;
	for (int i = 0; i < n; ++i) {
		if(row[i].min < gMin) 
			gMin = row[i].min;
		if(row[i].max > gMax) 
			gMax = row[i].max;
	}

	printf("Global min = %d, global max = %d\n", gMin, gMax);

	for (int i = 0; i < n; i++) 
		free(a[i]);
	free(a);
	free(row);
	free(hThreads);
	free(IDThread);

	return 0;
}