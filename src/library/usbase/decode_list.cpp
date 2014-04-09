/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "decode_list.h"

namespace UniSim {


void chopParentheses(QString &s, QObject *concerning) {
    if (s.left(1) != "(")
        throw UniSim::Exception("Value list miss left parenthesis: " + s, concerning);
    s.remove(0, 1);

    if (s.right(1) != ")")
        throw UniSim::Exception("Value list miss right parenthesis: " + s, concerning);

    s.chop(1);
    s = s.simplified();
}

void chopRightParenthesis(QString &s, QObject *concerning) {
    if (s.right(1) != ")")
        throw Exception("Missing right parenthesis: (" + s, concerning);
    s.chop(1);
    s = s.simplified();
}


//! Parses a simple list "(A B C)"
/*! Throws an Exception if the list is ill-formated or is empty.
*/
QStringList decodeSimpleList(QString parenthesizedList, QObject *concerning) {
    QString s = parenthesizedList.simplified();
    if (s.size() == 0)
        return QStringList();
    if (s[0] != '(') {
        if (s.endsWith(')'))
            throw UniSim::Exception("Value list misses left parenthesis", concerning);
        else
            return QStringList() << s;
    }
    chopParentheses(s, concerning);
    return s.split(" ", QString::SkipEmptyParts);
}


namespace {

    bool nextItem(QString *s, QString *item, QObject *context) {
        *s = s->trimmed();
        if (s->isEmpty())
            return false;

        if (s->startsWith("(")) {
            int left = 1, right = 0, n = s->size();
            int i = 1;
            while (i < n && left != right) {
                if (s->at(i) == '(')
                    ++left;
                else if (s->at(i) == ')')
                    ++right;
                ++i;
            }
            if (left != right)
                throw Exception("Unmatched parentheses in " + *s, context);
            int end = i - 1;
            *item = s->mid(0, end+1);
            s->remove(0, end+1);
            return true;
        }

        if (s->startsWith(")"))
            throw Exception("Unmatched parentheses in " + *s, context);

        int end = s->indexOf(QRegExp("[ ()]"));
        *item = s->mid(0, end);
        if (end==-1) {
            s->clear();
		}
        else {
			if (s->at(end) == ')')
				throw Exception("Surplus ')' in" + *s, context);
            s->remove(0, end+1);
		}
        return true;

    }
}

QStringList decodeList(QString s_, QObject *context) {
    QStringList result;
    QString s = s_.trimmed();

    if (!s.startsWith("("))
        throw Exception("Missing '(' at beginning of " + s, context);
    if (!s.endsWith(")"))
        throw Exception("Missing ')' at end of " + s, context);
    s.remove(0,1);
    s.chop(1);

    QString item;
    while (nextItem(&s, &item, context))
        result.append(item);

    return result;
}


} // namespace
