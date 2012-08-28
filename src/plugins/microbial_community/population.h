/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef POPULATION_H
#define POPULATION_H
#include <QObject>
#include <usbase/model.h>

namespace MicrobialCommunity {

    class Population : public UniSim::Model {
        Q_OBJECT

    public:
        Population(UniSim::Identifier name, QObject *parent=0);

        // standard methods
        void initialize();
        void reset();
        void update();

    private:
        // parameters
        QString speciesName;
        double initialDensity, carryingCapacity, lagPhase, growthRate, mortalityRate, searchRate, demandRate;
        double rateConstant_Zero, rateConstant_First, rateConstant_Km, rateConstant_Vmax;
        bool isActive;

        // state
        double density, densityActivityCoef;

        // push
        double sdRatio;

        //link
        UniSim::Model *calendar;

    };

} //namespace

#endif // POPULATION_H
