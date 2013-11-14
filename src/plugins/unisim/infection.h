/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_INFECTION_H
#define UNISIM_INFECTION_H
#include <usbase/matrix.h>
#include "functional_response_multiway.h"

namespace UniSim {

class Infection : public FunctionalResponseMultiway
{
public:
    Infection(UniSim::Identifier name, QObject *parent=0);
private:
    // variables
    Matrix<double> attacked, propAttacked;
    QVector<double> attacksVector, attackedVector, attacksHost, attackedHost;
    // methods
    void createVariables();
    void updateCreatedVariables();
    void updateAttacksByPrey(int ixPrey);
};

}
#endif
