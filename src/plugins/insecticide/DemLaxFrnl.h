/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef INSECTICIDE_DEMLAXFRNL_H
#define INSECTICIDE_DEMLAXFRNL_H

#include <usbase/model.h>

namespace insecticide {

class DemLaxFrnl : public UniSim::Model
{
public:
    DemLaxFrnl(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();

private:
    // Input
    double Doseafl, DLosses3, DLosses2, DLosses4, DLosses1,
    DLosses5, DLosses6, DLosses7, DLosses8,
    DLosses9, DLosses10, Fn3, Fn2, Fn4, Fn1, Fn5, Fn6, Fn7, Fn8, Fn9, Fn10;

    // Output
    double Dem1, Dem2, Dem3, Dem4, Dem5, Dem6, Dem7, Dem8, Dem9, Dem10,
    Drnl1, Drnl2, Drnl3, Drnl4, Drnl5, Drnl6, Drnl7, Drnl8, Drnl9, Drnl10,
    Lax1, Lax2, Lax3, Lax4, Lax5, Lax6, Lax7, Lax8, Lax9, Lax10,
    Subsoilfdd, Subsoilfdrgw, Subsoilfdbd;
};

} //namespace


#endif
