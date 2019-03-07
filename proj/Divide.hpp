//
//  Divide.hpp
//  project
//
//  Created by malina on 2018/12/6.
//  Copyright © 2018年 malina. All rights reserved.
//

#ifndef Divide_hpp
#define Divide_hpp

#include <stdio.h>
#include <string>
#include <sstream>
#include <vector>
#include <locale>
#include <iomanip>
#include <iostream>
#include <fstream>  // for ifstream
#include <cassert>
#include <map>
#include <list>
#include "operator_overloading.h"
using namespace std;



// Read txt file
MyMatrix<string> readTxt(string file);

// Change Matrix to map for manu to pull information
map<string,vector<string>> PutInMap(MyMatrix<string> & m);

// Divide all the Russell 1000 stocks into three groups. m1 is beat, m2 is meet, m3 is miss.
void DivideIntoThreeGroups(MyMatrix<string> matrix, vector<string>& m1, vector<string>& m2, vector<string>& m3);

#endif /* Divide_hpp */
