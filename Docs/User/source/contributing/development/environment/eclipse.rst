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

.. |rarrow| unicode:: U+2192

Eclipse configuration
=====================

Eclipse is the preferred tool for the development of source code related to the MARTe project. In particular it promotes and guides the implementation of the framework coding standard and rules.

Prerequisites
-------------

- `Java Runtime Environment <http://www.oracle.com/technetwork/java/javase/downloads/index.html>`_;
- `Eclipse IDE for C/C++ Developers <http://www.eclipse.org/downloads/>`_; (version used for this tutorial: Oxygen.3 Release (4.7.3))
- Git clone of the `MARTe2 development infrastructure <https://vcis-gitlab.f4e.europa.eu/aneto/MARTe2.git>`_;

   - In the following instructions, it is assumed that this is cloned into a folder named ``MARTe2-dev``.

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

      a. Select the file ``Docs/Assets/Configuration/marte_cpp_code_templates.xml``.

   II. Select **C/C++** |rarrow| **Code Style** |rarrow| **Formatter** and click on **Import**;

      a. Select the file ``Docs/Assets/Configuration/marte_cpp_formatting_rules.xml``.

   III. Select **C/C++** |rarrow| **Editor** |rarrow| **Templates** and click on **Import**;

      a. Select the file ``Docs/Assets/Configuration/marte_cpp_editor_templates.xml``.
      b. Select the "Use code formatter" option.
      
      .. image:: C_CppEditorTemplatesImport.png

   IV. Select **General** |rarrow| **Editor** |rarrow| **Text Editors** |rarrow| *Spelling**;

      a. Make sure to select **first** the option ``Enable spell checking``;
      b. In **User defined dictionary** click on **Browse...** and select the file ``.dictionary`` which is the root folder of the ``MARTe2-dev`` repository (in Linux this file may not be visible in the dialog and consequently its path needs to be explicitly written in the text field).
	  
	  .. image:: GeneralEditorsTextEditorsSpellin.png
   
   V. Click **Apply and Close** to close the Preferences window.

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
         
   II. Select **C/C++ General** |rarrow| **Paths and Symbols**;

      a. Select the **# Symbols** tab;
      
         1. Select **[ All configurations ]** in the **Configuration** list;
         2. Select **GNU C** in the languages list;
         3. Click on **Add**:

            a. Write ``ARCHITECTURE`` in the **Name** text field;
            b. Write ``x86_cl`` in the **Value** text field if you are developing in Microsoft Windows;
            c. Write ``x86_gcc`` in the **Value** text field if you are developing in Linux;
            
            .. image:: eclipse-configuration-3.png
            
            d. Select **Add to all configurations**;
            e. Select **Add to all languages**;
            f. Click **OK**;
         
         4. Click on **Add**:
   
            a. Write ``ENVIRONMENT`` in the **Name** text field;
            b. Write ``Windows`` in the **Value** text field if you are developing in Microsoft Windows;
            c. Write ``Linux`` in the **Value** text field if you are developing in Linux;
            d. Select **Add to all configurations**;
            e. Select **Add to all languages**;
            f. Click **OK**;
         
   III. Click **Apply and Close** to close the Properties window.
   
5. Compile the project by selecting **Project** |rarrow| **Build All**;
6. Select **Help** |rarrow| **Install New Software**;

   I. Select **Work with** - ``-All available sites-``;
   II. Select **Programming Languages** |rarrow| **C/C++ Unit Testing Support**;
   III. Click on **Next** and follow the instructions;
   
   .. image:: HelpInstallNewSoftwareUniteTest.png
   
7. Open **Run** |rarrow| **Run Configurations**;

   I. Select the **C/C++ Unit** on the left pane;
   II. Press the **New launch configuration button** (on the top left side);
   
   .. image:: RunConfigurationC_CppUnitNew.png
   
   III. Select the **Main** tab;
   
      1. Write ``MARTe2 GTest`` in the **Name** text field;
      2. Write ``MARTe2-dev`` in the **Project** text field;
      3. In the **C/C++ Application** text field:
      
         a. Write ``Build\msc\GTest\MainGTest.exe`` if you are developing in Microsoft Windows;
         b. Write ``Build/x86-linux/GTest/MainGTest.ex`` if you are developing in Linux;
         
        .. image:: RunConfigurationsC_CppUnitTestCreation.png
         
      4. In the **Build configuration** select **Default**;

   IV. Select the **C/C++ testing** tab;
   
      1. In the **Tests Runner** drop-box, select ``Google Tests Runner``;

   V. Select the **Common** tab;
   
      1. Select ``Run`` in the **Display in favorites** menu pane;
      
   VI. If you are developing Microsoft Windows:

      1. Select the **Environment** tab;
      
         a. Click on **New...**;
         
            1. In the **Name** text field write ``PATH``;
            2. In the **Value** text field write ``${workspace_loc:/MARTe2-dev}/Build/msc/Core``

         b. Make sure that the radio button **Append environment to native environment** is selected;

   VII. Click on **Apply**;
   VIII. Click on **Run** to check if everything was setup properly and then **Close**.

8. Select **Help** |rarrow| **Install New Software**;

   I. Click on the **Add..** button;
      
      a. In the **Name** text field write ``Eclox``;
      b. In the **Location** text field write: ``http://anb0s.github.io/eclox``;
   
   II. In the **Work with** select ``Eclox - http://anb0s.github.io/eclox``;
   III. Select ``eclox 0.12`` and ``Eclox Doxygen`` in the menu;
   IV. Click on **Next** and follow the instructions to install the pulg-in.
   
      .. image:: InstallEclox.png

9. On the toolbar click in the @ sign and select **Build Doxyfile**.

   I. Select the **MARTe2-dev** |rarrow| **Doxyfile**;
   II. Click **OK**;
   III. Verify that html files are created in ``MARTe2-dev/Build/dox/html`` directory.

10. Select **Help** |rarrow| **Eclipse MarketPlace**.

   I. Search for ``cppcheclipse`` and install it following the instructions.

   
Coverage
~~~~~~~~

Linux developers can integrate the coverage tool (lcov) with-in the eclipse development environment. 
It should be noted that the gcov integration delivered with the `Linux Tools - Gcov Support <https://eclipse.org/linuxtools/projectPages/gcov/>`_ does not work well with the folder structure of the MARTe2 project.
   
1. Open **Run** |rarrow| **External Tools** |rarrow| **External Tools Configurations...**;

   I. Click **New launch configuration**
   II. In the **Name** text field write: ``MARTe2 lcov``
   III. Select the **Main** tab:
      
      a. In the **Location** text field write: ``${workspace_loc:/MARTe2-dev/EclipseLcov.sh}``
      b. In the **Working Directory** text field write: ``${workspace_loc:/MARTe2-dev}``

   IV. Select the **Refresh** tab:
   
      a. Select **Refresh resources upon completion**.
      b. Select **The project containing the selected resources**
      c. Mark **Recursively include sub-folders**
      
   V. Select the **Build** tab:

      a. Unselect **Build before launch**

   VI. Select the **Common** tab:

      a. Select **External Tools** in the **Display in favorites menu** pane.

   VII. Click **Apply**;
   VIII. Click **Run**.

The coverage html will be generated in a folder named cov_html under the Build folder. Open the index.html file to browse the results.

   .. image:: eclipse-configuration-8.png
   
Linting
~~~~~~~

Core developers in the F4E premises can configure the linting procedure as follows:

1. Make sure that you can login using SSH to the lint server (defined with the variable HOST) using public key authentication (i.e. without a password);

2. Open **Project** |rarrow| **Build Configurations** |rarrow| **Manage**;

   I. Click **New**;
   II. In the **Name** text field write ``MARTe2 linting``;
   III. In the **Description** text field write ``MARTe2 linting procedure``;
   IV. Select **Default** in the **Copy settings from** |rarrow| **Existing configuration** list;
   V. Click **OK**;
   
3. Open **Project** |rarrow| **Properties**;

   I. Select **C/C++ Build**
   
      a. Select ``MARTe2 linting`` in the **Configuration** list;
      b. Unselect **Use default built command**;
      c. In the **Build command** text field:

         + Write the full path to the ``EclipseLint.bat`` script if you are developing in Microsoft Windows;
         + Write the full path to the ``EclipseLint.sh`` script if you are developing in Linux;
         
         .. image:: eclipse-configuration-9.png
         
      d. Unselect Generate Makefiles automatically;
      e. In the **Build** directory text field write ``${workspace_loc:/MARTe2-dev}``
      f. In the **Behavior** tab:

         1. Unselect **Stop on first build error**;
         2. Unselect **Clean**;

      g. Click **Apply and close**.

Tips and tricks
---------------

- To format code, click on the file that you want format and then select **Source** |rarrow| **Format**.
- The html generated by eclox will be available in a folder named html under the **Assets** |rarrow| **Configuration** folder.
- To run CppCheck, open the file that you want to check and do right click |rarrow| **cppcheck** |rarrow| **Run cppcheck**.

   .. image:: eclipse-configuration-10.png