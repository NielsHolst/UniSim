/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/integrator.h>
#include <usbase/parameter.h>
#include <usbase/variable.h>
#include "publish.h"
#include "range.h"

namespace UniSim{

PUBLISH(Range)

Range::Range(Identifier name, QObject *parent)
    : Model(name, parent)
{
    addParameterRef<double>(Name(progress), "steps[progress]");
    addParameter<double>("min", &minValue, 0., "Minimum value in range");
    addParameter<double>("max", &maxValue, 100., "Maximum value in range");
    addParameter<QString>("scale", &scaleAsString, QString("linear"),
	"Scale is either @F linear or @F {log10}. "
	"With a @F log10 scale, @F min=-2 and @F {max=3}, values in the range [0.01,1000] will result");
    addVariable<double>(Name(value), "Current value in range");

}
void Range::initialize() {
	decodeScale();
}

void Range::decodeScale() {
	QString s = scaleAsString.toLower();
	if (s=="linear")
		scale = Linear;
	else if (s=="log10")
		scale = Log10;
	else {
		QString msg = "Invalid value ('%1') for parameter ('scale'). Only valid values are: \"linear\" or \"log10\"";
        throw Exception(msg.arg(scaleAsString), this);
	}
}

void Range::reset() {
	value = 0.;
}

void Range::update() {
    value = minValue + progress*(maxValue-minValue);
	if (scale == Log10)
		value = pow(10, value);
}

} //namespace

