/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <QDesktopServices>
#include <QFile>
#include <QMessageBox>
#include <QProcess>
#include <QUrl>
#include <usbase/exception.h>
#include "xml_editor.h"

namespace UniSim {

XmlEditor::XmlEditor()
{
    process = new QProcess;
    xmlEditorPath = "C:/Program Files/Notepad++/notepad++.exe";
    //xmlEditorPath = "C:/WINDOWS/system32/notepad.exe";
}

XmlEditor::~XmlEditor() {
    process->close();
    delete process;
}

void XmlEditor::execute(QString filePath) {
    if (!QFile(xmlEditorPath).exists())
        throw UniSim::Exception("Could not find dot program to edit XML file:\n'" + xmlEditorPath + "'");

    QString command = "\"" + xmlEditorPath + "\"";
    //QStringList arguments = QStringList() << "\"" + filePath + "\"";
    QStringList arguments = QStringList() << "\"C:/data/Universal Simulator/models/test.xml\"";

    //QMessageBox::information(0,"Test execute",command+"\n"+arguments[0]);

    //process->start(command, arguments);
    QUrl url("C:\\data\\Universal Simulator\\models\\test.xml");
    QMessageBox::information(0,"URL",url.path());
    QDesktopServices::openUrl(url);
    //process->start("C:/data/QDev/unisim/doc/test editor.bat", QStringList());
}

}

