/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "exception.h"
#include "named_object.h"
#include "reference.h"
#include "utilities.h"
#include "variable_base.h"

namespace UniSim {

Reference::Reference() {}

Reference::Reference(QString s, NamedObject *parent)
    : _s(s), _parent(parent)
{
    splitReference();
}

Reference::Reference(const char *s, NamedObject *parent)
    : Reference(QString(s), parent) {}


QString Reference::path() const {
    return _path;
}

QString Reference::name() const {
    return _name;
}

QString Reference::toString() const {
    return _s;
}

bool Reference::isEmpty() const {
    return _s.isEmpty();
}

const NamedObject* Reference::parent() const {
    return _parent;
}

void Reference::splitReference() {
    QString s = _s.trimmed();
    int begin = s.indexOf('[');
    int end = s.indexOf(']');

    QString msg;
    if (begin == -1)
        msg = "Missing '[' in reference '%1'";
    else if (end == -1)
        msg = "Missing ']' in reference '%1'";
    else if (end < s.size() - 1)
        msg = "Reference '%1' must end with ']'";
    else if (begin == 0)
        msg = "Reference '%1' misses path before '['";
    else if (end - begin == 1)
        msg = "Reference '%1' misses name inside the brackets";
    if (!msg.isEmpty())
        throw Exception(msg.arg(s), _parent);

    _path = s.left(begin).trimmed();
    _name = s.mid(begin + 1, end - begin -1).trimmed();
}

//bool Reference::exists() const {
//    return count()==0;
//}

//bool Reference::isUnique() const {
//    return count()==1;
//}

//int Reference::count() const {
//    resolve();
//    return targets.size();
//}

//Reference::Target Reference::target(int index) const {
//    resolve();
//    return targets.at(index);
//}

//namespace {
//    inline bool isRelative(QString s) {
//        return s.left(2) == "..";
//    }
//}

//void Reference::doResolve() {
//    QString targetPath = _path;
//    if (isRelative(_path[0])) {
//        NamedObject *origin = _parent;
//        QString p = _path;
//        while (isRelative(p)) {
//            origin = origin->seekParent<NamedObject*>("*");
//            p = (p.size()==2) ? "" : p.mid(3);
//        }
//        targetPath = origin->fullLabel();
//        if (!p.isEmpty())
//            targetPath += "/" + p;
//    }
//    auto parents = seekMany<TParent>(parentName);
//    QList<TChild> result;
//    for (auto parent : parents)
//        result << parent->seekChildren<TChild>(childName);

//}

QString Reference::notFoundMessage() const {
    // No parent
    if (_parent) {
        QString msg = "Cannot resolve reference '%1'' without parent";
        return msg.arg(toString());
    }
    QList<NamedObject*> models = _parent->seekMany<NamedObject*>(path());
    QString detail;

    // No model found
    if (models.isEmpty()) {
        QString msg = "Model '%1' not found";
        detail = msg.arg(path());
    }
    // More than one model found
    else if (models.size() > 1) {
        QString msg = "More than one model found matching '%1':\n";
        detail = msg.arg(path());
        for (auto model : models)
            detail += model->fullLabel() + "\n";
    }
    // No variable found
    else {
        NamedObject *model = models[0];
        QList<Identifier> variableIds;
        bool idFound{false};
        for (auto variable : model->seekChildren<VariableBase*>("*")) {
            variableIds << variable->id();
            idFound = idFound || variable->id()==Identifier(name());
        }

        // Variable exists but of different type
        if (idFound) {
            QString msg = "Parameter named '%1' has the wrong type";
            detail = msg.arg(name());
        }

        // Model has no variables
        else if (variableIds.isEmpty()) {
            QString msg = "Model '%1' has no inputs or outputs";
            detail = msg.arg(path());
        }

        // Variable does not exist
        else {
            QString msg = "Parameter named '%1' not found.\n"
                          "Model '%2' contains these inputs and outputs:\n%3";
            QStringList list;
            for (auto id : variableIds)
                list << ("'" + id.label() + "'");
            detail = msg.arg(name()).arg(path()).arg(list.join("\n"));
        }
    }
    return QString("Error in reference '%1' referenced from '%2:' ")
            .arg(toString())
            .arg(_parent->fullLabel())
            + detail;
}

bool operator==(const Reference &a, const Reference &b) {
    return a.toString().toLower() == b.toString().toLower()
           && a.parent() == b.parent();
}

} //namespace
