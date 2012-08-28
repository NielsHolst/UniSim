/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/variable.h>
#include "reaction.h"

using namespace UniSim;

namespace MicrobialCommunity {

    Reaction::Reaction(UniSim::Identifier name, QObject *parent) : Model(name, parent) {
        new Parameter<double>("InitialSubstrate", &initialSubstrate, 100, this, "Initial concentration of substrate");

        new Variable<double>("Substrate", &substrate, this, "Concentration of substrate");
        new Variable<double>("SubstratePDD", &substratePDD, this, "Concentration of substrate, population density dependent");
        new Variable<double>("Velocity", &velocity, this, "Reaction velocity");
        new Variable<double>("VelocityPDD", &velocityPDD, this, "Reaction velocity, population density dependent");
    }

    void Reaction::initialize() {
        populationList = seekSiblings<Population*>("*");

        if(populationList.isEmpty()) {
            throw Exception("No Population model is found for model \"" + this->objectName() +"\". Reaction model must be used together with one or more Population models, and they should be placed on the same level in the XML file.", this);
        }

        reset();
    }

    void Reaction::reset() {
        substrate = initialSubstrate;
        substratePDD = initialSubstrate;
    }

} //namespace
