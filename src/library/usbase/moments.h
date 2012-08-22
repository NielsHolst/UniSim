/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_MOMENTS_H
#define UNISIM_MOMENTS_H

#include <QVector>

namespace UniSim{

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

} //namespace

#endif
