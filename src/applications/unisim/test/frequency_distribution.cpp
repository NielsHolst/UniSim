#include <cmath>
#include "frequency_distribution.h"

using namespace NHolst;

FrequencyDistribution::FrequencyDistribution()
{
	clear();
}

void FrequencyDistribution::clear()
{
	f.clear();
	x.clear();
	fTotal = 0;
}

void FrequencyDistribution::add(double value, double frequency)
{
	x.append(value);
	f.append(frequency);
	fTotal += frequency;
}

double FrequencyDistribution::average() const
{
	double avg(0);
	for (int i = 0; i < x.size(); ++i) {		
		avg += f.at(i)*x.at(i);
	}
	return avg/fTotal;
}

double FrequencyDistribution::variance() const
{
	double 	avg(average()),
			var(0.);
	for (int i = 0; i < x.size(); ++i) {		
		var += f.at(i)*pow(x.at(i) - avg, 2);
	}
	return var/fTotal;
}
