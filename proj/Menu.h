#pragma once
#ifndef Menu_H
#define Menu_H

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
#include <algorithm>
#include <random>
#include <chrono>
#include "operator_overloading.h"

#include "Bootstrap.h"


//Option1: Retrieve historical price data for all stocks + Option6: Process
void Option1() {
	pool = retrieve_all_multithread();
}

//Option2: Pull information for one stock from one group
void Option2() {
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

//Option3: Show AAR or CAAR for one group
void Option3() {
	Bootstrap b(pool);
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

//Option4: Show the Excel graph with CAAR for all 3 groups
void Option4(){}

//Option5: Exit the program
void Option5() {
	system("exit");
}


#endif