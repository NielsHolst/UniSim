/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/object_pool.h>
#include <usbase/utilities.h>
#include "ipm_factory.h"
#include "demand.h"
#include "even_odd.h"
#include "g1_loss_rate.h"
#include "scheduled_g1_loss_rate.h"
#include "g2_loss_rate.h"
#include "brierei_time.h"
#include "briereii_time.h"
#include "../unisim/calendar.h"
#include "cuadratic_long_time.h"
#include "fecundity.h"
#include "fecundity5_time.h"
#include "lactin2_time.h"
#include "lamb_time.h"
#include "linear_time.h"
#include "loganb_time.h"

using namespace UniSim;

namespace ipm{

void IpmFactory::defineProducts() {
    // Add your own models here...
    AddProduct(EvenOdd, "Even-and-odd dynamics model");

	AddProduct(G1LossRate, "Type G1 loss rate");

	AddProduct(G2LossRate, "Type G1 loss rate");

	AddProduct(BriereITime,
    "This is the physiological time scale of Briere I et al. (reference to be added). "
    "It pulls the daily average temperature from the @F weather object just like the @F "
    "DayDegrees model. The equation used to calculate the daily increment in physiological "
    "time is @Math{exp(ax) - exp(ab-(b-x) fraction c) + d}, where @I x is daily average "
    "temperature.");
	AddProduct(BriereIITime,
    "This is the physiological time scale of Briere II et al. (reference to be added). "
    "It pulls the daily average temperature from the @F weather object just like the @F "
    "DayDegrees model. The equation used to calculate the daily increment in physiological "
    "time is @Math{exp(ax) - exp(ab-(b-x) fraction c) + d}, where @I x is daily average "
    "temperature.");	
	
	AddProduct(CuadraticLongTime,
    "This is the physiological time scale of Cuadratic Longevity. (reference to be added). "
    "It pulls the daily average temperature from the @F weather object just like the @F "
    "DayDegrees model. The equation used to calculate the daily increment in physiological "
    "time is @Math{exp(ax) - exp(ab-(b-x) fraction c) + d}, where @I x is daily average "
    "temperature.");
	
	AddProduct(Demand,
    "Demand of predator");
	
	
	AddProduct(Fecundity,
    "This is the physiological time scale of Fecundity. (reference to be added). "
    "It pulls the daily average temperature from the @F weather object just like the @F "
    "DayDegrees model. The equation used to calculate the daily increment in physiological "
    "time is @Math{exp(ax) - exp(ab-(b-x) fraction c) + d}, where @I x is daily average "
    "temperature.");
	
	AddProduct(Fecundity5Time,
    "This is the physiological time scale of Fecundity. (reference to be added). "
    "It pulls the daily average temperature from the @F weather object just like the @F "
    "DayDegrees model. The equation used to calculate the daily increment in physiological "
    "time is @Math{exp(ax) - exp(ab-(b-x) fraction c) + d}, where @I x is daily average "
    "temperature.");
	
     AddProduct(LambTime,
    "This is the physiological time scale of Lamb. (reference to be added). "
    "It pulls the daily average temperature from the @F weather object just like the @F "
    "DayDegrees model. The equation used to calculate the daily increment in physiological "
    "time is @Math{exp(ax) - exp(ab-(b-x) fraction c) + d}, where @I x is daily average "
    "temperature.");	
	
	AddProduct(LinearTime,
    "This is the physiological time scale of Linear et al. (reference to be added). "
    "It pulls the daily average temperature from the @F weather object just like the @F "
    "DayDegrees model. The equation used to calculate the daily increment in physiological "
    "time is @Math{exp(ax) - exp(ab-(b-x) fraction c) + d}, where @I x is daily average "
    "temperature.");
	
	AddProduct(Lactin2Time,
    "This is the physiological time scale of Lactin2 et al. (reference to be added). "
    "It pulls the daily average temperature from the @F weather object just like the @F "
    "DayDegrees model. The equation used to calculate the daily increment in physiological "
    "time is @Math{exp(ax) - exp(ab-(b-x) fraction c) + d}, where @I x is daily average "
    "temperature.");
	
	AddProduct(LoganBTime,
    "This is the physiological time scale of Logan et al. (reference to be added). "
    "It pulls the daily average temperature from the @F weather object just like the @F "
    "DayDegrees model. The equation used to calculate the daily increment in physiological "
    "time is @Math{exp(ax) - exp(ab-(b-x) fraction c) + d}, where @I x is daily average "
    "temperature.");
	
	AddProduct(ScheduledG1LossRate,
     "Type G1 insecticide schedule application.");
	
}

UniSim::Identifier IpmFactory::id() const {
    return "ipm";
}

QString IpmFactory::description() const {
    return
    "The ipm plugin includes models for teaching purposes";
}

QStringList IpmFactory::authors() const {
    return QStringList()
        << "Niels" << "Eduardo" << "Julio" << "Carlos";
}

QObject* IpmFactory::asQObject() {
   return this;
}

#if QT_VERSION < 0x50000
Q_EXPORT_PLUGIN2(ipm_factory, IpmFactory)
#endif

} //namespace
