/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <QList>
#include <QMessageBox>
#include <QObject>
#include "exception.h"
#include "integrator.h"
#include "utilities.h"

/*! \class UniSim::Exception
    \brief The %Exception class is a base class for all exceptions thrown by the UniSim library

    Exception can be used to throw simple exceptions that are charaterised only by a message text.
    Special-purpose exceptions should be derived from %Exception.
*/


namespace UniSim {

bool Exception::excepted = false;

Exception::Exception(QString message, QObject *concerning_)
    : _message(message), concerning(concerning_)
{
    if (excepted) {
        excepted = false;
        return;
    }
    excepted = true;

    QList<Integrator*> integrators =seekMany<Integrator*>("*");
    for (int i = 0; i < integrators.size(); ++i)
        integrators[i]->acceptException(this);
}


QString Exception::message() const
{
    QString msg = _message;
    if (concerning)
        msg += "\nConcerning: " + fullName(concerning);
    return msg;
}

void Exception::show() const
{
	QMessageBox::warning(0, "Error", message());
}

} //namespace
