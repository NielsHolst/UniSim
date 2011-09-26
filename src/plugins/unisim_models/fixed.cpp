#include <usbase/parameter.h>
#include <usbase/pull_variable.h>
#include <usbase/utilities.h>          
#include "fixed.h"

namespace UniSim {

Fixed::Fixed(Identifier name, QObject *parent)
    : Model(name, parent)
{
    new Parameter<QString>("parameters", &parametersAsString, QString("()"), this,
    "Parameters as name value pairs. E.g., @F {((Tavg 22.5)(I 40))}. "
    "Currently, values are assumed to be of type double.");
    new Parameter<QString>("pullvariables", &pullVariablesAsString, QString("()"), this,
    "Pull variables as name value pairs. E.g., @F {((Tavg 22.5)(I 40))}. "
    "Currently, values are assumed to be of type double.");
    new Parameter<QString>("pushvariables", &pushVariablesAsString, QString("()"), this,
    "Push variables as name value pairs. E.g., @F {((Tavg 22.5)(I 40))}. "
    "Currently, values are assumed to be of type double.");
}



void Fixed::initialize() {
    initParameters();
    initPullVariables();
    initPushVariables();
}

void Fixed::initParameters() {
    QMap<QString, double> pList = decodeList<QString, double>(parametersAsString, this);
    int n = pList.size();
    parameters.fill(0, n);

    int i = -1;
    QMapIterator<QString, double> pa(pList);
    while(pa.hasNext()) {
        ++i;
        pa.next();
        Identifier id = pa.key();
        parameters[i] = pa.value();
        double *valuePtr = &parameters[i];
        new Parameter<double>(id, valuePtr, pa.value(), this, id.label() + " set to fixed value");
    }
}

void Fixed::initPullVariables() {
    QMap<QString, double> pvList = decodeList<QString, double>(pullVariablesAsString, this);
    int n = pvList.size();
    pullVariables.fill(0, n);

    int i = -1;
    QMapIterator<QString, double> pv(pvList);
    while(pv.hasNext()) {
        ++i;
        pv.next();
        Identifier id = pv.key();
        pullVariables[i] = pv.value();
        double *valuePtr = &pullVariables[i];
        new PullVariable<double>(id, valuePtr, this, id.label() + " set to fixed value");
    }
}

void Fixed::initPushVariables() {
    QMap<QString, double> pvList = decodeList<QString, double>(pushVariablesAsString, this);
    int n = pvList.size();
    pushVariables.fill(0, n);

    int i = -1;
    QMapIterator<QString, double> pv(pvList);
    while(pv.hasNext()) {
        ++i;
        pv.next();
        Identifier id = pv.key();
        pushVariables[i] = pv.value();
        double *valuePtr = &pushVariables[i];
        new PushVariable<double>(id, valuePtr, this, id.label() + " set to fixed value");
    }
}


} //namespace
