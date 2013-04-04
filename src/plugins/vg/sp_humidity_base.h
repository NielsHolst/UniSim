/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_SP_HUMIDITY_BASE_H
#define VG_SP_HUMIDITY_BASE_H

#include <usbase/model.h>

namespace vg {

class SpHumidityBase : public UniSim::Model
{
public:
    SpHumidityBase(UniSim::Identifier name, QObject *parent);
protected:
    // Variables
    double spRh, spDeltaX;
};
} //namespace


#endif
