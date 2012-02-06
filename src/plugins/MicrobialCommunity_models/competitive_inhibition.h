/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef COMPETITIVE_INHIBITION_H
#define COMPETITIVE_INHIBITION_H
#include <QObject>
#include <QRegExp>
#include <usbase/model.h>
#include "effect.h"
#include "population.h"

namespace MicrobialCommunity {

    class CompetitiveInhibition : public Effect {
        Q_OBJECT

        public:
            CompetitiveInhibition(UniSim::Identifier name, QObject *parent=0);

            // standard methods
            void initialize();
            void reset();

        private:
            // parameters
            double inhibitionConst, inhibitorConc;

    };

} //namespace

#endif // COMPETITIVE_INHIBITION_H
