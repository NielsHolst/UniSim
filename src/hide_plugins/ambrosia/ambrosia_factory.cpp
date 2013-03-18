/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "ambrosia_factory.h"
#include "plant.h"
#include "ambrosia_time.h"

using namespace UniSim;

namespace ambrosia{

void AmbrosiaFactory::defineProducts() {
    addProduct<Plant>("Plant", this, "Description pending");
    addProduct<AmbrosiaTime>("AmbrosiaTime", this, "Description pending");
}

UniSim::Identifier AmbrosiaFactory::id() const {
    return "ambrosia";
}

QString AmbrosiaFactory::description() const {
    return
    "!Exclude\n"
    "The @F ambrosia plugin is an implementation of the phenology model for @I {Ambrosia artemisiifolia}, "
    "as described by @Cite{$label{Deen et al. 1998a}deen_et_al_1998a} and @Cite{$label{1998b}deen_et_al_1998b}";
}

QStringList AmbrosiaFactory::authors() const {
    return QStringList()
        << "Niels";
}

QObject* AmbrosiaFactory::asQObject() {
   return this;
}

#if QT_VERSION < 0x50000
Q_EXPORT_PLUGIN2(ambrosia_factory, AmbrosiaFactory)
#endif

} //namespace
