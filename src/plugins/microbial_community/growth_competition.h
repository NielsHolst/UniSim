/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef GROWTH_COMPETITION_H
#define GROWTH_COMPETITION_H
#include <QObject>
#include <usbase/model.h>
#include "population.h"

namespace MicrobialCommunity {

    class GrowthCompetition : public UniSim::Model
    {
        Q_OBJECT

    public:
        GrowthCompetition(UniSim::Identifier name, QObject *parent=0);

        // standard methods
        void initialize();
        void reset();
        void update();
        void updateDensity();

    private:
        // parameters
        double carryingCapacity, initialResource;

        // state
        double availableResource, totalDensity;

        //link
        UniSim::Model *Timeline;
        QList<Population*> Populations;

    };

} //namespace

#endif // GROWTH_COMPETITION_H
