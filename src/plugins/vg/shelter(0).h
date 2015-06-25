/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_SHELTER_H
#define VG_SHELTER_H

#include "shelter_outputs.h"

namespace UniSim {
    class DataGrid;
}

namespace vg {

class Shelter : public ShelterOutputs
{
public:
    Shelter(UniSim::Identifier name, QObject *parent);
    void initialize();
    void localReset();
    void update();

private:
    // inputs
    QString directTransmissionFile;
    double latitude, azimuth, greenhouseShade, chalk;

    // Data
    QList<UniSim::Model*> screens;
    const double *pCoverU, *pCoverHaze, *pCoverDiffuseTransmission;
    double zero, one;

    UniSim::DataGrid *dirTransTable;

    // Methods
    void updateU();
    void updateHaze();
    void updateAirTransmission();
    void updateLightTransmission();

};
} //namespace


#endif
