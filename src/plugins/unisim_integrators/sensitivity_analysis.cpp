/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <iostream>
#include <QList>
#include <QMessageBox>
#include <QString>
#include <usbase/file_locations.h>
#include <usbase/model.h>
#include <usbase/name_list.h>
#include <usbase/parameter.h>
#include <usbase/pull_variable.h>
#include <usbase/utilities.h>
#include "sensitivity_analysis.h"

namespace UniSim {
	
SensitivityAnalysis::SensitivityAnalysis(Identifier name, QObject *parent)
    : TimeStepLimited(name, parent)
{
    new Parameter<double>("factor", &factor, -1., this, "description");
    new Parameter<double>("relative", &relative, -1., this, "description");
    new Parameter<double>("absolute", &absolute, -1., this, "description");
    new Parameter<int>("days", &days, 10., this, "description");
    new Parameter<int>("seconds", &seconds, 60., this, "description");
}

void SensitivityAnalysis::initialize() {
    TimeStepLimited::initialize();
    checkParameters();
    findParameters();
    stratifyParameters();
    setNames();

    QString path = FileLocations::location(FileLocationInfo::Output).absolutePath();
    QString fn = path + QString("/sa-%1.txt").arg(names.size());
    QFile file(fn);
    file.open(QIODevice::Text | QIODevice::WriteOnly);
    for (int i = 0; i < names.size(); ++i) {
        QString s = names[i] + "\n";
        file.write(s.toAscii());
    }
}

void SensitivityAnalysis::checkParameters() {
    int n = 0;
    if (factor<0.)
        ++n;
    else {
        type = StrataBase::Factor;
        deviance = factor;
    }
    if	(relative<0.)
        ++n;
    else {
        type = StrataBase::Relative;
        deviance = relative;
    }
    if (absolute<0.)
        ++n;
    else {
        type = StrataBase::Absolute;
        deviance = absolute;
    }
    QString s = " of these parameters to a positive number: absolute, factor, or relative";
    if (n == 3)
        throw Exception("Set one" + s, this);
    if (n < 2)
        throw Exception("Set at most one" + s, this);
}

void SensitivityAnalysis::findParameters() {
    Models models = seekMany<Model*>("*");
    int me = models.indexOf(this);
    Q_ASSERT(me > -1);
    models.removeAt(me);
    for (int i = 0; i < models.size(); ++i) {
        parameters << models[i]->seekChildren<ParameterBase*>("*");
    }
}

void SensitivityAnalysis::stratifyParameters() {
    Model *iterator = seekOneChild<Model*>("RunIterator");
    int iterations = iterator->pullVariable<int>("numIterations");
    for (int i = 0; i < parameters.size(); ++i) {
        double useDeviance = deviance;
        ParameterBase *parameter = parameters[i];
        if (dynamic_cast<Parameter<QDate>*>(parameter)) {
            if (days == -1)
                throw Exception("You must specify 'days' parameter for sensitivity analysis", parameter);
            useDeviance = days;
        }
        else if (dynamic_cast<Parameter<QTime>*>(parameter)) {
            if (seconds == -1)
                throw Exception("You must specify 'seconds' parameter for sensitivity analysis", parameter);
            useDeviance = seconds;
        }
        parameters[i]->createStrata(useDeviance, iterations, type);
    }
}

void SensitivityAnalysis::setNames() {
    QStringList fullNames;
    for (int i = 0; i < parameters.size(); ++i) {
        fullNames.append(UniSim::fullName(parameters[i]));
    }
    NameList nl(fullNames);
    names = nl.simplified();
}

bool SensitivityAnalysis::nextRun() {
    bool doNext = Integrator::nextRun();
    if (doNext) {
        for (int i = 0; i < parameters.size(); ++i) {
            parameters[i]->setValueFromNextSample();
        }
    }
    return doNext;
}

} //namespace

