/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef DIFFEVOL_YEAR_H
#define DIFFEVOL_YEAR_H

#include <usbase/model.h>

namespace diffevol {

class Year : public UniSim::Model
{
public:
    Year(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();

private:
    // Parameters
    double fertiliser, fungicide, yield;
};

} //namespace


#endif
