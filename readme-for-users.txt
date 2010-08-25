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

Requirements
============

* Graphviz software (tested with 2.20)
  - Included with UniSim installation, separate installation not 
  needed.
* Currently available for Microsoft Windows only.
  - Linux and Mac OS versions are scheduled.

Acknowledgements
================

UniSim development is supported by 
* European Network for the Durable Exploitation 0f Crop Protection 
Strategies (ENDURE) 
* International Centre for Research in Organic Food Systems 
(ICROFS), Denmark
* BiosafeTrain Project of Danida, Denmark
* Aarhus University, Denmark

Installation
============

1. Download and install Universal Simulator [www.ecolmod.org].

2. To run a model with Universal Simulator.
2a. Open a model XML file (File | Open), for example any of 
the weed models in the awe folder.
2b. Run (Simulation |Run) the model

3. To edit parameter settings:
3a. Open the XML file of your choice (e.g. one of those in the 
awe folder). You can use any text editor for this, e.g. Notepad 
or Notepad++ [notepad-plus.sourceforge.net]. 
3b. Save the file.
3c. Re-open (File | Open or File | Reopen) it in UniSim and run 
the model again.
3d. The XML file can remain open in the text editor while also
being open in UniSim.

---------------------------------------
Niels Holst, Aarhus University, Denmark
[niels.holst@agrsci.dk] 
---------------------------------------

