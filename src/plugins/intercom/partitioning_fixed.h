/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef INTERCOM_PARTITIONING_FIXED
#define INTERCOM_PARTITIONING_FIXED
#include <QObject>
#include "partitioning.h"

namespace intercom{

class PartitioningFixed : public Partitioning
{
	Q_OBJECT
public: 
    PartitioningFixed(UniSim::Identifier name, QObject *parent=0);
};

} //namespace
#endif
