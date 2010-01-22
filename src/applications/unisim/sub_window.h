/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UniSim_SUB_WINDOW_H
#define UniSim_SUB_WINDOW_H
#include <QMdiSubWindow>
#include <QObject>
#include <QString>
#include "stored_widget.h"

class QCloseEvent;
class QMdiArea;

class SubWindow : public QMdiSubWindow, public StoredWidget
{
	Q_OBJECT

public:
    typedef enum{View, Output, Log, Untyped} Type;
    SubWindow(QMdiArea *area, QString title);
    void closeEvent(QCloseEvent *event);

    void setType(Type type);
    Type type() const;


private:
    Type _type;
};	


#endif
