/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef INSECTICIDE_APPARENTDIFFUSIONCOEFFICIENT_H
#define INSECTICIDE_APPARENTDDIFFUSIONCOEFFICIENT_H

#include <usbase/model.h>

namespace insecticide {

class apparentdiffusioncoefficient : public UniSim::Model
{
public:
    apparentdiffusioncoefficient(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();

private:
    // Input
    double Bew;

    // Output
    double apparentDiffusionCoeff;
};

} //namespace


#endif
