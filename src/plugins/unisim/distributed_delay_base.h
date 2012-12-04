/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_DISTRIBUTED_DELAY_BASE_H
#define UNISIM_DISTRIBUTED_DELAY_BASE_H
#include <QObject>
#include <QVector>

namespace UniSim {

class DistributedDelayBase
{
public:
    DistributedDelayBase(QObject *parent);
    void scale(double factor);
    const QVector<double>& take(double proportion);
    void put(const QVector<double> &addition);
    const double *data() const;
    double content() const;

protected:
    QVector<double> x;
    double xSum;
    QObject *parent;

private:
    QVector<double> taken;
};

}
#endif
