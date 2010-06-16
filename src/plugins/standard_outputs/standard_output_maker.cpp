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

UniSim::Identifier StandardOutputMaker::pluginName() const {
    return "UniSim";
}

QString StandardOutputMaker::pluginDesc() const {
    return
    "The @F UniSim plugin contains a collection of output classes of general utility.";
}

QStringList StandardOutputMaker::authors() const {
    return QStringList()
        << "Niels Holst, Aarhus University, Denmark";
}

const QMap<Identifier, QString>& StandardOutputMaker::supportedClasses() {
    if (!desc.isEmpty())
        return desc;

    desc["Plot"] =
    "@I pending";

    desc["Table"] =
    "@I pending";

    return desc;
}

void StandardOutputMaker::useObjectPool(ObjectPool *pool) const {
    objectPool()->deferTo(pool);
}

Output* StandardOutputMaker::create(Identifier outputType, Identifier objectName, QObject *parent)
{
	// Remember to add integratorType to the list above as well
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
