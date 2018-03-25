.. date: 25/03/2018
   author: Andre' Neto
   copyright: Copyright 2017 F4E | European Joint Undertaking for ITER and
   the Development of Fusion Energy ('Fusion for Energy').
   Licensed under the EUPL, Version 1.1 or - as soon they will be approved
   by the European Commission - subsequent versions of the EUPL (the "Licence")
   You may not use this work except in compliance with the Licence.
   You may obtain a copy of the Licence at: http://ec.europa.eu/idabc/eupl
   warning: Unless required by applicable law or agreed to in writing, 
   software distributed under the Licence is distributed on an "AS IS"
   basis, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
   or implied. See the Licence permissions and limitations under the Licence.

Building
========

Linux
-----

The MARTe development environment can be setup in any modern desktop Linux distribution.

For the sake of simplicity these instructions are shown for the CentOS distribution but should be easily adaptable to any other mainstream distribution.

If you wish to integrate with Eclispe follow :doc:`these steps </contributing/development/environment/linux>`.

Instructions
~~~~~~~~~~~~

1. Install `CentOS 7 <http://www.centos.org/download/>`_;
2. Open a terminal;

   a. Update the distribution: ``yum -y update``
   b. Make sure that the development tools are installed: ``yum groupinstall "Development tools"``

3. In the following instructions, it is assumed that this is cloned into a folder named ``MARTe2-dev``.
   
   a. Git clone of the `MARTe2 documentation infrastructure <https://vcis-gitlab.f4e.europa.eu/aneto/MARTe2-doc.git>`_;

4. ``cd MARTe2-dev``
5. ``make -f Makefile.linux``


Windows
-------

Follow :doc:`these instructions </contributing/development/environment/windows>`.


