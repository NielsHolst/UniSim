/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_DATASET_H
#define UNISIM_DATASET_H

#include <QList>
#include <QVector>
#include "component.h"

namespace UniSim{	

class Dataset : public Component
{
	Q_OBJECT
public:
    Dataset(Identifier name, QObject *parent = 0);
	
    // standard methods
	void reset();
	void update();

    // special methods

private:
};

} //namespace


#endif
