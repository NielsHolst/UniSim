/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "vapour_flux_base.h"
#include "vapour_flux_sum_base.h"

using namespace UniSim;

namespace vg {

/*! \class VapourFluxSumBase
 * \brief Base class for summed vapour fluxes
 *
 * Outputs
 * ------
 * - _conductance_ is the vapour flux conductance, summed over given models [m/s]
 * - _vapourFlux_ is the condensation rate, summed over given models [kg/m<SUP>2</SUP>/s]
 * - _gain_ is used to integrate over all water vapour fluxes (GCC, p. 148), summed over given models [kg/m<SUP>2</SUP>/s]
 *
 * Models are given by the the virtual method fluxes().
 */

VapourFluxSumBase::VapourFluxSumBase(Identifier name, QObject *parent)
    : VapourFluxBase(name, parent)
{
}

void VapourFluxSumBase::initialize() {
    ptrs.clear();
    for (auto flux : fluxes()) {
      const double *co = flux->pullValuePtr<double>("conductance"),
                   *va = flux->pullValuePtr<double>("vapourFlux"),
                   *ga = flux->pullValuePtr<double>("gain");
      ptrs << Ptr{co, va, ga};
    }
}

void VapourFluxSumBase::update() {
    conductance = vapourFlux = gain = 0;
    for (auto p : ptrs) {
        conductance += *(p.conductance);
        vapourFlux += *(p.vapourFlux);
        gain += *(p.gain);
    }
}

} //namespace

