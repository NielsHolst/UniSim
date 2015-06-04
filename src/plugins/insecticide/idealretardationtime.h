/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef INSECTICIDE_IDEALRETARDATIONTIME_H
#define INSECTICIDE_IDEALRETARDATIONTIME_H

#include <usbase/model.h>

namespace insecticide {

class idealretardationtime : public UniSim::Model
{
public:
    idealretardationtime(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();

private:
    // Input
    double porewatervelocity, apparentdiffusionanddispersioncoefficient,
    Htc3, Htc2, Htc4, Htc1, Htc5, Htc6, Htc7, Htc8, Htc9, Htc10;

    // Output
    double Irt3, Irt2, Irt4, Irt1, Irt5, Irt6, Irt7, Irt8, Irt9, Irt10;
};

} //namespace


#endif
