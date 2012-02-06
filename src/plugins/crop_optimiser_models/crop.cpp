/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/

#include <usbase/parameter.h>
#include <usbase/pull_variable.h>
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


    //pull parameters....added 12.01.2012
    new PullVariable<double>("InitialArea", &initialArea, this, "Description");
    new PullVariable<double>("Subsidy", &subsidy, this, "Description");
    new PullVariable<double>("PriceLM", &priceLM, this, "Description");
    new PullVariable<double>("PriceHerb", &priceHerb, this, "Description");
    new PullVariable<double>("PriceFi", &priceFi, this, "Description");
    new PullVariable<double>("PriceG", &priceG, this, "Description");
    new PullVariable<double>("PriceH", &priceH, this, "Description");
    new PullVariable<double>("PriceW", &priceW, this, "Description");
    new PullVariable<double>("AlfaHerb", &alfaHerb, this, "Description");
    new PullVariable<double>("BetaHerb", &betaHerb, this, "Description");
    new PullVariable<double>("AlfaFi", &alfaFi, this, "Description");
    new PullVariable<double>("BetaFi", &betaFi, this, "Description");
    new PullVariable<double>("AlfaG", &alfaG, this, "Description");
    new PullVariable<double>("BetaG", &betaG, this, "Description");
    new PullVariable<double>("AlfaH", &alfaH, this, "Description");
    new PullVariable<double>("BetaH", &betaH, this, "Description");
    new PullVariable<double>("AlfaW", &alfaW, this, "Description");
    new PullVariable<double>("BetaW", &betaW, this, "Description");
    new PullVariable<double>("Alfa", &alfa,  this, "Description");
    new PullVariable<double>("Beta1", &beta1, this, "Description");
    new PullVariable<double>("Beta2", &beta2, this, "Description");
    new PullVariable<double>("Nnorm", &Nnorm, this, "Description");
    new PullVariable<double>("FUKey", &FUKey, this, "Description");
    new PullVariable<double>("SellingPrice", &sellingPrice, this, "Description");
    new PullVariable<double>("RotationMax", &rotationMax, this, "Description");
    new PullVariable<double>("RotationMin", &rotationMin, this, "Description");
    new PullVariable<bool>("Fixed", &fixed, this, "Description");
    new PullVariable<bool>("Fodder", &fodder, this, "Description");

    //SIMPLIFIED METHOD
    new PushVariable<double>("N", &n, this, "Description");
    new PushVariable<double>("Nt", &nt, this, "Description");
    new PushVariable<double>("Response", &resp, this, "Description");
    new PushVariable<double>("BIHerb", &BIHerb, this, "Description");
    new PushVariable<double>("BIFi", &BIFi, this, "Description");
    new PushVariable<double>("BI", &BI, this, "Description");
    new PushVariable<double>("Grooming", &grooming, this, "Description");
    new PushVariable<double>("Hoeing", &hoeing, this, "Description");
    new PushVariable<double>("Weeding", &weeding, this, "Description");
    new PushVariable<double>("LossHerb", &lossHerb, this, "Description");
    new PushVariable<double>("LossFi", &lossFi, this, "Description");
    new PushVariable<double>("TotalLoss", &totalLoss, this, "Description");
    new PushVariable<double>("Costs_ha", &costs_ha, this, "Description");
    new PushVariable<double>("Income_ha", &income_ha, this, "Description");
    new PushVariable<double>("GM", &GM, this, "Description");
    new PushVariable<double>("CostFU", &costFU, this, "Description");
    new PushVariable<double>("Savings", &savings, this, "Description");
    new PushVariable<double>("AreaPercent", &areaPercent, this, "Description");
    new PushVariable<double>("AreaMinPercent", &areaMinPercent, this, "Description");
    new PushVariable<double>("Area_ha", &area_ha, this, "Description");
    new PushVariable<double>("AreaVariable", &areaVariable, this, "Description");
    new PushVariable<double>("RotationMax", &rotationMax, this, "Description");

    //FULL METHOD
    new PushVariable<double>("LossHerbBIzero", &lossHerbBIzero, this, "Description");
    new PushVariable<double>("LossHerbBImax", &lossHerbBImax, this, "Description");
    new PushVariable<double>("LossFiBIzero", &lossFiBIzero, this, "Description");
    new PushVariable<double>("LossFiBImax", &lossFiBImax, this, "Description");

    new PushVariable<double>("TotalLossNOHerbNOFi", &totalLossNOHerbNOFi, this, "Description");
    new PushVariable<double>("TotalLossNOHerbYESFi", &totalLossNOHerbYESFi, this, "Description");
    new PushVariable<double>("TotalLossYESHerbNOFi", &totalLossYESHerbNOFi, this, "Description");
    new PushVariable<double>("TotalLossYESHerbYESFi", &totalLossYESHerbYESFi, this, "Description");

    new PushVariable<double>("N_NONO", &n_NONO, this, "Description");
    new PushVariable<double>("N_NOYES", &n_NOYES, this, "Description");
    new PushVariable<double>("N_YESNO", &n_YESNO, this, "Description");
    new PushVariable<double>("N_YESYES", &n_YESYES, this, "Description");

    new PushVariable<double>("Nt_NONO", &nt_NONO, this, "Description");
    new PushVariable<double>("Nt_NOYES", &nt_NOYES, this, "Description");
    new PushVariable<double>("Nt_YESNO", &nt_YESNO, this, "Description");
    new PushVariable<double>("Nt_YESYES", &nt_YESYES, this, "Description");

    new PushVariable<double>("Response_NONO", &resp_NONO, this, "Description");
    new PushVariable<double>("Response_NOYES", &resp_NOYES, this, "Description");
    new PushVariable<double>("Response_YESNO", &resp_YESNO, this, "Description");
    new PushVariable<double>("Response_YESYES", &resp_YESYES, this, "Description");



    //SIMPLIFIED METHOD
    new PullVariable<double>("N", &n, this, "Description");
    new PullVariable<double>("Nt", &nt, this, "Description");
    new PullVariable<double>("Response", &resp, this, "Description");
    new PullVariable<double>("BI", &BI, this, "Description");
    new PullVariable<double>("BIHerb", &BIHerb, this, "Description");
    new PullVariable<double>("BIFi", &BIFi, this, "Description");
    new PullVariable<double>("Grooming", &grooming, this, "Description");
    new PullVariable<double>("Hoeing", &hoeing, this, "Description");
    new PullVariable<double>("Weeding", &weeding, this, "Description");
    new PullVariable<double>("TotalLoss", &totalLoss, this, "Description");
    new PullVariable<double>("Costs_ha", &costs_ha, this, "Description");
    new PullVariable<double>("Income_ha", &income_ha, this, "Description");
    new PullVariable<double>("GM", &GM, this, "Description");
    new PullVariable<double>("CostFU", &costFU, this, "Description");
    new PullVariable<double>("Savings", &savings, this, "Description");
    new PullVariable<double>("AreaPercent", &areaPercent, this, "Description");
    new PullVariable<double>("AreaMinPercent", &areaMinPercent, this, "Description");
    new PullVariable<double>("Area_ha", &area_ha, this, "Description");
    new PullVariable<double>("AreaVariable", &areaVariable, this, "Description");

    //FULL METHOD
    new PullVariable<double>("TotalLossNOHerbNOFi", &totalLossNOHerbNOFi, this, "Description");
    new PullVariable<double>("TotalLossNOHerbYESFi", &totalLossNOHerbYESFi, this, "Description");
    new PullVariable<double>("TotalLossYESHerbNOFi", &totalLossYESHerbNOFi, this, "Description");
    new PullVariable<double>("TotalLossYESHerbYESFi", &totalLossYESHerbYESFi, this, "Description");

    new PullVariable<double>("Response_NONO", &resp_NONO, this, "Description");
    new PullVariable<double>("Response_NOYES", &resp_NOYES, this, "Description");
    new PullVariable<double>("Response_YESNO", &resp_YESNO, this, "Description");
    new PullVariable<double>("Response_YESYES", &resp_YESYES, this, "Description");

    new PullVariable<double>("N_NONO", &n_NONO, this, "Description");
    new PullVariable<double>("N_NOYES", &n_NOYES, this, "Description");
    new PullVariable<double>("N_YESNO", &n_YESNO, this, "Description");
    new PullVariable<double>("N_YESYES", &n_YESYES, this, "Description");

    new PullVariable<double>("Nt_NONO", &nt_NONO, this, "Description");
    new PullVariable<double>("Nt_NOYES", &nt_NOYES, this, "Description");
    new PullVariable<double>("Nt_YESNO", &nt_YESNO, this, "Description");
    new PullVariable<double>("Nt_YESYES", &nt_YESYES, this, "Description");


}

void Crop::initialize(){

}

void Crop::reset() {

}

void Crop::update() {

}


} //namespace


