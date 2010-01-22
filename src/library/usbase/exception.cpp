/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <QMessageBox>
#include "exception.h"

/*! \class UniSim::Exception
    \brief The %Exception class is a base class for all exceptions thrown by the UniSim library

    Exception can be used to throw simple exceptions that are charaterised only by a message text.
    Special-purpose exceptions should be derived from %Exception.
*/


namespace UniSim {

Exception::Exception(QString message)
	: _message(message)
{
}

QString Exception::message() const
{
	return _message;
}

void Exception::show() const
{
	QMessageBox::warning(0, "Error", message());
}

} //namespace
