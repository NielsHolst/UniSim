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
‘authors.txt’).

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

* Qt Creator software (tested with 1.3.0)
* Graphviz software (tested with 2.20)
* Boost library (tested with 1.41.0)
* Qwt library (supplied with UniSim source for convenience)
* Runs on all platforms supported by Qt 4.6.0
  - currently tested on MS Windows only

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

1. Download and install the software packages:
* Qt Creator
* Graphviz

2. Download and unpack:
* Boost library
* UniSim source

3. On Windows make certain the installed software packages are on 
the system path (i.e. the PATH system variable). For example,
* ‘C:\Qt\2009.05\qt\bin’
* ‘C:\Program Files\Graphviz2.20\Bin’

4. On Windows set up these new environment variables:
* BOOST_ROOT (e.g. ‘C:\Program Files\boost_1_41_0’)
* UNISIM_ROOT (e.g. ‘C:\qdev\unisim’)

5. Make sure all the needed dynamic link libraries are accessible. 
The Qwt library was packaged together with the UniSim source and 
resides in UNISIM/vendor/qwt/lib. On Windows you can make this 
library acessible by putting the folder on the system path (thus 
extending step 3 above). For example,
* ‘C:\qdev\unisim\vendor\qwt\lib’

6. For Git users: The Git repository resides in the ‘.git’ sub-
folder of UNISIM_ROOT.

Building
========

1. Do either 1a or 1b below.

1a. Automatic Building
The project file ‘build_all.pro’ in UNISIM_ROOT/src initiates a 
cascade of makefiles which together builds both the UniSim main 
module and all the plug-ins. Windows users can simply execute the 
‘build_all.bat’ batch file and everything should build 
automatically. Users of other platforms can aither study the batch 
file and implement one for their own platform (and send a copy to me 
for the benefit of other users), or they can forgo the option of the 
automatic build and obtain the same result with some mouse-clicking 
in Qt Creator.

1b. Qt Creator Building
Load the following projects into Qt Creator and then hit Build All.

* UNISIM_ROOT/src/applications/unisim/unisim.pro
* UNISIM_ROOT/src/plugins/*/*.pro

The second line cannot be carried out as one operation. You must 
visit the sub-folders of the ‘plugins’ folder one by one and load 
the project file found in each.

2. You do not need to edit the configuration file ‘config.pri’ in 
UNISIM_ROOT/src but you may study it to learn how the build 
environent is set up. It is not that complicated.

3. After building UniSim you will find the UniSim executable file in
UNISIM_ROOT/src/applications and the plug-in libraries in 
UNISIM_ROOT/src/plugins

---------------------------------------
Niels Holst, Aarhus University, Denmark
[niels.holst@agrsci.dk] 
---------------------------------------

niels.holst@agrsci.dk	3/3 
