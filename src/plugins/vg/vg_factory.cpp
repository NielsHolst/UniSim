/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "cover.h"
#include "crop_wetness.h"
#include "greenhouse.h"
#include "greenhouse_transmission.h"
#include "indoors.h"
#include "indoors_humidity.h"
#include "indoors_radiation.h"
#include "indoors_temperature.h"
#include "lamp_hpsl_400.h"
#include "lamp_led.h"
#include "lamp_switch.h"
#include "outdoors.h"
#include "proportional_control.h"
#include "screen.h"
#include "screen_air_transmission.h"
#include "screen_energy_balance.h"
#include "screen_energy_light.h"
#include "screen_light_transmission.h"
#include "screen_shade.h"
#include "screen_temperature.h"
#include "sp_heating.h"
#include "sp_humidity_regular.h"
#include "sp_ventilation_temperature.h"
#include "sky.h"
#include "vg_factory.h"

using namespace UniSim;

namespace vg{

void VgFactory::defineProducts() {
    addProduct<Cover>("Cover", this, "desc");
    addProduct<CropWetness>("CropWetness", this, "desc");
    addProduct<Greenhouse>("Greenhouse", this, "desc");
    addProduct<GreenhouseTransmission>("GreenhouseTransmission", this, "desc");
    addProduct<Indoors>("Indoors", this, "desc");
    addProduct<IndoorsHumidity>("IndoorsHumidity", this, "desc");
    addProduct<IndoorsRadiation>("IndoorsRadiation", this, "desc");
    addProduct<IndoorsTemperature>("IndoorsTemperature", this, "desc");
    addProduct<LampHpsl400>("LampHpsl400", this, "desc");
    addProduct<LampLed>("LampLed", this, "desc");
    addProduct<LampSwitch>("LampSwitch", this, "desc");
    addProduct<Outdoors>("Outdoors", this, "desc");
    addProduct<ProportionalControl>("ProportionalControl", this, "desc");
    addProduct<Screen>("Screen", this, "desc");
    addProduct<ScreenAirTransmission>("ScreenAirTransmission", this, "desc");
    addProduct<ScreenEnergyBalance>("ScreenEnergyBalance", this, "desc");
    addProduct<ScreenEnergyLight>("ScreenEnergyLight", this, "desc");
    addProduct<ScreenLightTransmission>("ScreenLightTransmission", this, "desc");
    addProduct<ScreenShade>("ScreenShade", this, "desc");
    addProduct<ScreenTemperature>("ScreenTemperature", this, "desc");
    addProduct<Sky>("Sky", this, "desc");
    addProduct<SpHeating>("SpHeating", this, "desc");
    addProduct<SpHumidityRegular>("SpHumidityRegular", this, "desc");
    addProduct<SpVentilationTemperature>("SpVentilationTemperature", this, "desc");
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
