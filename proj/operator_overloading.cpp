#include "operator_overloading.h"
#include <iomanip>

vector<double> operator*(const vector<double>& v, double c) {
	
	vector<double> temp(v.size());
	for (unsigned int j = 0; j < v.size(); j++) temp[j] = v[j] * c;

	return temp;
}

vector<double> operator+(const vector<double>& v1, const vector<double>& v2) {
	vector<double> temp(v1.size());
	for (unsigned int j = 0; j < v1.size(); j++) temp[j] = v1[j] + v2[j];

	return temp;
}

ostream & operator<<(ostream & out, const MyMatrix<double> m) {
	for (unsigned int i = 0; i < m.size(); i++) {
		for (unsigned int j = 0; j < m[i].size(); j++) out << m[i][j] << " ";
		out << endl;
	}
	return out;
}

ostream & operator<<(ostream & out, const vector<double> v) {
	for (unsigned int i = 0; i < v.size(); i++) {
		if (i % 6 == 0) out << endl;
		out << v[i] << "\t";
	}
	return out;
}

vector<double> operator-(const vector<double>& v, const vector<double>& w) {
	vector<double> temp(v.size());
	for (unsigned int i = 0; i < v.size(); i++) temp[i] = v[i] - w[i];
	return temp;
}

Vector<double, long> trans_vec(const vector<double>& v, long N) {
	Vector<double,long> V(N);
	for (int i = V.MinIndex();i <= V.MaxIndex();i++) {
		V[i] = v[i-V.MinIndex()];
	}
	return V;
}