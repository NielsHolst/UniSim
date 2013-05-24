/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/

#ifndef MUSSEL_BED_SEARCHRATE_H
#define MUSSEL_BED_SEARCHRATE_H

#include <usbase/model.h>

namespace mussel_bed {

class searchrate : public UniSim::Model
{
public:
    searchrate(UniSim::Identifier name, QObject *parent);
    void reset();
    void initialize();
    void update();

private:
    // Parameters
    double maxSR;

    // Variables
    double value;

    // Links
    QList<Model*> scales;
};

} //namespace

#endif

