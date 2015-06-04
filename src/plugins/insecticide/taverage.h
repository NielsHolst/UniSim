/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef INSECTICIDE_TAVERAGE_H
#define INSECTICIDE_TAVERAGE_H

#include <usbase/model.h>

namespace insecticide {

class taverage : public UniSim::Model
{

public:
    taverage(UniSim::Identifier name, QObject *parent);
    void initialize();
    void reset();
    void update();

private:
    // Input
    //double ;

    // Output
    double value, Tav;

    // links
    const double *ptrTav;
    bool temperatureIsPresent;

};

} //namespace


#endif
