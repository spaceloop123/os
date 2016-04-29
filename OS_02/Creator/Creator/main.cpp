#include <windows.h>
#include <stdio.h>
#include "Order.h"

using namespace std;

int main(int argc, char* argv[]) {

	printf("Enter number of records: ");
	int n = 0;
	scanf("%d", &n);

	printf("Enter records (format = int, char[10], int, double): \n");
	Order* orders = (Order*) malloc(n * sizeof(Order));
	for (int i = 0; i < n; ++i)
		scanf("%d%s%d%lf", &orders[i].n, orders[i].name, &orders[i].amount, &orders[i].price);

	FILE *binaryFile = fopen(argv[1], "wb");
	if (!binaryFile) {
		printf("Unable to open %s", argv[1]);
		return 1;
	}

	fwrite(&n, sizeof(int), 1, binaryFile);
	for (int i = 0; i < n; ++i)
		fwrite(&orders[i], sizeof(struct Order), 1, binaryFile);
	
	fclose(binaryFile);

	free(orders);

	return 0;
}