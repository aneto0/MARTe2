.. date: 15/04/2026
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

EPICSCAClient
=============

The MARTe2 :vcisdoxygenmccl:`EPICSCAClient` allows triggering messages based on the values of EPICS Channel Access (CA) process variables (PVs).

.. warning::
    
    The EPICSCAClient is only available in distributions where `EPICS Channel Access <https://epics-controls.org/>`__ is installed.

In this example, the ``EPICSCAClient`` is used to:

- Trigger ``StateMachine`` transitions based on PV values.
- Monitor the application status by publishing state updates to EPICS.

.. literalinclude:: /_static/tutorial/Configurations/MassSpring/RTApp-MassSpring-56.cfg
    :language: c++
    :lines: 4-24
    :caption: EPICSCAClient configuration. PV names are automatically updated by ``Makefile.cfg``.
    :linenos:
    :emphasize-lines: 8,13,16-18

The ``StateMachine`` is configured to send a message every time the state changes, allowing external systems (via EPICS) to track the application status.

.. literalinclude:: /_static/tutorial/Configurations/MassSpring/RTApp-MassSpring-56.cfg
    :language: c++
    :lines: 110-111,149-163,227-241,307-319
    :caption: StateMachine configuration highlighting messages sent on state changes (starting from the ``ENTER`` substate).
    :linenos:
    :emphasize-lines: 9-10,14,24-25,29,37-38,42

Running the application
-----------------------

1. Start the ``softIOC`` in a separate terminal:

.. code-block:: bash

    ../Test/Integrated/GenerateEPICSSoftIOCDb.sh
    softIoc -d ../Test/Integrated/MassSpring-1.db

2. Start the MARTe2 application:

.. code-block:: bash

    make -C ../Configurations/MassSpring/ -f Makefile.cfg
    ./MARTeApp.sh -f ../Configurations/MassSpring/RTApp-MassSpring-56_Gen.cfg -l RealTimeLoader -m StateMachine::START

3. Verify that the application runs correctly. The log should show entries similar to:

.. code-block:: console

    $ |E=Information|TM=13:35:40 (7271154714286675)|o=StateMachine|O=0x000000000247CED0|C=StateMachine|T=|P=x86-linux|f=virtual MARTe::ErrorManagement::ErrorType MARTe::StateMachine::SendMultipleMessagesAndWaitReply(MARTe::ReferenceContainer, const MARTe::TimeoutType&)|F=StateMachine.cpp:340|D=In state (INITIAL) triggered message (StartNextStateExecutionRTApp)
    $ |E=Information|TM=13:35:40 (7271154714327563)|o=StateMachine|O=0x000000000247CED0|C=StateMachine|T=|P=x86-linux|f=virtual MARTe::ErrorMManagement::ErrorType MARTe::StateMachine::SendMultipleMessagesAndWaitReply(MARTe::ReferenceContainer, const MARTe::TimeoutType&)|F=StateMachine.cpp:340|D=In state (WAVEFORM_REF) triggered message (InformStatusUpdate)
    $ |E=Information|TM=13:35:41 (7271155716506427)|o=|O=0x0000000002540BE0|C=LoggerBroker|T=|P=x86-linux|f=virtual bool MARTe::LoggerBroker::Execute()|F=LoggerBroker.cpp:152|D=Time [0:0]:1000000
    $ |E=Information|TM=13:35:41 (7271155716512697)|o=|O=0x0000000002540BE0|C=LoggerBroker|T=|P=x86-linux|f=virtual bool MARTe::LoggerBroker::Execute()|F=LoggerBroker.cpp:152|D=ReferencePosition [0:0]:0.400000
    ...

Open another terminal and change the application state by writing to the command PV:

.. code-block:: bash

    caput MARTE2-TUTORIAL-USERNAME-APP-COMMAND 0
    caput MARTE2-TUTORIAL-USERNAME-APP-COMMAND 1

Replace ``USERNAME`` with your username in capital letters.

Monitor the application state:

.. code-block:: bash

    camonitor MARTE2-TUTORIAL-USERNAME-APP-STATUS

The PV should update (e.g. value ``1``), indicating a state change.

At the same time, the application log will reflect the new behaviour. For example, when switching to a constant reference state:

.. code-block:: console

    |E=Information|TM=13:40:35 (7271449679705588)|o=|O=0x0000000001C6DBE0|C=LoggerBroker|T=|P=x86-linux|f=virtual bool MARTe::LoggerBroker::Execute()|F=LoggerBroker.cpp:152|D=ReferencePosition [0:0]:2.000000
