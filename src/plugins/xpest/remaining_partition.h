/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef XPEST_REMAINING_PARTITION_H
#define XPEST_REMAINING_PARTITION_H

#include <usbase/model.h>

namespace xpest {

class RemainingPartition : public UniSim::Model
{
public:
    RemainingPartition(UniSim::Identifier name, QObject *parent);
    void amend();
    void initialize();
    void reset();
    void update();

private:
    // Parameters

    // Variables
    double yValue;

    // Links
    QList<const double*> parts;
    const double *belowGroundPartition;
};

} //namespace


#endif
