#include <vector>
#include "ExcelDriver\VectorsAndMatrices\Vector.hpp"
#include "ExcelDriver\ExcelDriver.hpp"
#include "ExcelDriver\ExcelMechanisms.hpp"
#include "ExcelDriver\VectorsAndMatrices\Matrix.hpp"
using namespace std;

// Define Matrix to store the information returned by readTxt()
// And Matrix can also be used to store AAR, CAAR.
template <typename T>
using MyMatrix = vector<vector<T>>;


vector<double> operator*(const vector<double>& v, double c);

vector<double> operator+(const vector<double>& v1, const vector<double>& v2);

ostream & operator<<(ostream & out, const MyMatrix<double> m);

ostream & operator<<(ostream & out, const vector<double> v);

vector<double> operator-(const vector<double>& v, const vector<double>& w);

Vector<double, long> trans_vec(const vector<double>& v, long N);