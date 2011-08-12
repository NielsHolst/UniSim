#include <usbase/exception.h>
#include "name_list.h"

namespace UniSim{

NameList::NameList(const QStringList &names_)
    : names(names_)
{
}

QStringList NameList::simplified() {
    if (names.size() == 1) {
        names[0] = names[0].mid(names[0].lastIndexOf('/') + 1);
    }
    else {
        prepareNames();
        fromIndex = 0;
        for (fromIndex = 0; fromIndex < names.size()-1; ++fromIndex) {
            while (nextSubset())
                simplifySubset();
        }
    }
    return names;
}

void NameList::prepareNames() {
	names.sort();
	for (int i = 1; i < names.size(); ++i) {
		if (names[i-1] == names[i])
			throw Exception(QString("Name is not unique: %1").arg(names[i]));
	}
}

bool NameList::nextSubset() {
    QString nextHead = head(names[fromIndex]);
	subset.clear();
	for (int i = fromIndex+1; i < names.size(); ++i) {
        if (head(names[i]) == nextHead)
            subset.append(i);
	}
    return !subset.isEmpty();
}

QString NameList::head(QString name) {
    int ix = name.indexOf('/');
	return (ix == -1) ? name : name.left(ix);
}

void NameList::simplifySubset() {
	for (int i = 0; i < subset.size(); ++i) {
		removeHead(&names[subset[i]]);
	}
}

void NameList::removeHead(QString *name) {
    int ix = name->indexOf('/');
	Q_ASSERT(ix > -1);
    *name = name->mid(ix+1);
}

} //namespace
