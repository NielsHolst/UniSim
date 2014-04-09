/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "adjustable_setpoint.h"
#include "air_infiltration.h"
#include "blackout_screen_controller.h"
//#include "boundary_layer_resistance.h"
#include "construction_cover.h"
#include "construction_geometry.h"
#include "construction_ventilation.h"
#include "control_element.h"
#include "controlled_switch_setpoint.h"
//#include "control_element_asym.h"
//#include "cover_dew.h"
//#include "cover_temperature.h"
//#include "cover_vapour_flux.h"
//#include "crop.h"
//#include "crop_layer.h"
//#include "crop_radiation_absorption.h"
#include "energy_screen_balance.h"
#include "energy_screen_controller.h"
#include "outdoors.h"
#include "greenhouse_construction.h"
//#include "greenhouse_energetics.h"
//#include "heating_demand.h"
#include "heat_pipe.h"
#include "heating_flow_controller.h"
#include "heating_temperature_controller.h"
#include "high_light_switch.h"
#include "humidity_screen_balance.h"
#include "humidity_setpoints.h"
#include "indoors_humidity.h"
#include "indoors_radiation.h"
#include "indoors_temperature.h"
//#include "lamp_attributes.h"
#include "indoors_ventilation.h"
#include "light_controller.h"
#include "low_light_switch.h"
//#include "microclimate_humidity.h"
//#include "microclimate_temperature.h"
//#include "pid_control.h"
//#include "pipe.h"
//#include "pipes.h"
#include "proportional_control.h"
//#include "screen_dew.h"
#include "screen.h"
#include "screen_controller.h"
//#include "screen_temperature.h"
//#include "screen_transmission.h"
//#include "screens.h"
#include "shade_screen_controller.h"
#include "surface.h"
//#include "stomatal_resistance.h"
#include "switch_setpoint.h"
#include "switch_setpoint_collection.h"
#include "time_switch.h"
#include "ventilation_by_temperature_diff.h"
#include "ventilation_controller.h"
//#include "ventilation_latent_energy_balance.h"
#include "ventilation_max.h"
#include "vents_ventilation.h"
#include "vg_factory.h"


using namespace UniSim;

namespace vg{

void VgFactory::defineProducts() {
    AddProduct(AdjustableSetpoint, "Desc");
    AddProduct(AirInfiltration, "Desc");
    AddProduct(BlackoutScreenController, "Desc");
//    AddProduct(BoundaryLayerResistance, "Desc");
    AddProduct(ConstructionCover, "Desc");
    AddProduct(ConstructionGeometry, "Desc");
    AddProduct(ConstructionVentilation, "Desc");
    AddProduct(ControlElement, "Desc");
    AddProduct(ControlledSwitchSetpoint, "Desc");
//    AddProduct(ControlElementAsym, "Desc");
//    AddProduct(CoverDew, "Desc");
//    AddProduct(CoverTemperature, "Desc");
//    AddProduct(CoverVapourFlux, "Desc");
//    AddProduct(Crop, "Desc");
//    AddProduct(CropLayer, "Desc");
//    AddProduct(CropRadiationAbsorption, "Desc");
    AddProduct(EnergyScreenBalance, "Desc");
    AddProduct(EnergyScreenController, "Desc");
//    AddProduct(GreenhouseEnergetics, "Desc");
//    AddProduct(HeatingDemand, "Desc");
    AddProduct(HeatPipe,"");
    AddProduct(HeatingFlowController, "");
    AddProduct(HeatingTemperatureController, "");
    AddProduct(HighLightSwitch, "");
    AddProduct(HumidityScreenBalance, "");
    AddProduct(HumiditySetpoints, "Desc");
    AddProduct(IndoorsHumidity, "Desc");
    AddProduct(IndoorsRadiation, "Desc");
    AddProduct(IndoorsTemperature, "Desc");
    AddProduct(IndoorsVentilation, "Desc");
//    AddProduct(LampAttributes, "Desc");
//    AddProduct(MicroclimateHumidity, "Desc");
//    AddProduct(MicroclimateTemperature, "Desc");
    AddProduct(LightController, "Desc");
    AddProduct(LowLightSwitch, "");
//    AddProduct(PidControl, "Desc");
//    AddProduct(Pipe, "Desc");
//    AddProduct(Pipes, "Desc");
    AddProduct(Outdoors, "Desc");
    AddProduct(ProportionalControl, "Desc");
    AddProduct(ScreenController, "Desc");
    AddProduct(Screen, "Desc");
//    AddProduct(ScreenDew, "Desc");
//    AddProduct(ScreenTemperature, "Desc");
//    AddProduct(ScreenTransmission, "Desc");
//    AddProduct(Screens, "Desc");
    AddProduct(ShadeScreenController, "Desc");
    AddProduct(Surface, "Desc");
//    AddProduct(StomatalResistance, "Desc");
    AddProduct(SwitchSetpoint, "Desc");
    AddProduct(SwitchSetpointCollection, "Desc");
    AddProduct(TimeSwitch, "");
    AddProduct(VentilationByTemperatureDiff, "Desc");
    AddProduct(VentilationController, "Desc");
//    AddProduct(VentilationLatentEnergyBalance, "Desc");
    AddProduct(VentilationMax, "Desc");
    AddProduct(VentsVentilation, "Desc");
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
