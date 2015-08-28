/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <stdlib.h>
#include "publish.h"
#include "shelters.h"

using std::max;
using namespace UniSim;

namespace vg {

PUBLISH(Shelters)

/*! \class Shelters
 * \brief Collection of all greenhouse shelters
 *
 * Inputs
 * ------
 * - _directTransmissionFile_ is a table with transmission values [0;1]
 * according to latitude (rows) and sun azimuth (columns)
 * - _latitude_ is the geographical latitude of the greenhouse [-180;180]
 * - _azimuth_ is the sun azimuth [-90;90]
 * - _greenhouseShade_ is the fraction of light caught by the greenhouse construction [0;1]
 * - _chalk_ is the chalk efficacy [0;1]
 */

Shelters::Shelters(Identifier name, QObject *parent)
    : Model(name, parent)
{
    Output(double, airTransmissivity);
    Output(double, U);
    Output(double, area);
    Output(double, maxScreenState);
}

void Shelters::initialize() {
    infos.clear();
    auto shelters = seekChildren<Model*>("*");
    for (auto shelter : shelters) {
        infos <<
            ShelterInfo {
                shelter->pullValuePtr<double>("airTransmissivity"),
                shelter->pullValuePtr<double>("U"),
                shelter->pullValuePtr<double>("area"),
                shelter->pullValuePtr<double>("maxScreenState")
            };
    }
}

void Shelters::reset() {
    update();
}

void Shelters::update() {
    airTransmissivity = U = area = maxScreenState = 0.;
    for (ShelterInfo info : infos) {
        airTransmissivity += (*info.pArea) * (*info.pAirTransmissivity);
        U += (*info.pArea) * (*info.pU);
        area += (*info.pArea);
        maxScreenState = max(maxScreenState, *info.pMaxState);
    }
    airTransmissivity /= area;
    U /= area;
}

} //namespace

