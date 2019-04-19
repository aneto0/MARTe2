.. date: 23/03/2018
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


Development environment for Linux
=================================

The MARTe development environment can be setup in any modern desktop Linux distribution.

For the sake of simplicity these instructions are shown for the CentOS distribution but should be easily adaptable to any other mainstream distribution.

Prerequisites
-------------

- `CentOS 7 <http://www.centos.org/download/>`_;
- `Cppcheck <http://cppcheck.sourceforge.net/>`_;
- `Doxyget <http://www.doxygen.org/>`_;
- `gcov <http://gcc.gnu.org/onlinedocs/gcc/Gcov.html>`_;
- `lcov <http://downloads.sourceforge.net/ltp/lcov-1.11.tar.gz>`_;

Configuration
-------------

1. Install the CentOS distribution;
2. Open a terminal;

   a. Update the distribution: ``yum -y update``
   b. Make sure that the development tools are installed: ``yum groupinstall "Development tools"``
   c. Install Cppcheck: ``yum install -y cppcheck``
   d. Install Doxygen: ``yum install -y doxygen``
   
3. lcov can be deployed in any folder of the operating provided that the developer has write and execution access
   
   a. ``wget http://downloads.sourceforge.net/ltp/lcov-1.11.tar.gz``
   b. ``tar zxf lcov-1.11.tar.gz``

