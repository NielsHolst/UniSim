/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "microbial_community_factory.h"
#include "community.h"
#include "population.h"
#include "competition.h"
#include "reaction.h"
#include "enzymatic_reaction.h"
#include "zero_order_reaction.h"
#include "first_order_reaction.h"
#include "effect.h"
#include "general_effect.h"
#include "competitive_inhibition.h"

using namespace UniSim;

namespace MicrobialCommunity{

void MicrobialCommunityFactory::defineProducts() {
    addProduct<Community>("Community", this, "Description pending");
    addProduct<Population>("Population", this, "Description pending");
    addProduct<Competition>("Competition", this, "Description pending");
    addProduct<Reaction>("Reaction", this, "Description pending");
    addProduct<ZeroOrderReaction>("ZeroOrderReaction", this, "Description pending");
    addProduct<FirstOrderReaction>("FirstOrderReaction", this, "Description pending");
    addProduct<EnzymaticReaction>("EnzymaticReaction", this, "Description pending");
    addProduct<Effect>("Effect", this, "Description pending");
    addProduct<GeneralEffect>("GeneralEffect", this, "Description pending");
    addProduct<CompetitiveInhibition>("CompetitiveInhibition", this, "Description pending");
}

UniSim::Identifier MicrobialCommunityFactory::id() const {
    return "MicrobialCommunity";
}

QString MicrobialCommunityFactory::description() const {
    return "Description of MicrobialCommunity";
}

QStringList MicrobialCommunityFactory::authors() const {
    return QStringList() << "Kevin" << "Niels";
}

QObject* MicrobialCommunityFactory::asQObject() {
   return this;
}

Q_EXPORT_PLUGIN2(microbial_community_factory, MicrobialCommunityFactory)

} //namespace
