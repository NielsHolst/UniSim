/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <QList>
#include <QMessageBox>
#include <QObject>
#include "exception.h"
#include "integrator.h"
#include "model.h"
#include "utilities.h"

/*! \class UniSim::Exception
    \brief The %Exception class is a base class for all exceptions thrown by the UniSim library

    Exception can be used to throw simple exceptions that are charaterised only by a message text.
    Special-purpose exceptions should be derived from %Exception.
*/


namespace UniSim {

bool Exception::excepted = false;

Exception::Exception(QString message, const QObject *concerning_)
    : _message(message), concerning(concerning_)
{
    if (excepted) {
        excepted = false;
        return;
    }
    excepted = true;

    /* This causes problems with seekMany depending on the specific error condition
       Needs reworking or nmust be dropped
    try {
        QList<Integrator*> integrators =seekMany<Integrator*>("*");
        for (int i = 0; i < integrators.size(); ++i)
            integrators[i]->acceptException(this);
    }
    catch (...) {
    }
    */
}


QString Exception::message() const
{
    QString msg = _message;
    if (concerning) {
        msg += "\nConcerning: " + fullName(concerning);
        auto conc = const_cast<QObject*>(concerning);
        Model *model = dynamic_cast<Model*>(conc);
        if (model) {
            auto calendars = model->seekMany<Model*>("calendar");
            if (calendars.size() == 1) {
                auto step = calendars.at(0)->peekValuePtr<int>("totalTimeSteps");
                if (step)
                    msg += "\n In integration step: " + QString::number(*step);
            }
        }
    }
    return msg;
}

void Exception::show() const
{
	QMessageBox::warning(0, "Error", message());
}

} //namespace
