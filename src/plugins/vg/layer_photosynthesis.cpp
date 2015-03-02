/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <stdlib.h>
#include "layer_photosynthesis.h"
#include "publish.h"

using namespace UniSim;
using namespace std;

namespace vg {
	
PUBLISH(LayerPhotosynthesis)

/*! \class LayerPhotosynthesis
 * \brief Assimilation of a canopy layer
 *
 * Inputs
 * ------
 * - _sinb_ is the sine of sun elevation [-1;1]
 * - _radiationDif_ is the indoors diffuse radiation [W//m<SUP>2</SUP>]
 * - _radiationDir_ is the indoors direct radiation [W//m<SUP>2</SUP>]
 * - _lai_ is leaf area index [m<SUP>2</SUP>/m<SUP>2</SUP>]
 * - _xGauss_ is the LAI coefficient [0;1]
 * - _wGauss_ is weighing coefficient [0;1]
 * - _LUE_ is the light use efficiency [mg CO<SUB>2</SUB>/J]
 * - _Pnmax_ is the maximum net assimilation rate [mg CO<SUB>2</SUB>/m<SUP>2</SUP> leaf/s]
 * - _Pgmax_ is the maximum gross assimilation rate [mg CO<SUB>2</SUB>/m<SUP>2</SUP> leaf/s]
 * - _Rd_ is the dark respiration rate [mg CO<SUB>2</SUB>/m<SUP>2</SUP> leaf/s]
 * - _Kdif_ is extinction coefficient for difuse light [0;1]
 * - _SCV_ is light scattering coefficient [0;1]
 *
 * Outputs
 * ------
 * - _Pn_ is the net assimilation rate [g CO<SUB>2</SUB>/m<SUP>2</SUP> ground/h]
 * - _Pg_ is the gross assimilation rate [g CO<SUB>2</SUB>/m<SUP>2</SUP> ground/h]
 *
 * Default dependencies
 * ------------
 * - a _calendar_ model with a _sinb_ port [-1;1]
 * - an _indoors/radiation_ mode with two ports:
 *   + _radiationDif_ [W//m<SUP>2</SUP>]
 *   + _radiationDir_  [W//m<SUP>2</SUP>]
 * - a parent model with _xGauss_ and _wGauss_ ports [0;1]
 * - a _lightResponse_ sibling model four ports:
 *   + _LUE_ [mg CO<SUB>2</SUB>/J]
 *   + _Pnmax_ [mg CO<SUB>2</SUB>/m<SUP>2</SUP> leaf/s]
 *   + _Pgmax_ [mg CO<SUB>2</SUB>/m<SUP>2</SUP> leaf/s]
 *   + _Rd_ [mg CO<SUB>2</SUB>/m<SUP>2</SUP> leaf/s]
 */

LayerPhotosynthesis::LayerPhotosynthesis(Identifier name, QObject *parent)
    : Model(name, parent)
{
    InputRef(double, sinB, "calendar[sinB]");
    InputRef(double, radiationDif, "indoors/radiation[diffuse]");
    InputRef(double, radiationDir, "indoors/radiation[direct]");
    InputRef(double, lai, "crop/lai[lai]");
    InputRef(double, xGauss, "..[xGauss]");
    InputRef(double, wGauss, "..[wGauss]");
    InputRef(double, LUE, "./lightResponse[LUE]");
    InputRef(double, Pnmax, "./lightResponse[Pnmax]");
    InputRef(double, Pgmax, "./lightResponse[Pgmax]");
    InputRef(double, Rd, "./lightResponse[Rd]");
    Input(double, Kdif, 0.8);
    Input(double, SCV, 0.15);
    Output(double, Pn);
    Output(double, Pg);
}

void LayerPhotosynthesis::reset() {
    Pn = Pg = 0;
}

void LayerPhotosynthesis::update() {
    const double xSunGauss[3]={0.1127, 0.5, 0.8873},
    wSunGauss[3]={0.2778, 0.4444, 0.2778};
    double LAIC=lai*xGauss;
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
   double Pgshade=(Pgmax==0)?0:Pgmax*(1-exp(-VISshd*LUE/Pgmax));

//6)-------assimilation of sunlit leaf area results in a 3-culumn matrix

    //direct flux absorbed by leaves perpendicular on direct beam (VISpp)[J*m-2 leaf s-1]
    double VISpp=(sinB==0)?0:(1-SCV)*PARDIR/(sinB);

    double Pgsun=0;
    for(int i=0; i<3; ++i)
    {
        //Total absorbed flux of sunlit leaves [J*m-2 leaf s-1]
    double VISsun=VISshd+VISpp*xSunGauss[i];

    //Gross assimilation of sunlit leaves for three leaf layers [mg CO2in*m-2 leaf s-1]
    Pgsun+=(Pgmax==0)?0:wSunGauss[i]*Pgmax*(1-exp(-VISsun*LUE/Pgmax));
    }

//7)--------local assimilation rate for three leaf layers

    //fraction sunlit leaf area (FSLLA)
    double FSLLA=exp(-KdirBL*LAIC);

   //local assimilation rate Pgl [mg CO2in m-2 leaf s-1 layer-1]
    double Pgl=FSLLA*Pgsun+(1-FSLLA)*Pgshade;

    Pg = Pgl*wGauss*lai;
    Pn = Pg + Rd*wGauss*lai;

    //g CO2 m-2 h-1
    Pg*=3.6;
    Pn*=3.6;
}


} //namespace

