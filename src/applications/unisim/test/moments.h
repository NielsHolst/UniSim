#ifndef UNISIM_MOMENTS_H
#define UNISIM_MOMENTS_H

#include <QVector>
	
class Moments
{
public:
    Moments();
	void clear();
    void append(double value, double frequency);
    void append(QVector<double> values, QVector<double> frequencies);
    double average() const;
	double variance() const;
    double sum() const;
private:
    QVector<double> x, f;
	double fTotal;
};


#endif
