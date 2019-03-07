#pragma once
#include "Divide.hpp"
#include "Stock.h"
#include "operator_overloading.h"

class Bootstrap
{
private:
	int s_num = 100;
	int T = 240;
	int btstrp_num = 5;
	int group_num = 3;
	vector<string> beat, meet, miss; //store information for each group
	vector<string> SampleBeat, SampleMeet, SampleMiss; //store 100 stocks in each group
	
	vector<double> AARBeat, AARMiss, AARMeet;
	vector<double> CAARBeat, CAARMiss, CAARMeet;

	MyMatrix<double> AAR;
	MyMatrix<double> CAAR;

	// Set default size for AAR and CAAR matrix. /
	void init();

	// Yield 3 vector<string> as 3 groups(beat, meet, miss) from a Matrix<string> dataset.
	void group();

	// Given a population m, generate 100 random samples from it and set to a certain sample vector<string>.
	void sample(vector<string> & m, vector<string> & n);
	// Do sampling for 3 groups.
	void sample();


	// Calculate 3 sample groups' AAR if a pool is given.
	void calc_AAR(map<string, Stock> & pool);
	// Calculate a group's AAR and store in the corresponding vector<double> if a pool is given.
	void calc_AAR(map<string, Stock> & pool, vector<string> & sample, vector<double> & grp_aar);
	// Calculate 3 sample groups' CAAR.
	void calc_CAAR();
	// Calculate the AAR/CAAR for every newly sampled groups and update the average AAR/CAAR in every iteration.
	void calculate(map<string, Stock> & pool, double i);

	// Clear sample, AAR info of every group, CAAR info of every group.
	void clear();

public:
	// Perform bootsrapping.
	void do_btstrp(map<string, Stock> & pool);
	Bootstrap();
	Bootstrap(map<string, Stock> & pool);
	~Bootstrap();

public:
	MyMatrix<double>& get_AAR() { return AAR; }
	MyMatrix<double>& get_CAAR() { return CAAR; }
};



