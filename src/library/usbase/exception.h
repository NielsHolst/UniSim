/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UniSim_EXCEPTION_H
#define UniSim_EXCEPTION_H
#include <QString>

namespace UniSim {
	
class Exception {
public:
    Exception(QString message);
	virtual ~Exception() { }
	
	//! Get message
	QString message() const;
	
	//! Show exception message as a QMessageBox::warning
    virtual void show() const;
	
protected:
    QString _message;
};

} //namespace

#endif
