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

double calcFunction(double input) {
    return sqrt(input);
}

double TrapezoidalRule(double a, double b, double n) {
    double sum = 0;
    double h = (b-a) / n;
    for (int k = 0; k < n; k++) {
        sum += calcFunction(a + k * h);
    }
    double T = (calcFunction(a) + calcFunction(b) + 2*sum) * h / 2;
    return T;
}

double SimpsonRule(double a, double b, double n) {
    double h = (b-a) / n;
    double m = n / 2;
    double S1 = 0, S2 = 0;
    for (int k = 1; k <= m; k++) {
        S1 += calcFunction(a + (2*k-1)*h);
    }
    for (int k = 1; k < m; k++) {
        S2 += calcFunction(a + 2*k*h);
    }
    double S = (calcFunction(a) + calcFunction(b) + 4*S1 + 2*S2) * h / 3;
    return S;
}

double RombergRule(double a, double b, double epsilon) {
    vector<vector<double> > T;
    int k = 1;
    T.resize(k);
    T[0].push_back((b-a) / 2 * (calcFunction(a) + calcFunction(b)));
    while(true) {
        cout << "current-k " << k;
        T.resize(k + 1);
        auto last = *(T[0].end() - 1);
        double sum = 0.0;
        for(int i = 1; i <= pow(2, k-1); i++) {
            sum += calcFunction(a + (2*i-1)*(b-a)/pow(2,k));
        }
        T[0].push_back((last + (b-a) * sum / pow(2, k-1)) / 2);
        for(int i = 1; i <= k; i++) {
            auto last_1 = *(T[i-1].end() - 1);
            auto last_2 = *(T[i-1].end() - 2);
            T[i].push_back((pow(4, i) * last_1 - last_2) / (pow(4, i) - 1));
        }
        auto left = *(T[k].end() - 1);
        auto right = *(T[k-1].end() - 2);
        auto estimate = fabs(left - right);
        cout << "\testimate : " << estimate << endl;
        if(estimate < epsilon) {
            return left;
        }
        k += 1;
    }

}

int main(int argc, char** argv) {

	// parse command
    string file_name = argv[1];
    int useMethod = atoi(argv[2]);

    // main variables
    double a, b, n, epsilon;

    string line;
    ifstream in(file_name);
    getline(in, line);
    auto set = split(line, " ");
    a = static_cast<double>(stof(set[0]));
    b = static_cast<double>(stof(set[1]));
    n = static_cast<double>(stof(set[2]));
    if(useMethod == 3) {
        getline(in, line);
        epsilon = static_cast<double>(stof(line));
    }

    // use method
    double result = 0.0;
	switch(useMethod) {
		case 1:
			result = TrapezoidalRule(a, b, n);
			break;
		case 2:
			result = SimpsonRule(a, b, n);
			break;
		case 3:
            result = RombergRule(a, b, epsilon);
			break;
		default:
			break;
	}
    cout << "result : " << result << endl;
}
