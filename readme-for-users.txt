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

1. Download these two software packages:
* Universal Simulator [www.ecolmod.org]
* Graphviz [www.graphviz.org]
  - follow the download link, then select your operating system 
  (e.g. Windows), finally select the file with the current stable 
  release.

2. Install both software packages.

3. Your first run of Universal Simulator.
3a. Open any of the weed models in the ess2009 folder.
3b. Universal Simulator will ask you for the location of the 
Graphviz software. Point and click until you find the folder, for 
example: C:\Program Files\Graphviz2.20\bin.

4. Models are in various stages of development. Currently, these 
work:
* ambrosia (ambrosia development)
* conductance model (plant growth under competition)
* ess2009 (annual weed population dynamics).

5. To edit parameter settings:
5a. Open the XML file of your choice (e.g. one of those in the 
ess2009 folder). You can use any text editor for this, e.g. Notepad 
or Notepad++ [notepad-plus.sourceforge.net]. 
5b. Save the file.
5c. Open it again in UniSim before running the model with your new 
parameter settings.

---------------------------------------
Niels Holst, Aarhus University, Denmark
[niels.holst@agrsci.dk] 
---------------------------------------

