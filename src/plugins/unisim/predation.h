/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_PREDATION_H
#define UNISIM_PREDATION_H
#include <QVector>
#include "functional_response_multiway.h"

namespace UniSim {

class Predation : public FunctionalResponseMultiway
{
public:
    Predation(UniSim::Identifier name, QObject *parent=0);
private:
    // variables
    QVector<double> supplies, losses;
    // methods
    void createVariables();
    void updateCreatedVariables();
    void updateAttacksByPrey(int ixPrey);
};

}
#endif
