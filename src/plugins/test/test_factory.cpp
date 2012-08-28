/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/object_pool.h>
#include <usbase/utilities.h>
#include "constant_world.h"
#include "crop.h"
#include "farm.h"
#include "life_cycle.h"
#include "life_stage.h"
#include "operation.h"
#include "test_factory.h"

using namespace UniSim;

namespace test{

void TestFactory::defineProducts() {
    addProduct<ConstantWorld>("ConstantWorld", this, "Description pending");
    addProduct<Crop>("Crop", this, "Description pending");
    addProduct<Farm>("Farm", this, "Description pending");
    addProduct<LifeCycle>("LifeCycle", this, "Description pending");
    addProduct<LifeStage>("LifeStage", this, "Description pending");
    addProduct<Operation>("Operation", this, "Description pending");
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

Q_EXPORT_PLUGIN2(test_factory, TestFactory)

} //namespace
