/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "aphid_assimilate.h"
#include "aphid_rue.h"
#include "crop_component.h"
#include "growth.h"
#include "day_degree_scale.h"
#include "input_table.h"
#include "reduction.h"
#include "remaining_partition.h"
#include "remobilization.h"
#include "rust_damage.h"
#include "stage.h"
#include "weeds.h"
#include "xpest_factory.h"

using namespace UniSim;

namespace xpest{

void XPestFactory::defineProducts() {
    AddProduct(AphidAssimilate, "");
    AddProduct(AphidRue, "");
    AddProduct(CropComponent, "");
    AddProduct(Growth, "");
    AddProduct(DayDegreeScale, "");
    AddProduct(InputTable, "");
    AddProduct(Reduction, "");
    AddProduct(RemainingPartition, "");
    AddProduct(Remobilization, "");
    AddProduct(RustDamage, "");
    AddProduct(Stage, "");
    AddProduct(Weeds, "");
}


UniSim::Identifier XPestFactory::id() const {
    return "xpest";
}

QString XPestFactory::description() const {
    return
    "The xpest plugin includes models for teaching purposes";
}

QStringList XPestFactory::authors() const {
    return QStringList()
        << "Niels";
}

QObject* XPestFactory::asQObject() {
   return this;
}

#if QT_VERSION < 0x50000
Q_EXPORT_PLUGIN2(xpest_factory, XPestFactory)
#endif

} //namespace
