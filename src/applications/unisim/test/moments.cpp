#include <cmath>
#include "moments.h"


Moments::Moments()
{
	clear();
}

void Moments::clear()
{
    x.clear();
    f.clear();
	fTotal = 0;
}

void Moments::append(double value, double frequency)
{
    x << value;
    f << frequency;
    fTotal += frequency;
}

void Moments::append(QVector<double> values, QVector<double> frequencies) {
    x << values;
    f << frequencies;

    double sum(0);
    for (int i = 0; i < frequencies.size(); ++i) {
        sum += frequencies.at(i);
    }
    fTotal += sum;
}

double Moments::average() const
{
	double avg(0);
	for (int i = 0; i < x.size(); ++i) {		
		avg += f.at(i)*x.at(i);
	}
	return avg/fTotal;
}

double Moments::variance() const
{
	double 	avg(average()),
			var(0.);
	for (int i = 0; i < x.size(); ++i) {		
		var += f.at(i)*pow(x.at(i) - avg, 2);
	}
	return var/fTotal;
}

double Moments::sum() const {
    return fTotal;
}
