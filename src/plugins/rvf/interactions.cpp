/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/

#include "interactions.h"

using namespace UniSim;

namespace rvf {

Interactions::Interactions(Identifier name, QObject *parent)
     : Model(name, parent)
{
    // Fecundity effects
    addVariable<double>(Name(susceptibleAedesBloodmeals), "Susceptible Aedes Blood meals to host per day");
    addVariable<double>(Name (susceptibleCulexBloodmeals), "Susceptible Culex Blood meals to host per day");
    addVariable<double>(Name (infectiousAedesBloodmeals), "Infectious Aedes Blood meals to host per day");
    addVariable<double>(Name (infectiousCulexBloodmeals), "Infectious Culex Blood meals to host per day");

    // Transmission to host effects
    addVariable<double>(Name(susceptibleLambsAttackedByInfectiousMosquitoesProportion), "Susceptible lambs attack by infectious mosquitoes per day");
    addVariable<double>(Name(susceptilbeAdultSheepAttackedByInfectiousMosquitoesProportion), "Susceptible adult sheep attack by infectious mosquitoes per day");

    // Transmission to vector effects
    addVariable<double>(Name (susceptibleAedesOnInfectiousHostBloodmealsProportion), "Susceptible Aedes Blood meals on infectious host per day");
    addVariable<double>(Name (susceptibleCulexOnInfectiousHostBloodmealsProportion), "Susceptible Culex Blood meals on infectious host per day");
}

void Interactions::initialize() {
    infection = seekOneChild<Model*>("Infection");
    culexSAdult = seekOne<Model*>("culex/S/adult");
    aedesSAdult = seekOne<Model*>("aedes/S/adult");
    sheepSAdult = seekOne<Model*>("sheep/S/adult");
    sheepSLamb = seekOne<Model*>("sheep/S/lamb");

}

void Interactions::reset() {
    susceptibleAedesBloodmeals = 0.;
    susceptibleCulexBloodmeals = 0.;
    infectiousAedesBloodmeals = 0.;
    infectiousCulexBloodmeals = 0.;
    susceptibleLambsAttackedByInfectiousMosquitoesProportion = 0.;
    susceptilbeAdultSheepAttackedByInfectiousMosquitoesProportion = 0.;
    susceptibleAedesOnInfectiousHostBloodmealsProportion = 0.;
    susceptibleCulexOnInfectiousHostBloodmealsProportion = 0.;
}
void Interactions::update() {
    susceptibleAedesBloodmeals =
            infection->pullValue<double>("Aedes_S_Demand-sheep_S_adult-attacks") +
            infection->pullValue<double>("Aedes_S_Demand-sheep_E_adult-attacks") +
            infection->pullValue<double>("Aedes_S_Demand-sheep_I_adult-attacks") +
            infection->pullValue<double>("Aedes_S_Demand-sheep_R_adult-attacks") +

            infection->pullValue<double>("Aedes_S_Demand-sheep_S_lamb-attacks") +
            infection->pullValue<double>("Aedes_S_Demand-sheep_E_lamb-attacks") +
            infection->pullValue<double>("Aedes_S_Demand-sheep_I_lamb-attacks") +
            infection->pullValue<double>("Aedes_S_Demand-sheep_R_lamb-attacks");

    infectiousAedesBloodmeals =
            infection->pullValue<double>("Aedes_I_Demand-sheep_S_adult-attacks") +
            infection->pullValue<double>("Aedes_I_Demand-sheep_E_adult-attacks") +
            infection->pullValue<double>("Aedes_I_Demand-sheep_I_adult-attacks") +
            infection->pullValue<double>("Aedes_I_Demand-sheep_R_adult-attacks") +

            infection->pullValue<double>("Aedes_I_Demand-sheep_S_lamb-attacks") +
            infection->pullValue<double>("Aedes_I_Demand-sheep_E_lamb-attacks") +
            infection->pullValue<double>("Aedes_I_Demand-sheep_I_lamb-attacks") +
            infection->pullValue<double>("Aedes_I_Demand-sheep_R_lamb-attacks");

    susceptibleCulexBloodmeals =
            infection->pullValue<double>("Culex_S_Demand-sheep_S_adult-attacks") +
            infection->pullValue<double>("Culex_S_Demand-sheep_E_adult-attacks") +
            infection->pullValue<double>("Culex_S_Demand-sheep_I_adult-attacks") +
            infection->pullValue<double>("Culex_S_Demand-sheep_R_adult-attacks") +

            infection->pullValue<double>("Culex_S_Demand-sheep_S_lamb-attacks") +
            infection->pullValue<double>("Culex_S_Demand-sheep_E_lamb-attacks") +
            infection->pullValue<double>("Culex_S_Demand-sheep_I_lamb-attacks") +
            infection->pullValue<double>("Culex_S_Demand-sheep_R_lamb-attacks");

    infectiousCulexBloodmeals =
            infection->pullValue<double>("Culex_I_Demand-sheep_S_adult-attacks") +
            infection->pullValue<double>("Culex_I_Demand-sheep_E_adult-attacks") +
            infection->pullValue<double>("Culex_I_Demand-sheep_I_adult-attacks") +
            infection->pullValue<double>("Culex_I_Demand-sheep_R_adult-attacks") +

            infection->pullValue<double>("Culex_I_Demand-sheep_S_lamb-attacks") +
            infection->pullValue<double>("Culex_I_Demand-sheep_E_lamb-attacks") +
            infection->pullValue<double>("Culex_I_Demand-sheep_I_lamb-attacks") +
            infection->pullValue<double>("Culex_I_Demand-sheep_R_lamb-attacks");

    double attacks, density;

    attacks =
        infection->pullValue<double>("Aedes_I_Demand-sheep_S_lamb-attacked") +
        infection->pullValue<double>("Culex_I_Demand-sheep_S_lamb-attacked");
    density = sheepSLamb->pullValue<double>("value");
    susceptibleLambsAttackedByInfectiousMosquitoesProportion = attacks/density;

    attacks =
        infection->pullValue<double>("Aedes_I_Demand-sheep_S_adult-attacked") +
        infection->pullValue<double>("Culex_I_Demand-sheep_S_adult-attacked");
    density = sheepSAdult->pullValue<double>("value");
    susceptilbeAdultSheepAttackedByInfectiousMosquitoesProportion = attacks/density;

    attacks =
        infection->pullValue<double>("Aedes_S_Demand-sheep_I_adult-attacks") +
        infection->pullValue<double>("Aedes_S_Demand-sheep_I_lamb-attacks");
    density = aedesSAdult->pullValue<double>("value");
    susceptibleAedesOnInfectiousHostBloodmealsProportion = attacks/density;

    attacks =
        infection->pullValue<double>("Culex_S_Demand-sheep_I_adult-attacks") +
        infection->pullValue<double>("Culex_S_Demand-sheep_I_lamb-attacks");
    density =   culexSAdult->pullValue<double>("value");
    susceptibleCulexOnInfectiousHostBloodmealsProportion = attacks/density;
}
} //namespace
