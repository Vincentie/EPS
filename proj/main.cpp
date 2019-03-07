#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <vector>

#include <string>
#include "Stock.h"
#include "Retrieve.h"
#include "Divide.hpp"

#include <cstdlib>
#include <ctime>
#include "operator_overloading.h"

#include "Bootstrap.h"



using namespace std;

int main() {
	map<string, Stock> pool;
	Bootstrap b;

	//Menu
	cout << "Press 1 to retrieve historical price data for all stocks" << endl;
	cout << "Press 2 to pull information for one stock from one group" << endl;
	cout << "Press 3 to do bootstrap" << endl;
	cout << "Press 4 to show AAR or CAAR for one group" << endl;
	cout << "Press 5 to show the Excel graph with CAAR for all 3 groups" << endl;
	cout << "Press 6 to exit the program" << endl;

	while (true)
	{
		char c; 
		cout << "select an option:";
		c = getchar();
		if (c == '1')
		{
			pool = retrieve_all_multithread();
		}
		if (c == '2')
		{
			if (pool.size() == 0) {
				pool = retrieve_all_multithread();
			}
			string StockSymbol;
			cout << "Please enter stock symbol: ";
			cin >> StockSymbol;
			if (pool.count(StockSymbol) > 0)
			{
				MyMatrix<string> matrix = readTxt("eps.txt");
				map<string, vector<string>>s_info = PutInMap(matrix);
				Stock s = pool[StockSymbol];
				cout << "Stock Symbol: " << s.get_symbol() << endl;
				cout << "Start Day: " << s_info[StockSymbol][0] << endl;
				cout << "End Day: " << s_info[StockSymbol][1] << endl;
				cout << "Estimate_EPS: " << s_info[StockSymbol][2] << endl;
				cout << "Actual_EPS: " << s_info[StockSymbol][3] << endl;
				cout << "Announcement Date: " << s_info[StockSymbol][4] << endl;
				cout << "241 days prices: " << endl;
				cout << s.get_price() << endl;
			}
			else {
				cout << "Invalid Stock Symbol Input..." << endl;
			}
		}
		if (c == '3')
		{
			if (pool.size() == 0) {
				pool = retrieve_all_multithread();
			}
			b = Bootstrap(pool);
		}
		if (c == '4')
		{
			// if not retrieve data yet, do the retrieve
			if (pool.size() == 0) {
				pool = retrieve_all_multithread();
			}
			// if not bootstrap yet, do the bootstrap
			if (b.get_AAR().size() == 0) {
				b = Bootstrap(pool);
			}
			cout << "Please enter group name (Beat, Meet or Miss): ";
			string Group;
			cin >> Group;
			cout << "Please enter AAR or CAAR: ";
			string type;
			cin >> type;
			if (Group == "Beat") {
				cout << "Beat:" << endl;
				if (type == "AAR") {
					cout << "AAR: " << endl;
					cout << (b.get_AAR())[0] << endl;
				}
				else if (type == "CAAR") {
					cout << "CAAR: " << endl;
					cout << (b.get_CAAR())[0] << endl;
				}
				else {
					cout << "Invaild Return Type Input...";
				}
			}
			else if (Group == "Meet") {
				if (type == "AAR") {
					cout << "AAR: " << endl;
					cout << (b.get_AAR())[1] << endl;
				}
				else if (type == "CAAR") {
					cout << "CAAR: " << endl;
					cout << (b.get_CAAR())[1] << endl;
				}
				else {
					cout << "Invaild Return Type Input...";
				}
			}
			else if (Group == "Miss") {
				if (type == "AAR") {
					cout << "AAR: " << endl;
					cout << (b.get_AAR())[2] << endl;
				}
				else if (type == "CAAR") {
					cout << "CAAR: " << endl;
					cout << (b.get_CAAR())[2] << endl;
				}
				else {
					cout << "Invaild Return Type Input...";
				}
			}
			else {
				cout << "Invaild Group Name Input...";
			}
		}
		if (c == '5')
		{
			// if not retrieve data yet, do the retrieve
			if (pool.size() == 0) {
				pool = retrieve_all_multithread();
			}
			// if not bootstrap yet, do the bootstrap
			if (b.get_AAR().size() == 0) {
				b = Bootstrap(pool);
			}
			long N = 240;
			Vector<double, long> xarr(N + 1);
			for (long i = 1; i < N + 1; i++) {
				xarr[i] = i - 120;
			}
			list< Vector<double, long >>yResult;
			yResult.push_back(trans_vec(b.get_CAAR()[0], N));
			yResult.push_back(trans_vec(b.get_CAAR()[1], N));
			yResult.push_back(trans_vec(b.get_CAAR()[2], N));

			list<string>VectorLable({ "Beat","Meet","Miss" });

			ExcelDriver& excel = ExcelDriver::Instance();
			excel.MakeVisible(true);
			printInExcel(xarr, VectorLable, yResult, string("CAAR"), string("Days to Report day"), string("CAAR"));
		}
		if (c == '6')
		{
			break;
		}
	}
	
	return 0;
}