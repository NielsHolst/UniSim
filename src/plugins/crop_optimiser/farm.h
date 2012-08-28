/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef CROP_OPTIMISER_FARM
#define CROP_OPTIMISER_FARM
#include <QObject>
#include <usbase/model.h>
#include <math.h>
#include <algorithm>
//#include <landscape.h>



namespace crop_optimiser {

class Crop;        //NH better than including the header files
class Field;       //NH
class Livestock;   //NH
//class Landscape;


class Farm : public UniSim::Model
{
	Q_OBJECT
public: 
	Farm(UniSim::Identifier name, QObject *parent=0);

        ~Farm();


	// standard methods
        void initialize();
        void reset();
	void update();


private:
        // parameters
        QString farmID;
        QString farmType;
        QString soilType;
        QString farmRealID;
        QString farmSize;
        double minFodderProduction;

        /** \brief Method for determining fertilizer and pesticides - simplified (NO), full (YES).*/
        bool method_fert;
        /** \brief Price of fodder unit. [DKK/FU]*/
        double priceFU;
        /** \brief Price of fertilizer. [DKK/kg]*/
        double priceNt;
        /** \brief Maximal area taken by winter crops. [%]*/
        double winterMax;

        bool fakeCropTest; //true if the fake crop is present after determineAreas function

        //data
        Crop *fakeCrop;

	// pull variables
        /** \brief Total area of a farm. A sum of initial crop areas.*/
        double totalArea;
        /** \brief Total animal fertilizer at a farm. A sum of NanimUsable (from all types of livestock). [kg]*/
        double totalNanim;
        /** \brief Amount of animal fertilizer available at a farm - per hactar. [kg/ha]*/
        double Nanim;
        /** \brief Farm's total demand for fodder. [fodder units]*/
        double totalFUdemand;
        double totalFUdemandBefore;
        double totalFodderToProduce;
        /** \brief Fodder that has to be produced to fulfill the min. fodder production restriction. Accounts for fodder from fixed crops and from min. areas of variable crops.*/
        double fodderToProduce;
        double fodderToProduceBefore;
        /** \brief Fodder from trade (has to be purchased). [fodder units]*/
        double totalFUt;
        /** \brief Fodder grown - obtained from growing fodder crops. [fodder units]*/
        double totalFUgrown;
        /** \brief Variable holding a value of area already reserved for certain crops at a farm. [0-100%]*/
        double assigned;

        /** \brief Total amount of fertilizer purchased at a farm [kg]. */
        double totalN;
        double totalNt;
        double totalBIHerb;
        double totalBIFi;
        double totalBI;
        double totalGrooming;
        double totalHoeing;
        double totalWeeding;
        double totalCosts;
        double totalIncome;
        double totalProfit;


        //links
        /** \brief List of pointers to fields belonging to a farm*/
        QList<Field*>fields;
        /** \brief List of pointers to animals belonging to a farm*/
        QList<Livestock*>animals;
        /** \brief List of pointers to all crops*/
        QList<Crop*>crops;

        /** \brief Struct used for sorting crops.*/
        struct CropSort {
            double key;
            Crop *crop;
            bool operator<(const CropSort& other) const{
            return key >= other.key;
            }
        };


        /** \brief List of structs containing pointers to crops which are not fixed.*/
        QList<CropSort>variableCrops;
        /** \brief List of structs containing pointers to crops which are not fixed and:
            in case of the winter rotation restriction - are not winter rotation crops,
            in case of the cattle rotation restriction - are not one of the three crops
            that form the condition of the restriction.*/
        QList<CropSort>variableCrops2;
        /** \brief List of pointers to variable crops that are grown on area larger than areaMin (after determineAreas function was called).*/
        QList<Crop*>grownVariableCrops;
        /** \brief List of structs containing pointers to (winter) rotation crops.*/
        QList<CropSort>rotationCrops;
        /** \brief List of structs containing pointers to winter crops.*/
        QList<CropSort>winterCrops;
        /** \brief List of structs containing pointers to cattle rotation crops.*/
        QList<CropSort>cattleCrops;
        /** \brief List of pointers to fixed crops.*/
        QList<Crop*>fixedCrops;
        /** \brief List of pointers to variable fodder crops.*/
        QList<CropSort>fodderCrops;
        /** \brief Struct holding values of four profits and their number (used by the full optimisation method, function #findAll).*/
        struct Profit {
            double profit;
            int option;
        };



        // methods
//        /** \brief Creates a list of pointers to all crops.*/
//        void createCrops();
        /** \brief Creates a list of pointers to all variable crops - included in the optimisation and a list of pointers to fixed crops*/
        void createVariableCrops();
        /** \brief Creates a list of pointers to all fodder crops.*/
        void createFodderCrops();
//        /** \brief Creates a list of pointers to all types of livestock*/
//        void createLivestock();

        //METHODS FOR FARM VARIABLES
        /** \brief Determines #totalArea of a farm.*/
        void findTotalArea();
        /** \brief Determines #totalNanim of a farm.*/
        void findTotalNanim();
        /** \brief Determines #Nanim of a farm.*/
        void findNanim();
        /** \brief Determines #totalFUdemand of a farm.*/
        void findFodderDemand();

        /** \brief Carries out crop optimisation at a farm.*/
        void optimizeCrops();

        //SIMPLIFIED METHOD
        /** \brief Determines the optimal amounts of: total fertilizer (Crop::n) and purchased fertilizer (Crop::nt) for an optimised crop at a farm.*/
        void findFertilizer(int i, QString cropID);
        /** \brief Determines the response (Crop::resp) of a crop at a farm.*/
        void findResponse(int i, QString cropID);
        /** \brief Determines the optimal 'behandling indices'(Crop::BIHerb, Crop::BIFi, Crop::BI) of a crop at a farm.*/
        void findBIs(int i, QString cropID);
        /** \brief Determines the optimal mechanic weed control means (Crop::grooming, Crop::hoeing, Crop::weeding) of a crop at a farm.*/
        void findMWeedControl(int i, QString cropID);
        /** \brief Determines the yield losses (Crop::lossHerb, Crop::lossFi, Crop::totalLoss) of a crop at a farm.*/
        void findYieldLoss(int i, QString cropID);
        /** \brief Determines the gross margin (Crop::GM) of a crop for a farm.*/
        void findGrossMargin(int i, QString cropID);

        //FULL METHOD
        /** \brief Determines values of four possible yield losses: #BIHerb=0, #BIFi=0; #BIHerb=0, #BIFi=BIFiMax; #BIHerb=BIHerbMax, #BIFi=0; #BIHerb=BIHerbMax, #BIFi=BIFiMax.*/
        void findYieldLossBefore(int i, QString cropID);
        /** \brief Determines the optimal amount of fertilizer in each of four possible situations: #BIHerb=0, #BIFi=0; #BIHerb=0, #BIFi=BIFiMax; #BIHerb=BIHerbMax, #BIFi=0; #BIHerb=BIHerbMax, #BIFi=BIFiMax.*/
        void findFertilizerAll (int i, QString cropID);
        /** \brief Assigns a given amount of fertilizer to all four options of pesticide use. Used by #findFertilizerAll function.*/
        void assignFertilizerToAllOptions(int i, double N);
        /** \brief Determines the amount of #n and #nt in case there is not enough animal fertlizer. Used by #findFertilizerAll function.*/
        void fertilizer(int i, QString cropID, double totalLoss, QString N, QString Nt);
        //void findResponseAll (int i, QString cropID);
        /** \brief Determines the most profitable combination of pesticide use as well as corresponding values of fertilizer use and mechanic weed control - for a given crop.*/
        void findAll(int i, QString cropID);
        /** \brief Saves the values of all variables for a given option of pesticide use. Used by #findAll function.*/
        void saveAllVariables(int i, double N, double Nt, double resp, double totalLoss,
                                    double BIHerb, double BIFi, double g, double h, double w,
                                    double income, double costs, double profit);

        //AREA ASSIGNMENT
        /** \brief Assigns areas of fixed crops to the variable #assigned and saves areas of fixed crops - as percentages.*/ 
        void assignFixed();
        /** \brief Determines minimal required areas of variable crops.*/
        void assignMinAreas();
        /** \brief Determines areas of variable crops.*/
        void determineAreas();
        /** \brief Determines areas of crops in ha.*/
        void determineAreas_ha(QList<Crop*>crops);


        //FODDER CROPS
        ///** \brief Determines the cost of a fodder unit (FU) of a fodder crop at a farm.(porbably will not be used after the change:)*/
        //void findFUcost();
        /** \brief Determines the savings resulting from growing a fodder crop - in relation to fodder from trade.*/
        void findFodderCropSavings();
        /** \brief Determines how much fodder is produced from fixed crops and min. areas of variable crops.*/
        void correctFodderDemand();
        /** \brief Determines areas of fodder crops and corrects areas of non-fodder crops.*/
        void determineFodderAreas();
        void determineMinFodder(); //cover min fodder production for animal farms

        //RESTRICTIONS - CROPS AREA
        /** \brief Checks if the restrictions are fulfilled and corrects crops' areas if necessary.*/
        void checkRestrictions();
        void checkWinterCrops();
        void checkWinterCropsAnimal();
        void checkWinterRotation();
        void checkWinterRotationAnimal();
        //next 2 - used in checkWinterRotation (and its animal version), checkWinterCrops(and animal) and checkCattleRotation
        void increaseCrops(QList<CropSort>cropsToIncrease, double &howMuchToIncrease);
        void decreaseCrops(QList<CropSort>cropsToDecrease, double &howMuchToDecrease);
        void setRotationCropsAtMax();
        void checkCattleRotation();
        //next ... used un checkCattleRotation
        void setCattleCropsAtMin();


         /** \brief Function for determining total values - per farm.*/
        double total(QString variable_name);

        /** \brief Reads crops' parameters from input files.*/
        double cropParameter(QString cropId, QString parameterId);
        /** \brief Reads crops' parameters from input files.*/
        bool cropParameterBool(QString cropID, QString parameterID);
        /** \brief Sorts structs of type #CropSort.*/
        void sortCrops(QList<CropSort> &cropsToSort, QString sortingKey);


};

} //namespace
#endif
