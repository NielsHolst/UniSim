/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "builder.h"

Builder* Builder::theBuilder;

Builder& builder() {
    if (!Builder::theBuilder)
        Builder::theBuilder = new Builder;
    return *Builder::theBuilder;
}

Builder& Builder::simulation() {
    return *this;
}

Builder& Builder::controller(QString type, QString name) {

    return *this;
}

Builder& Builder::model(QString type, QString name) {

    return *this;
}

Builder& Builder::bare(QString name) {

    return *this;
}

Builder& Builder::parameter(QString name, QString value) {

    return *this;
}

Builder& Builder::begin() {

    return *this;
}

Builder& Builder::end() {

    return *this;
}
