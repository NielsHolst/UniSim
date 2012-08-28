/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef ECOTOX_LOG_LOGISTIC
#define ECOTOX_LOG_LOGISTIC
#include <QObject>
#include <usbase/model.h>

namespace ecotox {

class LogLogistic : public UniSim::Model
{
	Q_OBJECT
public: 
    LogLogistic(UniSim::Identifier name, QObject *parent=0);
	// standard methods
    void initialize();
	void reset();
	void update();

private:
    // parameters
    double lower, upper, ED50, slope;

    // pull variables
    double value;

    // push variables
    double dose;

    // methods
    double f(double z);

};

} //namespace
#endif
