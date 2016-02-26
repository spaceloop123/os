#include <windows.h>
#include <iostream>
#include <fstream>
#include <limits.h>
#include <math.h>

using namespace std;

struct Result {
	int min;
	int max;

	Result() {
		min = INT_MAX;
		max = INT_MIN;
	}

	Result& operator=(const Result& result) {
		min = result.min;
		max = result.max;
		return *this;
	}

	friend ostream& operator<< (ostream& out, const Result& result){
		out << "min = " << result.min << ", max = " << result.max;
		return out;
	}

	void update(int value) {
		if (value > max) {
			max = value;
		}
		if (value < min) {
			min = value;
		}
	}
};

struct Row {
	int idx;
	int length;
	int* a;
	Result result;
	
	Row() {
		idx = 0;
		length = 0;
		a = NULL;
	}

	Row& operator=(const Row& row) {
		idx = row.idx;
		length = row.length;
		result = row.result;
		delete[] a;
		a = new int[length];
		for (int i = 0; i < row.length; ++i) {
			a[i] = row.a[i];
		}
		return *this;
	}

	friend istream& operator>> (istream& in, Row& row){
		row.a = new int[row.length];
		for (int i = 0; i < row.length; ++i) {
			in >> row.a[i];
		}
		return in;
	}

	friend ostream& operator<< (ostream& out, const Row& row){
		for (int i = 0; i < row.length; ++i) {
			out << row.a[i] << " ";
		}
		return out;
	}

	~Row() {
		delete[] a;
	}
};

struct Matrix {
	int n;
	Row* a;

	friend istream& operator>> (istream& in, Matrix& matrix) {
		cout << "Enter the dimension of the matrix: "; 
		in >> matrix.n;
		matrix.a = new Row[matrix.n];
		cout << "Enter the elements of the matrix: " << endl;
		for (int i = 0; i < matrix.n; ++i) {
			matrix.a[i].idx = i;
			matrix.a[i].length = matrix.n;
			in >> matrix.a[i];
		}
		return in;
	}

	friend ostream& operator<< (ostream& out, const Matrix& matrix){
		for (int i = 0; i < matrix.n; ++i) {
			cout << matrix.a[i] << endl;
		}
		return out;
	}

	~Matrix() {
		delete[] a;
	}
};

DWORD WINAPI min_max(LPVOID row) {
	Row* tRow = (Row*)row;	
	
	for (int i = 0; i < tRow->length; ++i) {
		tRow->result.update(tRow->a[i]);
	}

	Sleep(1000); //7 

	cout << "row " << tRow->idx << ": " << tRow->result << endl;
	return 0;
}

int main() {
	//ifstream in("input.txt");
	Matrix matrix;
	cin >> matrix;
	cout << endl;
	
	HANDLE *hThreads = new HANDLE[matrix.n];
    DWORD  *IDThread = new DWORD[matrix.n];

	Result result;
	
	for (int i = 0; i < matrix.n; ++i) {
		Row* row = &matrix.a[i];

		hThreads[i] = CreateThread(NULL, 0, min_max, (void*)row, 0, &IDThread[i]);
		if(hThreads[i] == NULL)
			return GetLastError();
	}

	if (WaitForMultipleObjects(matrix.n, hThreads, TRUE, INFINITE) == WAIT_FAILED) {
		cout << "Wait for multiple objects failed." << endl;
		cout << "Press any key to exit." << endl;
	}

	for(int i = 0; i < matrix.n; i++) {
		CloseHandle(hThreads[i]);
	}

	for (int i = 0; i < matrix.n; ++i) {
		if (matrix.a[i].result.min < result.min) {
			result.min = matrix.a[i].result.min;
		}
		if (matrix.a[i].result.max > result.max) {
			result.max = matrix.a[i].result.max;
		}
	}

	cout << endl << result << endl;

	return 0;
}