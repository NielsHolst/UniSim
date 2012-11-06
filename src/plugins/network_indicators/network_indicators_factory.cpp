/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "network_indicators_factory.h"
#include "farm.h"
#include "import_indicator.h"

using namespace UniSim;

namespace network_indicators{

void NetworkIndicatorsFactory::defineProducts() {
    addProduct<Farm>("Farm", this, "Model with paramers for farm nitrogen flows");
    addProduct<ImportIndicator>("ImportIndicator", this, "Computes import indicator");
}

UniSim::Identifier NetworkIndicatorsFactory::id() const {
    return "network_indicators";
}

QString NetworkIndicatorsFactory::description() const {
    return
    "The network_indicators plugin calculates various indicators of nitrogen flows on farms";
}

QStringList NetworkIndicatorsFactory::authors() const {
    return QStringList()
        << "Niels";
    // Add Walter here
}

QObject* NetworkIndicatorsFactory::asQObject() {
   return this;
}

Q_EXPORT_PLUGIN2(network_indicators_factory, NetworkIndicatorsFactory)

} //namespace
