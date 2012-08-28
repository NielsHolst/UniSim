/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef INTERCOM_PARTITIONING_FIXED_BY_STAGE
#define INTERCOM_PARTITIONING_FIXED_BY_STAGE
#include <QMap>
#include <QObject>
#include "partitioning.h"

namespace intercom{

class PartitioningFixedByStage : public Partitioning
{
	Q_OBJECT
public: 
    PartitioningFixedByStage(UniSim::Identifier name, QObject *parent=0);

    // standard methods
    void initialize();
    void update();

private:
    // parameters
    QString valueAsString;

    // data
    QMap<QString,double> partitions;
    UniSim::Model* phenology;
};

} //namespace
#endif
