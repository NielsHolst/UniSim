/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef REACTION_H
#define REACTION_H
#include <QObject>
#include <usbase/model.h>
#include "population.h"

namespace MicrobialCommunity {

    class Reaction : public UniSim::Model {
        Q_OBJECT

        public:
            Reaction(UniSim::Identifier name, QObject *parent=0);

            // standard methods
            void initialize();
            void reset();

        protected:
            // parameters
            double initialSubstrate, reactionFinishThreshold;

            // state
            double substrate, substratePDD;
            double velocity, velocityPDD;

            //link
            QList<Population*> populationList;

    };

} //namespace

#endif // REACTION_H
