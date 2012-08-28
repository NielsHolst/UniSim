/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_PARAMETER_INDEX_FROM_CONDENSED_TABLE_H
#define UNISIM_PARAMETER_INDEX_FROM_CONDENSED_TABLE_H

#include "parameter_index.h"

namespace UniSim{

class Model;

class ParameterIndexFromCondensedTable : public ParameterIndex
{
public:
    ParameterIndexFromCondensedTable(QString filePath, QString parameterName);
    void reset(Model *model);
    bool hasNext();
    Result next();
private:
    QString parameterName;
    int nextRowIndex, nextColumnIndex;
    bool _hasNext;
};

} //namespace

#endif
