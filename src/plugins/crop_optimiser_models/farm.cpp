/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/parameter.h>
#include <usbase/pull_variable.h>
#include <QtAlgorithms>
#include <QFile>
#include <usbase/file_locations.h>
#include "crop.h"  //NH
#include "farm.h"
#include "field.h"          //NH
#include "livestock.h"      //NH


using namespace UniSim;


namespace crop_optimiser {

Farm::Farm(UniSim::Identifier name, QObject *parent)
	: Model(name, parent)
{

    new Parameter<double>("PriceNt", &priceNt, 1.93, this, "Description");
    new Parameter<double>("PriceFU", &priceFU, 1.157, this, "Description");
    new Parameter<QString>("FarmRealID", &farmRealID, "0", this, "Farm's real ID number");
    new Parameter<QString>("FarmType", &farmType, "Plant", this, "Farm's real ID number");
    new Parameter<QString>("SoilType", &soilType, "Sand", this, "Farm's real ID number");
    new Parameter<QString>("FarmSize", &farmSize, "business", this, "Farm's size - business/private");
    new Parameter<bool>("Method_fert", &method_fert, 1, this, "Yes for full method - checking all 4 possible combinations of applying herbicides and fung-and insecticides, no for simplified");
    new Parameter<double>("MinFodderProduction", &minFodderProduction, 20, this, "Description");
    new Parameter<double>("WinterMax", &winterMax, 65, this, "Description");

    new PullVariable<QString>("FarmType", &farmType, this, "Description");

    new PullVariable<double>("TotalArea", &totalArea, this, "Description");
    new PullVariable<double>("TotalNanim", &totalNanim, this, "Description");
    new PullVariable<double>("Nanim", &Nanim, this, "Description");
    new PullVariable<double>("TotalFUdemand", &totalFUdemand, this, "Description");
    new PullVariable<double>("TotalFUdemandBefore", &totalFUdemandBefore, this, "Description");
    new PullVariable<double>("TotalFUt", &totalFUt, this, "Description");
    new PullVariable<double>("FodderToProduce", &fodderToProduce, this, "Description");
    new PullVariable<double>("FodderToProduceBefore", &fodderToProduceBefore, this, "Description");
    new PullVariable<double>("TotalFUgrown", &totalFUgrown, this, "Description");
    new PullVariable<double>("Assigned", &assigned, this, "Description");

    new PullVariable<double>("TotalN", &totalN, this, "Description");
    new PullVariable<double>("TotalNt", &totalNt, this, "Description");
    new PullVariable<double>("TotalBIHerb", &totalBIHerb, this, "Description");
    new PullVariable<double>("TotalBIFi", &totalBIFi, this, "Description");
    new PullVariable<double>("TotalBI", &totalBI, this, "Description");
    new PullVariable<double>("TotalGrooming", &totalGrooming, this, "Description");
    new PullVariable<double>("TotalHoeing", &totalHoeing, this, "Description");
    new PullVariable<double>("TotalWeeding", &totalWeeding, this, "Description");
    new PullVariable<double>("TotalCosts", &totalCosts, this, "Description");
    new PullVariable<double>("TotalIncome", &totalIncome, this, "Description");
    new PullVariable<double>("TotalProfit", &totalProfit, this, "Description");

    fakeCrop = new Crop("FakeCrop", 0);

//    farmID=this->id().label();
}

Farm::~Farm(){
    delete fakeCrop;
}

void Farm::initialize(){

    crops=seekChildren<Crop*>("*");
    animals=seekChildren<Livestock*>("*"); //finds all Farm's children, with any name, and makes a list of Field pointers
    createVariableCrops();
    if (farmType=="Pig" || farmType=="Cattle"){
        createFodderCrops();
    }

}

void Farm::createVariableCrops() {

/**Creates two lists: pointers to fixed crops and structs containing variable crops.*/

    int n = crops.size();
    for (int i=0; i < n; ++i) {
        if (crops[i]->pullVariable<bool>("Fixed")) { //if the crop is fixed, attach it to the list fixedCrops
            fixedCrops.append(crops[i]);
        }
        else{
            CropSort cs = {0., crops[i]}; //an object with a key and pointer to crop
            variableCrops.append(cs);
        }
    }
}

//ONLY ANIMAL FARMS
void Farm:: createFodderCrops(){

/**Creates a lists of pointers to variable fodder crops (those fixed excluded!).*/

    int n = crops.size();
     for (int i=0; i < n; ++i) {
        if(crops[i]->pullVariable<bool>("Fodder")){ //if the crop is a fodder crop, attach it to the list
            if(!crops[i]->pullVariable<bool>("Fixed")){
                CropSort cs={0., crops[i]};
                fodderCrops.append(cs);
            }
        }
    }
}

void Farm::reset() {

     findTotalArea();
     //PLANT AND OTHER FARMS ALSO HAVE LIVESTOCK!!!
     findTotalNanim();
     findNanim();
     findFodderDemand();
}

void Farm::findTotalArea(){ //find total area -> using crop initial areas
/**Function determining farm's total area. Sums initial crop areas on a farm.*/

    totalArea=0.;
     for (int i=0; i<crops.size(); i++) {
         totalArea+=crops[i]->pullVariable<double>("InitialArea");
     }
 }

void Farm::findTotalNanim(){ //find total animal fertilizer
/**Function determining farm's total animal fertilizer. Sums usable fertilizer from all types
of livestock.*/

     totalNanim = 0;
     for (int i=0; i<animals.size(); i++){
         double AUKey = animals[i]->pullVariable<double>("AUKey"); //Number of animal units (AU, DE-dyre enheder) per year
         double Nusable = animals[i]->pullVariable<double>("Nusable"); // [%] Usable fertilizer from livestock
         double number = animals[i]->pullVariable<double>("Number");
         double NanimUsable1 = number*AUKey*Nusable; // [kg] Usable fertilizer from livestock
         animals[i]->pushVariable<double>("NanimUsable", NanimUsable1);
         totalNanim+=NanimUsable1;
     }
}
void Farm::findNanim(){  //find Nanim - animal fertilizer per ha
/**Determines #Nanim - amount of animal fertilizer per ha for a farm.*/
    Nanim = totalNanim/totalArea;
 }
void Farm::findFodderDemand(){
/**Function determining farm's demand for fodder. Sums fodder needed for all types
of livestock. For plant and other farms (which do not grow fodder crops) it assigns
the value of the total fodder demand to the variable #totalFUt (total fodder purhcased).*/

    totalFUdemand=0;
    for(int i=0; i<animals.size(); i++){
        double FUuKey=animals[i]->pullVariable<double>("FUuKey"); //number of fodder units needed per one animal per year
        double number = animals[i]->pullVariable<double>("Number");
        if(number>0){
            double FUdemand=FUuKey*number;
            animals[i]->pushVariable<double>("FUdemand", FUdemand);
            totalFUdemand+=FUdemand;
        }
    }
    totalFUdemandBefore=totalFUdemand; //just to know afterwards the total demand

    if(farmType=="Plant" || farmType=="Other"){
        totalFUt=totalFUdemand;
        totalFUdemand=0;
    }

}


void Farm::update() {

    //writing to a file
    QFile file;
    QString path = FileLocations::location(FileLocationInfo::Output).absolutePath();
    QString fileName = "output.txt";
    QString filePath = path + "/" + fileName;

    file.setFileName(filePath);
    if (!file.open(QIODevice::Text | QIODevice::WriteOnly))
            throw Exception("Could not open output file:'" + filePath + "'");
    //file created

    optimizeCrops();
    sortCrops(variableCrops, "GM" );//ORDER OPTIMISED CROPS WRT HIGHEST GM

    file.write("\n\n after sorting\n");
    for (int i=0; i<variableCrops.size(); i++){
        QString line=variableCrops[i].crop->id().label();
        line+=" " + QString::number(variableCrops[i].crop->pullVariable<double>("GM"))+"\n";
        file.write(qPrintable(line));
    }

    //all farms
    assignFixed();
    assignMinAreas();
    determineAreas();

    //only animal farms
    if (farmType=="Pig" || farmType=="Cattle"){
        findFodderCropSavings();
        sortCrops(fodderCrops, "Savings"); //order fodder crops from the highest to the lowest savings resulting from growing a fodder crop - in relation to purchased fodder

        file.write("\n\n after sorting fodder crops\n");
        for (int i=0; i<fodderCrops.size(); i++){
            QString line=fodderCrops[i].crop->id().label();
            line+=" " + QString::number(fodderCrops[i].crop->pullVariable<double>("Savings"))+"\n";
            file.write(qPrintable(line));
        }

        correctFodderDemand(); //correct demand for min areas and fixed crops
        determineFodderAreas();
    }

    //all farms - but there will be differences between animal and other farms!
    if(farmSize=="business"){
        checkRestrictions();
    }

    //all farms
    determineAreas_ha(crops); //after changes 09/01/12 - it can be done here only

    //all farms
    totalN=total("N");
    totalNt=total("Nt"); //total amount of Nt at a farm - summed for all crops
    totalBIHerb=total("BIHerb");
    totalBIFi=total("BIFi");
    totalBI=total("BI");
    totalGrooming=total("Grooming");
    totalHoeing=total("Hoeing");
    totalWeeding=total("Weeding");
    totalIncome=total("Income_ha");
    totalCosts=total("Costs_ha");
    if(farmType=="Pig" || farmType=="Cattle"){
        totalProfit=total("GM") - totalFUt*priceFU ; //for animal farms deduct costs of fodder!
    }
    else{
        totalProfit=total("GM");
    }

} //update



void Farm::optimizeCrops(){ //loop for all crops - optimise each

/**Function carrying out the optimisation of each crop for a given farm. It contains functions
determining the optimal amounts (per ha) of fertilizer (animal and purchased), pesticides and
mechanic weed control as well as functions finding response (yield per ha), yield loss and
gross margin (profitability) of each crop.
There are two alternative methods of optimisation - simplified, which supposes no yield loss
when detemining the optimal fertilizer use, and full, which compares all possible combinations
of pesticides use (herbicides - max or zero, fung- and insectidies - max or zero) and chooses
the one with highest profit.*/

    for (int i=0; i<crops.size(); i++){

        QString cropID = crops[i]->id().label();

        if(method_fert){ //FULL METHOD
            findYieldLossBefore(i, cropID);
            findFertilizerAll (i, cropID);
            findAll(i, cropID);
        }
        else{ //SIMPLIFIED METHOD
            findFertilizer(i, cropID);
            findResponse(i, cropID);
            findBIs(i, cropID);
            findMWeedControl(i, cropID);
            findYieldLoss(i, cropID);
            findGrossMargin(i, cropID);
        }
    }
}//optimizeCrops




//------------------------------------------------------------------------------//
//---------------------FUNCTIONS FOR THE FULL METHOD----------------------------//
//------------------------------------------------------------------------------//

void Farm::findYieldLossBefore(int i, QString cropID){ //find yield losses

/**Function determining 4 possible total yield losses - 4 possible combinations of not applying
or applying BImax of herbicides and fung-and insecticides. Losses are expressed as percentages
of yield (take values from 0 to 100).*/

    double betaFi=crops[i]->pullVariable<double>("betaFi");
    double betaHerb=crops[i]->pullVariable<double>("betaHerb");

    double lossHerbBIzero = betaHerb;
    crops[i]->pushVariable<double>("LossHerbBIzero", lossHerbBIzero);
    double lossHerbBImax = 0;
    crops[i]->pushVariable<double>("LossHerbBImax", lossHerbBImax);

    double lossFiBIzero = betaFi;
    crops[i]->pushVariable<double>("LossFiBIzero", lossFiBIzero);
    double lossFiBImax = 0;
    crops[i]->pushVariable<double>("LossFiBImax", lossFiBImax);

    double totalLossNOHerbNOFi = lossHerbBIzero + lossFiBIzero;// betaHerb+betaFi
    crops[i]->pushVariable<double>("TotalLossNOHerbNOFi", totalLossNOHerbNOFi);
    double totalLossNOHerbYESFi = lossHerbBIzero + lossFiBImax;// betaHerb+0
    crops[i]->pushVariable<double>("TotalLossNOHerbYESFi", totalLossNOHerbYESFi);
    double totalLossYESHerbNOFi = lossHerbBImax + lossFiBIzero;// 0+betaFi
    crops[i]->pushVariable<double>("TotalLossYESHerbNOFi", totalLossYESHerbNOFi);
    double totalLossYESHerbYESFi = lossHerbBImax + lossFiBImax;// 0+0
    crops[i]->pushVariable<double>("TotalLossYESHerbYESFi", totalLossYESHerbYESFi);
}

void Farm::findFertilizerAll(int i, QString cropID){

/**Function determining the optimal amounts of: total fertilizer (Crop::n) and purchased
fertilizer (Crop::nt) per ha of a crop at a farm. [kg/ha]*/

    double beta1=crops[i]->pullVariable<double>("beta1");
    double beta2=crops[i]->pullVariable<double>("beta2");
    double nNorm =crops[i]->pullVariable<double>("Nnorm");

    if(!beta2==0){ //if parameters are specified - beta2 is not 0.
        double n1=-(beta1/beta2)*0.5; //optimal fertilizer supposing it's for free
        if(Nanim >= n1){ //there is more than you need, so check the norm:
            if (n1<nNorm){
               assignFertilizerToAllOptions(i, n1); //apply optimal amount
            }
            else {
               assignFertilizerToAllOptions(i, nNorm); //apply max allowed amount
            }
            crops[i]->pushVariable<double>("Nt_NONO",0);//don't buy fertilizer
            crops[i]->pushVariable<double>("Nt_NOYES",0);
            crops[i]->pushVariable<double>("Nt_YESNO",0);
            crops[i]->pushVariable<double>("Nt_YESYES",0);
        }

        else { //the optimal amount is larger than Nanim

          //NO herb, NO fi
          double totalloss1=crops[i]->pullVariable<double>("TotalLossNOHerbNOFi");
          fertilizer(i, cropID, totalloss1, "N_NONO", "Nt_NONO");

          //NO herb, YES fi
          double totalloss2=crops[i]->pullVariable<double>("TotalLossNOHerbYESFi");
          fertilizer(i, cropID, totalloss2, "N_NOYES", "Nt_NOYES");

          //YES herb, NO fi
          double totalloss3=crops[i]->pullVariable<double>("TotalLossYESHerbNOFi");
          fertilizer(i, cropID, totalloss3, "N_YESNO", "Nt_YESNO");

          //YES herb, YES fi
          double totalloss4=crops[i]->pullVariable<double>("TotalLossYESHerbYESFi");
          fertilizer(i, cropID, totalloss4, "N_YESYES", "Nt_YESYES");
        }
    }
    else{ //beta2 is = 0
        crops[i]->pushVariable<double>("Nt_NONO",0);//don't buy fertilizer
        crops[i]->pushVariable<double>("Nt_NOYES",0);
        crops[i]->pushVariable<double>("Nt_YESNO",0);
        crops[i]->pushVariable<double>("Nt_YESYES",0);
        assignFertilizerToAllOptions(i, Nanim); //apply fertilizer..but i should change it - it seems a waste of fertilizer...
    }
}

void Farm::assignFertilizerToAllOptions(int i, double N){

    crops[i]->pushVariable<double>("N_NONO", N);
    crops[i]->pushVariable<double>("N_NOYES",N);
    crops[i]->pushVariable<double>("N_YESNO",N);
    crops[i]->pushVariable<double>("N_YESYES",N);
}

void Farm::fertilizer(int i, QString cropID, double totalLoss, QString N, QString Nt){
/**Determines the amount of #n and #nt in case there is not enough animal fertlizer.
Used by #findFertilizerAll function.*/

    double beta1=crops[i]->pullVariable<double>("beta1");
    double beta2=crops[i]->pullVariable<double>("beta2");
    double nNorm = crops[i]->pullVariable<double>("Nnorm");
    double sellingPrice = crops[i]->pullVariable<double>("sellingPrice");

    double x = (100 - totalLoss)/100;
    double nt=(0.5*(priceNt/sellingPrice*beta2*x)-beta1/beta2) - Nanim; //optimal fertilizer Nt
    if (nt>0){
        double n2=nt+Nanim; //optimal total N
        if (n2<nNorm){
          crops[i]->pushVariable<double>(Nt,nt);
          crops[i]->pushVariable<double>(N, n2);
        }
        else {
          double nToSave=nNorm-Nanim;
          crops[i]->pushVariable<double>(Nt,nToSave); //buy the diff between what you have (Nanim) and what is the max allowed
          crops[i]->pushVariable<double>(N, nNorm); //total amount is the max allowed, Nanim+(nNorm-Nanim)=nNorm
        }
    }
    else { //it doesn't pay off to buy fertilizer, so just use Nanim
        crops[i]->pushVariable<double>(N, Nanim);
        crops[i]->pushVariable<double>(Nt, 0);
    }
}

void Farm::findAll(int i, QString cropID){ //change the name!

    //get all the parameters
    double sellingPrice = crops[i]->pullVariable<double>("sellingPrice");

    double alfa=crops[i]->pullVariable<double>("alfa");
    double beta1=crops[i]->pullVariable<double>("beta1");
    double beta2=crops[i]->pullVariable<double>("beta2");

    double alfaFi =crops[i]->pullVariable<double>("alfaFi");
    double alfaHerb =crops[i]->pullVariable<double>("alfaHerb");
    double betaFi =crops[i]->pullVariable<double>("betaFi");
    double betaHerb =crops[i]->pullVariable<double>("betaHerb");

    double alfaG =crops[i]->pullVariable<double>("alfaG");
    double alfaH =crops[i]->pullVariable<double>("alfaH");
    double alfaW =crops[i]->pullVariable<double>("alfaW");
    double betaG =crops[i]->pullVariable<double>("betaG");
    double betaH =crops[i]->pullVariable<double>("betaH");
    double betaW =crops[i]->pullVariable<double>("betaW");

    double priceFi = crops[i]->pullVariable<double>("priceFi");
    double priceHerb=crops[i]->pullVariable<double>("priceHerb");
    double priceG=crops[i]->pullVariable<double>("priceG");
    double priceH=crops[i]->pullVariable<double>("priceH");
    double priceW=crops[i]->pullVariable<double>("priceW");
    double priceLM=crops[i]->pullVariable<double>("priceLM");
    double subsidy=crops[i]->pullVariable<double>( "subsidy");

    //get the values of fertilizer - total and purchased
    double n_NONO=crops[i]->pullVariable<double>("N_NONO"); //n total
    double n_NOYES=crops[i]->pullVariable<double>("N_NOYES");
    double n_YESNO=crops[i]->pullVariable<double>("N_YESNO");
    double n_YESYES=crops[i]->pullVariable<double>("N_YESYES");

    double nt_NONO=crops[i]->pullVariable<double>("Nt_NONO"); //n trade
    double nt_NOYES=crops[i]->pullVariable<double>("Nt_NOYES");
    double nt_YESNO=crops[i]->pullVariable<double>("Nt_YESNO");
    double nt_YESYES=crops[i]->pullVariable<double>("Nt_YESYES");

    //determine the responses
    double respNONO=alfa + beta1*n_NONO + beta2*pow(n_NONO,2);
    double respNOYES=alfa + beta1*n_NONO + beta2*pow(n_NOYES,2);
    double respYESNO=alfa + beta1*n_NONO + beta2*pow(n_YESNO,2);
    double respYESYES=alfa + beta1*n_NONO + beta2*pow(n_YESYES,2);

    crops[i]->pushVariable<double>("Response_NONO",respNONO);
    crops[i]->pushVariable<double>("Response_NOYES",respNOYES);
    crops[i]->pushVariable<double>("Response_YESNO",respYESNO);
    crops[i]->pushVariable<double>("Response_YESYES",respYESYES);

    //get the values of total losses
    double totalLossNONO=crops[i]->pullVariable<double>("TotalLossNOHerbNOFi");
    double totalLossNOYES=crops[i]->pullVariable<double>("TotalLossNOHerbYESFi");
    double totalLossYESNO=crops[i]->pullVariable<double>("TotalLossYESHerbNOFi");
    double totalLossYESYES=crops[i]->pullVariable<double>("TotalLossYESHerbYESFi");

    //calculate BIMax
    double BIHerbMax=0;
    double BIFiMax=0;

    if(alfaHerb>0){ //alfaHerb is zero? if so, just leave BIHerbMax as it was declared,0
        BIHerbMax = betaHerb/alfaHerb;
    }

    if(alfaFi>0){
        BIFiMax = betaFi/alfaFi;
    }


    //manually set the special cases: potatoes, beetroots
    if(cropID=="FodderBeet" || cropID=="SugarBeet"){ //seems it's constant - in the results,but paramters are =zero! this is value of the BIherb max - in restriktioner
        BIHerbMax=2.28;
    }
    if(cropID=="Potato" || cropID=="PotatoFood"){ //potatoes - everything fixed (this is both min and max value for BIHerb)
        BIHerbMax=1.41;
        BIFiMax=9.28;
    }


    //determine grooming, hoeing, weeding
    double g = betaG - BIHerbMax*alfaG;
    double g_NO = betaG; //no herb, BIHerb=0
    double g_YES = (g <= 0) ? 0 : g; //for BIHerbMax

    double h = betaH - BIHerbMax*alfaH;
    double h_NO = betaH;
    double h_YES = (h <= 0) ? 0 : h;

    double w = betaW - BIHerbMax*alfaW;
    double w_NO = betaW;
    double w_YES = (w <= 0) ? 0 : w;


    //determine income, costs and profit for all 4 options
    double incomeNONO= respNONO*(1-totalLossNONO/100)*sellingPrice+subsidy;
    double costsNONO= priceNt*nt_NONO + g_NO*priceG + h_NO*priceH + w_NO*priceW + priceLM;

    double incomeNOYES= respNOYES*(1-totalLossNOYES/100)*sellingPrice+subsidy;
    double costsNOYES= priceNt*nt_NOYES + g_NO*priceG + h_NO*priceH + w_NO*priceW + priceFi*BIFiMax + priceLM;

    double incomeYESNO= respYESNO*(1-totalLossYESNO/100)*sellingPrice+subsidy;
    double costsYESNO= priceNt*nt_YESNO + g_YES*priceG + h_YES*priceH + w_YES*priceW + priceHerb*BIHerbMax + priceLM;

    double incomeYESYES=respYESYES*(1-totalLossYESYES/100)*sellingPrice+subsidy;
    double costsYESYES=priceNt*nt_YESYES + g_YES*priceG + h_YES*priceH + w_YES*priceW + priceHerb*BIHerbMax + priceFi*BIFiMax + priceLM;

    double profitNONO=incomeNONO-costsNONO;
    double profitNOYES=incomeNOYES-costsNOYES;
    double profitYESNO=incomeYESNO-costsYESNO;
    double profitYESYES=incomeYESYES-costsYESYES;

    //find the highest profit (out of four options)
    QList<Profit>profits;
    Profit p1={profitNONO, 1};
    Profit p2={profitNOYES, 2};
    Profit p3={profitYESNO, 3};
    Profit p4={profitYESYES, 4};

    profits.append(p1);
    profits.append(p2);
    profits.append(p3);
    profits.append(p4);

    double profitMax=-100000; //initialize at a very low level, so that at least one of the profits will be higher than this one
    int optionMax;
    //bool check=false; //keeps value false if none of the 4 profits was higher than -100000
    for(int j=0; j<profits.size();j++){
        if(profits[j].profit > profitMax){
            profitMax=profits[j].profit;
            optionMax=profits[j].option;
            //check=true; //profitMax has actually changed
        }
    }

    switch(optionMax){

    case 1: //NONO
        saveAllVariables(i, n_NONO, nt_NONO, respNONO, totalLossNONO, 0, 0,
                         g_NO, h_NO, w_NO, incomeNONO, costsNONO, profitNONO);
        break;

    case 2: //NOYES
        saveAllVariables(i, n_NOYES, nt_NOYES, respNOYES, totalLossNOYES, 0, BIFiMax,
                         g_NO, h_NO, w_NO, incomeNOYES, costsNOYES, profitNOYES);
        break;

    case 3: //YESNO
        saveAllVariables(i, n_YESNO, nt_YESNO, respYESNO, totalLossYESNO, BIHerbMax, 0,
                         g_YES, h_YES, w_YES, incomeYESNO, costsYESNO, profitYESNO);
        break;

    case 4: //YESYES
        saveAllVariables(i, n_YESYES, nt_YESYES, respYESYES, totalLossYESYES, BIHerbMax, BIFiMax,
                         g_YES, h_YES, w_YES, incomeYESYES, costsYESYES, profitYESYES);
        break;
    }

    //special case for potatoes...
    if(cropID=="Potato" || cropID=="PotatoFood"){
        saveAllVariables(i, n_YESYES, nt_YESYES, respYESYES, totalLossYESYES, BIHerbMax, BIFiMax,
                         g_YES, h_YES, w_YES, incomeYESYES, costsYESYES, profitYESYES);
    }
    //would be better not to have this...

}

void Farm::saveAllVariables(int i, double N, double Nt, double resp, double totalLoss,
                            double BIHerb, double BIFi, double g, double h,
                            double w, double income, double costs, double profit){

    crops[i]->pushVariable<double>("N", N);
    crops[i]->pushVariable<double>("Nt",Nt);
    crops[i]->pushVariable<double>("Response", resp);
    crops[i]->pushVariable<double>("TotalLoss", totalLoss);
    crops[i]->pushVariable<double>("BIHerb", BIHerb);
    crops[i]->pushVariable<double>("BIFi", BIFi);
    double BIsum=BIFi+BIHerb;
    crops[i]->pushVariable<double>("BI", BIsum);
    crops[i]->pushVariable<double>("Grooming", g);
    crops[i]->pushVariable<double>("Hoeing", h);
    crops[i]->pushVariable<double>("Weeding", w);
    crops[i]->pushVariable<double>("Income_ha", income);
    crops[i]->pushVariable<double>("Costs_ha", costs);
    crops[i]->pushVariable<double>("GM", profit); //=profitMax
}


//------------------------------------------------------------------------------//
//-----------------FUNCTIONS FOR THE SIMPLIFIED METHOD--------------------------//
//------------------------------------------------------------------------------//

void Farm::findFertilizer(int i, QString cropID){

/**Function determining the optimal amounts of: total fertilizer (Crop::n) and purchased
fertilizer (Crop::nt) per ha of a crop at a farm. [kg/ha]*/

    double beta1=crops[i]->pullVariable<double>("beta1");
    double beta2=crops[i]->pullVariable<double>("beta2");
    double sellingPrice = crops[i]->pullVariable<double>("sellingPrice");
    double nNorm = crops[i]->pullVariable<double>("Nnorm");

    if(!beta2==0){ //beta2 is not 0
        double n1=-(beta1/beta2)*0.5; //optimal fertilizer supposing it's for free

        if(Nanim >= n1){ //there is more than you need, so check the norm:
            if (n1<nNorm){
               crops[i]->pushVariable<double>("N", n1); //apply optimal amount
            }
            else {
               crops[i]->pushVariable<double>("N", nNorm); //apply max allowed amount
            }
            crops[i]->pushVariable<double>("Nt",0);  //don't buy fertilizer
        }
        else { //the optimal amount is larger than Nanim
           double nt1=(0.5*(priceNt - (sellingPrice*beta1))/(sellingPrice*beta2)) - Nanim; //optimal fertilizer Nt
           if (nt1>0){
               double n2=nt1+Nanim; //optimal total N
               if (n2<nNorm){
                 crops[i]->pushVariable<double>("Nt",nt1);
                 crops[i]->pushVariable<double>("N", n2);
               }
               else {
                 double nToSave=nNorm-Nanim;
                 crops[i]->pushVariable<double>("Nt",nToSave); //buy the diff between what you have (Nanim) and what is the max allowed
                 crops[i]->pushVariable<double>("N", nNorm); //total amount is the max allowed, Nanim+(nNorm-Nanim)=nNorm
               }
           }
           else { //it doesn't pay off to buy fertilizer, so just use Nanim
               crops[i]->pushVariable<double>("N", Nanim);
               crops[i]->pushVariable<double>("Nt",0);
           }
        }
    }
    else{ //beta2=0, so don't apply fertilizer...but in this model - apply Nanim if there is any
        crops[i]->pushVariable<double>("N", Nanim);
        crops[i]->pushVariable<double>("Nt",0);
    }
}

void Farm::findResponse (int i, QString cropID){ //find response for a given N

/**Function determining the response of a crop - yield per ha. It's a function of
 total fertilizer (n) applied for a crop at a farm. [hkg/ha]*/

    double beta1=crops[i]->pullVariable<double>("beta1");
    double beta2=crops[i]->pullVariable<double>("beta2");
    double alfa=crops[i]->pullVariable<double>("alfa");

    double ntotal=crops[i]->pullVariable<double>("N");
    double resp1=alfa + beta1*ntotal + beta2*pow(ntotal,2);
    crops[i]->pushVariable<double>("Response",resp1);
}

void Farm::findBIs(int i, QString cropID){ //find BIFi, BIHerb and BI (the sum)

/**Function determining the optimal 'behandling indeks' (treatment frequency index)
for herbicides (BIHerb), fungicides and insecticides (BIFi) and a summary BI per ha.*/

    double sellingPrice = crops[i]->pullVariable<double>("sellingPrice");

    double alfaFi =crops[i]->pullVariable<double>("alfaFi");
    double alfaHerb =crops[i]->pullVariable<double>("alfaHerb");
    double alfaG =crops[i]->pullVariable<double>("alfaG");
    double alfaH =crops[i]->pullVariable<double>("alfaH");
    double alfaW =crops[i]->pullVariable<double>("alfaW");
    double betaFi =crops[i]->pullVariable<double>("betaFi");
    double betaHerb =crops[i]->pullVariable<double>("betaHerb");

    double priceFi = crops[i]->pullVariable<double>("priceFi");
    double priceHerb=crops[i]->pullVariable<double>("priceHerb");
    double priceG=crops[i]->pullVariable<double>("priceG");
    double priceH=crops[i]->pullVariable<double>("priceH");
    double priceW=crops[i]->pullVariable<double>("priceW");

    double resp1=crops[i]->pullVariable<double>("Response");

    if(alfaHerb>0){//make sure it is not zero!it is for Fodder beet

        double BIHerbMax = betaHerb/alfaHerb;
        double gainHerb = sellingPrice*resp1*alfaHerb/100 + priceG*alfaG + priceH*alfaH + priceW*alfaW;
        double BIHerb1 = (gainHerb>priceHerb)? BIHerbMax : 0;
        crops[i]->pushVariable<double>("BIHerb", BIHerb1);
    }
    else{
        crops[i]->pushVariable<double>("BIHerb", 0); //BI for fodder beet in the original model - don't know where did they take this value from!
    }

    if(alfaFi>0){
        double BIFiMax = betaFi/alfaFi;
        double gainFi = sellingPrice*resp1*alfaFi/100;
        double BIFi1 = (gainFi > priceFi) ? BIFiMax : 0;
        crops[i]->pushVariable<double>("BIFi", BIFi1);
    }
    else{
        crops[i]->pushVariable<double>("BIFi", 0);
    }

    Crop *fodderBeet = seekOneChild<Crop*>("FodderBeet");
    Crop *sugarBeet = seekOneChild<Crop*>("SugarBeet");
    Crop *potato = seekOneChild<Crop*>("Potato");
    Crop *potatoFood = seekOneChild<Crop*>("PotatoFood");

    fodderBeet->pushVariable<double>("BIHerb", 2.28); //seems it's constant - in the results,but paramters are =zero!
    sugarBeet->pushVariable<double>("BIHerb", 2.28);

    //potatoes - everything fixed
    potato->pushVariable<double>("BIHerb", 1.41);
    potatoFood->pushVariable<double>("BIHerb", 1.41);
    potato->pushVariable<double>("BIFi", 9.28);
    potatoFood->pushVariable<double>("BIFi", 9.28);
    potato->pushVariable<double>("BI", 10.69);
    potatoFood->pushVariable<double>("BI", 10.69);


    double BIFi1=crops[i]->pullVariable<double>("BIFi");
    double BIHerb1=crops[i]->pullVariable<double>("BIHerb");

    double BI1 = BIFi1+BIHerb1;
    crops[i]->pushVariable<double>("BI", BI1);
}

void Farm::findMWeedControl(int i, QString cropID){ //find grooming, hoeing, weeding

/**Function determining optimal values of mechanic weed control means: grooming, hoeing
and weeding. All of them are functions of 'behandling indeks' for herbicides (BIHerb). */

    double betaG =crops[i]->pullVariable<double>("betaG");
    double betaH =crops[i]->pullVariable<double>("betaH");
    double betaW =crops[i]->pullVariable<double>("betaW");
    double alfaG =crops[i]->pullVariable<double>("alfaG");
    double alfaH =crops[i]->pullVariable<double>("alfaH");
    double alfaW =crops[i]->pullVariable<double>("alfaW");
    double BIHerb1=crops[i]->pullVariable<double>("BIHerb");

    double g = betaG - BIHerb1*alfaG;
    double grooming1 = (g <= 0) ? 0 : g;
    double h = betaH - BIHerb1*alfaH;
    double hoeing1 = (h <= 0) ? 0 : h;
    double w = betaW - BIHerb1*alfaW;
    double weeding1 = (w <= 0) ? 0 : w;

    crops[i]->pushVariable<double>("Grooming", grooming1);
    crops[i]->pushVariable<double>("Hoeing", hoeing1);
    crops[i]->pushVariable<double>("Weeding", weeding1);
}

void Farm::findYieldLoss(int i, QString cropID){ //find yield losses

/**Function determining yield losses resulting from not using maximal amount of
herbicides (lossHerb), fungicides and insecticides (lossFi) and a total yield loss,
which is a sum of lossHerb and lossFi. Losses are expressed as percentages of yield
(take values from 0 to 100).*/

    double alfaFi =crops[i]->pullVariable<double>("alfaFi");
    double alfaHerb =crops[i]->pullVariable<double>( "alfaHerb");
    double betaFi =crops[i]->pullVariable<double>("betaFi");
    double betaHerb =crops[i]->pullVariable<double>("betaHerb");

    double BIHerb1=crops[i]->pullVariable<double>("BIHerb");
    double BIFi1=crops[i]->pullVariable<double>("BIFi");

    double lossHerb1 = betaHerb - alfaHerb*BIHerb1;
    crops[i]->pushVariable<double>("LossHerb", lossHerb1);

    double lossFi1 = betaFi - alfaFi*BIFi1;
    crops[i]->pushVariable<double>("LossFi", lossFi1);

    double totalLoss1 = lossHerb1 + lossFi1;// [%]
    crops[i]->pushVariable<double>("TotalLoss", totalLoss1);
}

void Farm::findGrossMargin(int i, QString cropID){

/**Function determining gross margin (GM, profit per ha) of a crop. [DKK]*/

    double sellingPrice = crops[i]->pullVariable<double>("sellingPrice");

    double priceFi=crops[i]->pullVariable<double>("priceFi");
    double priceHerb=crops[i]->pullVariable<double>("priceHerb");
    double priceG=crops[i]->pullVariable<double>("priceG");
    double priceH=crops[i]->pullVariable<double>("priceH");
    double priceW=crops[i]->pullVariable<double>("priceW");
    double priceLM=crops[i]->pullVariable<double>("priceLM");
    double subsidy=crops[i]->pullVariable<double>("subsidy");

    double resp=crops[i]->pullVariable<double>("Response");
    double BIHerb=crops[i]->pullVariable<double>("BIHerb");
    double BIFi=crops[i]->pullVariable<double>("BIFi");
    double grooming=crops[i]->pullVariable<double>("Grooming");
    double hoeing=crops[i]->pullVariable<double>("Hoeing");
    double weeding=crops[i]->pullVariable<double>("Weeding");
    double totalLoss=crops[i]->pullVariable<double>("TotalLoss");
    double nt=crops[i]->pullVariable<double>("Nt");

    double income_ha = sellingPrice*resp*(1-totalLoss/100)+subsidy;
    double costs_ha = BIHerb*priceHerb + BIFi*priceFi + grooming*priceG + hoeing*priceH + weeding*priceW + nt*priceNt + priceLM;
    double GM = income_ha - costs_ha;
    crops[i]->pushVariable<double>("Income_ha", income_ha);
    crops[i]->pushVariable<double>("Costs_ha", costs_ha);
    crops[i]->pushVariable<double>("GM", GM);
}


//------------------------------------------------------------------------------//
//-----------------FUNCTIONS RELATED TO CROPS AREA------------------------------//
//------------------------------------------------------------------------------//

void Farm::assignFixed(){

/**Function assigning to fixed crops their initial areas as percentages of farm's total area.
It determines the share of a farm (%) reserved for fixed crops - excluded from the optimisation
and saves it in a variable #assigned.*/

    assigned = 0;
    for (int i=0; i<fixedCrops.size(); i++){//take only crops that are fixed
        double area = fixedCrops[i]->pullVariable<double>("InitialArea");
        double areaPercent= area/totalArea*100;
        fixedCrops[i]->pushVariable<double>("AreaPercent", areaPercent); //assign initial area in % for fixed crops
        assigned += areaPercent;//add areaPercent of a fixed crop to assigned
    }//now assigned is a number <0, 100> - excluded from optimisation
}

void Farm::assignMinAreas(){

/**Function determining minimal required areas to all optimised (not fixed) crops at a farm
as percentages of a farm's total area. The function also updates the variable #assigned
holding a share of a farm excluded from the optimisation. */

    for(int i=0; i<variableCrops.size(); i++){
        double rotationMin=variableCrops[i].crop->pullVariable<double>("RotationMin");
        variableCrops[i].crop->pushVariable<double>("AreaMinPercent", rotationMin); //'save' the min area
        assigned+=rotationMin; //the min area added to area already assigned
    }
}

void Farm::determineAreas(){

/**Function determining areas of all optimised (not fixed, non-fodder) crops at a farm
as percentages of a farm's total area. It goes through a list of crops sorted from the
highest to the lowest Gross Margin (i.e., most to least profitable) and assigns a maximal
allowed area for each crop - until the remaining farm's area is equal to or lower than
a maximal allowed area for a given crop. In such case it assigns the remaining area to this
crop. All remaining crops are assigned areas equal to zero (above their minimal required areas).
In case of some farms (i.e. cattle farms) the sum of maximum allowed areas of non-fodder
crops does not reach 100%, therefore it is necessary to introduce a virtual crop to 'fill'
the area til 100%. This virtual crop is substituted by fodder crops in the
determineFodderAreas function.*/

    for(int i=0; i<variableCrops.size(); i++){
        double areaPercent=0;
        double minArea=variableCrops[i].crop->pullVariable<double>("AreaMinPercent");

        //next two lines necessary for animal farms' function version
        bool fodder=variableCrops[i].crop->pullVariable<double>("Fodder");
        if (!fodder){ //this should be checked for animal farms, for other and plant - will always be false
            if (assigned<100){ //check if there is any area left at a farm
                double rotationMax=variableCrops[i].crop->pullVariable<double>("RotationMax");
                areaPercent=(rotationMax-minArea <= 100-assigned)? rotationMax : (100 - assigned+minArea); //check if the remaining area (100-assigned) is enough to assign max allowed area for a crop (max-min, which was already assigned, if not - assign the remaining area+min area
                assigned += areaPercent-minArea; //add the assigned area (final area percent minus minimum; minimum was assigned before) to the total assigned area
                if(areaPercent>minArea){grownVariableCrops.append(variableCrops[i].crop);} //this might not be used for other than animal farms...but maybe - it would be nice to have such a list anyway
            }
            else {areaPercent = minArea;}
            variableCrops[i].crop->pushVariable<double>("AreaPercent", areaPercent);
            double areaVar=areaPercent-minArea;
            variableCrops[i].crop->pushVariable<double>("AreaVariable", areaVar); //save the value of the area that can be changed! For farms other than animal this maybe could be used just in restrictions functions...
        }
        else{ //it is a fodder crop, so just save its min area under areaPercent
            variableCrops[i].crop->pushVariable<double>("AreaPercent", minArea);
        }
    }//after this loop - area assigned should be 100%; update 24.01.12 - but apparently in case
    //of cattle farms - it is not, as most of the crops are fodder crops and the sum of
    //rot max of remaining crops does not reach 100! so - introduce a 'fake' crop -
    //temporarily filling the gap - will be substituted in a determine fodder crops areas func.

    //writing to a file
    QFile file;
    QString path = FileLocations::location(FileLocationInfo::Output).absolutePath();
    QString fileName = "output3 assigned.txt";
    QString filePath = path + "/" + fileName;

    file.setFileName(filePath);
    if (!file.open(QIODevice::Text | QIODevice::WriteOnly))
            throw Exception("Could not open output file:'" + filePath + "'");
    //file created

    file.write("\n\n assigned before fake crop\n");
        QString line=this->id().label();
        line+="  assigned " + QString::number(assigned)+"\n";
        file.write(qPrintable(line));

    fakeCropTest=false;
    if(assigned<100){ //use virtual 'fake crop'
        fakeCropTest=true;
        //Crop *fakeCrop = new Crop("FakeCrop", 0);
        grownVariableCrops.append(fakeCrop); //append at the end - it is the worst crop
//        CropSort fc={0.,fakeCrop};
//        variableCrops.append(fc);
        double areaP=100-assigned;
        fakeCrop->pushVariable<double>("GM", -1000000);
        fakeCrop->pushVariable<double>("RotationMax", 100);       
        fakeCrop->pushVariable<double>("AreaMinPercent", 0); //this is maybe not necessary...
        fakeCrop->pushVariable<double>("AreaPercent", areaP);
        fakeCrop->pushVariable<double>("AreaVariable", areaP);
        assigned+=areaP;
    }
    file.write("\n\n assigned after fake crop\n");
        line+="  assigned " + QString::number(assigned)+"\n";
        file.write(qPrintable(line));

}


//------------------------------------------------------------------------------//
//---------FUNCTIONS RELATED TO RESTRICTIONS ON RELATIVE CROP AREAS-------------//
//------------------------------------------------------------------------------//

void Farm::checkRestrictions(){
/**Function checking if restrictions concerning crop rotations are fulfilled. If not,
it corrects the crop areas in order to fulfill the restrictions. It starts with checking
the restriction on winter crops rotation. Then it checks restriction on max winter crops'
area. At last it checks the restriction on crop rotation on cattle farms.
The order of checking the first two restrictions does matter: in case the first restriction is
not fullfilled, the first function always decreases winter wheat's area and only in a
particular case increases area of winter rape. Thus, the total area of winter crops
(->second restriction) most likely is decreased or, otherwise - unchanged by the function
dealing with the first restriction. The second restriction, on max area of winter crops,
does not allow to decrease area of winter rape; this, in certain cases, might result
in suboptimal final result, but ensures that the first restriction is not violated.
The third restriction does not coincide with the two previous ones.*/

    //later - after implementing animal farm subclass - no if statements necessary, only the
    //last one - for cattle

    //1st restriction: winter rotation
    if(farmType=="Cattle" || farmType=="Pig"){
        checkWinterRotationAnimal();
    }
    else{
        checkWinterRotation();
    }

    //2nd restriction: max share of winter crops area
    if(farmType=="Cattle" || farmType=="Pig"){
        checkWinterCropsAnimal();
    }
    else{
        checkWinterCrops();
    }

    //3rd restriction: cattle farms only
    if(farmType=="Cattle"){
        checkCattleRotation();
    }
}


void Farm::checkWinterRotation () {
/**Function checks if the restriction on rotation:
area(WWheat) - (area(WRape) + area(SRape) + area(Peas) + area(Oats)) <=0,
is fulfilled. If not, first it corrects crops areas (excluding the fixed crop, Peas).
Possible suboptimal result: first the crops forming the condition of the restriction
are changed, only then - other variable crops. It could be that it pays off to change
a variable crop instead of a 'restriction' crop, provided that it is twice as much/twice
as little profitable as a given 'restriction' crop.*/

Crop *wWheat = seekOneChild<Crop*>("WWheat");
Crop *wRape = seekOneChild<Crop*>("WRape");
Crop *sRape = seekOneChild<Crop*>("SRape");
Crop *oats = seekOneChild<Crop*>("Oats");
Crop *peas = seekOneChild<Crop*>("Peas"); //fixed

double areaWWheat=wWheat->pullVariable<double>("AreaPercent");
double areaWRape=wRape->pullVariable<double>("AreaPercent");
double areaSRape=sRape->pullVariable<double>("AreaPercent");
double areaOats=oats->pullVariable<double>("AreaPercent");
double areaPeas=peas->pullVariable<double>("AreaPercent");

double diff = areaWWheat - (areaWRape + areaSRape + areaOats + areaPeas); //difference;
if (diff > 0){ //restriction is not fulfilled
    //QList<CropSort>rotationCrops;
    CropSort cs1 = {0., wRape}; //an object with a key for sorting and a pointer to crop
    CropSort cs2 = {0., sRape};
    CropSort cs3 = {0., oats};
    rotationCrops.append(cs1);
    rotationCrops.append(cs2);
    rotationCrops.append(cs3);

    sortCrops(rotationCrops, "GM"); //sorting the rotation crops wrt GM

    double areaMin=wWheat->pullVariable<double>("AreaMinPercent");

    double available=0; //calculate available area for 'needed' crops - will be needed often:)
    for(int i=0; i<rotationCrops.size(); i++){
        double area=rotationCrops[i].crop->pullVariable<double>("AreaPercent");
        double rotationMax=rotationCrops[i].crop->pullVariable<double>("RotationMax");
        available+=rotationMax-area;
    }

    variableCrops2=variableCrops; //initiate the new list and remove all 'winter rotation' crops:
    for(int s=0; s<variableCrops2.size(); s++){
        if(variableCrops2[s].crop==wWheat || variableCrops2[s].crop==wRape || variableCrops2[s].crop==sRape || variableCrops2[s].crop==oats){
            variableCrops2.removeAt(s);
        }
    }

    if(areaWWheat>areaMin){ //Y1
        if(areaWWheat-areaMin>=diff/2){ //Y2
            if(available>=diff/2){ //Y 3.2
                areaWWheat-=diff/2; //cut wwheat by diff/2
                diff/=2; //updating diff
                increaseCrops(rotationCrops, diff);//increase the needed by diff/2 (now=to diff)
            }
            else{ //N 3.2
                areaWWheat-=available; //cut wwheat by available
                diff-=2*available;//update diff
                setRotationCropsAtMax(); //increase the needed to max each (by available<diff/2),

                if(areaWWheat-diff>=areaMin){//if possible:
                    areaWWheat-=diff;//cut wwheat by diff
                    increaseCrops(variableCrops2, diff);//& increase the best (available) normal crop by diff
                }
                else{
                    //do as much as possible
                    double toIncrease=areaWWheat-areaMin;
                    areaWWheat=areaMin; //cut wwheat to min
                    diff-=toIncrease; //update diff
                    increaseCrops(variableCrops2, toIncrease);//add normal crops
                    //and throw an exception! restriction must be violated
                }
            }
        }
        else{ //N2
            //cut wwheat to area min - in both cases
            if(available>=areaWWheat-areaMin){ //Y 3.1
                double toIncrease=areaWWheat-areaMin;
                areaWWheat=areaMin; //cut WWheat to min
                diff-=2*toIncrease; //update diff
                available-=toIncrease; //decrease by what you add in the next line
                increaseCrops(rotationCrops, toIncrease);//increase needed by areaWWheat-areaMin

                double diffBefore=diff;
                if(available>=diff){
                    increaseCrops(rotationCrops, diff);//increase needed crops by diff
                    decreaseCrops(variableCrops2, diffBefore);//increase them by diff, decrease normal crops by diff
                }
                else{
                    //do as much as possible
                    setRotationCropsAtMax(); //ass as much as possible to the needed crops
                    diff-=available; //update diff
                    double toDecrease=available;
                    decreaseCrops(variableCrops2, toDecrease); //cut some normal crops!
                    //and throw exception - restriction must be violated!
                }
            }
            else{ //N 3.1 - restriction violated, but try to do as much as possible
                areaWWheat-=available; //cut WWheat by available
                setRotationCropsAtMax(); //increase needed crops to max
                diff-=2*available; //update diff
                double toIncrease=areaWWheat-areaMin;
                areaWWheat=areaMin;//cut wwheat  to min
                diff-=toIncrease; //update diff
                increaseCrops(variableCrops2, toIncrease);//add normal crops-but still-violation!
                //AND !!! throw an exception - violation
            }
        }
    wWheat->pushVariable<double>("AreaPercent", areaWWheat);
    }

    else{ // N 1
         if(available>=diff){
             double diffBefore=diff;
             increaseCrops(rotationCrops, diff); //cover the diff be increasing one/more needed crops
             decreaseCrops(variableCrops2, diffBefore); //decrease some normal crops
         }
         else{//impossible to increase the needed crops - restr must be violated
             setRotationCropsAtMax(); //set needed crops at max
             diff-=available;
             double toDecrease=available;
             decreaseCrops(variableCrops2, toDecrease); //decrease normal crops by available
             //throw an exception - restriction must be violated!
         }
    }
 }
}//end of checkWinterRotation def

void Farm::checkWinterRotationAnimal () {
/**Function checks if the restriction on rotation:
area(WWheat) - (area(WRape) + area(SRape) + area(Peas) + area(Oats)) <=0,
is fulfilled. If not, first it corrects crops areas excluding the fixed crop (Peas).
Possible suboptimal result: first the crops forming the condition of the restriction
are changed, only then - other variable crops. It could be that it pays off to change
a variable crop instead of a 'restriction' crop, provided that it is twice as much/twice
as little profitable as a given 'restriction' crop.
In case an area of a fodder crop was changed - the value of totalFUt is updated.*/

Crop *wWheat = seekOneChild<Crop*>("WWheat");
Crop *wRape = seekOneChild<Crop*>("WRape");
Crop *sRape = seekOneChild<Crop*>("SRape");
Crop *oats = seekOneChild<Crop*>("Oats"); //fodder crop for both cattle and pig farms
Crop *peas = seekOneChild<Crop*>("Peas"); //fixed

double areaWWheat=wWheat->pullVariable<double>("AreaPercent");
double areaWRape=wRape->pullVariable<double>("AreaPercent");
double areaSRape=sRape->pullVariable<double>("AreaPercent");
double areaOats=oats->pullVariable<double>("AreaPercent");
double areaPeas=peas->pullVariable<double>("AreaPercent");

double diff = areaWWheat - (areaWRape + areaSRape + areaOats + areaPeas); //difference; Peas is fixed
if (diff > 0){ //restriction is not fulfilled
    //QList<CropSort>rotationCrops;
    CropSort cs1 = {wRape->pullVariable<double>("GM"), wRape}; //an object with a key for sorting and a pointer to crop
    CropSort cs2 = {sRape->pullVariable<double>("GM"), sRape};
    CropSort cs3 = {oats->pullVariable<double>("Savings"), oats};
    rotationCrops.append(cs1);
    rotationCrops.append(cs2);
    rotationCrops.append(cs3);

    qSort(rotationCrops); //sorting the rotation crops

    double areaMin=wWheat->pullVariable<double>("AreaMinPercent");

    //NEW VERSION - STARTED 19/12 11.39:)
    double available=0; //calculate available area for 'needed' crops - will be needed often:)
    for(int i=0; i<rotationCrops.size(); i++){
        double area=rotationCrops[i].crop->pullVariable<double>("AreaPercent");
        double rotationMax=rotationCrops[i].crop->pullVariable<double>("RotationMax");
        available+=rotationMax-area;
    }

    variableCrops2=variableCrops; //initiate the new list and remove all 'winter rotation' crops:
    for(int s=0; s<variableCrops2.size(); s++){
        if(variableCrops2[s].crop==wWheat || variableCrops2[s].crop==wRape || variableCrops2[s].crop==sRape || variableCrops2[s].crop==oats){
            variableCrops2.removeAt(s);
        }
    }

    if(areaWWheat>areaMin){ //Y1
        if(areaWWheat-areaMin>=diff/2){ //Y2
            if(available>=diff/2){ //Y 3.2
                areaWWheat-=diff/2; //cut wwheat by diff/2
                diff/=2; //updating diff
                increaseCrops(rotationCrops, diff);//increase the needed by diff/2 (now=to diff)
            }
            else{ //N 3.2
                areaWWheat-=available; //cut wwheat by available
                diff-=2*available;//update diff
                setRotationCropsAtMax(); //increase the needed to max each (by available<diff/2),

                if(areaWWheat-diff>=areaMin){//if possible:
                    areaWWheat-=diff;//cut wwheat by diff
                    increaseCrops(variableCrops2, diff);//& increase the best (available) normal crop by diff
                }
                else{
                    //do as much as possible
                    double toIncrease=areaWWheat-areaMin;
                    areaWWheat=areaMin; //cut wwheat to min
                    diff-=toIncrease; //update diff
                    increaseCrops(variableCrops2, toIncrease);//add normal crops
                    //and throw an exception! restriction must be violated
                }
            }
        }
        else{ //N2
            //cut wwheat to area min - in both cases
            if(available>=areaWWheat-areaMin){ //Y 3.1
                double toIncrease=areaWWheat-areaMin;
                areaWWheat=areaMin; //cut WWheat to min
                diff-=2*toIncrease; //update diff
                available-=toIncrease; //decrease by what you add in the next line
                increaseCrops(rotationCrops, toIncrease);//increase needed by areaWWheat-areaMin

                double diffBefore=diff;
                if(available>=diff){
                    increaseCrops(rotationCrops, diff);//increase needed crops by diff
                    decreaseCrops(variableCrops2, diffBefore);//increase them by diff, decrease normal crops by diff
                }
                else{
                    //do as much as possible
                    setRotationCropsAtMax(); //ass as much as possible to the needed crops
                    diff-=available; //update diff
                    double toDecrease=available;
                    decreaseCrops(variableCrops2, toDecrease); //cut some normal crops!
                    //and throw exception - restriction must be violated!
                }
            }
            else{ //N 3.1 - restriction violated, but try to do as much as possible
                areaWWheat-=available; //cut WWheat by available
                setRotationCropsAtMax(); //increase needed crops to max
                diff-=2*available; //update diff
                double toIncrease=areaWWheat-areaMin;
                areaWWheat=areaMin;//cut wwheat  to min
                diff-=toIncrease; //update diff
                increaseCrops(variableCrops2, toIncrease);//add normal crops-but still-violation!
                //AND !!! throw an exception - violation
            }
        }
    wWheat->pushVariable<double>("AreaPercent", areaWWheat);
    }

    else{ // N 1
         if(available>=diff){
             double diffBefore=diff;
             increaseCrops(rotationCrops, diff); //cover the diff be increasing one/more needed crops
             decreaseCrops(variableCrops2, diffBefore); //decrease some normal crops
         }
         else{//impossible to increase the needed crops - restr must be violated
             setRotationCropsAtMax(); //set needed crops at max
             diff-=available;
             double toDecrease=available;
             decreaseCrops(variableCrops2, toDecrease); //decrease normal crops by available
             //throw an exception - restriction must be violated!
         }
    }
 }
}//end of checkWinterRotationAnimal def

void Farm::increaseCrops(QList<CropSort>cropsToIncrease, double &howMuchToIncrease){

    for(int i=0; howMuchToIncrease>0 && i<cropsToIncrease.size(); i++){
        double area=cropsToIncrease[i].crop->pullVariable<double>("AreaPercent");
        double areaBefore=area; //this wont be necessary for plant farm funciton
        double rotationMax=cropsToIncrease[i].crop->pullVariable<double>("RotationMax");
        if(area+howMuchToIncrease<=rotationMax){
            area+=howMuchToIncrease; //finito
            howMuchToIncrease=0;
        }
        else{
            howMuchToIncrease-=rotationMax-area;
            area=rotationMax;
        }
        cropsToIncrease[i].crop->pushVariable<double>("AreaPercent", area);

        //this just in an animal version, add if - checking if area has changed
        if(area>areaBefore){
            bool fodder=cropsToIncrease[i].crop->pullVariable<bool>("Fodder");
            if(fodder){ //this is a fodder crop - so you need to buy less fodder - totalFUt
                double resp=cropsToIncrease[i].crop->pullVariable<double>("Response");
                double loss=cropsToIncrease[i].crop->pullVariable<double>("TotalLoss");
                double FUKey=cropsToIncrease[i].crop->pullVariable<bool>("FUKey");
                double FUha=resp*(1-loss/100)*FUKey; //[ha * hkg/ha * FU/hkg = FU]
                double changeFU=(area-areaBefore)*totalArea/100*FUha; //positive
                totalFUt-= changeFU;
                if(totalFUt<0){totalFUt=0;} //changed 23.01.12, see dziennik
                totalFUgrown+=changeFU;
            }
        }
    }
}

void Farm::decreaseCrops(QList<CropSort>cropsToDecrease, double &howMuchToDecrease){

    for(int j=cropsToDecrease.size()-1; howMuchToDecrease>0 && j>=0; j--){ //start with the worst
        double areaCrop=cropsToDecrease[j].crop->pullVariable<double>("AreaPercent");
        double areaBefore=areaCrop; //this wont be necessary for plant farm funciton
        double rotMinCrop=cropsToDecrease[j].crop->pullVariable<double>("RotationMin");
        if(areaCrop-howMuchToDecrease >= rotMinCrop){
            areaCrop-=howMuchToDecrease;
            howMuchToDecrease=0; //finito
        }
        else{
            howMuchToDecrease-=areaCrop - rotMinCrop;
            areaCrop=rotMinCrop;
        }
        cropsToDecrease[j].crop->pushVariable<double>("AreaPercent", areaCrop);

        //this just in an animal version, add if - checking if area has changed
        if(areaCrop<areaBefore){
            bool fodder=cropsToDecrease[j].crop->pullVariable<bool>("Fodder");
            if(fodder){ //this is a fodder crop - so you need to buy more fodder - totalFUt
                double resp=cropsToDecrease[j].crop->pullVariable<double>("Response");
                double loss=cropsToDecrease[j].crop->pullVariable<double>("TotalLoss");
                double FUKey=cropsToDecrease[j].crop->pullVariable<double>("FUKey");
                double FUha=resp*(1-loss/100)*FUKey; //[ha * hkg/ha * FU/hkg = FU]
                double changeFU=(areaBefore-areaCrop)*totalArea/100*FUha; //positive number
                totalFUt+= changeFU;
                totalFUgrown-=changeFU;
            }
        }
    }
}

void Farm::setRotationCropsAtMax(){

    for(int i=0; i<rotationCrops.size(); i++){ //just go through all of them
        double area=rotationCrops[i].crop->pullVariable<double>("AreaPercent");
        double areaBefore=area;
        double rotationMax=rotationCrops[i].crop->pullVariable<double>("RotationMax");
        area=rotationMax; //set all at max (maybe put if here, but not necessary
        rotationCrops[i].crop->pushVariable<double>("AreaPercent", area);

        //animal version only
        if(area>areaBefore){
            bool fodder=rotationCrops[i].crop->pullVariable<bool>("Fodder");
            if(fodder){ //that is oats
                double resp=rotationCrops[i].crop->pullVariable<double>("Response");
                double loss=rotationCrops[i].crop->pullVariable<double>("TotalLoss");
                double FUKey=rotationCrops[i].crop->pullVariable<double>("FUKey");
                double FUha=resp*(1-loss/100)*FUKey; //[ha * hkg/ha * FU/hkg = FU]
                double changeFU=(area-areaBefore)*totalArea/100*FUha; //positive number
                totalFUt-= changeFU; //so you buy less
                if(totalFUt<0){totalFUt=0;} //changed 23.01.12, see dziennik
                totalFUgrown+=changeFU; //and grow more
            }
        }
    }
}

void Farm::checkWinterCrops(){

/**Function checking if the restriction on maximal winter crops area is fulfilled. If not,
it corrects winter crops areas (starting with the one with the lowest GM) and assigns the
area above the limit for winter crops to the most profitable variable crop(s) that haven't
reached the maximimum area after determineAreas function was called. None of the winter crops
is fixed.*/

Crop *wBarley = seekOneChild<Crop*>("WBarley");
Crop *wWheat = seekOneChild<Crop*>("WWheat");
Crop *wRye = seekOneChild<Crop*>("WRye");
Crop *wRape = seekOneChild<Crop*>("WRape"); //should not be decreased - might cause violation of winterRotation restriction
Crop *wCerealSil = seekOneChild<Crop*>("WCerealSil");

double areaWBarley=wBarley->pullVariable<double>("AreaPercent");
double areaWWheat=wWheat->pullVariable<double>("AreaPercent");
double areaWRye=wRye->pullVariable<double>("AreaPercent");
double areaWRape=wRape->pullVariable<double>("AreaPercent");
double areaWCerealSil=wCerealSil->pullVariable<double>("AreaPercent");

double sum=areaWBarley+areaWWheat+areaWRye+areaWRape+areaWCerealSil;

if(sum>winterMax){

    variableCrops2=variableCrops; //initiate the new list and remove all winter crops:
    for(int s=0; s<variableCrops2.size(); s++){
        if(variableCrops2[s].crop==wBarley || variableCrops2[s].crop==wWheat || variableCrops2[s].crop==wRye || variableCrops2[s].crop==wRape || variableCrops2[s].crop==wCerealSil){
            variableCrops2.removeAt(s);
        }
    }

    double diff=sum-winterMax;
    double diffBefore=diff;

    //QList<CropSort>winterCrops;
    CropSort cs1 = {0., wBarley}; //an object with a key for sorting and a pointer to crop
    CropSort cs2 = {0., wWheat};
    CropSort cs3 = {0., wRye};
    //CropSort cs4 = {0., wRape};
    CropSort cs5 = {0., wCerealSil};
    winterCrops.append(cs1);
    winterCrops.append(cs2);
    winterCrops.append(cs3);
    //winterCrops.append(cs4);
    winterCrops.append(cs5);

    sortCrops(winterCrops, "GM"); //changed 06.12, 15.12 - WRape excluded

    //decrease area of one or more winter crops by diff
    decreaseCrops(winterCrops, diff);

    if(diff>0){ //loop stopped by the other condition: impossible to cut more
        //violation of the restriction
    }

    //assign area cut from winter crops to one or more variable crops (that haven't reached the max yet)
    double toIncrease=diffBefore-diff; //should be diffBefore...but if there's violation - then it is smaller
    increaseCrops(variableCrops2, toIncrease);
    //increaseCrops(variableCrops2, diffBefore);

}//else nothing!:) it's ok

}//end of checkWinterCrops

void Farm::checkWinterCropsAnimal(){

/**Function for ANIMAL farms checking if restriction on maximal winter crops area is fulfilled.
Both for cattle and pig farms two of them - WBarley and WCerealSil - are fodder crops.
If not, it corrects winter crops areas (starting with the one with the lowest GM or lowest
Savings - in case of fodder crops). In case a fodder crop's area was cut, the function
 increases the value of fodder to be purchased (totalFUt). After cutting winter crops area,
the area above the winter crops limit is assigned to the variable crop(s) that haven't
reached the maximimum area after determineAreas function was called.*/

Crop *wBarley = seekOneChild<Crop*>("WBarley"); //fodder
Crop *wWheat = seekOneChild<Crop*>("WWheat");
Crop *wRye = seekOneChild<Crop*>("WRye");
Crop *wRape = seekOneChild<Crop*>("WRape"); //should not be decreased - might cause violation of winterRotation restriction
Crop *wCerealSil = seekOneChild<Crop*>("WCerealSil"); //fodder

double areaWBarley=wBarley->pullVariable<double>("AreaPercent");
double areaWWheat=wWheat->pullVariable<double>("AreaPercent");
double areaWRye=wRye->pullVariable<double>("AreaPercent");
double areaWRape=wRape->pullVariable<double>("AreaPercent");
double areaWCerealSil=wCerealSil->pullVariable<double>("AreaPercent");

double sum=areaWBarley+areaWWheat+areaWRye+areaWRape+areaWCerealSil;



if(sum>winterMax){
    double diff=sum-winterMax;
    double diffBefore=diff;

    variableCrops2=variableCrops; //initiate the new list and remove all winter crops:
    for(int s=0; s<variableCrops2.size(); s++){
        if(variableCrops2[s].crop==wBarley || variableCrops2[s].crop==wWheat || variableCrops2[s].crop==wRye || variableCrops2[s].crop==wRape || variableCrops2[s].crop==wCerealSil){
            variableCrops2.removeAt(s);
        }
    }

    //QList<CropSort>winterCrops;
    //assign keys for sorting - for fodder crops these will be Savings!
    CropSort cs1 = {wBarley->pullVariable<double>("Savings"), wBarley}; //an object with a key for sorting and a pointer to crop
    CropSort cs2 = {wWheat->pullVariable<double>("GM"), wWheat};
    CropSort cs3 = {wRye->pullVariable<double>("GM"), wRye};
    //CropSort cs4 = {wRape->pullVariable<double>("GM"), wRape};
    CropSort cs5 = {wCerealSil->pullVariable<double>("Savings"), wCerealSil};
    winterCrops.append(cs1);
    winterCrops.append(cs2);
    winterCrops.append(cs3);
    //winterCrops.append(cs4);
    winterCrops.append(cs5);

    qSort(winterCrops); //use this basic function; the keys assigned 'manually' - they differ!

    //decrease area of one or more winter crops by diff
    decreaseCrops(winterCrops, diff);

    if(diff>0){ //loop stopped by the other condition: impossible to cut more
        //violation of the restriction
    }

    //assign area cut from winter crops to one or more variable crops (that haven't reached the max yet)
    double toIncrease=diffBefore-diff; //should be diffBefore...but if there's violation - then it is smaller
    increaseCrops(variableCrops2, toIncrease);

}//else nothing!:) it's ok

}//end of checkWinterCropsAnimal

void Farm::setCattleCropsAtMin(){

    for(int i=0; i<cattleCrops.size(); i++){ //just go through all 2 of them:)
        double area=cattleCrops[i].crop->pullVariable<double>("AreaPercent");
        double areaBefore=area;
        double rotationMin=cattleCrops[i].crop->pullVariable<double>("RotationMin");
        area=rotationMin; //set all at min (maybe put if here, but not necessary
        cattleCrops[i].crop->pushVariable<double>("AreaPercent", area);

        //animal version only
    if(areaBefore>area){
        bool fodder=cattleCrops[i].crop->pullVariable<bool>("Fodder");
        if(fodder){
            double resp=cattleCrops[i].crop->pullVariable<double>("Response");
            double loss=cattleCrops[i].crop->pullVariable<double>("TotalLoss");
            double FUKey=cattleCrops[i].crop->pullVariable<double>("FUKey");
            double FUha=resp*(1-loss/100)*FUKey; //[ha * hkg/ha * FU/hkg = FU]
            double changeFU=(areaBefore-area)*totalArea/100*FUha; //positive number
            totalFUt+= changeFU; //so you buy more
            totalFUgrown-=changeFU; //and grow less:)
        }
    }
    }
}

void Farm:: checkCattleRotation(){
/**Function checking if restriction for cattle farms on attachment (?) of spring barley,
clover-grass and grass seed is fulfilled. (None of the three crops is fixed.
Spring barley and grass-clover are fodder crops.) If not, it corrects crops' areas
- if possible, increases area of Spring Barley and cuts grass-clover and/or grass seed.
Might give suboptimal result: it first tries always to correct one of the three crops
forming the restriction's condition, while it could be that it pays off to change some
other, 'normal' crop instead - provided it is at least twice as much/little
profitable as one of these changed three crops.*/

//This function can be called only after fodder crops were assigned areas! because two of them
//are fodder crops! so before assiging the fodder crops, their areas will be zero!

Crop *sBarley = seekOneChild<Crop*>("SBarley"); //fodder
Crop *gClover = seekOneChild<Crop*>("GrassClover"); //fodder
Crop *gSeed = seekOneChild<Crop*>("GrassSeed");

double areaSBarley=sBarley->pullVariable<double>("AreaPercent");
double areaGClover=gClover->pullVariable<double>("AreaPercent");
double areaGSeed=gSeed->pullVariable<double>("AreaPercent");

double diff= areaGClover+areaGSeed - areaSBarley;

if(diff>0){ //restriction is not fulfilled
    QList<CropSort>cattleCrops;
    CropSort cs1 = {gClover->pullVariable<double>("Savings"), gClover}; //an object with a key for sorting and a pointer to crop
    CropSort cs2 = {gSeed->pullVariable<double>("GM"), gSeed};
    cattleCrops.append(cs1);
    cattleCrops.append(cs2);

    qSort(cattleCrops); //sorting the two crops

    double areaSBarleyMax=sBarley->pullVariable<double>("RotationMax");
    double SBarleyAvailableArea=areaSBarleyMax-areaSBarley;
    double areaSBarleyBefore=areaSBarley;

    double available=0; //available to cut from GClover and GSeed
    for(int i=0; i<cattleCrops.size(); i++){  //or simply: i<2
        double areaMin=cattleCrops[i].crop->pullVariable<double>("AreaMinPercent");
        double area=cattleCrops[i].crop->pullVariable<double>("AreaPercent");
        available+=(area-areaMin);
    }

    variableCrops2=variableCrops; //initiate the new list and remove all three crops:
    for(int s=0; s<variableCrops2.size(); s++){
        if(variableCrops2[s].crop==sBarley || variableCrops2[s].crop==gClover || variableCrops2[s].crop==gSeed){
            variableCrops2.removeAt(s);
        }
    }

    //writing to a file
    QFile file;
    QString path = FileLocations::location(FileLocationInfo::Output).absolutePath();
    QString fileName = "output5 cattle rot.txt";
    QString filePath = path + "/" + fileName;

    file.setFileName(filePath);
    if (!file.open(QIODevice::Text | QIODevice::WriteOnly))
            throw Exception("Could not open output file:'" + filePath + "'");
    //file created

    file.write("\n\n before changes \n");
        QString line=this->id().label();
        double areaSB=sBarley->pullVariable<double>("AreaPercent");
        line+="  SBarley area " + QString::number(areaSB)+"\n";
        double areaGC=gClover->pullVariable<double>("AreaPercent");
        line+="  grass Clover area " + QString::number(areaGC)+"\n";
        double areaGS=gSeed->pullVariable<double>("AreaPercent");
        line+="  grass Seed area " + QString::number(areaGS)+"\n";
        file.write(qPrintable(line));




    if(areaSBarley<areaSBarleyMax){ //Y1
        if(SBarleyAvailableArea>=diff/2){ //Y2
            if(available>=diff/2){ //Y 3.2
                areaSBarley+=diff/2; //increase Sbarley by diff/2
                diff/=2; //updating diff
                decreaseCrops(cattleCrops, diff);//decrease the two crops by diff/2 (now=to diff)       


            file.write("\n\n after changes? \n");
                QString line2=this->id().label();
                //double areaSB=sBarley->pullVariable<double>("AreaPercent");
                line2+="  SBarley area " + QString::number(areaSBarley)+"\n";
                double areaGC=gClover->pullVariable<double>("AreaPercent");
                line2+="  grass Clover area " + QString::number(areaGC)+"\n";
                double areaGS=gSeed->pullVariable<double>("AreaPercent");
                line2+="  grass Seed area " + QString::number(areaGS)+"\n";
                file.write(qPrintable(line2));
            }

            else{ //N 3.2
                areaSBarley+=available; //
                diff-=2*available;//update diff
                setCattleCropsAtMin(); //decrease the two crops to min each (by available<diff/2),

                if(SBarleyAvailableArea>=diff){//if possible:
                    areaSBarley+=diff;//increase Sbarley by diff
                    decreaseCrops(variableCrops2, diff);//& decrease the worst (available) normal crop by diff
                }
                else{
                    //do as much as possible
                    double toDecrease=areaSBarleyMax-areaSBarley; //neccesary -area has changed
                    areaSBarley=areaSBarleyMax; //increase Sbarley to max
                    diff-=toDecrease; //update diff
                    decreaseCrops(variableCrops2, toDecrease);//decrease normal crops
                    //and throw an exception! restriction must be violated
                }
            }
        }
        else{ //N2
            //increase SBarley to max in both cases
            if(available>=SBarleyAvailableArea){ //Y 3.1
                //double toDecrease=SBarleyAvailableArea;//not necessary, also in other cases..
                areaSBarley=areaSBarleyMax; //increase Sbarley to max
                diff-=2*SBarleyAvailableArea; //update diff
                available-=SBarleyAvailableArea; //decrease by what you add in the next line
                decreaseCrops(cattleCrops, SBarleyAvailableArea);//increase needed by areaWWheat-areaMin

                double diffBefore=diff;
                if(available>=diff){
                    decreaseCrops(cattleCrops, diff);//decrease the two crops by diff
                    increaseCrops(variableCrops2, diffBefore);//increase normal crops by diff
                }
                else{
                    //do as much as possible
                    setCattleCropsAtMin();
                    diff-=available; //update diff
                    double toDecrease=available;
                    increaseCrops(variableCrops2, toDecrease); //add some normal crops!
                    //and throw exception - restriction must be violated!
                }
            }
            else{ //N 3.1 - restriction violated, but try to do as much as possible
                areaSBarley+=available; //
                setCattleCropsAtMin(); //
                diff-=2*available; //update diff
                double toDecrease=areaSBarleyMax-areaSBarley; //here it is necessary! cause area has changed
                areaSBarley=areaSBarleyMax;//
                diff-=toDecrease; //update diff
                decreaseCrops(variableCrops2, toDecrease);//add normal crops-but still-violation!
                //AND !!! throw an exception - violation
            }
        }

    sBarley->pushVariable<double>("AreaPercent", areaSBarley);
    //this is a fodder crop - so you need to buy less fodder - totalFUt
        double resp=sBarley->pullVariable<double>("Response");
        double loss=sBarley->pullVariable<double>("TotalLoss");
        double FUKey=sBarley->pullVariable<bool>("FUKey");
        double FUha=resp*(1-loss/100)*FUKey; //[ha * hkg/ha * FU/hkg = FU]
        double changeFU=(areaSBarley - areaSBarleyBefore)*totalArea/100*FUha; //positive
        totalFUt-= changeFU;
        if(totalFUt<0){totalFUt=0;} //changed 23.01.12, see dziennik
        totalFUgrown+=changeFU;
    }

    else{ // N 1
         if(available>=diff){
             double diffBefore=diff;
             decreaseCrops(cattleCrops, diff); //cover the diff
             increaseCrops(variableCrops2, diffBefore); //increase some normal crops
         }
         else{//restr must be violated
             setCattleCropsAtMin(); //set the two crops at min
             diff-=available;
             double toIncrease=available;
             increaseCrops(variableCrops2, toIncrease);
             //throw an exception - restriction must be violated!
         }
    }
}//end of the if(diff>0)?

}//end of check CattleRotation


void Farm::findFodderCropSavings(){

/**Function determining savings resulting from growing 1 ha of a fodder crop (instead of
purchasing the corresponding amount of fodder). Includes only variable fodder crops. [DKK]*/

    for (int i=0; i<fodderCrops.size(); i++){

        double FUKey=fodderCrops[i].crop->pullVariable<double>("FUKey"); //number of FU per hkg of a crop
        double resp=fodderCrops[i].crop->pullVariable<double>("Response");
        double loss=fodderCrops[i].crop->pullVariable<double>("TotalLoss");
        double cost_ha=fodderCrops[i].crop->pullVariable<double>("Costs_ha");
        double savings=priceFU*resp*(1-loss/100)*FUKey - cost_ha;
        fodderCrops[i].crop->pushVariable<double>("Savings", savings);
    }
}

void Farm::correctFodderDemand(){
/**Function determining how much fodder is produced from fixed crops and minimial areas
of variable crops.Updates two variables: first - holding the value of the total fodder demand
for a farm, second - holding the value of the fodder that has to be obtained from own
production.*/

    totalFUgrown=0; //initialize
    totalFodderToProduce=(minFodderProduction/100)*totalFUdemand; //[FU]

    double fodderFromFixed=0;
    for (int i=0; i<fixedCrops.size(); i++){
        double area_percent=fixedCrops[i]->pullVariable<double>("AreaPercent");
        double area=area_percent/100* totalArea; //this and previous line-changed 09.01.12 & 23.01.12
        double resp=fixedCrops[i]->pullVariable<double>("Response");
        double loss=fixedCrops[i]->pullVariable<double>("TotalLoss");
        double FUKey=fixedCrops[i]->pullVariable<double>("FUKey"); //if it is not a fodder crop, key=0
        double FU=area*resp*(1-loss/100)*FUKey; //[ha * hkg/ha * FU/hkg = FU]
        fodderFromFixed+=FU;//0 will be added in case of non-fodder crops
        totalFUgrown+=FU;
    }
    double fodderFromMinAreas=0;
    for (int i=0; i<variableCrops.size(); i++){
        double areaMinPercent=variableCrops[i].crop->pullVariable<double>("AreaMinPercent");
        double area=areaMinPercent/100*totalArea; //min area in ha
        double resp=variableCrops[i].crop->pullVariable<double>("Response");
        double loss=variableCrops[i].crop->pullVariable<double>("TotalLoss");
        double FUKey=variableCrops[i].crop->pullVariable<double>("FUKey");
        double FU=area*resp*(1-loss/100)*FUKey; //[ha * hkg/ha * FU/hkg = FU]
        fodderFromMinAreas+=FU;
        totalFUgrown+=FU;
    }
    //this much fodder HAS to be obtained from self-production IF the requirment
    //on min fodder prod holds; might be negative!
    fodderToProduce=totalFodderToProduce-(fodderFromFixed+fodderFromMinAreas);
    fodderToProduceBefore=fodderToProduce;
    //decrease the fodder demand by the amount obtained from growing fixed crops and from
    //min areas of variable crops; could be also negative now;
    totalFUdemand-=(fodderFromFixed+fodderFromMinAreas);


    //writing to a file
    QFile file;
    QString path = FileLocations::location(FileLocationInfo::Output).absolutePath();
    QString fileName = "output1.txt";
    QString filePath = path + "/" + fileName;

    file.setFileName(filePath);
    if (!file.open(QIODevice::Text | QIODevice::WriteOnly))
            throw Exception("Could not open output file:'" + filePath + "'");
    //file created

    file.write("\n\n correct fodder demand function\n");
        QString line=this->id().label();
        line+="  tot fodder to prod " + QString::number(totalFodderToProduce)+"\n";
        line+="  minFodderProduction " + QString::number(minFodderProduction)+"\n";
        line+="  totalFUdemand " + QString::number(totalFUdemand)+"\n";
        line+=" totalFUdemandBefore " + QString::number(totalFUdemandBefore)+"\n";
        line+=" totalFUgrown " + QString::number(totalFUgrown)+"\n";
        line+=" fodder from fixed " + QString::number(fodderFromFixed)+"\n";
        line+=" fodder from min " + QString::number(fodderFromMinAreas)+"\n";
        line+=" fodder to produce " + QString::number(fodderToProduce)+"\n";

        file.write(qPrintable(line));

}

void Farm::determineFodderAreas(){
/**Function determining areas for fodder crops. It starts with a function determining areas
for fodder that has to be grown to fulfill the condition on min. requirement of own fodder
production. This function should not be called if the 20 (pig) or 35% (cattle farm)is treated
as a maximum, ant not a minimum share of own fodder production in total fodder usage.
The second part of the function chooses the most profitable alternative of covering the
remaining fodder demand: purchasing fodder, growing fodder crops, or a mix of those two.
In case any fodder crops are grown (above their minimum area), one or more of the variable
crops' areas has to be corrected (decreased).
The general structure of the part of the function determining whether to grow fodder, buy it or
mix the two: The outer for-loop goes through fodder crops until total fodder demand is zero –
and it starts with the crop with highest savings (the best one). At the beginning it checks
if the fodder crop has reached its max rotation area. The inner for-loop goes through all
non-fodder variable crops that after they were assigned area had an area above area min
(there’s no point in checking other crops – their area cannot be decreased). It starts with
the crop with the lowest GM (the worst one) and the condition in the loop is a Boolean value
which turns to true, once a current fodder crop reached its area max. Then the outer loop
takes the next best fodder crop.
In some cases even when totalFUdemand is covered in 100% by fodder crops, there is still
a virtual (non-fodder) crop present at a farm. In such case this virtual crop has to be
substituted by fodder crops and the total fodder production is higher than the total
fodder demand (totalFUdemand).*/


//assigning area to fodder crops in order to cover the min fodder production: don't call this
//function if the 20 or 35% is treated as a max, not min own fodder production!

    //determineMinFodder();

    totalFUt=0; //(initialize - just in case all fodder crops had savings higher than GM)

 //now the choice - produce or purchase fodder

    //go through fodder crops - start with the one with max savings. Check if it reached its max area (could have happened in the previous step)

    int option=0;

for(int p=0; totalFUdemand>0 && p<fodderCrops.size(); p++){ //outer for loop - fodder crops;

    double rotationMax=fodderCrops[p].crop->pullVariable<double>("RotationMax");
    double areaFodder=fodderCrops[p].crop->pullVariable<double>("AreaPercent");
    if(areaFodder<rotationMax){ //if not -> go to the next fodder crop
        double savings=fodderCrops[p].crop->pullVariable<double>("Savings");
        bool assigningFinished=false; //changed to true when a fodder crop reaches rotationMax

        int count=0;
        bool o1=false;
        bool o2=false;
        bool o3=false;
        bool o4=false;

        //for(int v=grownVariableCrops.size()-1; assigningFinished==false && v>=0; v--){
        //inner for loop - normal crops; starts with the crop with the lowest GM; changed to do-while on 30.01
        int v=grownVariableCrops.size()-1; //added 30.01
        do{ //inner loop
            double minGM=grownVariableCrops[v]->pullVariable<double>("GM");
            if(savings>minGM){  //compare savings to GM of the worst 'available' normal crop - which is currently planted and have areaVariable>0
               double areaVariable=grownVariableCrops[v]->pullVariable<double>("AreaVariable");
               double FUKey=fodderCrops[p].crop->pullVariable<double>("FUKey"); //number of FU per hkg of a crop
               double resp=fodderCrops[p].crop->pullVariable<double>("Response");
               double loss=fodderCrops[p].crop->pullVariable<double>("TotalLoss");
               double FUha=resp*(1-loss/100)*FUKey;
               double haNeeded=totalFUdemand/FUha;
               double percNeeded=haNeeded*100/totalArea;

               count++;

               if(percNeeded<=rotationMax-areaFodder){//you can plant as much as need of fodder crop, but check if there is enough space in the normal crop
                   if(percNeeded<=areaVariable){//if so, just plant as much as you need of this one fodder crop and cut the normal crop by percNeeded
                        areaFodder+=percNeeded; //increase the area (not just assign!)
                        totalFUdemand=0; //finish outer loop
                        totalFUgrown+=percNeeded*FUha*totalArea/100;
                        areaVariable-=percNeeded; option=1;  o1=true;
                        assigningFinished=true; //finish inner loop
                   }
                   else{//you can plant fodder crop on an area of a normal crop and then check with the next normal crop
                        areaFodder+=areaVariable; //increase by the whole 'available' area of a normal crop
                        totalFUdemand-=FUha*areaVariable*totalArea/100; //update fodder demand
                        totalFUgrown+=areaVariable*FUha*totalArea/100;
                        areaVariable=0; option=2; o2=true; //whole replaced by a fodder crop
                   }
               }
               else{//this fodder crop will not cover all the needs; will have to check the next one
                   if(rotationMax-areaFodder<=areaVariable){//if so, just plant as much as you need of this one fodder crop and cut the normal crop by...                     
                        areaVariable-=rotationMax-areaFodder;
                        totalFUgrown+=(rotationMax-areaFodder)*FUha*totalArea/100;
                        totalFUdemand-=FUha*(rotationMax-areaFodder)*totalArea/100;
                        areaFodder=rotationMax; //max
                        assigningFinished=true; option=3;  o3=true; //finish inner loop-you have to take the next fodder crop
                   }
                   else{//you can plant fodder crop on an area of a normal crop and then check with the next normal crop
                        areaFodder+=areaVariable; //increase by the whole 'available' area of a normal crop
                        totalFUdemand-=FUha*areaVariable*totalArea/100;
                        totalFUgrown+=areaVariable*FUha*totalArea/100;
                        areaVariable=0; option=4; o4=true; //whole replaced by a fodder crop

                   }
               }
               double areaMin=grownVariableCrops[v]->pullVariable<double>("AreaMinPercent");
               double areaPer=areaMin+areaVariable;
               grownVariableCrops[v]->pushVariable<double>("AreaPercent", areaPer);
               grownVariableCrops[v]->pushVariable<double>("AreaVariable", areaVariable);

               //writing to a file
               QFile file;
               QString path = FileLocations::location(FileLocationInfo::Output).absolutePath();
               QString fileName = "output6.txt";
               QString filePath = path + "/" + fileName;

               file.setFileName(filePath);
               if (!file.open(QIODevice::Text | QIODevice::WriteOnly))
                       throw Exception("Could not open output file:'" + filePath + "'");
               //file created

               file.write("\n\n fodder...\n");
                   QString line=this->id().label();
                   line+=" tot fodder to prod " + QString::number(totalFodderToProduce)+"\n";
                   line+=" minFodderProduction " + QString::number(minFodderProduction)+"\n";
                   line+=" totalFUdemand " + QString::number(totalFUdemand)+"\n";
                   line+=" totalFUdemandBefore " + QString::number(totalFUdemandBefore)+"\n";
                   line+=" totalFUgrown " + QString::number(totalFUgrown)+"\n";
                   line+=" fodder to produce " + QString::number(fodderToProduce)+"\n";
                   line+=" option " + QString::number(option)+"\n";
                   line+=" count " + QString::number(count)+"\n";
                   line+=" option 1 " + QString::number(o1)+"\n";
                   line+=" option 2 " + QString::number(o2)+"\n";
                   line+=" option 3 " + QString::number(o3)+"\n";
                   line+=" option 4 " + QString::number(o4)+"\n";

                   file.write(qPrintable(line));


            }
            else{//buy all the remaining fodder demand
                totalFUt=totalFUdemand; //demand is already updated for fodder from fixed crops and min areas
                totalFUdemand=0; //finish outer loop
                assigningFinished=true; //finish inner loop
            }
            v--;    //added 30.01 - now use do-while loop
        }while (assigningFinished==false && v>=0);//inner for-loop (normal crops)/changed to do-while loop, 30.01

        fodderCrops[p].crop->pushVariable<double>("AreaPercent", areaFodder);//update fodder crop - after the inner loop

        if(assigningFinished==false){ //loop was stopped by the condition->no more space on normal crops available
            //there's no more space on normal crops-have to buy the remaining totalFUdemand
            totalFUt+=totalFUdemand;
        }
    }
}//outer for-loop (fodder crops)


if(totalFUdemand>0){ //added 30.01 - for some farms the condition 'if(areaFodder<rotationMax)' is never fulfilled, so the inner loop will never run
    totalFUt=totalFUdemand;
    totalFUdemand=0;
}


if(fakeCropTest==true){ //only if it was used in determineAreas - check if it is >0
    //writing to a file
    QFile file;
    QString path = FileLocations::location(FileLocationInfo::Output).absolutePath();
    QString fileName = "output4 area fake crop.txt";
    QString filePath = path + "/" + fileName;

    file.setFileName(filePath);
    if (!file.open(QIODevice::Text | QIODevice::WriteOnly))
            throw Exception("Could not open output file:'" + filePath + "'");
    //file created

    file.write("\n\n fake crop\n");
        QString line=this->id().label();
        int s=grownVariableCrops.size()-1;
        double areaF=grownVariableCrops[s]->pullVariable<double>("AreaPercent");
        line+="  fake crop area " + QString::number(areaF)+"\n";
        file.write(qPrintable(line));

    //check if the fake crop is still there - if so, substitute it with fodder crops

    double areaFakeCrop=grownVariableCrops[s]->pullVariable<double>("AreaPercent");
    if(areaFakeCrop>0){
        increaseCrops(fodderCrops, areaFakeCrop);
        grownVariableCrops[s]->pushVariable<double>("AreaPercent", areaFakeCrop);
    }

    file.write("\n\n fake crop after it is corrected\n");
        areaF=grownVariableCrops[s]->pullVariable<double>("AreaPercent");
        line+="  fake crop area " + QString::number(areaF)+"\n";
        file.write(qPrintable(line));
}



}// end of determineFodderAreas


void Farm::determineMinFodder(){

/**Function assigning area to fodder crops in order to cover the min fodder production - if it
  is not covered by the fodder from fixed crops and min areas. */

    double areaAssignedToFodder=0; //holds the value of area that has to be grown to cover the min fodder production requirement

    if(fodderToProduce>0){ //do it only if fodderToProduce is >0!*/

        double available=0; //area of  normal crops available to substitute with fodder crops
        for(int i=0; i<grownVariableCrops.size(); i++){
              double areaVariableCrop=grownVariableCrops[i]->pullVariable<double>("AreaVariable");
              available+=areaVariableCrop;
        }

        //writing to a file
        QFile file;
        QString path = FileLocations::location(FileLocationInfo::Output).absolutePath();
        QString fileName = "output2.txt";
        QString filePath = path + "/" + fileName;

        file.setFileName(filePath);
        if (!file.open(QIODevice::Text | QIODevice::WriteOnly))
                throw Exception("Could not open output file:'" + filePath + "'");
        //file created

        file.write("\n\n grown var crops\n");
        for (int i=0; i<grownVariableCrops.size(); i++){
            QString line=grownVariableCrops[i]->id().label();
            line+=" " + QString::number(grownVariableCrops[i]->pullVariable<double>("AreaVariable"))+"\n";
            line+=" " + QString::number(available)+"\n";
            file.write(qPrintable(line));
        }


        for(int i=0; available>0 && i<fodderCrops.size() && fodderToProduce>0 ; i++){// if so, more has to be produced (than it is already-from min areas and fixed crops)
            double rotationMax=fodderCrops[i].crop->pullVariable<double>("RotationMax");
            double area=fodderCrops[i].crop->pullVariable<double>("AreaPercent"); //fodder crop's area
            double areaBefore=area; //could be positive - if areaMin>0, so save it
            double FUKey=fodderCrops[i].crop->pullVariable<double>("FUKey"); //number of FU per hkg of a crop
            double resp=fodderCrops[i].crop->pullVariable<double>("Response");
            double loss=fodderCrops[i].crop->pullVariable<double>("TotalLoss");
            double FUha=resp*(1-loss/100)*FUKey;
            double haNeeded=fodderToProduce/FUha;
            double percNeeded=haNeeded*100/totalArea;

            if(percNeeded<=rotationMax-area){ //area might be positive! (min area)
                if(percNeeded<=available){
                    area+=percNeeded; //the end, fodderToProduce will be 0.
                    available-=percNeeded;
                }
                else{
                    area+=available; //so we can't grow any  more fodder - no space left!
                    available=0;
                }
            }
            else{
                if(rotationMax-area>=available){
                    area+=available;
                    available=0; //no space left
                }
                else{
                    available-=rotationMax-area;
                    area=rotationMax;
                }
            }
            fodderCrops[i].crop->pushVariable<double>("AreaPercent", area);
            double Funits=FUha*(area-areaBefore)*totalArea/100;
            fodderToProduce-=Funits; //update fodderToProduce; in 1st case - it's now = 0.
            totalFUdemand-=Funits; //update the total demand as well!
            totalFUgrown+=Funits; //update the amount of grown fodder units
            areaAssignedToFodder+=(area-areaBefore);

            //writing to a file
            file.write("\n\n fodder crops-area\n");
            QString line=fodderCrops[i].crop->id().label();
            line+=" " + QString::number(fodderCrops[i].crop->pullVariable<double>("AreaPercent"))+"\n";
            line+=" " + QString::number(areaAssignedToFodder)+"\n";
            line+=" " + QString::number(fodderToProduce)+"\n";
            file.write(qPrintable(line));
        }

        if(fodderToProduce>0){
            //restriction broken! impossible to produce the min amount of fodder
        }

        //decreasing area of variable crops that has just been assigned to fodder crops

        int nrOfRemovedCrops=0; //will probbaly be useful in determining fodder crops afterwards-no:)
        for(int j=grownVariableCrops.size()-1; areaAssignedToFodder>0 && j>=0; j--){//need to cut some of the variable crops-start with the last one on the list - that's the one with the smallest GM
            double areaVariableCrop=grownVariableCrops[j]->pullVariable<double>("AreaVariable");
            if(areaVariableCrop<areaAssignedToFodder){
                areaAssignedToFodder-=areaVariableCrop;
                areaVariableCrop=0;
                nrOfRemovedCrops++;
            }
            else{
                areaVariableCrop-=areaAssignedToFodder;
                areaAssignedToFodder=0; //so this is the last run of the loop
            }
            //now save all the changes
            grownVariableCrops[j]->pushVariable<double>("AreaVariable", areaVariableCrop);
            double areaMin=grownVariableCrops[j]->pullVariable<double>("AreaMinPercent");
            double areaPer=areaVariableCrop+areaMin;
            grownVariableCrops[j]->pushVariable<double>("AreaPercent", areaPer);
        }

        if(areaAssignedToFodder>0){
            //there is not enough space to grow fodder to cover the min required...
        }

        //just checking if this area is zero
        QString line;
        line+="area assigned to fodder (should be zero) " + QString::number(areaAssignedToFodder)+"\n";
        file.write(qPrintable(line));

    }
} //determineMinFodder



void Farm::determineAreas_ha(QList<Crop*>allCrops){ //area in ha
    for(int i=0; i<allCrops.size(); i++){
        double areaPercent=allCrops[i]->pullVariable <double>("AreaPercent");
        double area_ha = (areaPercent==0) ? 0 : (areaPercent/100 * totalArea);
        crops[i]->pushVariable<double>("Area_ha", area_ha);
    }
}


double Farm::total(QString variable_name){
    double totalAmount =0;
    for (int i=0; i<crops.size(); i++){
        double area_ha=crops[i]->pullVariable<double>("Area_ha");
        if(!area_ha==0){ //otherwise don't change totalAmount
           double value_per_ha=crops[i]->pullVariable<double>(variable_name);
           totalAmount+= value_per_ha * area_ha; //each crop's/(field's) optimal values per ha multiplied by acreage of a crop
        }
    }
    return totalAmount;
}

//double Farm::cropParameter(QString cropID, QString parameterID) {
//    return cropParameters3->value<double>(cropID, farmType, soilType, parameterID);
//}

//bool Farm::cropParameterBool(QString cropID, QString parameterID) {
//   return cropParameters3->value<bool>(cropID, farmType, soilType, parameterID);
//}

void Farm::sortCrops(QList<CropSort> &cropsToSort, QString sortKey){

/**Function for sorting crops using sortingKey (public name of a crop's paramter
as a criterion for sorting. First argument is a QList of structs containing a pointer
to crop and a key.*/
    QString sortingKey=sortKey;

    for (int i=0; i<cropsToSort.size(); i++){//define the key to sorting
        cropsToSort[i].key = cropsToSort[i].crop->pullVariable<double>(sortingKey);
    }
    qSort(cropsToSort);
}


//void Farm::setInitialCrops(QMap<QString, double>areas){
//    cropAreas=areas;
//}


//void Farm::setCropParameters(CropParameters3 *cropParameters){
///**Sets the pointer as equal to the pointer (? can you say so?) which is a paramter of the landscape class. */
//    cropParameters3=cropParameters;
//}

//void Farm::setLivestockParameters(LivestockParameters *livestockParameters1){
///**Sets the pointer as equal to the pointer (? can you say so?) which is a paramter of the landscape class. */
//    livestockParameters=livestockParameters1;
//}

//void Farm::setLivestockNumbers(LivestockNumbers *livestockNumbers1){
///**Sets the pointer as equal to the pointer (? can you say so?) which is a paramter of the landscape class. */
//    livestockNumbers=livestockNumbers1;
//}

//void Farm::setWinterMaxValues(WinterMaxValues *winterMaxValues1){
///**Sets the pointer as equal to the pointer (? can you say so?) which is a parameter of the landscape class. */
//    winterMaxValues=winterMaxValues1;
//}

//void Farm::setPriceNt (double priceNt1){
///** */
//    priceNt=priceNt1;
//}
//void Farm::setPriceFU(double priceFU1){
//    priceFU=priceFU1;
//}
//void Farm::setMethod_fert(bool method_fert1){
///** */
//    method_fert=method_fert1;
//}
//void Farm::setFarmType(QString farmType1){
///**  . */
//    farmType=farmType1;
//}
//void Farm::setSoilType(QString soilType1){
///**  . */
//    soilType=soilType1;
//}
////void Farm::setFarmID(QString farmID1){
////    farmID=farmID1;
////}
//void Farm::setMinFodder(int minPercent){
//    minFodderProduction=minPercent;
//}





} //namespace

