#include <iostream>
#include <cmath>
#include <vector>
#include <fstream>
#include <string>

using std::ifstream;
using std::vector;
using std::cout;
using std::endl;
using std::string;

vector<string> split(const string &s, const string &seperator) {
	vector<string> result;
	typedef string::size_type string_size;
	string_size i = 0;

	while (i != s.size()) {
		//找到字符串中首个不等于分隔符的字母；
		int flag = 0;
		while (i != s.size() && flag == 0) {
			flag = 1;
			for (string_size x = 0; x < seperator.size(); ++x)
				if (s[i] == seperator[x]) {
					++i;
					flag = 0;
					break;
				}
		}

		//找到又一个分隔符，将两个分隔符之间的字符串取出；
		flag = 0;
		string_size j = i;
		while (j != s.size() && flag == 0) {
			for (string_size x = 0; x < seperator.size(); ++x)
				if (s[j] == seperator[x]) {
					flag = 1;
					break;
				}
			if (flag == 0)
				++j;
		}
		if (i != j) {
			result.push_back(s.substr(i, j - i));
			i = j;
		}
	}
	return result;
}

void selectMax(vector<vector<double> >& toSelect, int startRow, int endRow, int startCol, int endCol, int& selectRow, int& selectCol) {
	double maxValue = 0.0;
	for(int i = startRow; i <= endRow; i++) {
		for(int j = startCol; j <= endCol; j++) {
			if(fabs(toSelect[i][j]) >maxValue) {
				selectRow = i;
				selectCol = j;
				maxValue = fabs(toSelect[i][j]);
			}
		}
	}
}

void swap(vector<vector<double> >& toSwap, int start, int end, int method) {
	if(method == 1) {// swap rows
		auto temp = toSwap[start];
		toSwap[start] = toSwap[end];
		toSwap[end] = temp;
	}
	else if(method == 2) {
		for (auto& item : toSwap) {
			auto temp = item[start];
			item[start] = item[end];
			item[end] = temp;
		}
	}
}

void swap(vector<double>& toSwap, int start, int end) {
	auto temp = toSwap[start];
	toSwap[start] = toSwap[end];
	toSwap[end] = temp;
}

vector<double> OrderGaussianElimination(vector<vector<double> > A, vector<double> b, double epsilon) {
	cout << "Executing OrderGaussianElimination\n";
	int n = b.size();
	vector<double> result(n, 0.0);
	double T;
	for (int k = 0; k < n - 1; k++) {
		if (fabs(A[k][k]) <= epsilon) {
			cout << "failed to solve" << fabs(A[k][k]) << endl;;
			exit(-1);
		}
		for (int i = k + 1; i < n; i++) {
			T = A[i][k] / A[k][k];
			b[i] = b[i] - T * b[k];
			for(int j = k + 1; j < n; j++) {
				A[i][j] = A[i][j] - T * A[k][j];
			}
		}
	}
	if (fabs(A[n-1][n-1]) <= epsilon) {
		cout << "failed to solve\n";
		exit(-1);
	}
	result[n-1] = b[n-1] / A[n-1][n-1];
	for (int i = n-2; i >= 0; i--) {
		double sum = 0.0;
		for(int j = i + 1; j < n; j++) {
			sum += A[i][j] * result[j];
		}
		result[i] = (b[i]-sum) / A[i][i];
	}
	return result;
}

vector<double> ColumnGaussianElimination(vector<vector<double> > A, vector<double> b, double epsilon) {
	cout << "Executing ColumnGaussianElimination\n";
	int n = b.size();
	vector<double> result(n, 0.0);
	double T;
	for(int k = 0; k < n-1; k++) {
		int rowIdx, colIdx;
		selectMax(A, k, n-1, k, k, rowIdx, colIdx);
		if(fabs(A[rowIdx][colIdx]) <= epsilon) {
			cout << "failed to solve" << fabs(A[k][k]) << endl;;
			exit(-1);
		}
		if(rowIdx != k) {
			swap(A, rowIdx, k, 1);
			swap(b, rowIdx, k);
		}
		for(int i = k + 1; i < n; i++) {
			T = A[i][k] / A[k][k];
			b[i] = b[i] - T * b[k];
			for(int j = k + 1; j < n; j++) {
				A[i][j] = A[i][j] - T * A[k][j];
			}
		}
	}
	if (fabs(A[n-1][n-1]) <= epsilon) {
		cout << "failed to solve\n";
		exit(-1);
	}
	result[n-1] = b[n-1] / A[n-1][n-1];
	for (int i = n-2; i >= 0; i--) {
		double sum = 0.0;
		for(int j = i + 1; j < n; j++) {
			sum += A[i][j] * result[j];
		}
		result[i] = (b[i]-sum) / A[i][i];
	}
	return result;
}

vector<double> AllGaussianElimination(vector<vector<double> > A, vector<double> b, double epsilon) {
	cout << "Executing AllGaussianElimination\n";
	int n = b.size();
	vector<double> result(n, 0.0);
	vector<double> d(n, 0.0);
	vector<double> Z(n, 0.0);
	double T;
	for(int i = 0; i < n; i++) {
		d[i] = i;
	}
	for(int k = 0; k < n-1; k++) {
		int rowIdx, colIdx;
		selectMax(A, k, n-1, k, n-1, rowIdx, colIdx);
		if(fabs(A[rowIdx][colIdx]) <= epsilon) {
			cout << "failed to solve" << fabs(A[k][k]) << endl;;
			exit(-1);
		}
		if(rowIdx != k) {
			swap(A, rowIdx, k, 1);
			swap(b, rowIdx, k);
		}
		if(colIdx != k) {
			swap(A, colIdx, k, 2);
			swap(d, colIdx, k);
		}
		for(int i = k + 1; i < n; i++) {
			T = A[i][k] / A[k][k];
			b[i] = b[i] - T * b[k];
			for(int j = k + 1; j < n; j++) {
				A[i][j] = A[i][j] - T * A[k][j];
			}
		}
	}
	Z[n-1] = b[n-1] / A[n-1][n-1];
	for(int i = n-2; i >= 0; i--) {
		double sum = 0;
		for(int j = i + 1; j < n; j++) {
			sum += A[i][j] * Z[j];
		}
		Z[i] = (b[i] - sum) / A[i][i];
	}
	for(int j = 0; j < n; j++) {
		result[d[j]] = Z[j];
	}
	return result;
}

int main(int argc, char** argv) {
	// parse command
    string file_name = argv[1];
    int useMethod = atoi(argv[2]);

	// main variables
	vector<vector<double> > A;
	vector<double> b;
	vector<double> result;
	double epsilon;
	uint32_t n;

	// read dataset
	string line;
    ifstream in(file_name);
    getline(in, line);
	n = stoi(line);
	A.resize(n);
	for(int i = 0; i < n; i++) {
		getline(in, line);
		auto set = split(line, " ");
		for(int j = 0; j < n; j++) {
			A[i].push_back(static_cast<double>(stof(set[j])));
		}
	}
	getline(in, line);
	auto set = split(line, " ");
	for(int i = 0; i < n; i++) {
		b.push_back(static_cast<double>(stof(set[i])));
	}
	getline(in, line);
	epsilon = static_cast<double>(stof(line));
	// use method
	switch(useMethod) {
		case 1:
			result = OrderGaussianElimination(A, b, epsilon);
			break;
		case 2:
			result = ColumnGaussianElimination(A, b, epsilon);
			break;
		case 3:
			result = AllGaussianElimination(A, b, epsilon);
			break;
		default:
			break;
	}

	// print
	for(int i = 0; i < n; i++) {
		cout << result[i] << " ";
	}
	cout << endl;
}
