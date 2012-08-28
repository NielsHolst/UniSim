/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "lakeoxygen_factory.h"
#include "lake_environment.h"
#include "net_production.h"
#include "oxygen_concentration.h"

using namespace UniSim;

namespace lakeoxygen{

void LakeOxygenFactory::defineProducts() {
    addProduct<LakeEnvironment>("LakeEnvironment", this, "Description pending");
    addProduct<NetProduction>("NetProduction", this, "Description pending");
    addProduct<OxygenConcentration>("OxygenConcentration", this, "Description pending");
}

UniSim::Identifier LakeOxygenFactory::id() const {
    return "lakeoxygen";
}

QString LakeOxygenFactory::description() const {
    return
    "!Exclude\n"
    "Description of lakeoxygen";
}

QStringList LakeOxygenFactory::authors() const {
    return QStringList() << "Anders" << "Niels";
}

QObject* LakeOxygenFactory::asQObject() {
   return this;
}

Q_EXPORT_PLUGIN2(lakeoxygen_factory, LakeOxygenFactory)

} //namespace
