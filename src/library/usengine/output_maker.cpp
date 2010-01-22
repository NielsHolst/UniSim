/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
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
#include "output_maker.h"

namespace UniSim{

OutputMaker *OutputMaker::_me = 0;

OutputMaker::OutputMaker()
{
	QDir dir = FileLocations::location(FileLocations::Plugins);
	
	foreach (QString filename, dir.entryList(QDir::Files))  {
		QPluginLoader loader(dir.absoluteFilePath(filename));
        OutputMakerPlugIn *outputMaker = qobject_cast<OutputMakerPlugIn*>(loader.instance());
        if (outputMaker) {
            outputMaker->useObjectPool(objectPool());
            foreach (Identifier outputType, outputMaker->outputTypes()) {
                _outputMakers[outputType] = outputMaker;
                Identifier nsModelType = outputMaker->plugInName().label()+"::"+outputType.label();
                _outputMakers[nsModelType] = outputMaker;
			}
		}
	}
    if (_outputMakers.size() == 0) {
        QString msg = "Found no output plugins in: " + dir.absolutePath();
        if (!dir.exists()) msg += ".\nThe folder does not exist.";
        throw Exception(msg);
    }
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
