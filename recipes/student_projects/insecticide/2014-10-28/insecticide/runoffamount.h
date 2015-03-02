/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef INSECTICIDE_RUNOFFAMOUNT_H
#define INSECTICIDE_RUNOFFAMOUNT_H

#include <usbase/model.h>

namespace insecticide {

class runoffamount : public UniSim::Model
{
public:
    runoffamount(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();

private:
    // Input
    double P, fsand;

    // Output
    double Q;
};

} //namespace


#endif
