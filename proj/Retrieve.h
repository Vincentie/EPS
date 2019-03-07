#ifndef Retrieve_h
#define Retrieve_h
#include <string>
#include "Stock.h"
#include <map>
using namespace std;

struct MemoryStruct
{
    char *memory;
    size_t size;
};

string getTimeinSeconds(string Time);

void *myrealloc(void *ptr, size_t size);

size_t write_data2(void *ptr, size_t size, size_t nmemb, void *data);

map<string, Stock> retrieve_all_multithread();

#endif
