/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef INSECTICIDE_WINDDRIFTTOTAL_H
#define INSECTICIDE_WINDDRIFTTOTAL_H

#include "winddrift.h"

namespace insecticide {

class winddrifttotal : public winddrift //notice "public UniSim::Model" is replaced by "public WindDrift"
{
public:
    winddrifttotal(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();

private:

    // Output
    double totaldrift;
    int npass;
};

} //namespace


#endif
