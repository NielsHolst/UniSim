/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef COMMUNITY_H
#define COMMUNITY_H
#include <QObject>
#include <usbase/model.h>
#include "population.h"
#include "random.h"

namespace MicrobialCommunity {

    class Community : public UniSim::Model
    {
        Q_OBJECT

    public:
        Community(UniSim::Identifier name, QObject *parent=0);

        // standard methods
        void initialize();
        void reset();
        void update();

    private:
        // parameters
        bool enableStochasticCommunity;
        int minPopulations, maxPopulations;
        QString alwaysIncluded;

        // local variables
        QStringList alwaysIncludedPopulationList;
        QList<Population*> requiredPopulationList;
        QList<Population*> optionalPopulationList;

        Random random;

        // state
        double density;
        int richness;
        double simpsonIndex, shannonIndex, evenness, giniCoeff;

        //link
        QList<Population*> populationList;

    };

} //namespace

#endif // COMMUNITY_H
