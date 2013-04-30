/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_GREENHOUSE_TRANSMISSION_H
#define VG_GREENHOUSE_TRANSMISSION_H

#include <usbase/model.h>

namespace vg {

class GreenhouseTransmission : public UniSim::Model
{
public:
    GreenhouseTransmission(UniSim::Identifier name, QObject *parent);
    void initialize();
    void reset();
    void update();

private:
    // Parameters
    double sinb, diffuseDiffusion;
    QString glassTypeStr;
    typedef enum{Single, Double, Hortiplus} GlassType;
    GlassType glassType;

    // Variables
    double directDiffusion;

    // Data
    double a[3], b[3], c[3];

    // Methods
    void defineConstants();
    void decodeGlassType();
};
} //namespace


#endif
