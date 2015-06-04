/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/exception.h>
#include <usbase/test_num.h>
#include "crop_component.h"
#include "publish.h"
#include "remaining_partition.h"

using namespace UniSim;

namespace xpest {
	
PUBLISH(RemainingPartition)

RemainingPartition::RemainingPartition(Identifier name, QObject *parent)
	: Model(name, parent)
{
    Output(double, yValue); // Remaining after sibling partitions
}

void RemainingPartition::amend() {
    belowGroundPartition = seekOne<Model*>("belowGroundPartition")->pullValuePtr<double>("yValue");
}

void RemainingPartition::initialize() {
    Model *parentComponent = seekParent<Model*>("*");
    QList<CropComponent*> otherComponents = parentComponent->seekSiblings<CropComponent*>("*");
    parts.clear();
    for (int i = 0; i < otherComponents.size(); ++i) {
        Model *component = otherComponents[i];
        parts << component->pullValuePtr<double>("netPartition");
    }
}

void RemainingPartition::reset() {
    yValue = 0.;
}

void RemainingPartition::update() {
    double sum = 0.;
    for (int i = 0; i < parts.size(); ++i) {
        sum += *parts.at(i);
    }
    TestNum::snapTo(sum, 1.);
    if (sum > 1.) {
        QString msg = "Sum of net partitions (%1) is greater than 1";
        throw Exception(msg.arg(sum), this);
    }
    yValue = (1. - sum)/(1. - *belowGroundPartition);
}


} //namespace

