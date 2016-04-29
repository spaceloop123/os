#include <windows.h>
#include <stdio.h>
#include "Order.h"

using namespace std;

int main(int argc, char* argv[]) {	

	FILE *binaryFile = fopen(argv[1], "rb");
	if (!binaryFile) {
		printf("Unable to open %s\n", argv[1]);
		return 1;
	}

	int n = 0;
	fread(&n, sizeof(int), 1, binaryFile);
	Order* orders = (Order*) malloc(n * sizeof(Order));
	for(int i = 0; i < n; ++i)
		fread(&orders[i], sizeof(struct Order), 1, binaryFile);
	
	fclose(binaryFile);

	FILE *reportFile = fopen(argv[2], "w");
	if (!reportFile) {
		printf("Unable to open %s\n", argv[2]);
		return 1;
	}

	fprintf(reportFile, "Binary file name: %s\n", argv[1]);

	int minGoodsAmount = atoi(argv[3]);
	int minGoodsCost = atoi(argv[4]);
	printf("%d %d\n", minGoodsAmount, minGoodsCost);
	for(int i = 0; i < n; ++i) {
		double cost = orders[i].amount * orders[i].price;
		if((orders[i].amount < minGoodsAmount) || (cost < minGoodsCost)) 
			fprintf(reportFile, "%d %s %d %lf\n", orders[i].n, orders[i].name, orders[i].amount, cost);
	}

	fclose(reportFile);

	free(orders);

	return 0;
}