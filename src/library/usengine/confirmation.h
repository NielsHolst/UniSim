/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_CONFIRMATION_H
#define UNISIM_CONFIRMATION_H

#include <QString>

class QWidget;

namespace UniSim{
	
class Confirmation
{
public:
    Confirmation(QString title, QString question, QWidget *parent=0);
    bool accepted(bool defaultAnswer = true) const;
private:
    QString title, question;
    QWidget *parent;
};

} //namespace

#endif
