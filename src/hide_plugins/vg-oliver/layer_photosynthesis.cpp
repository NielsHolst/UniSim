/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "layer_photosynthesis.h"
#include "publish.h"
#include <stdlib.h>

using namespace UniSim;
using namespace std;

namespace vg {
	
PUBLISH(LayerPhotosynthesis)



LayerPhotosynthesis::LayerPhotosynthesis(Identifier name, QObject *parent)
    : Model(name, parent)
{
    InputRef(double, Pnmax, "./LightResponse[Pnmax]");
    InputRef(double, Pgmax, "./LightResponse[Pgmax]");
    InputRef(double, Eff, "./LightResponse[Eff]");
    InputRef(double, Rd, "./LightResponse[Rd]");
    InputRef(double, sinB, "calendar[sinB]");
    InputRef(double, radiationDif, "indoors/radiation[diffuse]");
    InputRef(double, radiationDir, "indoors/radiation[direct]");
    Input(double, Kdif, 0.8);
    Input(double, SCV, 0.15);

    Input(double, LAI, 3); //kommer senere fra vækstmodellerne
    InputRef(double, xGauss, "..[xGauss]");
    InputRef(double, wGauss, "..[wGauss]");

    Output(double, Pn);
    Output(double, Pg);
}

void LayerPhotosynthesis::reset() {
    Pn = 0;
    Pg = 0;

}

void LayerPhotosynthesis::update() {
    const double xSunGauss[3]={0.1127, 0.5, 0.8873},
    wSunGauss[3]={0.2778, 0.4444, 0.2778};
    double LAIC=LAI*xGauss;
    double PARDIF = 0.47*radiationDif;
    double PARDIR = 0.47*radiationDir;

    //1)----calculation of reflection of horizontal and spherical leaf angle distribution
        //Intermediate variable SQV
       double SQV=sqrt(1-SCV);

       //reflection of horizontal leaf angle distribution
       double ReflHorizontal=(1-SQV)/(1+SQV);

       //reflection of spherical leaf angle distribution
       double ReflSpherical=(ReflHorizontal*2)/(1+1.6*sinB);

    //2)-----extinction coefficient for direct radiation and total direct flux

        //Extinction coefficient for direct component of direct PAR flux (KdirBL)
       double KdirBL=(sinB==0.) ? 0. : (0.5/sinB)*Kdif/(0.8*SQV);

        //Extinction coefficient for total direct PAR flux (Kdirtot)
       double KdirTot=KdirBL*SQV;


    //1. absorbed diffuse flux	 (VISdif)[J*m-2 leaf s-1]
   double VISdif=(1-ReflHorizontal)*PARDIF*Kdif*exp(-Kdif*LAIC);

    //2. absorbed total direct flux (VIStot)[J*m-2 leaf s-1]
   double VIStot=(1-ReflSpherical)*PARDIR*KdirTot*exp(-KdirTot*LAIC);

   //3. absorbed direct component of direct flux (VISdir)[J*m-2 leaf s-1]
   double VISdir=(1-SCV)*PARDIR*KdirBL*exp(-KdirBL*LAIC);

//5)------assimilation of shaded leaves results in a 3-culumn matrix

    //absorbed flux of shaded leaves (VISshd)[J/m2/leaf/s]
   double VISshd=VISdif+VIStot-VISdir;

   //gross assimilation of shaded leaves for three leaf layers [mg CO2in*m-2 leaf s-1]
   double Pgshade=(Pgmax==0)?0:Pgmax*(1-exp(-VISshd*Eff/Pgmax));

//6)-------assimilation of sunlit leaf area results in a 3-culumn matrix

    //direct flux absorbed by leaves perpendicular on direct beam (VISpp)[J*m-2 leaf s-1]
    double VISpp=(sinB==0)?0:(1-SCV)*PARDIR/(sinB);

    double Pgsun=0;
    for(int i=0; i<3; ++i)
    {
        //Total absorbed flux of sunlit leaves [J*m-2 leaf s-1]
    double VISsun=VISshd+VISpp*xSunGauss[i];

    //Gross assimilation of sunlit leaves for three leaf layers [mg CO2in*m-2 leaf s-1]
    Pgsun+=(Pgmax==0)?0:wSunGauss[i]*Pgmax*(1-exp(-VISsun*Eff/Pgmax));
    }

//7)--------local assimilation rate for three leaf layers

    //fraction sunlit leaf area (FSLLA)
    double FSLLA=exp(-KdirBL*LAIC);

   //local assimilation rate Pgl [mg CO2in m-2 leaf s-1 layer-1]
    double Pgl=FSLLA*Pgsun+(1-FSLLA)*Pgshade;

    Pg = Pgl*wGauss*LAI;
    Pn = Pg + Rd*wGauss*LAI;

    //g CO2 m-2 h-1
    Pg*=3.6;
    Pn*=3.6;

}


} //namespace

