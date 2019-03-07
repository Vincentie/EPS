#include "Bootstrap.h"
#include <random>
#include <chrono>
#include "operator_overloading.h"

Bootstrap::Bootstrap()
{

}

Bootstrap::Bootstrap(map<string, Stock> & pool)
{
	do_btstrp(pool);
}

void Bootstrap::do_btstrp(map<string, Stock> & pool) {
	group();
	init();

	for (int i = 0; i < btstrp_num; i++) {
		sample();
		calculate(pool, i);
		clear();
		cout << "Round " << i+1 << " of bootstrapping is finished." << endl;
	}
}


Bootstrap::~Bootstrap()
{
}

void Bootstrap::init() {
	vector<double> temp = vector<double>(T);
	for (int i = 0; i < group_num; i++) {
		AAR.push_back(temp);
		CAAR.push_back(temp);
	}
}

void Bootstrap::group() {
	MyMatrix<string> matrix = readTxt("eps.txt");
	DivideIntoThreeGroups(matrix, beat, meet, miss);
}

void Bootstrap::sample(vector<string> & m, vector<string> & n)
{
unsigned seed = chrono::system_clock::now().time_since_epoch().count();
	shuffle(m.begin(), m.end(), default_random_engine(seed)); //sort names in the vector<string> randomly
	
	for (int i = 0; i < s_num; i++)  //get the first 100 names
	{
		n.push_back(m[i]);
	}
}

void Bootstrap::sample() {
	sample(beat, SampleBeat);
	sample(meet, SampleMeet);
	sample(miss, SampleMiss);
}

void Bootstrap::calc_AAR(map<string, Stock> & pool, vector<string> & sample, vector<double> & grp_aar) {
	
	for (int i = 0; i < T; i++)
	{
		double a = 0.0;
		for (vector<string>::iterator itr = sample.begin(); itr != sample.end(); itr++)
		{
			a += (pool[*itr].get_AR())[i];
		}

		grp_aar.push_back(a / s_num);
	}
}

void Bootstrap::calc_AAR(map<string, Stock> & pool) {
	calc_AAR(pool, SampleBeat, AARBeat);
	calc_AAR(pool, SampleMeet, AARMeet);
	calc_AAR(pool, SampleMiss, AARMiss);
}

void Bootstrap::calc_CAAR() {
	CAARBeat.push_back(AARBeat[0]);
	CAARMiss.push_back(AARMiss[0]);
	CAARMeet.push_back(AARMeet[0]);

	for (int i = 1; i < T; i++)
	{
		CAARBeat.push_back(CAARBeat[i - 1] + AARBeat[i]);
		CAARMiss.push_back(CAARMiss[i - 1] + AARMiss[i]);
		CAARMeet.push_back(CAARMeet[i - 1] + AARMeet[i]);
	}
}

void Bootstrap::calculate(map<string, Stock> & pool, double i) {
	calc_AAR(pool);
	AAR[0] = AAR[0] * (i / (i + 1)) + AARBeat * (1 / (i + 1));
	AAR[1] = AAR[1] * (i / (i + 1)) + AARMeet * (1 / (i + 1));
	AAR[2] = AAR[2] * (i / (i + 1)) + AARMiss * (1 / (i + 1));

	calc_CAAR();
	CAAR[0] = CAAR[0] * (i / (i + 1)) + CAARBeat * (1 / (i + 1));
	CAAR[1] = CAAR[1] * (i / (i + 1)) + CAARMeet * (1 / (i + 1));
	CAAR[2] = CAAR[2] * (i / (i + 1)) + CAARMiss * (1 / (i + 1));

}

void Bootstrap::clear() {
	SampleBeat.clear(); SampleMeet.clear(); SampleMiss.clear(); 
	AARBeat.clear(); AARMiss.clear(); AARMeet.clear();
	CAARBeat.clear(); CAARMiss.clear(); CAARMeet.clear();
}