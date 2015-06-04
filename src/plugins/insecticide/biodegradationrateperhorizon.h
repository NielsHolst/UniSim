/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef INSECTICIDE_BIODEGRADATIONRATEPERHORIZON_H
#define INSECTICIDE_BIODEGRADATIONRATEPERHORIZON_H

#include <usbase/model.h>

namespace insecticide {

class biodegradationrateperhorizon : public UniSim::Model
{
public:
    biodegradationrateperhorizon(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();

private:
    // Input
    double k, tcf1, tcf2, tcf3, tcf4, tcf5,
    tcf6, tcf7, tcf8, tcf9, tcf10;

    // Output
    double k1, k2,k3, k4, k5, k6, k7, k8, k9, k10;
};

} //namespace


#endif
