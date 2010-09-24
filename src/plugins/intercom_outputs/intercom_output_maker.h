/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_OUTPUT_MAKER_H
#define UNISIM_OUTPUT_MAKER_H

#include <usbase/output_maker_plug_in.h>

namespace UniSim
{
    class ObjectPool;
    class Output;
}

namespace intercom{

    class IntercomOutputMaker : public QObject, public UniSim::OutputMakerPlugIn
{
	Q_OBJECT
    Q_INTERFACES(UniSim::OutputMakerPlugIn)
public:
    UniSim::Identifier pluginName() const;
    QString pluginDesc() const;
    QStringList authors() const;
    const QMap<UniSim::Identifier, QString>& supportedClasses();

    void useObjectPool(UniSim::ObjectPool *pool) const;

    UniSim::Output* create(UniSim::Identifier outputType, UniSim::Identifier objectName, QObject *parent=0);
};

} //namespace

#endif


