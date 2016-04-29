#include <windows.h>
#include <stdio.h>
#include <stdlib.h> 
#include "Order.h"

#define MAX_FILENAME_LEN 32
#define MAX_CHAR_LEN 256

wchar_t* CREATOR_PATH = L"C:\\Users\\Dell\\Documents\\Visual Studio 2012\\Projects\\OS_02\\Creator\\Debug\\Creator.exe";
wchar_t* REPORTER_PATH = L"C:\\Users\\Dell\\Documents\\Visual Studio 2012\\Projects\\OS_02\\Reporter\\Debug\\Reporter.exe";

using namespace std;

int main() {	
	//***************************************************** 1 ********************************************
	wchar_t* lpszcommandLine = (wchar_t*) malloc(MAX_CHAR_LEN * sizeof(wchar_t));
	wchar_t* binaryFilename = (wchar_t*) malloc(MAX_FILENAME_LEN * sizeof(wchar_t));

	printf("Enter binary filename: ");
	scanf("%S", binaryFilename);
	
	swprintf(lpszcommandLine, MAX_CHAR_LEN, L"%ls %ls", CREATOR_PATH, binaryFilename);

	//***************************************************** 2 ********************************************
	STARTUPINFO si;
	PROCESS_INFORMATION piCom;

	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);

	if(!CreateProcess(NULL, lpszcommandLine, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &piCom)) {
		printf("Process \"Creator\" is not created: %d\n", GetLastError());
		return 0;
	}

	//***************************************************** 3 ********************************************
	WaitForSingleObject(piCom.hProcess, INFINITE);

	CloseHandle(piCom.hThread);
	CloseHandle(piCom.hProcess);

	//***************************************************** 4 ********************************************
	char cBinaryFilename[32];
	int ret = wcstombs(cBinaryFilename, binaryFilename, sizeof(cBinaryFilename));
    if (ret==32) cBinaryFilename[31]='\0';

	FILE *binaryFile = fopen(cBinaryFilename, "rb");
	if (!binaryFile) {
		printf("Unable to open %s\n", cBinaryFilename);
		return 1;
	}

	int n = 0;
	fread(&n, sizeof(int), 1, binaryFile);
	Order* orders = (Order*) malloc(n * sizeof(Order));
	for(int i = 0; i < n; ++i)
		fread(&orders[i], sizeof(struct Order), 1, binaryFile);

	printf("\nThe content of a binary file: \n");
	for(int i = 0; i < n; ++i)
		printf("  %d %s %d %lf\n", orders[i].n, orders[i].name, orders[i].amount, orders[i].price);

	fclose(binaryFile);

	//***************************************************** 5 ********************************************
	wchar_t* reportFilename = (wchar_t*) malloc(MAX_FILENAME_LEN * sizeof(wchar_t));
	int minGoodsAmount = 0;
	int minGoodsCost = 0;

	printf("\nEnter report file name: ");
	scanf("%S", reportFilename);

	printf("\nEnter minimum amount of goods: ");
	scanf("%d", &minGoodsAmount);
	
	printf("\nEnter minimum total cost of goods: ");
	scanf("%d", &minGoodsCost);

	//***************************************************** 6 ********************************************
	swprintf(lpszcommandLine, MAX_CHAR_LEN, L"%ls %ls %ls %d %d", 
		REPORTER_PATH, binaryFilename, reportFilename, minGoodsAmount, minGoodsCost);

	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);

	if(!CreateProcess(NULL, lpszcommandLine, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &piCom)) {
		printf("Process \"Reporter\" is not created: %d\n", GetLastError());
		return 0;
	}
	//***************************************************** 7 ********************************************
	WaitForSingleObject(piCom.hProcess, INFINITE);

	CloseHandle(piCom.hThread);
	CloseHandle(piCom.hProcess);

	//***************************************************** 8 ********************************************
	char cReportFilename[32];
	ret = wcstombs(cReportFilename, reportFilename, sizeof(cReportFilename));
    if (ret==32) cReportFilename[31]='\0';

	FILE *reportFile = fopen(cReportFilename, "r");
	if (!reportFile) {
		printf("Unable to open %s\n", cReportFilename);
		return 1;
	}
	
	printf("\n*** Report ***\n");
	char buffer[100];
	while(!feof(reportFile)) {
		if(fgets(buffer, 100, reportFile) == NULL) break;
        printf("%s", buffer);
    }
	printf("**************\n");
	//***************************************************** 9 ********************************************
	printf("\nFinished\n");

	fclose(reportFile);
	free(reportFilename);

	fclose(binaryFile);
	free(binaryFilename);
		
	free(orders);

	free(lpszcommandLine);

	return 0;
}