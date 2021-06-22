#include "pch.h"
#include "CKoppenCalculator.h"

CKoppenCalculator::CKoppenCalculator(vector<double>* vector_temp, vector<double>* vector_precip) {
	m_vector_temper = vector_temp;
	m_vector_precip = vector_precip;
	m_koppen_sign = "??";
}

tuple<int, double> CKoppenCalculator::getHottestMonth(int init, int end) {
	init--,	end--;

	int hottest_month = init;
	double its_value = m_vector_temper->at(init);

	int i;
	for (i = init; i != end; i++, i %= 12) {
		if (its_value < m_vector_temper->at(i)) {
			hottest_month = i;
			its_value = m_vector_temper->at(i);
		}
	} 
	// one more time
	{
		if (its_value < m_vector_temper->at(i)) {
			hottest_month = i;
			its_value = m_vector_temper->at(i);
		}
	}

	return make_tuple(hottest_month, its_value);
}
tuple<int, double> CKoppenCalculator::getColdestMonth(int init, int end) {
	init--, end--;

	int coldest_month = init;
	double its_value = m_vector_temper->at(init);

	int i;
	for (i = init; i != end; i++, i %= 12) {
		if (its_value > m_vector_temper->at(i)) {
			coldest_month = i;
			its_value = m_vector_temper->at(i);
		}
	}
	// one more time
	{
		if (its_value > m_vector_temper->at(i)) {
			coldest_month = i;
			its_value = m_vector_temper->at(i);
		}
	}

	return make_tuple(coldest_month, its_value);
}
tuple<int, double> CKoppenCalculator::getWettestMonth(int init, int end) {
	init--, end--;

	int rainiest_moth = init;
	double its_value = m_vector_precip->at(init);

	int i;
	for (i = init; i != end; i++, i %= 12) {
		if (its_value < m_vector_precip->at(i)) {
			rainiest_moth = i;
			its_value = m_vector_precip->at(i);
		}
	}
	// one more time
	{
		if (its_value < m_vector_precip->at(i)) {
			rainiest_moth = i;
			its_value = m_vector_precip->at(i);
		}
	}

	return make_tuple(rainiest_moth, its_value);
}
tuple<int, double> CKoppenCalculator::getDriestMonth(int init, int end) {
	init--, end--;

	int driest_month = init;
	double its_value = m_vector_precip->at(init);

	int i;
	for (i = init; i != end; i++, i %= 12) {
		if (its_value > m_vector_precip->at(i)) {
			driest_month = i;
			its_value = m_vector_precip->at(i);
		}
	}
	// one more time
	{
		if (its_value > m_vector_precip->at(i)) {
			driest_month = i;
			its_value = m_vector_precip->at(i);
		}
	}

	return make_tuple(driest_month, its_value);
}

tuple<int, double> CKoppenCalculator::getHottestMonth(tuple<int, int> season) {
	auto [init, end] = season;
	return getHottestMonth(init, end);
}
tuple<int, double> CKoppenCalculator::getColdestMonth(tuple<int, int> season) {
	auto [init, end] = season;
	return getColdestMonth(init, end);
}
tuple<int, double> CKoppenCalculator::getWettestMonth(tuple<int, int> season) {
	auto [init, end] = season;
	return getWettestMonth(init, end);
}
tuple<int, double> CKoppenCalculator::getDriestMonth(tuple<int, int> season) {
	auto [init, end] = season;
	return getDriestMonth(init, end);
}

double CKoppenCalculator::getTotalTemp(int init, int end) {
	init--, end--;

	double total = 0;

	int i;
	for (i = init; i != end; i++, i %= 12) {
		total += m_vector_temper->at(i);
	}
	// one more time
	{
		total += m_vector_temper->at(i);
	}

	return total;
}
double CKoppenCalculator::getTotalPrecip(int init, int end) {
	init--, end--;

	double total = 0;

	int i;
	for (i = init; i != end; i++, i %= 12) {
		total += m_vector_precip->at(i);
	}
	// one more time
	{
		total += m_vector_precip->at(i);
	}

	return total;
}
double CKoppenCalculator::getAverageTemp(int init, int end) {
	init--, end--;

	double total = 0;

	int i, num = 0;
	for (i = init; i != end; i++, i %= 12, num++) {
		total += m_vector_temper->at(i);
	}
	// one more time
	{
		total += m_vector_temper->at(i);
		num++;
	}

	return total / num;
}
double CKoppenCalculator::getAveragePrecip(int init, int end) {
	init--, end--;

	double total = 0;

	int i, num = 0;
	for (i = init; i != end; i++, i %= 12, num++) {
		total += m_vector_precip->at(i);
	}
	// one more time
	{
		total += m_vector_precip->at(i);
		num++;
	}

	return total / num;
}

double CKoppenCalculator::getTotalTemp(tuple<int, int> season) {
	auto [init, end] = season;
	init--, end--;

	double total = 0;

	int i;
	for (i = init; i != end; i++, i %= 12) {
		total += m_vector_temper->at(i);
	}
	// one more time
	{
		total += m_vector_temper->at(i);
	}

	return total;
}
double CKoppenCalculator::getTotalPrecip(tuple<int, int> season) {
	auto [init, end] = season;
	init--, end--;

	double total = 0;

	int i;
	for (i = init; i != end; i++, i %= 12) {
		total += m_vector_temper->at(i);
	}
	// one more time
	{
		total += m_vector_temper->at(i);
	}

	return total;
}
double CKoppenCalculator::getAverageTemp(tuple<int, int> season) {
	auto [init, end] = season;
	init--, end--;

	double total = 0;

	int i;
	for (i = init; i != end; i++, i %= 12) {
		total += m_vector_temper->at(i);
	}
	// one more time
	{
		total += m_vector_temper->at(i);
	}

	return total;
}
double CKoppenCalculator::getAveragePrecip(tuple<int, int> season) {
	auto [init, end] = season;
	init--, end--;

	double total = 0;

	int i;
	for (i = init; i != end; i++, i %= 12) {
		total += m_vector_temper->at(i);
	}
	// one more time
	{
		total += m_vector_temper->at(i);
	}

	return total;
}

tuple<int, int> CKoppenCalculator::getSummerSeason() {
	if (isOnNorthen()) {
		return make_tuple(4, 9);
	}
	else {
		return make_tuple(10, 3);
	}
}
tuple<int, int> CKoppenCalculator::getWinterSeason() {
	if (isOnNorthen()) {
		return make_tuple(10, 3);
	}
	else {
		return make_tuple(4, 9);
	}
}

double CKoppenCalculator::getEvaporationRate() {
	auto summer = getSummerSeason();

	double evaporation_rate = getAverageTemp() * 20;
	double summer_precip = getTotalPrecip(summer);
	double total_precip = getTotalPrecip();

	if (total_precip < 0) return 0.0;

	if (summer_precip > total_precip * 0.7) {
		evaporation_rate += 280;
	}
	else if (summer_precip > total_precip * 0.3) {
		evaporation_rate += 70;
	}
	else {
		evaporation_rate += 0;
	}

	return evaporation_rate;
}


bool CKoppenCalculator::isOnNorthen() {
	auto [hot_mon, hot_temp] = getHottestMonth();
	return 4 <= hot_mon && hot_mon <= 9;
}

CKoppenCalculator& CKoppenCalculator::Calculate() {
	auto [hot_month, hot_temp] = getHottestMonth();
	auto [cold_month, cold_temp] = getColdestMonth();

	string sign = "??";

	if (string str = isDryClimate(); str != "??") {
		sign = str;
	}
	else {
		if (hot_temp < 10) {
			sign = getPolarClimate();
		}
		else {
			if (cold_temp < -3) {
				sign = getContinentalClimate();
			}
			else if (cold_temp < 18) {
				sign = getTemperateClimate();
			}
			else {
				sign = getTropicalClimate();
			}
		}
	}

	setKoppenSign(sign);

	return *this;
}

string CKoppenCalculator::getKoppenSign() {
	return m_koppen_sign;
}

void CKoppenCalculator::setKoppenSign(const string& str) {
	m_koppen_sign = str;
}


string CKoppenCalculator::getTropicalClimate() {
	auto [dry_month, dry_precip] = getDriestMonth();

	double total_precip = getTotalPrecip();

	bool flag = std::accumulate(m_vector_precip->begin(), m_vector_precip->end(), true,
				[](bool flag, double num) {
					return flag && (num > 60);
				});


	if (flag) {
		return string("Af");
	}
	else {
		if (dry_precip >= 100 - (total_precip / 25)) {
			return string("Am");
		}
		else {
			return string("Aw");
		}
	}


	return string("??");
}

string CKoppenCalculator::isDryClimate() {
	double evaporation_rate = getEvaporationRate();
	double total_precip = getTotalPrecip();

	if (total_precip < evaporation_rate * 0.5) {
		return string("BW");
	}
	else if (total_precip < evaporation_rate) {
		return string("BS");
	}
	else {
		return string("??");
	}

}

string CKoppenCalculator::getTemperateClimate() {
	auto summer = getSummerSeason();
	auto winter = getWinterSeason();

	auto [hot_month, hot_temp] = getHottestMonth();

	auto [summer_wet_month, summer_wet_precip] = getWettestMonth(summer);
	auto [summer_dry_month, summer_dry_precip] = getDriestMonth(summer);
	auto [winter_wet_month, winter_wet_precip] = getWettestMonth(winter);
	auto [winter_dry_month, winter_dry_precip] = getDriestMonth(winter);

	if (summer_wet_precip >= winter_dry_precip * 10) {
		return string("Cw");
	} 
	else if (winter_wet_precip >= summer_dry_precip * 3 && summer_dry_precip < 30) {
		return string("Cs");
	}
	else {
		if (hot_temp >= 22) {
			return string("Cfa");
		}
		else {
			return string("Cfb");
		}
	}

	return string("??");
}
string CKoppenCalculator::getContinentalClimate() {
	auto summer = getSummerSeason();
	auto winter = getWinterSeason();

	auto [summer_wet_month, summer_wet_precip] = getWettestMonth(summer);
	auto [winter_dry_month, winter_dry_precip] = getDriestMonth(winter);

	if (summer_wet_precip >= winter_dry_precip * 10) {
		return string("Dw");
	}
	else {
		return string("Df");
	}

	return string("??");
}
string CKoppenCalculator::getPolarClimate() {
	auto [hot_month, hot_temp] = getHottestMonth();
	
	if (hot_temp < 0) {
		return string("EF");
	}
	else {
		return string("ET");
	}

	return string("??");
}

