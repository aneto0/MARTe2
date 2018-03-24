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


Development environment for Windows
===================================

The MARTe development environment can be setup in any modern desktop Windows distribution.

For the sake of simplicity these instructions are shown for the Windows 7 distribution but should be easily adaptable to other versions.

Prerequisites
-------------

- `Visual Studio Community 2013 <https://www.visualstudio.com/products/visual-studio-community-vs>`_ or an equivalent version of Visual Studio which contains the cl.exe compiler;
- `GnuWin32 CoreUtils <http://gnuwin32.sourceforge.net/packages/coreutils.htm>`_;
- `GnuWin32 make <http://gnuwin32.sourceforge.net/packages/make.htm>`_;
- `GnuWin32 sed <http://gnuwin32.sourceforge.net/packages/sed.htm>`_;
- `Doxygen <http://gnuwin32.sourceforge.net/packages/coreutils.htm>`_;
- `MigGW <http://www.mingw.org/>`_;

Configuration
-------------

1. Add the folder which contains cl.exe (Usually in C:/Program Files (x86)/Microsoft Visual Studio .../VC/bin) to the environment PATH variable (Control Panel - System - Advanced system settings - Environment Variables);
2. Add to the PATH environment variable the IDE path (Usually in Program Files/Microsoft Visual Studio ../Common7/IDE) which contains the dll libraries for Visual Studio;
3. In the same folder of the compiler cl.exe there should be a script file called vcvars32 (or vcvars64 for the 64 bit compiler), open a console and write ``vcvars32`` setting the variables of Visual Studio environment;
4. Open a cmd console;
5. Type ``vcvars32`` on the console, followed by ``echo %PATH%`` and copy the result to the PATH environment variable (Control Panel - System...);
6. Write on the same console ``echo %INCLUDE%``, add an environment variable called INCLUDE and insert the result there (by Control Panel - System...);
7. Write on the same console ``echo %LIB%``, add an environment variable called LIB and insert the result there (by Control Panel - System...);
8. Install the GnuWin32 and MinGW downloaded above;
9. Add the path which contains the GnuWin32 programs (Usually in C:/Program Files (x86)/GnuWin32/bin) to the PATH environment variable;
10. Add the path which contains the MinGW programs (Usually in C:/Program Files (x86)/MinGW/bin) to the PATH environment variable;
11. Install Doxygen.

