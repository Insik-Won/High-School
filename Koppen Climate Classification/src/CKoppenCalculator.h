#pragma once
class CKoppenCalculator
{
public:
	CKoppenCalculator(vector<double>* vector_temp = nullptr, vector<double>* vector_precip = nullptr);
	virtual ~CKoppenCalculator() = default;

private:
	const vector<double>* m_vector_temper;	// pointer
	const vector<double>* m_vector_precip;	// pointer
	string m_koppen_sign;

public:
	tuple<int, double> getHottestMonth(int init = 1, int end = 12);	
	tuple<int, double> getColdestMonth(int init = 1, int end = 12);
	tuple<int, double> getWettestMonth(int init = 1, int end = 12);
	tuple<int, double> getDriestMonth(int init = 1, int end = 12);

	tuple<int, double> getHottestMonth(tuple<int,int> season);
	tuple<int, double> getColdestMonth(tuple<int,int> season);
	tuple<int, double> getWettestMonth(tuple<int,int> season);
	tuple<int, double> getDriestMonth(tuple<int,int> season);
	// 1~12th month, (5,9) -> (5,6,7,8,9), (10,2) -> (10,11,12,1,2);

	double getTotalTemp(int init = 1, int end = 12);
	double getTotalPrecip(int init = 1, int end = 12);
	double getAverageTemp(int init = 1, int end = 12);
	double getAveragePrecip(int init = 1, int end = 12);

	double getTotalTemp(tuple<int,int> season);
	double getTotalPrecip(tuple<int,int> season);
	double getAverageTemp(tuple<int,int> season);
	double getAveragePrecip(tuple<int,int> season);

	double getEvaporationRate();

	tuple<int,int> getSummerSeason();
	tuple<int,int> getWinterSeason();

public:
	bool isOnNorthen();
	CKoppenCalculator& Calculate();		// this function should be called before calling 'getKoppenSign';
	string getKoppenSign();	// this function should be called after calling 'Calculate';

public:
	void setKoppenSign(const string& sign);

	string getTropicalClimate();
	string isDryClimate();
	string getTemperateClimate();
	string getContinentalClimate();
	string getPolarClimate();
};

