/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <QMapIterator>
#include <usbase/exception.h>
#include <usbase/parameter.h>
#include <usbase/variable.h>
#include <usbase/test_num.h>
#include <usbase/utilities.h>
#include "partitioning_fixed_by_stage.h"
#include "plant.h"

using namespace UniSim;

namespace intercom{

PartitioningFixedByStage::PartitioningFixedByStage(UniSim::Identifier name, QObject *parent)
    : Partitioning(name, parent)
{
    new Parameter<QString>("values", &valueAsString, QString(), this,
    "Fixed partitioning coefficient for each plant growth stage, for example, "
    "(vegetative 0.5)(flowering 0.2)");
}

void PartitioningFixedByStage::initialize() {
    partitions = decodeList<QString,double>(valueAsString, this);
    Plant *plant = seekOneAscendant<Plant*>("*");
    phenology = plant->seekOneChild<Model*>("phenology");
}

void PartitioningFixedByStage::update() {
    value = 0.;
    QMapIterator<QString, double> pa(partitions);
    while (pa.hasNext()) {
        pa.next();
        QString name = pa.key();
        double partition = pa.value();
        double stageProportion = phenology->pullValue<double>(name);
        value += stageProportion*partition;
    }
    if (TestNum::gt(value, 1.))
        throw Exception("Partition greater than 1: " + QString::number(value), this);
    Q_ASSERT(value>=0.);
}


} //namespace

