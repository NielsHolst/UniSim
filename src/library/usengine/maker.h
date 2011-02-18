/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_MAKER_H
#define UNISIM_MAKER_H

#include <QMap>
#include <QObject>

#include <QStringList>
#include <usbase/identifier.h>

class QString;

namespace UniSim{
	
class Integrator;
class MakerPlugIn;

template <class MakerPlugIn>
class Maker : public QObject
{
    Q_OBJECT
public:
    Maker();
    template <class createType>
    create(Identifier integratorType, Identifier objectName, QObject *parent=0);

    bool canCreate(Identifier integratorType);

    Maker* findOne(Identifier id);


	private:
    static Maker *_me;

    MakerPlugIn* find(Identifier integratorType);

    typedef QMap<Identifier, MakerPlugIn*> Makers;
    QMap<Identifier, MakerPlugIn*> _Makers;
};
//@}
}//namespace

#endif
