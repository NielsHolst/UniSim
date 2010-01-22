/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef ESS2009_TREATMENTS_H
#define ESS2009_TREATMENTS_H

#include <QObject>
#include <usbase/model.h>


namespace ess2009 {

class Treatments : public UniSim::Model
{
	Q_OBJECT
public:
    Treatments(UniSim::Identifier name, QObject *parent=0);
	
private:
};


}
#endif
