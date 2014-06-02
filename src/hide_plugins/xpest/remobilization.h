/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef XPEST_REMOBILIZATION_H
#define XPEST_REMOBILIZATION_H

#include <usbase/model.h>

namespace xpest {

class Remobilization : public UniSim::Model
{
public:
    Remobilization(UniSim::Identifier name, QObject *parent);
    void amend();
    void reset();
    void update();

private:
    // Parameters
    QString from, to;
    double atGs, untilGs, proportion, currentGs;

    // Variables
    double rate;

    // Data
    bool inEffect;
    double slope, prevGs;

    // Links
    UniSim::Model *fromComponent, *toComponent;

    // Methods
    void setLine();
    void effectuate();
};

} //namespace


#endif
