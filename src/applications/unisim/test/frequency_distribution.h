#ifndef FREQUENCY_DISTRIBUTION_H
#define FREQUENCY_DISTRIBUTION_H

#include <QList>

namespace NHolst{
	
class FrequencyDistribution
{
public:
	FrequencyDistribution();
	void clear();
	void add(double value, double frequency);
	double average() const;
	double variance() const;
private:
	QList<double> f, x;	
	double fTotal;
};

}//namespace

#endif
