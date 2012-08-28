/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef EFFECT_H
#define EFFECT_H
#include <QObject>
#include <QRegExp>
#include <usbase/model.h>
#include "population.h"
#include "random.h"

namespace MicrobialCommunity {

    class Effect : public UniSim::Model {
        Q_OBJECT

        public:
            Effect(UniSim::Identifier name, QObject *parent=0);

            // standard methods
            void initialize();
            void reset();

        private:
            // local variables
            QString matchedPopulations;
            QStringList matchedPopulationList;

            int minPopulations, maxPopulations;

            QList<Population*> randomPopulationList;

            //link
            QList<Population*> populationList;

        protected:
            // parameters
            bool enableEffect;
            QString targetPopulations; // All/*, Specified:{MOB_1, MOB_2}, AllBut:{MOB_1, MOB_2}, random:{All/*}, random:{5, 10}, random:{MOB_1, MOB_2}

            // local variables
            QList<Population*> affectedPopulationList;
            Random random;

    };

} //namespace

#endif // EFFECT_H
