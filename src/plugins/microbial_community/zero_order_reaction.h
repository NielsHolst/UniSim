/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef ZERO_ORDER_REACTION
#define ZERO_ORDER_REACTION
#include <QObject>
#include <usbase/model.h>
#include "population.h"
#include "reaction.h"

namespace MicrobialCommunity {

    class ZeroOrderReaction : public Reaction {
        //Q_OBJECT

        public:
            ZeroOrderReaction(UniSim::Identifier name, QObject *parent=0);

            // standard methods
            void update();

    };

} //namespace

#endif
