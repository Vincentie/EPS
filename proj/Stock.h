#pragma once

#ifndef Stock_h
#define Stock_h
#include <vector>
#include <string>
#include <map>

using namespace std;

class Stock {
private:
	string symbol;
	string start_date;
	string end_date;
	string bchmk_symbol;

	int T = 241;
	vector<double> price; // store symbol's price from start_date to end_date.
	vector<double> bchmk_price;
	vector<double> AR;

private:
	/* When a vector of prices is given to fill the
	   price vector, check if s meets the required length. */
	
	void convert_time();

	void retrieve(string retri_opt);
	
	/* Use retrive method to retrive symbol's price
	   (from start_date to end_date) using libcurl and
       then set price vector. */
	void set_price();

	/* Use retrive method to retrive benchmark price
	   (from start_date to end_date) using libcurl and
	   then set bchmk_price vector. */
	void set_bchmk_price();

public:
	static string sCrumb;
	static string sCookies;
	static map<string, double> tt_bchmk;
	Stock();
	Stock(string symbol_, string start_date_, string end_date_, string bchmk_symbol_);

	Stock(const Stock & s);

	/* Set a vector of abnormal returns to AR
	   (from start_date to end_date) of this stock.*/
	void set_AR();
	
	/* Returns a vector of daily returns (from start_date to end_date)
	   of this stock or IWB. */
	vector<double> calculate_return(const vector<double> & s);

public:
	string get_symbol() { return symbol; };
	string get_bchmrk_symbol() { return bchmk_symbol; };
	string get_start_date() { return start_date; };
	string get_end_date() { return end_date; };
	vector<double>& get_price() { return price; };
	vector<double>& get_bchmk_price() { return bchmk_price; };
	vector<double>& get_AR() { return AR; }
};

#endif