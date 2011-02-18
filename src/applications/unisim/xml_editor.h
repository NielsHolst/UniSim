/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UniSim_XML_EDITOR_H
#define UniSim_XML_EDITOR_H

#include <QObject>

class QProcess;

namespace UniSim {

class XmlEditor : public QObject
{
	Q_OBJECT
	
public:

    XmlEditor();
    ~XmlEditor();
    void execute(QString filePath);

private:
    QProcess *process;
    QString xmlEditorPath;
};

}
#endif
