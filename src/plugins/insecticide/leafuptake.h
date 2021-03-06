/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef INSECTICIDE_LEAFUPTAKE_H
#define INSECTICIDE_LEAFUPTAKE_H

#include <usbase/model.h>

namespace insecticide {

class leafuptake : public UniSim::Model
{
public:
    leafuptake(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();

private:
    // Input
    double a, b, R, T, MV, leaftype;

    // Output
    double V, Ed, log_klu, klu;
};

} //namespace


#endif
