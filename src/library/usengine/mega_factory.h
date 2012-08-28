/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_MEGA_FACTORY_H
#define UNISIM_MEGA_FACTORY_H

#include <QMap>
#include <QObject>
#include <QStringList>
#include <usbase/exception.h>
#include <usbase/identifier.h>

namespace UniSim{
	
class FactoryPlugIn;

class MegaFactory : public QObject
{
    Q_OBJECT
public:
    MegaFactory();
    static QString id();

    template <class T>
    static T* create(Identifier className, Identifier objectName, QObject *parent);

    static const QList<UniSim::FactoryPlugIn*> &factories();

private:
    // methods
    static MegaFactory* me();
    static QObject* createObject(Identifier className, Identifier objectName, QObject *parent=0);
    // data
    typedef QMap<Identifier, FactoryPlugIn*> ProductIndex;
    static MegaFactory *_me;
    ProductIndex productIndex;
    QList<UniSim::FactoryPlugIn*> _factories;
};

template <class T>
T* MegaFactory::create(Identifier className, Identifier objectName, QObject *parent)
{
    QObject *object = MegaFactory::createObject(className, objectName, parent);
    QString msg = "Cannot create object '%1' of class '%2'";
    msg = msg.arg(objectName.label()).arg(className.label());
    if (!object) {
        throw Exception(msg, parent);
    }
    T *specificObject = dynamic_cast<T*>(object);
    if (!specificObject) {
        QString msg2 = msg + ", because '%1' was not derived from class '%2'";
        throw Exception(msg2.arg(className.label()).arg(object->metaObject()->className()));
    }
    return specificObject;
}


}//namespace

#endif
