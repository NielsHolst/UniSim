/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef COMPETITION_H
#define COMPETITION_H
#include <QObject>
#include <usbase/model.h>
#include "population.h"

namespace MicrobialCommunity {

    class Competition : public UniSim::Model
    {
        //Q_OBJECT

    public:
        Competition(UniSim::Identifier name, QObject *parent=0);

        // standard methods
        void initialize();
        void reset();
        void update();
        void updateDensity();
        void runCompetitionModel();

    private:
        // parameters
        bool enableCompetition;
        double initialResource;
        QString competitionModel;

        // state
        double availableResource;

        //link
        QList<Population*> populationList;

    };

} //namespace

#endif // COMPETITION_H
