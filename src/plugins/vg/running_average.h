/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_RUNNING_AVERAGE_H
#define VG_RUNNING_AVERAGE_H

#include <QVector>
#include <usbase/model.h>

namespace vg {

class RunningAverage : public UniSim::Model
{
public:
    RunningAverage(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();

private:
    // Inputs
    double initialValue, input;
    int length;

    // Output
    double value;

    // Data
    QVector<double> buffer;
    double sum;
    int ix;
};
} //namespace


#endif
