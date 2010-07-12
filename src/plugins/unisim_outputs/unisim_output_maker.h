/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_OUTPUT_MAKER_H
#define UNISIM_OUTPUT_MAKER_H

#include <usbase/output_maker_plug_in.h>

namespace UniSim{

class ObjectPool;

class UniSimOutputMaker : public QObject, public OutputMakerPlugIn
{
	Q_OBJECT
    Q_INTERFACES(UniSim::OutputMakerPlugIn)
public:
    UniSim::Identifier pluginName() const;
    QString pluginDesc() const;
    QStringList authors() const;
    const QMap<UniSim::Identifier, QString>& supportedClasses();

    void useObjectPool(ObjectPool *pool) const;

    Output* create(Identifier outputType, Identifier objectName, QObject *parent=0);
};

} //namespace

#endif


