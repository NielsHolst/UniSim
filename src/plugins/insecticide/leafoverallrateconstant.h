/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef INSECTICIDE_LEAFOVERALLRATECONSTANT_H
#define INSECTICIDE_LEAFOVERALLRATECONSTANT_H

#include <usbase/model.h>

namespace insecticide {

class leafoverallrateconstant : public UniSim::Model
{
public:
    leafoverallrateconstant(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();

private:
    // Input
    double klu, klv, kld;

    // Output
    double kov;
};

} //namespace


#endif
