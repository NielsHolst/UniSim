/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_GREENHOUSE_SHELTER_H
#define VG_GREENHOUSE_SHELTER_H

#include "shelter_outputs.h"

namespace vg {

class GreenhouseShelter : public ShelterOutputs
{
public:
    GreenhouseShelter(UniSim::Identifier name, QObject *parent);
    void localReset();
    void update();

private:
    // Inputs
    QString shelters;
    double groundArea;

    // Outputs
    double Ucover, temperature;

    // Data
    struct ShelterInfo {
        UniSim::Model *shelter;
        double area;
        const double *pTemperature;
        const double *pUcover;
        double weightedShelterOutput(QString output)
            { return shelter->pullValue<double>(output)*area; }
    };
    QVector<ShelterInfo> shelterInfos;
    double totalArea;
};
} //namespace


#endif
