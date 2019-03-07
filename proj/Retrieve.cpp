
#include "Retrieve.h"
#include <stdio.h>
#include <string>
#include <iostream>
#include <sstream>
#include <locale>
#include <iomanip>

#include <thread>
#include <mutex>


#pragma warning(disable:4996)

#include "Divide.hpp"
#include "operator_overloading.h"
#include "curl.h"

using namespace std;

string getTimeinSeconds(string Time)
{
	std::tm t = { 0 };
	std::istringstream ssTime(Time);
	char time[100];
	memset(time, 0, 100); // Clear to NULL
	if (ssTime >> std::get_time(&t, "%Y-%m-%dT%H:%M:%S"))
	{
		//cout << std::put_time(&t, "%c %Z") << "\n" << std::mktime(&t) << "\n";
		sprintf(time, "%lld", mktime(&t));
		return string(time);
	}
	else
	{
		cout << "Parse failed\n";
		return "";
	}
}

void *myrealloc(void *ptr, size_t size)
{
    /* There might be a realloc() out there that doesn't like reallocing
     NULL pointers, so we take care of it here */
    if (ptr)
        return realloc(ptr, size);
    else
        return malloc(size);
}


size_t write_data2(void *ptr, size_t size, size_t nmemb, void *data)
{
    size_t realsize = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *)data;
    
    mem->memory = (char *)myrealloc(mem->memory, mem->size + realsize + 1);
    if (mem->memory) {
        memcpy(&(mem->memory[mem->size]), ptr, realsize);
        mem->size += realsize;
        mem->memory[mem->size] = 0;
    }
    return realsize;
}

mutex data_mutex;
void func(map<string, vector<string>> &source, int num, int size_each_thread, int size, map<string, Stock> &target)
{
	int start, end;
	if (num != 4) {
		end = size_each_thread * num;
		start = size_each_thread * (num - 1);
	}
	else {
		end = size;
		start = size_each_thread * 3;
	}
	map<string, vector<string>>::iterator itr = source.begin();
	int cnt = start;
	for (int i = 0; i < (num - 1)*size_each_thread; i++) itr++;
	for (itr; itr != source.end(); itr++)
	{
		if (cnt == end) break;
		Stock s(itr->first, itr->second[0], itr->second[1], "IWB");
		lock_guard<mutex> guard(data_mutex);
		if (target.size() % 100 == 0) cout << int(float(target.size()) / size * 100) << "%...";
		target[itr->first] = s;
		cnt++;
	}
}


map<string, Stock> retrieve_all_multithread() {
	cout << "retrieving data..." << endl;
	cout << "0%...";
	MyMatrix<string> matrix = readTxt("eps.txt");
	int size = matrix.size();
	int size_each_thread = size / 4;
	map<string, vector<string>> s_info = PutInMap(matrix);
	map<string, Stock> pool;

	pool["IWB"] = Stock("IWB", "2016-01-01", "2018-12-31", "IWB");

	curl_global_init(CURL_GLOBAL_ALL);
	thread retriever1(func, ref(s_info), 1, size_each_thread, size, ref(pool));
	thread retriever2(func, ref(s_info), 2, size_each_thread, size, ref(pool));
	thread retriever3(func, ref(s_info), 3, size_each_thread, size, ref(pool));
	thread retriever4(func, ref(s_info), 4, size_each_thread, size, ref(pool));
	retriever1.join();
	retriever2.join();
	retriever3.join();
	retriever4.join();
	curl_global_cleanup();
	cout << endl << "retrieve finished..." << endl;
	return pool;
}
