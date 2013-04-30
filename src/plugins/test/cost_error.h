/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_TEST_COST_ERROR_H
#define UNISIM_TEST_COST_ERROR_H
#include <QObject>
#include <usbase/model.h>

namespace test{

class CostError : public UniSim::Model
{
public: 
    CostError(UniSim::Identifier name, QObject *parent=0);

private:
    // parameters
    int energy;
};

} //namespace
#endif
