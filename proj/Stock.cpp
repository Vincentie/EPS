#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <locale>
#include <iomanip>
#include "Stock.h"
#include "Retrieve.h"
#include "curl.h"
#include "operator_overloading.h"

#pragma warning(disable:4996)

using namespace std;


string Stock::sCrumb = "";
string Stock::sCookies = "";
map<string, double> Stock::tt_bchmk = map<string, double>();

Stock::Stock() {

}

Stock::Stock(string symbol_, string start_date_, string end_date_, string bchmk_symbol_):
	symbol(symbol_), start_date(start_date_), end_date(end_date_), bchmk_symbol(bchmk_symbol_) {

	convert_time();

	if (symbol == bchmk_symbol) {
		retrieve("bchmk");
	}
	else {
		set_price();
		set_bchmk_price();
		set_AR();
	}
}

Stock::Stock(const Stock & s): 
	symbol(s.symbol), start_date(s.start_date), end_date(s.end_date), bchmk_symbol(s.bchmk_symbol){
	price = s.price;
	bchmk_price = s.bchmk_price;
	AR = s.AR;
}

void Stock::convert_time() {
	start_date.append("T16:00:00");
	end_date.append("T16:00:00");
	start_date = getTimeinSeconds(start_date);
	end_date = getTimeinSeconds(end_date);
}


void Stock::set_price() {
	retrieve("stock");
}

void Stock::set_bchmk_price() {
	if (tt_bchmk.empty()) {
		cout << "Benchmark object has to be created initially." << endl;
	}
	for (auto it = tt_bchmk.find(start_date); it != (++tt_bchmk.find(end_date)); it++)
		bchmk_price.push_back(it->second);	
}


vector<double> Stock::calculate_return(const vector<double> & s) {
	vector<double> ret(s.size() - 1);
	for (unsigned int i = 0; i < (s.size() - 1); i++) {
		ret[i] = (s[i + 1] - s[i]) / s[i];
	}
	return ret;
}

void Stock::set_AR() {
	vector<double> ret = calculate_return(price);
	vector<double> b_ret = calculate_return(bchmk_price);
	AR = ret - b_ret;
};


void Stock::retrieve(string retri_opt) {
	struct MemoryStruct data;
	data.memory = NULL;
	data.size = 0;

	/* to open cookies.txt */
	FILE *fp;

	/* declaration of an object CURL */
	CURL *handle;

	/* result of the whole process */
	CURLcode result;

	/* the first functions */
	/* set up the program environment that libcurl needs */
	//curl_global_init(CURL_GLOBAL_ALL);

	/* curl_easy_init() returns a CURL easy handle that you're gonna reuse in other easy functions*/
	handle = curl_easy_init();

	/* if everything's all right with the easy handle... */
	if (handle)
	{
		while (true)
		{
			
			if (sCookies.length() == 0 || sCrumb.length() == 0)
			{
				curl_easy_setopt(handle, CURLOPT_URL, "https://finance.yahoo.com/quote/AMZN/history");
				curl_easy_setopt(handle, CURLOPT_SSL_VERIFYPEER, 0);
				curl_easy_setopt(handle, CURLOPT_SSL_VERIFYHOST, 0);
				curl_easy_setopt(handle, CURLOPT_COOKIEJAR, "cookies.txt");
				curl_easy_setopt(handle, CURLOPT_COOKIEFILE, "cookies.txt");

				curl_easy_setopt(handle, CURLOPT_ENCODING, "");

				curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data2);
				curl_easy_setopt(handle, CURLOPT_WRITEDATA, (void *)&data);

				// perform, then store the expected code in 'result'
				result = curl_easy_perform(handle);

				// Check for errors
				if (result != CURLE_OK)
				{
					// if errors have occured, tell us what is wrong with 'result'
					fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(result));
					break;
				}



				char cKey[] = "CrumbStore\":{\"crumb\":\"";
				char *ptr1 = strstr(data.memory, cKey);
				char *ptr2 = ptr1 + strlen(cKey);
				char *ptr3 = strstr(ptr2, "\"}");
				if (ptr3 != NULL)
					*ptr3 = NULL;

				sCrumb = ptr2;

				fp = fopen("cookies.txt", "r");
				char cCookies[100];
				if (fp)
				{
					while (fscanf(fp, "%s", cCookies) != EOF);
					fclose(fp);
				}
				else
					cerr << "cookies.txt does not exists" << endl;

				sCookies = cCookies;
				free(data.memory);
				data.memory = NULL;
				data.size = 0;
			}

			/* Below is to use cookies obtained above and the stock's info to retrieve data. */

			string urlA = "https://query1.finance.yahoo.com/v7/finance/download/";

			string urlB = "?period1=";
			string urlC = "&period2=";
			string urlD = "&interval=1d&events=history&crumb=";
			string url = urlA + symbol + urlB + start_date + urlC + end_date + urlD + sCrumb;
			
			const char * cURL = url.c_str();
			const char * cookies = sCookies.c_str();

			curl_easy_setopt(handle, CURLOPT_SSL_VERIFYPEER, 0);
			curl_easy_setopt(handle, CURLOPT_SSL_VERIFYHOST, 0);
			curl_easy_setopt(handle, CURLOPT_COOKIEFILE, "cookies.txt");

			curl_easy_setopt(handle, CURLOPT_COOKIE, cookies);
			curl_easy_setopt(handle, CURLOPT_URL, cURL);
			
			// write in memory
			curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data2);
			curl_easy_setopt(handle, CURLOPT_WRITEDATA, (void *)&data);
			result = curl_easy_perform(handle);

			/* Check for errors */
			if (result != CURLE_OK)
			{
				/* if errors have occurred, tell us what is wrong with 'result'*/
				fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(result));
				break;
			}

			stringstream sData;
			sData.str(data.memory);
			string sValue, sDate;
			double dValue = 0;

			string line;
			getline(sData, line);

			if (retri_opt == "bchmk")
			{
				while (getline(sData, line))
				{
					sDate = line.substr(0, line.find_first_of(','));
					line.erase(line.find_last_of(','));
					sValue = line.substr(line.find_last_of(',') + 1);
					dValue = strtod(sValue.c_str(), NULL);
					
					sDate.append("T16:00:00");
					tt_bchmk[getTimeinSeconds(sDate)] = dValue;
				}
				//cout << "Benchmark successful!" << endl;
			} else {
				while (getline(sData, line))
				{
					line.erase(line.find_last_of(','));
					sValue = line.substr(line.find_last_of(',') + 1);
					dValue = strtod(sValue.c_str(), NULL);
					price.push_back(dValue);
				}
			}
			break;
		}
		free(data.memory);
		data.size = 0;
	}
	else
	{
		fprintf(stderr, "Curl init failed!\n");
	}

	/* cleanup since you've used curl_easy_init */
	curl_easy_cleanup(handle);

	/* this function releases resources acquired by curl_global_init() */
	// curl_global_cleanup();
}
