/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_ENERGETICS_H
#define VG_ENERGETICS_H

#include <usbase/model.h>

namespace vg {

class Energetics : public UniSim::Model
{
public:
    Energetics(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();

private:
    // Inputs
    double floorTemperature;

    // Outputs
    double lightAbsorbedCover, lightAbsorbedScreens;

    // Data
//    struct Absorptivity {
//        double relativeArea;
//        const double *shelter, *cover, *screens;
//        double relativeShelter() const { return relativeArea*(*shelter); }
//        double relativeCover() const { return relativeArea*(*cover); }
//        double relativeScreens() const { return relativeArea*(*screens); }
//    };
//    QVector<Absorptivity> lightAbsorptivities;
    struct Component {
        const double *cover, *screens;
    };
    QVector<Component> lightAbsorbed;
    // Methods
    void zeroOutputs();

};
} //namespace


#endif
