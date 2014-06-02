/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_TRANSMISSION_BASE_H
#define VG_TRANSMISSION_BASE_H

#include <QVector>
#include <usbase/model.h>

namespace UniSim {
    class DataGrid;
}

namespace vg {

class TransmissionBase : public UniSim::Model
{
public:
    TransmissionBase(UniSim::Identifier name, QObject *parent);
    void initialize();
    void update();

private:
    // Parameters
    QString directTransmissionFile;
    double latitude, azimuth, greenhouseSurfaceArea, greenhouseGroundArea;
    // Variables
    double U, netU, haze, diffuse, directAsDirect, directAsDiffuse, air, area;
    // Methods
    virtual bool applyScreens() const = 0;
    // Data
    UniSim::DataGrid *dirTransTable;
    struct SurfaceInfo {
        double area;
        struct Cover {
            double U, diffuseTransmission, haze;
        } cover;
        struct ScreenInfo {
            double lightTransmission, energyLossReduction, haze, air;
            const double *state;
        };
        QVector<ScreenInfo> screens;
        double netU();
        double netLightTransmission();
        double netHaze();
        double netAirTransmission();
        double sumState();
    };
    QVector<SurfaceInfo> surfaces;
    // Methods
    void collectInfo();
    void updateU();
    void updateLightTransmission();
    void updateAirTransmission();
    void updateArea();
};
} //namespace


#endif
