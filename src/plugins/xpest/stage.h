/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef XPEST_STAGE_H
#define XPEST_STAGE_H

#include <QList>
#include <usbase/model.h>

namespace xpest {

class Stage : public UniSim::Model
{
public:
    Stage(UniSim::Identifier name, QObject *parent);
    void amend();
    void reset();
    void update();

private:
    // Parameters

    // Variables
    double growthStage;

    // Links
    QList<UniSim::Model*> ddScales;
};

} //namespace


#endif
