/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/parameter.h>
#include <usbase/pull_variable.h>
#include "separator.h"

using namespace UniSim;


namespace separation {

Separator::Separator(UniSim::Identifier name, QObject *parent)
	: Model(name, parent)
{
        new Parameter<double>("slurryTotal", &slurryTotal, 176, this, "Amount of slurry separated (Mg)");
        new Parameter<double>("slurryTAN", &slurryTAN, 3.7, this, "koncentation of TAN in raw slurry (kg/Mg)");
        new Parameter<int>("periode", &periode, 14, this, "interval of separation procedure");
        new Parameter<double>("sepindexTAN", &sepindexTAN, 0.11, this, "separation index TAN (Hjorth et al., 2010)");
        new Parameter<double>("sepindexVol", &sepindexVol, 0.11, this, "separation index TAN (Hjorth et al., 2010)");
}
void Separator::initialize(){
    liquid=seekOneChild<Model*>("liquid");
    solid=seekOneChild<Model*>("solid");
}

void Separator::reset() {
    days=0;
    load();

}
void Separator::load (){

    double TANTotal = slurryTAN * slurryTotal;
    solid->pushVariable<double> ("TANin", (TANTotal*sepindexTAN)/(slurryTotal*sepindexVol));
    liquid->pushVariable<double> ("TANin", (TANTotal*(1-sepindexTAN))/(slurryTotal*(1-sepindexVol)));
}

void Separator::update() {
  //  slurryTAN = slurryTotal*3.7;
    days++;
    if (days%periode==0)
        load();

}

} //namespace

