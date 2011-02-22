/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/object_pool.h>
#include <usbase/utilities.h>
#include "MicrobialCommunity_model_maker.h"
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

QString MicrobialCommunityModelMaker::pluginDesc() const
{
        return "Description of MicrobialCommunity";
}

Identifier MicrobialCommunityModelMaker::pluginName() const {
        return "MicrobialCommunity";
}

QStringList MicrobialCommunityModelMaker::authors() const
{
	return QStringList() << "Kevin" << "Niels";
}

const QMap<Identifier, QString>& MicrobialCommunityModelMaker::supportedClasses()
{
	if (!desc.isEmpty()) return desc;

        desc["Community"] =
        "Description";

        desc["Population"] =
        "Description";

        desc["Competition"] =
        "Description";

        desc["Reaction"] =
        "Description";

        desc["ZeroOrderReaction"] =
        "Description";

        desc["FirstOrderReaction"] =
        "Description";

        desc["EnzymaticReaction"] =
	"Description";

        desc["Effect"] =
        "Description";

        desc["GeneralEffect"] =
        "Description";

        desc["CompetitiveInhibition"] =
        "Description";

	return desc;
}

void MicrobialCommunityModelMaker::useObjectPool(ObjectPool *pool) const {
	objectPool()->deferTo(pool);
}
Model* MicrobialCommunityModelMaker::create(Identifier modelType, Identifier objectName, QObject *parent)
{
	setSimulationObjectFromDescendent(parent);
	Model *model = 0;
        if (modelType.equals("Community"))
            model = new Community(objectName, parent);
        else if (modelType.equals("Population"))
            model = new Population(objectName, parent);
        else if (modelType.equals("Competition"))
            model = new Competition(objectName, parent);
        else if (modelType.equals("Reaction"))
            model = new Reaction(objectName, parent);
        else if (modelType.equals("ZeroOrderReaction"))
            model = new ZeroOrderReaction(objectName, parent);
        else if (modelType.equals("FirstOrderReaction"))
            model = new FirstOrderReaction(objectName, parent);
        else if (modelType.equals("EnzymaticReaction"))
            model = new EnzymaticReaction(objectName, parent);
        else if (modelType.equals("Effect"))
            model = new Effect(objectName, parent);
        else if (modelType.equals("GeneralEffect"))
            model = new GeneralEffect(objectName, parent);
        else if (modelType.equals("CompetitiveInhibition"))
            model = new CompetitiveInhibition(objectName, parent);
        return model;
}


Q_EXPORT_PLUGIN2(MicrobialCommunity_model_maker, MicrobialCommunityModelMaker)

} //namespace
