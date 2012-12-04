/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "differential_evolution.h"
#include "diffevol_factory.h"
#include "plot.h"
#include "year.h"

using namespace UniSim;

namespace diffevol{

void DiffEvolFactory::defineProducts() {
    addProduct<DifferentialEvolution>("DifferentialEvolution", this,
    "Differential evolution algoritm");

    addProduct<Plot>("Plot", this,
    "Plot with biophysical parameters");

    addProduct<Year>("Year", this,
    "The management and yield of a plot in a certain year");
}

UniSim::Identifier DiffEvolFactory::id() const {
    return "diffevol";
}

QString DiffEvolFactory::description() const {
    return
    "The diffevol plugin includes an iterator class that implements the differential evolution algorihm for optimisation";
}

QStringList DiffEvolFactory::authors() const {
    return QStringList()
        << "Niels";
    // Add Sanjeeb here
}

QObject* DiffEvolFactory::asQObject() {
   return this;
}

Q_EXPORT_PLUGIN2(diffevol_factory, DiffEvolFactory)

} //namespace
