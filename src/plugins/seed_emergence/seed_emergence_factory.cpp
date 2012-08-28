/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/object_pool.h>
#include <usbase/utilities.h>
#include "seed_emergence_factory.h"
#include "cumulative_emergence.h"
#include "weather.h"

using namespace UniSim;

namespace seed_emergence{

void SeedEmergenceFactory::defineProducts() {
    addProduct<CumulativeEmergence>("CumulativeEmergence", this,
    "Cumulative emergence (%) modelled as a Gompertz curve.");

    addProduct<Weather>("Weather", this,
    "Converts units of soil water potential from kPa to MPa.");
}

UniSim::Identifier SeedEmergenceFactory::id() const {
    return "seed_emergence";
}

QString SeedEmergenceFactory::description() const {
    return
    "The @F seed_emergence plugin provides a @F CumulativeEmergence model which models "
    "emergence as a Gompertz curve running on hydrothermal time. The @F Unisim::HydrothermalTime model "
    "calculates time from weather input provided on a file with hourly readings of soil temperature "
    "and soil water potential.";
}

QStringList SeedEmergenceFactory::authors() const {
    return QStringList() << "Niels" << "Roberta";
}

QObject* SeedEmergenceFactory::asQObject() {
   return this;
}

Q_EXPORT_PLUGIN2(seed_emergence_factory, SeedEmergenceFactory)

} //namespace
