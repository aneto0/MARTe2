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

For the sake of simplicity these instructions are shown for the Windows 11 distribution but should be easily adaptable to other versions, as they simply rely on the Microsoft toolchain.
Shown instructions and backing code also rely on the 64 bit version of Windows and toolset. Software versions which are shown represent the tested ones, expect changes if you install
different revisions.

Prerequisites
-------------

- `Visual Studio Community 2022 <https://visualstudio.microsoft.com/it/vs/community/>`_ or an equivalent version of Visual Studio which contains the cl.exe compiler;
- `MigGW <https://www.mingw-w64.org/>`_;
- `GnuWin32  <https://gnuwin32.sourceforge.net/>`_;

Configuration
-------------
Note: match the given paths with your local installation.

1. Locate the 'x64 Native Tools Command Prompt for VS 2022' start menu icon, under the Visual Studio group item. Alternatively, run the ``vcvars64.bat`` script located under the Visual studio directory.
Note that the suite also includes other flavours of the command prompt, which essentially are needed when you have to compile from/to different targets. Ensure that the one you are running matches the intended
that is x64 host machine and x64 host target.
2. Add GnuWin32 bin directory to the PATH environment variable. You could do that locally (volatile) by issuing ``SET PATH=%PATH%;C:\Program Files (x86)\GnuWin32\bin\`` or permanently using the
Windows Control Panel.
3. Add MinGW64 bin directory to the PATH environment variable. Same as above applies (e.g. ``SET PATH=%PATH%;C:\mingw64\bin``)

Once the environment is ready, with above described steps, and you have the MARTe2 repository cloned locally, you can run
``make -f Makefile.msc``
to start the MARTe2 suite compilation.

Artifacts
-------------
Once the MARTe2 suite is successfully compiled, you will have a ``Build/msc`` subdirectory created which contains:
- Core: The MARTe2 framework static and dynamic libraries. Static libraries are under the Core subdirectory, dynamic link libraries are under the Core/dll subdirectory.
You will need the MARTe2.dll (dynamic) to run MARTeApp and MARTe2.lib under Core/dll to compile, link and run against MARTe2. Specifically, the .lib file under Core/dll is an import library,
which is needed when dynamically linking to the MARTe2 framework exported symbols. Conversely, if you plan to statically compile against MARTe2, use the .lib directly under Core. Note that the 
two files, despite having the same extension, are different;
- App: The MARTeApp executable. MARTeApp executable needs the MARTe2.dll to be reachable from the PATH environment. Moreover, if you plan using MARTe2-components, 
you will also need to have each component DLL in the PATH environment;
- GTest/Portable: These two directories contain the MARTe2 test suite, compiled in the Portable version, which is an extremely simplified GTest environment.
- Examples: Contains the MARTe2 examples.

Caveats
-------------
When compiling for Windows with MSVC suite, there are two methods to import/export public symbols from a library. MARTe2 relies on the __declspec(dllimport)/__declspec(dllexport) keywords.
The ``DLL_API`` symbol is defined in order to behave 'Polymorphically', as export in headers and as import in sources using them. All the exported symbols are marked with that specific define.
This implies that, if you plan to write your own custom MARTe2 framework based application or component, you must take into account that and add, accordingly, the needed DLL_API marker in your sources
to avoid wrong interpretations of the dllimport/dllexport symbols.
