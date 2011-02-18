/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <QDir>
#include <QPluginLoader>
#include <QSettings>
#include <usbase/exception.h>
#include <usbase/file_locations.h>
#include <usbase/output.h>
#include <usbase/output_maker_plug_in.h>
#include <usbase/object_pool.h>
#include <usbase/utilities.h>
#include "output_maker.h"

namespace UniSim{

OutputMaker *OutputMaker::_me = 0;

OutputMaker::OutputMaker()
{
    lookupPlugIns<OutputMakerPlugIn>(id(), &_outputMakers);
}

QString OutputMaker::id() {
    return "OutputMaker";
}

Output* OutputMaker::create(Identifier outputType, Identifier objectName, QObject *parent)
{
    OutputMakerPlugIn *maker;
    switch (me()->_outputMakers.count(outputType)) {
        case 0:
            throw Exception("No output of type: " + outputType.key());
        case 1:
            maker = me()->find(outputType);
            return maker->create(outputType.withoutNamespace(), objectName, parent);
        default:
            throw Exception("More than one output of type: " + outputType.key()+". Qualify type with plug-in name.");
    }

}

bool OutputMaker::canCreate(Identifier outputType)
{
    return me()->_outputMakers.count(outputType) > 0;
}

Identifiers OutputMaker::selection()
{
    Identifiers selection;
    for (OutputMakers::iterator ma = me()->_outputMakers.begin();
                               ma != me()->_outputMakers.end(); ++ma)
		selection << ma.key();
	return selection;
}

OutputMakerPlugIn* OutputMaker::find(Identifier outputType)
{
    OutputMakers::iterator ma = _outputMakers.find(outputType);
    return (ma == _outputMakers.end()) ? 0 : ma.value();
}

OutputMaker* OutputMaker::me() {
    if (!_me)
        _me = objectPool()->find<OutputMaker*>(id());
    return _me;
}

} //namespace
