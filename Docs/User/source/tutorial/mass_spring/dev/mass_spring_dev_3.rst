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

Interfaces
==========

The Manager can also be used to create new Interface components, including the boilerplate code for the ``Initialise`` method, along with the required Makefiles and tests.

For example, to create a new Interface component, the following command can be used:

.. code-block:: bash

    python3 -m marte2_manager.cli --project_name MARTe2-training-proj --project_path $HOME -l DEBUG add --cpt_type interfaces --cpt_name MyInterface --cpt_namespace Tutorial # Modify the project name and path if needed

This command creates a new Interface component named ``MyInterface`` in the ``Tutorial`` namespace, together with all the required Makefiles and tests.