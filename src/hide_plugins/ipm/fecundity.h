/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef IPM_FECUNDITY_H
#define IPM_FECUNDITY_H

#include <QObject>
#include <usbase/model.h>

using namespace UniSim;

namespace ipm{

class Fecundity : public UniSim::Model
{
public:
    Fecundity(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();

private:
    // Parameters
    double density, maxPerCapitaFecundity, sdRatio, timeStep;

    // Variables
    double value;
};

} //namespace

#endif
