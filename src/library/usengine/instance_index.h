/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_INSTANCE_INDEX_H
#define UNISIM_INSTANCE_INDEX_H

#include <QMap>
#include <QString>
#include <usbase/named_object.h>

namespace UniSim{

class DataGrid;
	
class InstanceIndex
{
public:
    struct Record {
        QString modelName;
        QMap<QString, QString> paramNameValue;
    };
    InstanceIndex();
    virtual ~InstanceIndex();
    virtual void reset(QString context, Model *parent) = 0;
    virtual bool hasNext() = 0;
    virtual Record next() = 0;
protected:
    DataGrid *table;
};

} //namespace

#endif
