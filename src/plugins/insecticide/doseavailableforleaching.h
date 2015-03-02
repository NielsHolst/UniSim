/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef INSECTICIDE_DOSEAVAILABLEFORLEACHING_H
#define INSECTICIDE_DOSEAVAILABLEFORLEACHING_H

#include <usbase/model.h>

namespace insecticide {

class doseavailableforleaching : public UniSim::Model
{
public:
    doseavailableforleaching(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();

private:
    // Input
    double Doseaflm, Dmacropore;

    // Output
    double  Doseafl;

};

} //namespace


#endif
