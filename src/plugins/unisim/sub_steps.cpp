/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/variable.h>
#include <usbase/utilities.h>
#include "publish.h"
#include "sub_steps.h"

namespace UniSim{

PUBLISH(SubSteps)

SubSteps::SubSteps(UniSim::Identifier name, QObject *parent)
    : Model(name, parent)
{
    setRecursionPolicy(Update,ChildrenNot);
    addParameter<int>(Name(numSubSteps), 10, "Number of update sub-steps performed on children when this is updated");
}

void SubSteps::initialize() {
    components = seekChildren<Component*>("*");
}

void SubSteps::update()
{
    for (int i = 0; i < numSubSteps; ++i)
        for (auto co : components)
            co->deepUpdate();
}

} //namespace

