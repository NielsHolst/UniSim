/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef CROP_OPTIMISER_LANDSCAPE
#define CROP_OPTIMISER_LANDSCAPE
#include <QObject>
#include <QVector>
#include <usbase/model.h>

namespace crop_optimiser {

class Farm; //AM after NH:) better than including the header files (see also farm.cpp)
//class Crop;

class Landscape : public UniSim::Model
{
        Q_OBJECT
public:
        Landscape(UniSim::Identifier name, QObject *parent=0);
        // standard methods
        void initialize();
        void reset();
        void update();
        void amend();


private:
        // parameters


        // pull variables
        //added 30.01
        QVector<double> cropTotals;


        //push variables


        //links
        //QList<Farm*>crops;
        /** \brief List of pointers to all farms*/
        QList<Farm*>farms;
        QList<Farm*>other_other_business;
        QList<Farm*>other_clay_business;
        QList<Farm*>other_sand_business;
        QList<Farm*>cattle_other_business;
        QList<Farm*>cattle_clay_business;
        QList<Farm*>cattle_sand_business;
        QList<Farm*>plant_other_business;
        QList<Farm*>plant_clay_business;
        QList<Farm*>plant_sand_business;
        QList<Farm*>pig_other_business;
        QList<Farm*>pig_clay_business;
        QList<Farm*>pig_sand_business;

        QList<Farm*>other_other_private;
        QList<Farm*>other_clay_private;
        QList<Farm*>other_sand_private;
        QList<Farm*>cattle_other_private;
        QList<Farm*>cattle_clay_private;
        QList<Farm*>cattle_sand_private;
        QList<Farm*>plant_other_private;
        QList<Farm*>plant_clay_private;
        QList<Farm*>plant_sand_private;
        QList<Farm*>pig_other_private;
        QList<Farm*>pig_clay_private;
        QList<Farm*>pig_sand_private;


        //methods
        //void createFarms();



};

} //namespace
#endif
