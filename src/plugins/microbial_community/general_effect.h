/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef GENERAL_EFFECT_H
#define GENERAL_EFFECT_H
#include <QObject>
#include <QRegExp>
#include <usbase/model.h>
#include "effect.h"
#include "population.h"

namespace MicrobialCommunity {

    class GeneralEffect : public Effect {
        Q_OBJECT

        public:
            GeneralEffect(UniSim::Identifier name, QObject *parent=0);

            // standard methods
            void initialize();
            void reset();

        private:
            // parameters
            QString targetParameter; // e.g. GrowthRate
            QString effectRatio; // e.g. Uniform:{0.1}, Range:{0.2, 0.3}

            //local variables
            double ratio;
            QList<QString> effectRatioList;
            double minRatio, maxRatio;

    };

} //namespace

#endif // GENERAL_EFFECT_H
