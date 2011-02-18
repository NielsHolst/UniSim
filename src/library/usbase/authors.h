/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_AUTHORS
#define UNISIM_AUTHORS
#include <QMap>
#include <QObject>
#include "object_pool.h"

class QXmlStreamReader;

namespace UniSim{

class Authors : public QObject
{
	Q_OBJECT
public: 
    Authors();
    ~Authors();
    static QString id();

    struct Author {
        Identifier id;
        QString name, address, email;
    };
    typedef QMap<Identifier, Author> Collection;

    Author find(Identifier id);
    const Collection* collection();

private:
    // Singleton
    static Authors* _authors;
    friend Authors* authors();

    // Methods
    void initialize();
    Authors *theAuthors;
    bool nextToken();
    bool elementNameEquals(QString s) const;
    QString elementName() const;
    QString elementText() const;
    void readAuthor();
    QString readText();
    QString message(QString text) const;

    // Data
    Collection theCollection;
    QXmlStreamReader *reader;
};

inline Authors* authors() {
    if (!Authors::_authors) {
        Authors::_authors = objectPool()->find<Authors*>(Authors::id());
        Authors::_authors->initialize();
    }
    return Authors::_authors;
}


} //namespace
#endif
