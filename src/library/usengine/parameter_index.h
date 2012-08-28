/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_PARAMETER_INDEX_H
#define UNISIM_PARAMETER_INDEX_H

#include <QPair>
#include <QString>
#include <usbase/named_object.h>

namespace UniSim{
	
class DataGrid;

class ParameterIndex
{
public:
    typedef QPair<QString, QString> Result;
    ParameterIndex(QString filePath);
    ~ParameterIndex();
    virtual void reset(Model *model) = 0;
    virtual bool hasNext() = 0;
    virtual Result next() = 0;
protected:
    DataGrid *table;
    QString filePath;
};

} //namespace

#endif
