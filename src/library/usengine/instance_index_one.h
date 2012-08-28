/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_INSTANCE_INDEX_ONE_H
#define UNISIM_INSTANCE_INDEX_ONE_H

#include "instance_index.h"

namespace UniSim{
	
class InstanceIndexOne : public InstanceIndex
{
public:
    InstanceIndexOne(QString objectName);
    void reset(QString modelType, Model *parent);
    bool hasNext();
    Record next();

private:
    QString objectName;
    bool _hasNext;
};

} //namespace

#endif
