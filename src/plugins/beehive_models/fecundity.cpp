/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/exception.h>
#include "../standard_models/stage.h"
#include "fecundity.h"

using namespace UniSim;


namespace beehive{

Fecundity::Fecundity(UniSim::Identifier name, QObject *parent)
    : Model(name, parent)
{
    setRecursionPolicy(Component::Update, Component::ChildrenNot);

    setState("eggsLaid", &eggsLaid);
}

void Fecundity::initialize() {
    setParameter("ageBegin", &ageBegin, 0);
    setParameter("ageEnd", &ageBegin, 10);
    setParameter("dailyFecundity", &dailyFecundity, 20.);
    setParameter("sexRatio", &sexRatio, 0.5);

    adult = dynamic_cast<Stage*>(parent());
    if (!adult)
        throw Exception("Fecundity must have a Stage model as parent");
}

void Fecundity::reset() {
    eggsLaid = 0;
}

void Fecundity::update() {
    const double *adultAgeClasses = adult->ageClasses();
    int k = adult->parameter<int>("k");
    double duration = adult->parameter<double>("duration");

    eggsLaid = 0;
    for (int i = 0; i < k; ++i) {
        double age = (i + 0.5)*duration/k;
        if (age >= ageBegin && age <= ageEnd)
            eggsLaid += adultAgeClasses[i]*dailyFecundity*sexRatio;
    }


}

} //namespace

