/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/

#include <usbase/parameter.h>
#include <usbase/variable.h>
#include "crop.h"

using namespace UniSim;


namespace crop_optimiser {

Crop::Crop(UniSim::Identifier name, QObject *parent)
        : Model(name, parent)
{
    //same for all farm parameters (17 on 11.01.2012)
    new Parameter<double>("InitialArea", &initialArea, 0, this, "Description");
    new Parameter<double>("Subsidy", &subsidy, 0, this, "Description");
    new Parameter<double>("PriceLM", &priceLM, 0, this, "Description");
    new Parameter<double>("PriceHerb", &priceHerb, 0, this, "Description");
    new Parameter<double>("PriceFi", &priceFi, 0, this, "Description");
    new Parameter<double>("PriceG", &priceG, 0, this, "Description");
    new Parameter<double>("PriceH", &priceH, 0, this, "Description");
    new Parameter<double>("PriceW", &priceW, 0, this, "Description");
    new Parameter<double>("AlfaHerb", &alfaHerb, 0, this, "Description");
    new Parameter<double>("BetaHerb", &betaHerb, 0, this, "Description");
    new Parameter<double>("AlfaFi", &alfaFi, 0, this, "Description");
    new Parameter<double>("BetaFi", &betaFi, 0, this, "Description");
    new Parameter<double>("AlfaG", &alfaG, 0, this, "Description");
    new Parameter<double>("BetaG", &betaG, 0, this, "Description");
    new Parameter<double>("AlfaH", &alfaH, 0, this, "Description");
    new Parameter<double>("BetaH", &betaH, 0, this, "Description");
    new Parameter<double>("AlfaW", &alfaW, 0, this, "Description");
    new Parameter<double>("BetaW", &betaW, 0, this, "Description");

    //differ per soil type (4 on 11.01.2012)
    new Parameter<double>("Alfa", &alfa, 0, this, "Description");
    new Parameter<double>("Beta1", &beta1, 0, this, "Description");
    new Parameter<double>("Beta2", &beta2, 0, this, "Description");
    new Parameter<double>("Nnorm", &Nnorm, 0, this, "Description");

    //differ per farm type (3 on 11.01.2012)
    new Parameter<bool>("Fixed", &fixed, 0, this, "Description");
    new Parameter<bool>("Fodder", &fodder, 0, this, "Description");
    new Parameter<double>("FUKey", &FUKey, 0, this, "Description");

    //differ per farm and soil type (1 on 11.01.2012)
    new Parameter<double>("SellingPrice", &sellingPrice, 0, this, "Description");

    //differ per farm, soil type and farm size  (2 on 12.01.2012)
    new Parameter<double>("RotationMax", &rotationMax, 0, this, "Description");
    new Parameter<double>("RotationMin", &rotationMin, 0, this, "Description");


    new Variable<double>("BetaW", &betaW, this, "Description");
    new Variable<double>("Alfa", &alfa,  this, "Description");
    new Variable<double>("Beta1", &beta1, this, "Description");
    new Variable<double>("Beta2", &beta2, this, "Description");

    new Variable<double>("SellingPrice", &sellingPrice, this, "Description");

    //SIMPLIFIED METHOD
    new Parameter<double>("N", &n, 0., this, "Description");
    new Parameter<double>("Nt", &nt, 0., this, "Description");
    new Parameter<double>("Response", &resp, 0., this, "Description");
    new Parameter<double>("BIHerb", &BIHerb, 0., this, "Description");
    new Parameter<double>("BIFi", &BIFi, 0., this, "Description");
    new Parameter<double>("BI", &BI, 0., this, "Description");
    new Parameter<double>("Grooming", &grooming, 0., this, "Description");
    new Parameter<double>("Hoeing", &hoeing, 0., this, "Description");
    new Parameter<double>("Weeding", &weeding, 0., this, "Description");
    new Parameter<double>("LossHerb", &lossHerb, 0., this, "Description");
    new Parameter<double>("LossFi", &lossFi, 0., this, "Description");
    new Parameter<double>("TotalLoss", &totalLoss, 0., this, "Description");
    new Parameter<double>("Costs_ha", &costs_ha, 0., this, "Description");
    new Parameter<double>("Income_ha", &income_ha, 0., this, "Description");
    new Parameter<double>("GM", &GM, 0., this, "Description");
    new Parameter<double>("CostFU", &costFU, 0., this, "Description");
    new Parameter<double>("Savings", &savings, 0., this, "Description");
    new Parameter<double>("AreaPercent", &areaPercent, 0., this, "Description");
    new Parameter<double>("AreaMinPercent", &areaMinPercent, 0., this, "Description");
    new Parameter<double>("Area_ha", &area_ha, 0., this, "Description");
    new Parameter<double>("AreaVariable", &areaVariable, 0., this, "Description");
    new Parameter<double>("RotationMax", &rotationMax, 0., this, "Description");

    //FULL METHOD
    new Parameter<double>("LossHerbBIzero", &lossHerbBIzero, 0., this, "Description");
    new Parameter<double>("LossHerbBImax", &lossHerbBImax, 0., this, "Description");
    new Parameter<double>("LossFiBIzero", &lossFiBIzero, 0., this, "Description");
    new Parameter<double>("LossFiBImax", &lossFiBImax, 0., this, "Description");

    new Parameter<double>("TotalLossNOHerbNOFi", &totalLossNOHerbNOFi, 0., this, "Description");
    new Parameter<double>("TotalLossNOHerbYESFi", &totalLossNOHerbYESFi, 0., this, "Description");
    new Parameter<double>("TotalLossYESHerbNOFi", &totalLossYESHerbNOFi, 0., this, "Description");
    new Parameter<double>("TotalLossYESHerbYESFi", &totalLossYESHerbYESFi, 0., this, "Description");

    new Parameter<double>("N_NONO", &n_NONO, 0., this, "Description");
    new Parameter<double>("N_NOYES", &n_NOYES, 0., this, "Description");
    new Parameter<double>("N_YESNO", &n_YESNO, 0., this, "Description");
    new Parameter<double>("N_YESYES", &n_YESYES, 0., this, "Description");

    new Parameter<double>("Nt_NONO", &nt_NONO, 0., this, "Description");
    new Parameter<double>("Nt_NOYES", &nt_NOYES, 0., this, "Description");
    new Parameter<double>("Nt_YESNO", &nt_YESNO, 0., this, "Description");
    new Parameter<double>("Nt_YESYES", &nt_YESYES, 0., this, "Description");

    new Parameter<double>("Response_NONO", &resp_NONO, 0., this, "Description");
    new Parameter<double>("Response_NOYES", &resp_NOYES, 0., this, "Description");
    new Parameter<double>("Response_YESNO", &resp_YESNO, 0., this, "Description");
    new Parameter<double>("Response_YESYES", &resp_YESYES, 0., this, "Description");
}

void Crop::initialize(){

}

void Crop::reset() {

}

void Crop::update() {

}


} //namespace


