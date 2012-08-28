/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef CROP_OPTIMISER_LIVESTOCK
#define CROP_OPTIMISER_LIVESTOCK
#include <QObject>
#include <usbase/model.h>
#include "crop.h"

namespace crop_optimiser {

class Livestock : public UniSim::Model
{
        Q_OBJECT
public:
        Livestock(UniSim::Identifier name, QObject *parent=0);
        // standard methods
        void initialize();
        void reset();
        void update();


private:
        // parameters
        int number;
        double AUKey;
        double Nusable;
        double FUuKey;

        // pull variables

        // push variables
        /** \brief Amount of usable animal fertilizer from a given type of livestock. [kg]*/
        double NanimUsable;
        /** \brief Amount of fodder needed for a given type of livestock. [fodder units]*/
        double FUdemand;


        //links


};

} //namespace
#endif
