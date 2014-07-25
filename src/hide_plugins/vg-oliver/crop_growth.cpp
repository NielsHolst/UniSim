/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "crop_growth.h"
#include "publish.h"

using namespace UniSim;

namespace vg {

PUBLISH(CropGrowth)

CropGrowth::CropGrowth(Identifier name, QObject *parent)
	: Model(name, parent)
{
    InputRef(double, Tcrop, "../CropTemperature[value]");
    InputRef(double,Pgc, "./Pgc[value]");
    Input(double, initDryWeight, 10);

    Output(double, dDryWeight);
    Output(double, DryWeight);
}

void CropGrowth::reset() {
    DryWeight = initDryWeight;
}

void CropGrowth::update() {

        double Q10mr = 2.0,
        ASRLv=1.39,
        ASRSt=1.45,
        ASRRt=1.39,
        ASRFr=1.35,

        MAINT_lv=0.015,
        MAINT_st=0.03,
        MAINT_gn=0.01;
        //MAINT_root=0.01;

        //MAINT=Maintenance respiration at 25 centigree g CH2O/gDM/h (lv=leaves,st=stem,gn=generative organs)
        double MAINT=(((0.7*MAINT_lv+0.25*MAINT_st+0.05*MAINT_gn)))/24;

        //Cf=Conversion Efficiency
        double Cf=(ASRLv+ ASRSt+ ASRRt+ ASRFr)/4;
        double Rm=MAINT*pow(Q10mr,0.1*(Tcrop-25));
        dDryWeight=(1/Cf)*(Pgc*30/44-(Rm*DryWeight));
        DryWeight+=dDryWeight;
}


} //namespace

