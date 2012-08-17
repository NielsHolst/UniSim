/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <string>
#include <QString>
#include <tclap/CmdLine.h>
#include <usbase/version.h>
#include "command_line.h"

using namespace TCLAP;
using namespace UniSim;

const char generalDesc[] =
    "GUI or batch model: If you supply no arguments then the program will run in GUI mode. "
    "Supply one or more arguments to run the program as a batch job.";

const char inputDesc[] =
    "Name of XML input file. "
    "The XML file is assumed to reside in the standard file location "
    "for model files (see Files|Location in program's GUI menu). "
    "Otherwise include an absolute path with the file name. Examples:\n"
    "  * -i cornfield.xml\n"
    "  * -i version2/cornfield.xml\n"
    "  * -i C:/data/models/cornfield.xml\n"
    "  * -i \"C:/data/models/corn field.xml\"";

CommandLine::CommandLine(int argc, char *argv[]) {
    CmdLine cmd(generalDesc, ' ', qPrintable(versionExtended()));

    QString s(inputDesc);
    s.replace("*", argv[0]);
    ValueArg<std::string> inputArg("i","input",qPrintable(s), false,"","file name");
    cmd.add(inputArg);

    cmd.parse(argc, argv);
    std::string inputValue = inputArg.getValue();
    _input = inputValue.c_str();
}

QString CommandLine::input() const {
    return _input;
}


	
