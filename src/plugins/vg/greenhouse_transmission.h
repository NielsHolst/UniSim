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
    void reset();
    void update();

private:
    // Parameters
    double diffuse, sinb;
    QString swartFileName;

    // Variables
    double direct;

    // Links

    // Data
};
} //namespace


#endif
