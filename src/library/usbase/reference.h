/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_REFERENCE_H
#define UNISIM_REFERENCE_H
#include <QString>

namespace UniSim{

class NamedObject;
class VariableBase;

class Reference
{
public: 
    Reference();
    Reference(QString s, NamedObject *parent = 0);
    Reference(const char *s, NamedObject *parent = 0);
    QString path() const;
    QString name() const;
    QString toString() const;
    bool isEmpty() const;
    const NamedObject* parent() const;

//    bool exists() const;
//    bool isUnique() const;
//    int count() const;
//    Target target(int index=0) const;
    QString notFoundMessage() const;

//    struct Target {
//        NamedObject *parent;
//        VariableBase *vaiable;
//    };

private:
    // Data
    QString _s, _path, _name;
    NamedObject *_parent;
//    bool _resolved;
//    QVector<Target> targets;
    // Methods
    void splitReference();
//    void doResolve();
//    void resolve() { if (!resolved) doResolve(); }
};

bool operator==(const Reference &a, const Reference &b);

} //namespace
#endif
