/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/object_pool.h>
#include <usbase/utilities.h>
#include "constant_world.h"
#include "cost.h"
#include "crop.h"
#include "farm.h"
#include "life_cycle.h"
#include "life_stage.h"
#include "operation.h"
#include "test_factory.h"


using namespace UniSim;

namespace test{

void TestFactory::defineProducts() {
    AddProduct(ConstantWorld, "Description");
    AddProduct(Cost, "Description");
    AddProduct(Crop, "Description");
    AddProduct(Farm, "Description");
    AddProduct(LifeCycle, "Description");
    AddProduct(LifeStage, "Description");
    AddProduct(Operation, "Description");
}

Identifier TestFactory::id() const {
    return "test";
}

QString TestFactory::description() const {
    return
    "!Exclude\n"
    "The @F test plugin contains models used for testing only (UniSim unit tests).";
}

QStringList TestFactory::authors() const {
    return QStringList()
        << "Niels";
}

QObject* TestFactory::asQObject() {
    return this;
}

#if QT_VERSION < 0x50000
Q_EXPORT_PLUGIN2(test_factory, TestFactory)
#endif

} //namespace
