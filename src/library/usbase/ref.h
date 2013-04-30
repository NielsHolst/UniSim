/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_REF_H
#define UNISIM_REF_H
#include <QList>
#include <QPair>

namespace UniSim{

class NamedObject;
class ParameterBase;

class Ref
{
public: 
    Ref(NamedObject *parameterParent, ParameterBase *parameter, QString reference);
    static void resolve();
private:
    NamedObject *parameterParent;
    ParameterBase *parameter;
    QString reference;

    static QList<Ref*> all;
    static void clear();
};


} //namespace
#endif
