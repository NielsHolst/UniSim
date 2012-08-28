/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "conductance_factory.h"
#include "community.h"
#include "plant.h"
#include "weather.h"

using namespace UniSim;

namespace conductance{

void ConductanceFactory::defineProducts() {
    addProduct<Community>("Community", this,
    "A @F Community object holds one or two @F Plant objects. It governs the passage of the plant(s) "
    "through the three stages of competition: @F {Unlimited}, @F {UnderCompression}, and @F {WeightProportional}.");

    addProduct<Plant>("Plant", this,
    "A @F Plant object grows, in terms of biomass, leaf area and crown zone area, "
    "according to its current competition phase");

    addProduct<Weather>("Weather", this,
    "This @F Weather object holds fixed values for global irradiation and "
    "daily average temperature.");
}

UniSim::Identifier ConductanceFactory::id() const {
    return "conductance";
}

QString ConductanceFactory::description() const {
    return
    "The @F conductance plugin is an implementation of the Conductance model for plant growth and competition, "
    "as described by @Cite{$label{Benjamin \"&\" Park (2007)}benjamin_park_2007} and references cited therein. "
    "The @F Conductance model always holds three child models, representing calendar, weather and plant community, "
    "respectively. The community model again holds one or two plant models.";
}

QStringList ConductanceFactory::authors() const {
    return QStringList()
        << "Niels" << "Mette" << "Gionata";
}

QObject* ConductanceFactory::asQObject() {
   return this;
}

Q_EXPORT_PLUGIN2(conductance_factory, ConductanceFactory)

} //namespace
