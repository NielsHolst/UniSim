/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_HEATING_TEMPERATURE_CONTROLLER_H
#define VG_HEATING_TEMPERATURE_CONTROLLER_H

#include <QVector>
#include <usbase/model.h>

namespace vg {

class ConstructionCover;
class HeatPipe;

class HeatingTemperatureController : public UniSim::Model
{
public:
    HeatingTemperatureController(UniSim::Identifier name, QObject *parent);
    void amend();
    void reset();
    void update();
private:
    // Parameters
    double timeStepSecs, greenhouseArea, greenhouseVolume,
        heatingSetpoint, heatingFlow,
        minimumSignal, maximumSignal,
        indoorsTemperature;
    // Variables
    double signal;
    // Data
    QList<ConstructionCover*> covers;
    QList<HeatPipe*> pipes;
    int numPipes;
    double coverK;
    // Methods
    double calcCoverK();

};
} //namespace


#endif
