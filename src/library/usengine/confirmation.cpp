/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <QMessageBox>
#include <QWidget>
#include "confirmation.h"

namespace UniSim{

Confirmation::Confirmation(QString title_, QString question_, QWidget *parent_)
        : title(title_), question(question_), parent(parent_)
{
}

bool Confirmation::accepted(bool defaultAnswer) const
{
    QMessageBox::StandardButtons showButtons = QMessageBox::Yes | QMessageBox::No;
    QMessageBox::StandardButton defaultButton = defaultAnswer ? QMessageBox::Yes : QMessageBox::No;
    return QMessageBox::question(parent, title, question, showButtons, defaultButton) == QMessageBox::Yes;
}

} //namespace

