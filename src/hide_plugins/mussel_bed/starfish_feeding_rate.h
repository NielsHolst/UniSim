/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/

#ifndef MUSSEL_BED_STARFISH_FEEDING_RATE_H
#define MUSSEL_BED_STARFISH_FEEDING_RATE_H

#include <usbase/model.h>

namespace mussel_bed {

class StarfishFeedingRate : public UniSim::Model
{
public:
    StarfishFeedingRate(UniSim::Identifier name, QObject *parent);
    void reset();
    void initialize();
    void update();

private:
    // Parameters
    double stdensity;
    double stsize;

    // Variables

    double FR;
    double value;


    // Links
    QList<Model*> scales;
};

} //namespace

#endif
