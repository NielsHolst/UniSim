/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "boundary_layer_resistance.h"
#include "cover.h"
#include "crop_wetness.h"
#include "greenhouse.h"
#include "greenhouse_energy.h"
#include "greenhouse_temperature.h"
#include "greenhouse_transmission.h"
#include "greenhouse_ventilation.h"
#include "indoors.h"
#include "indoors_humidity.h"
#include "indoors_radiation.h"
#include "indoors_radiation_absorption.h"
#include "indoors_temperature.h"
#include "lamp_hpsl_400.h"
#include "lamp_led.h"
#include "lamp_switch.h"
#include "outdoors.h"
#include "pipe.h"
#include "pipes.h"
#include "proportional_control.h"
#include "screen.h"
#include "screen_air_transmission.h"
#include "screen_energy_balance.h"
#include "screen_energy_light.h"
#include "screen_light_transmission.h"
#include "screen_shade.h"
#include "screen_temperature.h"
#include "sp_co2.h"
#include "sp_heating.h"
#include "sp_humidity_regular.h"
#include "sp_ventilation_opening.h"
#include "sp_ventilation_opening_max.h"
#include "sp_ventilation_temperature.h"
#include "sky.h"
#include "stomatal_resistance.h"
#include "vg_factory.h"

using namespace UniSim;

namespace vg{

void VgFactory::defineProducts() {
    AddProduct(BoundaryLayerResistance, "desc");
    AddProduct(Cover, "desc");
    AddProduct(CropWetness, "desc");
    AddProduct(Greenhouse, "desc");
    AddProduct(GreenhouseTemperature, "desc");
    AddProduct(GreenhouseTransmission, "desc");
    AddProduct(GreenhouseVentilation, "desc");
    AddProduct(GreenhouseEnergy, "desc");
    AddProduct(Indoors, "desc");
    AddProduct(IndoorsHumidity, "desc");
    AddProduct(IndoorsRadiation, "desc");
    AddProduct(IndoorsRadiationAbsorption, "desc");
    AddProduct(IndoorsTemperature, "desc");
    AddProduct(LampHpsl400, "desc");
    AddProduct(LampLed, "desc");
    AddProduct(LampSwitch, "desc");
    AddProduct(Outdoors, "desc");
    AddProduct(Pipe, "desc");
    AddProduct(Pipes, "desc");
    AddProduct(ProportionalControl, "desc");
    AddProduct(Screen, "desc");
    AddProduct(ScreenAirTransmission, "desc");
    AddProduct(ScreenEnergyBalance, "desc");
    AddProduct(ScreenEnergyLight, "desc");
    AddProduct(ScreenLightTransmission, "desc");
    AddProduct(ScreenShade, "desc");
    AddProduct(ScreenTemperature, "desc");
    AddProduct(Sky, "desc");
    AddProduct(SpCo2, "desc");
    AddProduct(SpHeating, "desc");
    AddProduct(SpHumidityRegular, "desc");
    AddProduct(SpVentilationOpening, "desc");
    AddProduct(SpVentilationOpeningMax, "desc");
    AddProduct(SpVentilationTemperature, "desc");
    AddProduct(StomatalResistance, "desc");
}

UniSim::Identifier VgFactory::id() const {
    return "vg";
}

QString VgFactory::description() const {
    return
    "Virtual greenhouse";
}

QStringList VgFactory::authors() const {
    return QStringList()
            << "Oliver"
            << "Niels";
}

QObject* VgFactory::asQObject() {
   return this;
}

#if QT_VERSION < 0x50000
Q_EXPORT_PLUGIN2(vg_factory, VgFactory)
#endif

} //namespace
