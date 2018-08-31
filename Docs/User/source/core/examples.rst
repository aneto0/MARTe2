.. date: 27/03/2018
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

Examples
========

To run the examples follow theses instructions:

.. code-block:: bash
   
   export MARTe2_DIR=FOLDER_WHERE_MARTe2_IS_INSTALLED
   cd $MARTe2_DIR/Docs/User/source/_static/examples/Core
   make -f Makefile.x86-linux
   export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MARTe2_DIR/Build/x86-linux/Core/
   
Standalone examples are run with:

.. code-block:: bash

   ../../../../../../Build/x86-linux/Examples/Core/EXAMPLE_TO_RUN.ex
   
Examples which are related to the real-time execution of a configuration file and which are provided with a given start-up **State** are launched :

.. code-block:: bash
   
   export MARTe2_DIR=FOLDER_WHERE_MARTe2_IS_INSTALLED
   export MARTe2_Components_DIR=FOLDER_WHERE_MARTe2_Components_IS_INSTALLED
   cd $MARTe2_DIR/Docs/User/source/_static/examples
   ./MARTeApp.sh -l RealTimeLoader -s NAME_OF_THE_STATE -f Configurations/NAME_OF_THE_CONFIGURATION_FILE

Examples which are related to the real-time execution of a configuration file and which are provided with a given start-up **Message** are launched :

.. code-block:: bash
   
   export MARTe2_DIR=FOLDER_WHERE_MARTe2_IS_INSTALLED
   export MARTe2_Components_DIR=FOLDER_WHERE_MARTe2_Components_IS_INSTALLED
   cd $MARTe2_DIR/Docs/User/source/_static/examples
   ./MARTeApp.sh -l RealTimeLoader -m MESSAGE -f Configurations/NAME_OF_THE_CONFIGURATION_FILE