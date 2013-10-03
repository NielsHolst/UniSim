/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_PIPE_H
#define VG_PIPE_H

#include <usbase/model.h>
#include <usbase/string_map.h>

namespace vg {

class Pipe : public UniSim::Model
{
public:
    Pipe(UniSim::Identifier name, QObject *parent);
    void initialize();
    void reset();
    void update();

private:
    // Parameters
    QString pipeType;
    double heatingDemand, Tunheated, Tindoors, Tminimum, actualTpipe, TmaxPipe, pipeLength;
    int energyScreenCourse;

    // Variables
    double signal, _heatFlux;

    // Data
    typedef enum {p25, s26, s33, s51} PipeType;
    UniSim::StringMap<PipeType> pipeTypes;
    double a, b, c;

    // Methods
    double energyScreenCorrection();
    double heatFlux(double Tpipe);
    double Tpipe (double _heatFlux);
    void selfTest();
};
} //namespace


#endif
