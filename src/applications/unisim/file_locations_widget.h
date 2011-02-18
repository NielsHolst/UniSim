/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef FILE_LOCATIONS_WIDGET_H
#define FILE_LOCATIONS_WIDGET_H
#include <QObject>
#include <QWidget>
#include <usbase/file_locations.h>

class QLayout;
class QPushButton;

class FileLocationsWidget : public QWidget
{
	Q_OBJECT
public:
    FileLocationsWidget(QWidget *parent = 0);
private slots:
    void doClose();
private:
    QLayout* createEntries();
    QLayout* createButtons();
    QPushButton *closeButton;
};


#endif
