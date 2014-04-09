/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_SURFACE_H
#define VG_SURFACE_H

#include <memory>
#include <QVector>
#include <usbase/model.h>

namespace UniSim {
    class DataGrid;
}

namespace vg {

class Surface : public UniSim::Model
{
public:
    Surface(UniSim::Identifier name, QObject *parent);
    void amend();
    void update();

private:
    // Parameters
    QString directTransmissionFile;
    double latitude, azimuth;
    // Variables
    double K, diffuseTransmission, directTransmissionAsDirect, directTransmissionAsDiffuse;
    // Data
    UniSim::DataGrid *dirTransTable;
    struct SurfaceInfo {
        double area;
        struct Common {
            double K, diffuseTransmission, haze;
        };
        struct Cover : virtual Common{
            double directTransmissionAtNoon;
        } cover;
        struct ScreenInfo : virtual Common {
            const double *state;
        };
        QVector<ScreenInfo> screens;
    };
    QVector<SurfaceInfo> surfaces;
    double totalArea;
    // Methods
    void collectInfo();
    void updateTotalArea();
    void updateK();
    void updateLightTransmission();
};
} //namespace


#endif
