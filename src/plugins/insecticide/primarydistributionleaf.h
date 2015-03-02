/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef INSECTICIDE_PRIMARYDISTRIBUTIONLEAF_H
#define INSECTICIDE_PRIMARYDISTRIBUTIONLEAF_H

#include <usbase/model.h>

namespace insecticide {

class primarydistributionleaf : public UniSim::Model
{
public:
    primarydistributionleaf(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();

private:
    // Input
    double Doseappl, Dosedrift,
        stage1, stage2, stage3, stage4;

    // Output
    double Doserl, fi;

};

} //namespace


#endif
