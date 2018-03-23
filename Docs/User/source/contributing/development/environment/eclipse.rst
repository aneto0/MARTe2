.. date: 23/04/2018
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

.. |rarrow| unicode:: U+2192

Eclipse configuration
=====================

Eclipse is the preferred tool for the development of source code related to the MARTe project. In particular it promotes and guides the implementation of the framework coding standard and rules.

Prerequisites
-------------

- `Java Runtime Environment <http://www.oracle.com/technetwork/java/javase/downloads/index.html>`_;
- `Eclipse IDE for C/C++ Developers <http://www.eclipse.org/downloads/>`_;
- Git clone of the `MARTe2 development infrastructure <https://vcis-gitlab.f4e.europa.eu/aneto/MARTe2.git>`_;

   - In the following instructions, it is assumed that this is cloned into a folder named ``MARTe2-dev``.
- Git clone of the `MARTe2 documentation infrastructure <https://vcis-gitlab.f4e.europa.eu/aneto/MARTe2-doc.git>`_;
   
    - In the following instructions, it is assumed that this is cloned into a folder named ``MARTe2-doc``.

Configuration
-------------

1. Open Eclipse;
2. Click on **File** |rarrow| **New** |rarrow| **Makefile Project with existing code**;

  I. In the section **Toolchain for Indexer Setting**:

    a. Select **<none>** if you are developing Microsoft Windows;
    b. Select **Linux GCC** if you are developing in Linux;

  II. Write **MARTe2-dev** as the Project Name;
  III. Point the Existing Code Location to the ``MARTe2-dev`` directory (the one cloned before);
  IV. Press **Finish**;

3. Open **Window** |rarrow| **Preferences**;

  I. Select **C/C++** |rarrow| **Code Style** |rarrow| **Code Templates** and click on **Import**;

    a. Select the file ``MARTe2-doc/Assets/Configuration/marte_cpp_code_templates.xml``.

  II. Select **C/C++** |rarrow| **Code Style** |rarrow| **Formatter** and click on **Import**;

    a. Select the file ``MARTe2-doc/Assets/Configuration/marte_cpp_formatting_rules.xml``.

  III. Select **C/C++** |rarrow| **Editor** |rarrow| **Templates** and click on **Import**;

    a. Select the file ``MARTe2-doc/Assets/Configuration/marte_cpp_editor_templates.xml``.
    b. Select the "User code formatter" option.

  IV. Select **General** |rarrow| **Editor** |rarrow| **Text Editors** |rarrow| *Spelling**;

    a. Make sure to select **first** the option ``C/C++ spelling engine`` in the **Select spelling engine to use** drop-box;
    b. In **User defined dictionary** click on **Browse...** and select the file ``.dictionary`` which is the root folder of the ``MARTe2-dev`` repository (in Linux this file may not be visible in the dialog and consequently its path needs to be explicitly written in the text field).

  V. Click OK to close the Preferences window.

4. Open **Project** |rarrow| **Properties**;

  I. Select **C/C++ Build**;

    a. Unselect **Use default built command**;
    b. In the **Build command** text field:
    
      1. Write ``make -f Makefile.msc`` if you are developing in Microsoft Windows;

      .. image:: eclipse-configuration-2.png

      2. Write ``make -f Makefile.linux`` if you are developing in Linux;

    c. If you are developing in Microsoft Windows:

      1. Open a Windows console (cmd.exe) and type ``vcvars32`` if you are compiling in 32 bits or ``vcvars64`` if you are compiling in 64 bits;
      2. Go back to the eclipse project properties (but leave the Windows console opened);
      3. Select **Environment** (under **C/C++ Build** in the tree)
      4. Click on **Add**
        
        a. Write ``INCLUDE`` in the **Name** text field;
        b. In the Windows console type ``echo %INCLUDE%``
        c. Write the output of the previous command in the **VALUE** text field;
        d. Select **Add to all configurations**;

      5. Click on **Add**

        a. Write ``LIB`` in the **Name** text field;
        b. In the Windows console type ``echo %LIB%``
        c. Write the output of the previous command in the **VALUE** text field;
        d. Select **Add to all configurations**;

      6. Click on **Add**

        a. Write ``PATH`` in the **Name** text field;
        b. In the Windows console type ``echo %PATH%``
        c. Write the output of the previous command in the **VALUE** text field;
        d. Select **Add to all configurations**;

      7. Click on **Apply**;
      8. Close the Windows console;

