/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UniSim_SUB_WINDOW_H
#define UniSim_SUB_WINDOW_H
#include <QMdiSubWindow>
#include <QObject>
#include <QString>

class QCloseEvent;
class QMdiArea;

class SubWindow : public QMdiSubWindow
{
	Q_OBJECT

public:
    typedef enum{ModelView, SettingsView, SimulationOutput, All} Type;
    SubWindow(QMdiArea *area, QString title, Type type);
    Type type() const;


private:
    Type _type;
};	


#endif
