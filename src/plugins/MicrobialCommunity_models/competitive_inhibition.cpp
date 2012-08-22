/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/variable.h>
#include <usbase/exception.h>
#include "random.h"
#include "competitive_inhibition.h"
#include "enzymatic_reaction.h"

using namespace UniSim;

namespace MicrobialCommunity {

    CompetitiveInhibition::CompetitiveInhibition(UniSim::Identifier name, QObject *parent) : Effect(name, parent) {
        new Parameter<double>("InhibitionConstant", &inhibitionConst, 1., this, "Inhibition constant");
        new Parameter<double>("InhibitorConcentration", &inhibitorConc, 0., this, "Concentration of inhibitor");
    }

    void CompetitiveInhibition::initialize() {

        if(enableEffect) {

            if(seekSiblings<EnzymaticReaction*>("*").isEmpty()) {
                throw Exception("No EnzymaticReaction model is found for model \"" + this->objectName() +
                                "\". CompetitiveInhibition model must be used together with EnzymeCatalyzedReaction model,"
                                " and they should be placed on the same level in the XML file.", this);
            }

        } //if(enableEffect)

        // Call initialize() from base class to generate a list of affected populations.
        Effect::initialize();

    }

    void CompetitiveInhibition::reset() {

        // Call reset() from base class first to generate a list of affected populations.
        Effect::reset();

        if(enableEffect) {

            for(int i = 0; i < affectedPopulationList.size(); i++) {
                affectedPopulationList[i]->pushValue<double>("MichaelisMentenConstant",
                                                                affectedPopulationList[i]->pullValue<double>("MichaelisMentenConstant") * (1 + inhibitorConc / inhibitionConst));
            } //for

        } //if(enableEffect)

    } //reset

} //namespace
