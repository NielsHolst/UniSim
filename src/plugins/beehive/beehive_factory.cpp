/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "beehive_factory.h"
#include "fecundity.h"
#include "growth.h"
#include "life_stage.h"
#include "resource.h"
#include "small_hive_beetle.h"

using namespace UniSim;

namespace beehive{

void BeehiveFactory::defineProducts() {
    addProduct<Fecundity>("Fecundity", this, "Description pending");
    addProduct<Growth>("Growth", this, "Description pending");
    addProduct<LifeStage>("LifeStage", this, "Description pending");
    addProduct<Resource>("Resource", this, "Description pending");
    addProduct<SmallHiveBeetle>("SmallHiveBeetle", this, "Description pending");
}

UniSim::Identifier BeehiveFactory::id() const {
    return "beehive";
}

QString BeehiveFactory::description() const {
    return
    "!Exclude\n"
    "Description of beehive";
}

QStringList BeehiveFactory::authors() const {
    return QStringList() << "William" << "Niels";
}

QObject* BeehiveFactory::asQObject() {
   return this;
}

#if QT_VERSION < 0x50000
Q_EXPORT_PLUGIN2(beehive_factory, BeehiveFactory)
#endif

} //namespace
