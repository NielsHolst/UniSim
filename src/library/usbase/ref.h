/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_REF_H
#define UNISIM_REF_H
#include <QFile>
#include <QList>

namespace UniSim{

class NamedObject;
class ParameterBase;
class VariableBase;

class Ref
{
public: 
    Ref(NamedObject *parameterParent, ParameterBase *parameter, QString reference);
    static void clear();
    static void remove(ParameterBase *parameter);
    static void resolve();
    static void writeEdges(QFile &f);
private:
    NamedObject *parameterParent;
    ParameterBase *parameter;
    QString reference;
    const VariableBase *source;

    static QList<Ref*> all;
    QString notFoundMessage(UniSim::Exception &ex);
};


} //namespace
#endif
