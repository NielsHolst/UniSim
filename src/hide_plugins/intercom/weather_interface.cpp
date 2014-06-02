/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/variable.h>
#include "weather_interface.h"

using namespace UniSim;

namespace intercom{

WeatherInterface::WeatherInterface(UniSim::Identifier name, QObject *parent)
    : WeatherFile(name, parent)
{
    new Variable<double>("Tavg", &Tavg, this, "description");
    new Variable<double>("Tday", &Tday, this, "description");
    new Variable<double>("irradiation", &irradiation, this, "description");
    new Variable<double>("parTotal", &par.total, this, "description");
    new Variable<double>("parDiffuse", &par.diffuse, this, "description");
    new Variable<double>("parDirect", &par.direct, this, "description");
}

} //namespace

