/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef INTERCOM_PARTITIONING_FIXED
#define INTERCOM_PARTITIONING_FIXED
#include <QObject>
#include <usbase/model.h>

namespace intercom{

class PartitioningFixed : public UniSim::Model
{
	Q_OBJECT
public: 
    PartitioningFixed(UniSim::Identifier name, QObject *parent=0);

private:
    // parameters and pull variables
    double value;
};

} //namespace
#endif
