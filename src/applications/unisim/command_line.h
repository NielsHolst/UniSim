/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_COMMAND_LINE_H
#define UNISIM_COMMAND_LINE_H

class CommandLine
{

public:
    CommandLine(int argc, char *argv[]);
    QString input() const;
private:
    int argc;
    char *argv[];
    QString _input;
};	


#endif
