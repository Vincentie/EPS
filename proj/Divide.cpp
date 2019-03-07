//
//  Divide.cpp
//  project
//
//  Created by malina on 2018/12/6.
//  Copyright © 2018年 malina. All rights reserved.
//

#include "Divide.hpp"
#include <stdio.h>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <locale>
#include <iomanip>
#include <iostream>
#include <map>
#include <list>

using namespace std;


MyMatrix<string> readTxt(string file)
{
    //cout<<"......readTxt() start......"<<endl;
    
    ifstream infile;
    infile.open(file);       // connect ifstream object and txt file
    assert(infile.is_open());       // if fail, output an error message and stop the program
    
    string s;
    string str;
    vector<string> v;
    while(getline(infile,s))        // read txt file
    {
        istringstream sin(s);       // create an istringstream object sin
        while(sin>>str)             // when there is a word, print the word (recognize a word by finding spaces)
        {
           
            v.push_back(str);     // store stock information in vector v
        }
    }
    
    
    // put information in a Matrix
    MyMatrix<string> m;
    vector<string> vs;
    for (int i=10; i<v.size();i++)   // we do not need the headline.
    {
        vector<string>().swap(vs);  // clear vs
        vs.push_back(v[i]);         // push_back stock name
        vs.push_back(v[++i]);       // push_back Actual_EPS
        vs.push_back(v[++i]);       // push_back Estimate_EPS
        vs.push_back(v[++i]);       // push_back ANNOUNCEMENT_DATE
        vs.push_back(v[++i]);       // push_back START_DATE
        vs.push_back(v[++i]);       // push_back END_DATE
       
        
        m.push_back(vs);            // push_back the vector which includes each stock's information
    }
	return m;
	
}



map<string,vector<string>> PutInMap(MyMatrix<string> & m)
{
    map<string, vector<string>> map;
    unsigned long size=m.size();
    vector<string> v;
    for (int i=0;i<size;i++)
    {
        vector<string>().swap(v);   // clear v
        v.push_back(m[i][4]);       // push_back START_DATE
        v.push_back(m[i][5]);       // push_back END_DATE
		v.push_back(m[i][1]);       // push_back Actual_EPS
		v.push_back(m[i][2]);       // push_back Estimate_EPS
		v.push_back(m[i][3]);       // push_back Announcement date
        map[m[i][0]]=v;
    }
    return map;
}


void DivideIntoThreeGroups(MyMatrix<string> matrix, vector<string>& m1, vector<string>& m2, vector<string>& m3)
{
	//cout << "......DivideIntoThreeGroups() start......" << endl;
	double a;
	double d1, d2;
	vector<string> vdate;
	for (int i = 0; i < matrix.size(); i++)
	{
		// change the form of EPS and EPS estimate from string to double
		stringstream sstr1(matrix[i][1]);
		stringstream sstr2(matrix[i][2]);
		sstr1 >> d1;
		sstr2 >> d2;

		a = (d1 - d2) / d2;

		// divide the stocks into 3 groups and store them into the three map<string,vector<string>> containers m1,m2 and m3
		if (a >= 0.08) // 0.08
		{
			m1.push_back(matrix[i][0]);
		}
		else if (a <= -0.0056) //0.0056
		{
			m3.push_back(matrix[i][0]);
		}
		else
		{
			m2.push_back(matrix[i][0]);
		}

	}

	//cout << "......DivideIntoThreeGroups() end......" << endl;
}