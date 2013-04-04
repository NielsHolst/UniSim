/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/

#ifndef RVF_EGG_H
#define RVF_EGG_H

#include <usbase/model.h>

namespace rvf {

class Egg : public UniSim::Model
{
public:
    Egg(UniSim::Identifier name, QObject *parent);
    void initialize();
    void reset();
    void update();

private:
    // Parameters

    // Variables
    double totalEggsInflow;

    // Links
    UniSim::Model *inactiveEgg, *adult;
};

} //namespace

#endif // ADULT_H
