/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_INDOORS_VENTILATION_H
#define VG_INDOORS_VENTILATION_H

#include <usbase/model.h>
#include "variable_collection.h"

namespace vg {

class IndoorsVentilation : public UniSim::Model
{
public:
    IndoorsVentilation(UniSim::Identifier name, QObject *parent);
    void amend();
    void reset();
    void update();

private:
    // Variables
    double value;
    // Data
    VariableCollection v;
};

} //namespace


#endif
