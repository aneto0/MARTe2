.. date: 17/04/2026
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

Development of new components
=============================

In this section, the development of new components will be covered.

.. important::

   The examples provided in this section are meant to be used as a reference for the development of new components. They are not meant to be used as templates for new applications, as they do not necessarily follow the best practices for application development. This is particularly true for the DataSources, which are implemented in a very basic way, without semaphores or mutexes to protect the shared data.

   The developer is strongly encouraged to read the MARTe2 documentation and to use the API, particularly with regard to low-level primitives, as this allows porting the components to other platforms and using the full potential of the framework.

The application structure is the same discussed in the :doc:`deploying section </deploying/deploying>`.

In this tutorial, the MARTe2 command line Manager will be used to create the project and the components. The Manager is a command line tool that allows managing the development of MARTe2 applications. It can be used to deploy a new project, add a new component (including the boilerplate code) and its corresponding tests and makefiles, and modify an existing component (including the boilerplate code).

.. note::

   If the manager cannot be installed on the system, clone the MARTe2 repository, change to the folder ``MARTe2/Resources/Manager`` and run ``export PYTHONPATH=$PYTHONPATH:src``.

Check that the manager is correctly installed by running the command ``python3 -m marte2_manager.cli -h``. This should print the help message of the manager.

.. toctree::
   :maxdepth: 1
   :caption: Contents:

   mass_spring_dev_1