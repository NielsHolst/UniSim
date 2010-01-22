/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/object_pool.h>
#include <usbase/utilities.h>
#include "output_plot.h"
#include "output_table.h"
#include "standard_output_maker.h"

namespace UniSim{

QList<Identifier> StandardOutputMaker::outputTypes() const
{
    return QList<Identifier>()
        << Identifier("Plot")
        << Identifier("Table");
}

UniSim::Identifier StandardOutputMaker::plugInName() const {
    return "UniSim";
}

void StandardOutputMaker::useObjectPool(ObjectPool *pool) const {
    objectPool()->deferTo(pool);
}

Output* StandardOutputMaker::create(Identifier outputType, Identifier objectName, QObject *parent)
{
	// Remember to add controllerType to the list above as well
    UniSim::setSimulationObjectFromDescendent(parent);
    Output *output = 0;
    if (outputType.equals("Table"))
        output = new OutputTable(objectName, parent);
    else if (outputType.equals("Plot"))
        output = new OutputPlot(objectName, parent);
    return output;
}

Q_EXPORT_PLUGIN2(standard_output_maker,StandardOutputMaker)

} //namespace
