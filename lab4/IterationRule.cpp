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

vector<double> operator- (vector<double>& v1, vector<double>& v2) {
    int n = v1.size();
    vector<double> result;
    for(int i = 0; i < n; i++) {
        result.push_back(v1[i] - v2[i]);
    }
    return result;
}

double evaluate(vector<double> result) {
    double sum = 0.0;
    for(auto item : result) {
        sum += pow(item, 2);
    }
    return sqrt(sum);
}

vector<double> JocobiIteration(vector<vector<double> > A, vector<double> b, vector<double> Y, double epsilon, int M) {
    cout << "Executing JocobiIteration\n";
	int n = b.size();
    vector<double> result(n, 0.0);
    vector<double> g(n, 0.0);
    double T = 0.0;
    int iter = 1;
    for(int i = 0; i < n; i++) {
        if (fabs(A[i][i]) <= epsilon) {
			cout << "failed to solve" << fabs(A[i][i]) << endl;;
			exit(-1);
		}
        T = A[i][i];
        for(int j = 0; j < n; j++) {
            A[i][j] = -A[i][j] / T;
            A[i][i] = 0;
            g[i] = b[i] / T;
        }
    }
    while(iter <= M) {
        for(int i = 0; i < n; i++) {
            double sum = 0.0;
            for(int j = 0; j < n; j++) {
                if(j != i) sum += A[i][j] * Y[j];
            }
            result[i] = g[i] + sum;
        }
        auto estimate = evaluate(result-Y);
        cout << "iter-" << iter << " estimate : " << estimate << endl;
        if(estimate < epsilon)
        {
            return result;
        }
        iter += 1;
        Y = result;
    }
    cout << "failed to solve" << endl;;
	exit(-1);
}

vector<double> SeidelIteration(vector<vector<double> > A, vector<double> b, vector<double> Y, double epsilon, int M) {
    cout << "Executing SeidelIteration\n";
	int n = b.size();
    vector<double> result = Y;
    vector<double> g(n, 0.0);
    double T = 0.0;
    int iter = 1;
    for(int i = 0; i < n; i++) {
        if (fabs(A[i][i]) <= epsilon) {
			cout << "failed to solve" << fabs(A[i][i]) << endl;;
			exit(-1);
		}
        T = A[i][i];
        for(int j = 0; j < n; j++) {
            A[i][j] = -A[i][j] / T;
            A[i][i] = 0;
            g[i] = b[i] / T;
        }
    }
    while(iter <= M) {
        for(int i = 0; i < n; i++) {
            double sum = 0.0;
            for(int j = 0; j < n; j++) {
                if(j != i) sum += A[i][j] * result[j];
            }
            result[i] = g[i] + sum;
        }
        auto estimate = evaluate(result-Y);
        cout << "iter-" << iter << " estimate : " << estimate << endl;
        if(estimate < epsilon)
        {
            return result;
        }
        iter += 1;
        Y = result;
    }
    cout << "failed to solve" << endl;;
	exit(-1);
}

vector<double> SORIteration(vector<vector<double> > A, vector<double> b, vector<double> Y, double epsilon, int M, double w) {
    cout << "Executing SORIteration\n";
	int n = b.size();
    vector<double> result = Y;
    vector<double> g(n, 0.0);
    double T = 0.0;
    int iter = 1;
    for(int i = 0; i < n; i++) {
        if (fabs(A[i][i]) <= epsilon) {
			cout << "failed to solve" << fabs(A[i][i]) << endl;;
			exit(-1);
		}
        T = A[i][i];
        for(int j = 0; j < n; j++) {
            A[i][j] = - w * A[i][j] / T;
            A[i][i] = 1 - w;
            g[i] = b[i] * w / T;
        }
    }
    while(iter <= M) {
        for(int i = 0; i < n; i++) {
            double sum = 0.0;
            for(int j = 0; j < n; j++) {
                sum += A[i][j] * result[j];
            }
            result[i] = g[i] + sum;
        }
        auto estimate = evaluate(result-Y);
        cout << "iter-" << iter << " estimate : " << estimate << endl;
        if(estimate < epsilon)
        {
            return result;
        }
        iter += 1;
        Y = result;
    }
    cout << "failed to solve" << endl;;
	exit(-1);
}

int main(int argc, char** argv) {
	// parse command
    string file_name = argv[1];
    int useMethod = atoi(argv[2]);

	// main variables
	vector<vector<double> > A;
	vector<double> b;
    vector<double> Y;
	vector<double> result;
	double epsilon;
    double w;
    uint32_t M;
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
	auto b_set = split(line, " ");
	for(int i = 0; i < n; i++) {
		b.push_back(static_cast<double>(stof(b_set[i])));
	}
	getline(in, line);
	auto Y_set = split(line, " ");
	for(int i = 0; i < n; i++) {
		Y.push_back(static_cast<double>(stof(Y_set[i])));
	}
	getline(in, line);
	epsilon = static_cast<double>(stof(line));
	getline(in, line);
	M = stoi(line);
    if(useMethod == 3) {
        getline(in, line);
        w = static_cast<double>(stof(line));
    }
	// use method
	switch(useMethod) {
		case 1:
			result = JocobiIteration(A, b, Y, epsilon, M);
			break;
        case 2:
			result = SeidelIteration(A, b, Y, epsilon, M);
			break;
        case 3:
            result = SORIteration(A, b, Y, epsilon, M, w);
            break;
		default:
			break;
	}

	// print
    cout << "\nresult : ";
	for(int i = 0; i < n; i++) {
		cout << result[i] << " ";
	}
	cout << endl;
}
