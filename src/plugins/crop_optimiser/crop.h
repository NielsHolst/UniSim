/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef CROP_OPTIMISER_CROP
#define CROP_OPTIMISER_CROP
#include <QObject>
#include <usbase/model.h>



namespace crop_optimiser {

class Crop : public UniSim::Model
{
        Q_OBJECT
public:
        Crop(UniSim::Identifier name, QObject *parent=0);
        // standard methods
        void initialize();
        void reset();
        void update();

//        bool operator<(Crop*);

private:
        // parameters
        double initialArea;
        double subsidy, sellingPrice;
        double priceLM, priceHerb, priceFi;
        double priceG, priceH, priceW;
        double alfaHerb, betaHerb, alfaFi, betaFi;
        double alfaG, betaG, alfaH, betaH, alfaW, betaW;
        double alfa, beta1, beta2;
        double Nnorm;
        bool fixed, fodder;
        double FUKey;
        double rotationMax, rotationMin;



        // pull variables


        //push variables
        /** \brief */
        double n, nt;
        double n_NONO, n_NOYES, n_YESNO, n_YESYES;
        double nt_NONO, nt_NOYES, nt_YESNO, nt_YESYES;
        double resp;
        double resp_NONO, resp_NOYES, resp_YESNO, resp_YESYES;
        double BIHerb, BIFi, BI;
        double grooming, hoeing, weeding;
        double lossHerbBIzero, lossHerbBImax, lossFiBIzero, lossFiBImax;
        double totalLossNOHerbNOFi, totalLossNOHerbYESFi, totalLossYESHerbNOFi, totalLossYESHerbYESFi;
        double lossHerb, lossFi, totalLoss;
        double costs_ha, income_ha, GM, costFU;
        double savings; //only for fodder crops
        double areaPercent, areaVariable, areaMinPercent, area_ha;



        //links



};

} //namespace
#endif


