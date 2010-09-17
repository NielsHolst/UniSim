/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/object_pool.h>
#include <usbase/utilities.h>
#include "output_file.h"
#include "intercom_output_maker.h"

namespace UniSim{

UniSim::Identifier IntercomOutputMaker::pluginName() const {
    return "intercom";
}

QString IntercomOutputMaker::pluginDesc() const {
    return
    "The @F Intercom output plugin contains an output class to study the details of INTERCOM.";
}

QStringList IntercomOutputMaker::authors() const {
    return QStringList()
        << "Niels Holst, Aarhus University, Denmark";
}

const QMap<Identifier, QString>& IntercomOutputMaker::supportedClasses() {
    if (!desc.isEmpty())
        return desc;

    desc["File"] =
    "@I pending";

    return desc;
}

void IntercomOutputMaker::useObjectPool(ObjectPool *pool) const {
    objectPool()->deferTo(pool);
}

Output* IntercomOutputMaker::create(Identifier outputType, Identifier objectName, QObject *parent)
{
	// Remember to add integratorType to the list above as well
    UniSim::setSimulationObjectFromDescendent(parent);
    Output *output = 0;
    if (outputType.equals("File"))
        output = new OutputFile(objectName, parent);
    return output;
}

Q_EXPORT_PLUGIN2(intercom_output_maker,IntercomOutputMaker)

} //namespace
