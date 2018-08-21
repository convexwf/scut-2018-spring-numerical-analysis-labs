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

int main(int argc, char** argv)
{
    // parse command
    string file_name = argv[1];
    int useMethod = atoi(argv[2]);

    // main variables
    vector<double> param_x, param_y, param_h, param_alpha, param_beta, param_m, param_a, param_b;
    double xx, m0, mn;
    uint32_t n;

    // read dataset
    string line;
    ifstream in(file_name);
    getline(in, line);
    auto x_set = split(line, " ");
    for(auto item : x_set) {
        param_x.push_back(static_cast<double>(stof(item)));
    }
    getline(in, line);
    auto y_set = split(line, " ");
    for(auto item : y_set) {
        param_y.push_back(static_cast<double>(stof(item)));
    }
    getline(in, line);
    xx = static_cast<double>(stof(line));
    if(useMethod == 1) {
        getline(in, line);
        auto m_set = split(line, " ");
        m0 =  static_cast<double>(stof(m_set[0]));
        mn =  static_cast<double>(stof(m_set[1]));
    }

    // calculate h, alpah, beta
    n = param_x.size() - 1;
    for(int i = 0; i < n; i++) {
        param_h.push_back(param_x[i+1] - param_x[i]);
    }
    if(useMethod == 1) {
        param_alpha.push_back(0.0);
        param_beta.push_back(2 * m0);
    }
    else {
        param_alpha.push_back(1.0);
        param_beta.push_back(3 * (param_y[1]-param_y[0])/param_h[0]);
    }
    for (int i = 1; i < n; i++) {
        param_alpha.push_back(param_h[i-1] / (param_h[i-1] + param_h[i]));
        param_beta.push_back( 3 * (
            (1-param_alpha[i])/param_h[i-1]*(param_y[i]-param_y[i-1]) +
            param_alpha[i]/param_h[i]*(param_y[i+1]-param_y[i])
              ));
    }
    if(useMethod == 1) {
        param_alpha.push_back(1.0);
        param_beta.push_back(2 * mn);
    }
    else {
        param_alpha.push_back(0.0);
        param_beta.push_back(3 * (param_y[n]-param_y[n-1])/param_h[n-1]);
    }

    // calculate a,b
    param_a.push_back(-param_alpha[0]/2);
    param_b.push_back(param_beta[0]/2);
    for (int i = 1; i <= n; i++) {
        param_a.push_back(-param_alpha[i] / (2+(1-param_alpha[i])*param_a[i-1]));
        param_b.push_back((param_beta[i]-(1-param_alpha[i])*param_b[i-1]) / (2+(1-param_alpha[i])*param_a[i-1]));
    }

    // calculate m
    param_m.resize(n+2);
    param_m[n+1] = 0.0;
    for(int i = n; i >= 0; i--) {
        param_m[i] = param_a[i]*param_m[i+1] + param_b[i];
    }

    // calculate s(x)
    int index = 0;
    for(index = 0; index < n; index++) {
        double pre = xx - param_x[index];
        double after = xx - param_x[index+1];
        if(pre * after <= 0) break;
    }
    double x_i = param_x[index];
    double x_i_1 = param_x[index+1];
    double s_x = (1+2*(xx-x_i)/(x_i_1-x_i))*pow((xx-x_i_1)/(x_i-x_i_1),2)*param_y[index] +
            (1+2*(xx-x_i_1)/(x_i-x_i_1))*pow((xx-x_i)/(x_i_1-x_i),2)*param_y[index+1] +
            (xx-x_i)*pow((xx-x_i_1)/(x_i-x_i_1),2)*param_m[index] +
            (xx-x_i_1)*pow((xx-x_i)/(x_i_1-x_i),2)*param_m[index+1];
    cout << "result : s(xx) = " << s_x << endl;
}
