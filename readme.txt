--------------------------------------------------------------------
Copyright (C) 2009-2010 by Niels Holst and co-authors. 
Released under the terms of the GNU General Public License version 
3.0 or later. See www.gnu.org/copyleft/gpl.html.
--------------------------------------------------------------------

Universal Simulator (UniSim) Software
=====================================

Universal Simulator is a software package for collaborative 
ecological modelling. It is composed of a GUI main module which is 
used to open and execute model specifications read from XML files. 
The XML files specify the components constituting a model. The 
functionality of these components are defined in plug-in libraries. 
This makes UniSim extendible and open for re-use. It is programmed 
in standard C++ but relies on the Qt library.

Real World Usage
================

UniSim software is intended for research and education. While the 
first scientific papers making use of UniSim are in press, it has 
already been used to teach students modelling, both at graduate and 
post-graduate level.

The academic use of UniSim is reflected by the author list (see 
authors.txt).

Future Plans
============

The UniSim main module will be extended with additional 
capabilities, such as stochastic and spatial modeling and 
sensitivity analysis. Some basic functionality for ease of use also 
needs implementation.

The UniSim plug-in library will be extended with still more models. 
Current work concentrates on weeds and honey bees. 

Requirements
============

* Qt Creator software (tested with 2010.02.1)
* Graphviz software (tested with 2.20)
* Boost library (tested with 1.41.0)
* Qwt library (supplied with UniSim source for convenience)
* Runs on all platforms supported by current Qt version

Acknowledgements
================

UniSim development has been supported by 
* European Network for the Durable Exploitation 0f Crop Protection 
Strategies (ENDURE) 
* International Centre for Research in Organic Food Systems 
(ICROFS), Denmark
* Aarhus University, Denmark

Installation
============

1.  Install Qt Creator
----------------------
1.1 Go to qt.nokia.com/downloads. 
1.2 Click the ‘LGPL’ tab to view the open-source page.
1.3 Download the ‘Qt SDK’ package appropriate to your operating 
system.
1.4 Install the downloaded package accepting all defaults.

--- On Windows proceed with:
1.5 Take note of the name of the Qt installation folder, e.g. 
‘C:\Qt\2010.02.1’.

1.6 Create an environment variable: 
name = QT_ROOT
value = C:\Qt\2010.02.1 (or correspondingly)

1.7 Append the qt\bin path to the system path:
C:\Qt\2010.02.1\qt\bin (or correspondingly)

[You can install a new version of Qt Creator alongside previous 
versions following the same steps. In that case just change the 
value of QT_ROOT and update the qt\bin path on the system path. It 
makes no diffence whether you uninstall the previous version(s) 
before, after or never.] 

2.  Install Graphviz
--------------------
2.1 Go to www.graphviz.org/Download..php.
2.2 Click the menu entry appropriate to your operating system.
2.3 Download the current stable release.
2.4 Install the downloaded package accepting all defaults.

3.  Download source code
-----------------------
3.1 Create a folder for your source code, e.g. ‘C:\qdev’
3.1 Go to www.boost.org/
3.2 Click the Get Boost button
3.3 Download the latest release
3.4 Unpack it as a sub-folder of you source code folder, e.g. 
‘C:\qdev\boost_1_41_0’. Avoid nested sub-folders, e.g. 
‘C:\qdev\boost_1_41_0\boost_1_41_0’.
3.5 Create an environment variable: 
name = BOOST_ROOT
value = C:\qdev\boost_1_41_0 (or correspondingly)

[You can install a new version of Boost alongside previous versions 
following the same steps. In that case just change the value of 
BOOST_ROOT. It makes no diffence whether you delete the previous 
version(s) before, after or never.]

3.6 Go to www.ecolmod.org.
3.7 Download the latest release of UniSim.
3.8 Unpack it as a sub-folder of you source code folder, e.g. 
‘C:\qdev\unisim_1_4’. Avoid nested sub-folders, e.g. ‘C:\qdev\ 
unisim_1_4\unisim_1_4’.

[You can install a new version of UniSim alongside previous versions 
following the same steps. Copy any project sub-folder(s), that you 
created for your own plugin(s), from the previous version to the new 
one. It makes no difference whether you delete folders holding 
previous UniSim versions.]

Building
========

[Below UNISIM_ROOT denotes the location of your UniSim source code 
folder, e.g. ‘C:\qdev\unisim_1_4’]

1. Automatic Building
The script file build_all.bat found in UNISIM_ROOT/src/build/win 
initiates a cascade of makefiles which in concert build both the 
UniSim main module and all the plug-ins. Build scripts for Linux and 
Mac OS have not yet been implemented

2. After building UniSim you will find the UniSim executable file in
UNISIM_ROOT/src/applications and the plug-in libraries in 
UNISIM_ROOT/src/plugins

Running
=======

Instructions for running UniSim are found in readme-for-users.txt. 

UniSim will ask for the location of various folders first time you 
run it. Beforehand you might as well create two folders to hold

* temporary files (e.g. C:\unisim\temp)
* output files (e.g. C:\unisim\output)

The exact location of these folders is not important.

---------------------------------------
Niels Holst, Aarhus University, Denmark
[niels.holst@agrsci.dk] 
---------------------------------------

