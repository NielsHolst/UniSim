/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_PIPE_H
#define VG_PIPE_H

#include <QMap>
#include <usbase/model.h>

namespace vg {

class Pipe : public UniSim::Model
{
public:
    Pipe(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();

private:
    // Parameters
    QString pipeType;
    double Tindoors, Tunheated, heatingDemand, spHeating, otherPipesHeatTransfer,
        pipeLength, Tmax, heatParameter, heatTransferParameter, pidK, pidRho;
    bool isEnergyScreenOpening;
    int timeStep;
    char timeUnit;

    // Variables
    double temperature, heatTransfer, heatEnergy;

    // Methods
    double heatingTemperature();
    double equilibrate(double Told, double Tnew) const;
    double dtSec() const;
    double dtMin() const;

    struct Par {
        Par() {}
        Par(double _a, double _b, double _c) : a(_a), b(_b), c(_c) {}
        double a, b, c;
    };
    QMap<QString, Par> par;
    QString pipeTypes();
    double parabola(QString name, double dTemp);
};
} //namespace


#endif
