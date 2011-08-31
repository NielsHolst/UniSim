/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_SENSITIVITY_ANALYSIS_H
#define UNISIM_SENSITIVITY_ANALYSIS_H

#include <usbase/strata_base.h>
#include "time_step_limited.h"

namespace UniSim {
	
class ParameterBase;

class SensitivityAnalysis : public TimeStepLimited
{
	Q_OBJECT

public:
    SensitivityAnalysis(Identifier name, QObject *parent=0);

    // standard methods
    void initialize();
    bool nextRun();

private:
    // Parameters
    double factor, relative, absolute, deviance;
    int days, seconds;
    StrataBase::Type type;

    // Links
    QList<ParameterBase*> parameters;
    QStringList names;

    // Methods
    void checkParameters();
    void findParameters();
    void stratifyParameters();
    void setNames();
};


} //namespace


#endif
