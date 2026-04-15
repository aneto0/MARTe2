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

The MARTe2 :vcisdoxygenmccl:`EPICSCAClient` allows to trigger messages based on EPICS Channel Access (CA) process variables values. 

.. warning::
    
    The EPICSCAClient is only available in distributions where `EPICS Channel Access <https://epics-controls.org/>`__ is installed.


In this example, the ``EPICSCAClient`` is used to trigger a message to send the ``StateMachine`` state as well as to monitor the application status.

.. literalinclude:: /_static/tutorial/Configurations/MassSpring/RTApp-MassSpring-56.cfg
    :language: c++
    :lines: 4-24
    :caption: EPICSCAClient configuration. Note that destination PV variable names will be automatically updated by the ``Makefile.cfg``.
    :linenos:
    :emphasize-lines: 8,13,16-18

The ``StateMachine`` is configured to send a message every time the state changes.

.. literalinclude:: /_static/tutorial/Configurations/MassSpring/RTApp-MassSpring-56.cfg
    :language: c++
    :lines: 110-111,149-163,227-241,307-319
    :caption: StateMachine configuration, highlighting the messages sent on state change (which will always start in the ``ENTER`` substate). 
    :linenos:
    :emphasize-lines: 9-10,14,24-25,29,37-38,42


Running the application
-----------------------

Start the ``softIOC`` in a separate terminal:

.. code-block:: bash

    ../Test/Integrated/GenerateEPICSSoftIOCDb.sh
    softIoc -d ../Test/Integrated/MassSpring-1.db

Start the application with:

.. code-block:: bash

    make -C ../Configurations/MassSpring/ -f Makefile.cfg
    ./MARTeApp.sh -f ../Configurations/MassSpring/RTApp-MassSpring-56_Gen.cfg -l RealTimeLoader -m StateMachine::START

Once the application is running, inspect the ``screen`` output and verify that the application is running without any issues. The log should show entries similar to the following:

.. code-block:: console

    $ |E=Information|TM=13:35:40 (7271154714286675)|o=StateMachine|O=0x000000000247CED0|C=StateMachine|T=|P=x86-linux|f=virtual MARTe::ErrorManagement::ErrorType MARTe::StateMachine::SendMultipleMessagesAndWaitReply(MARTe::ReferenceContainer, const MARTe::TimeoutType&)|F=StateMachine.cpp:340|D=In state (INITIAL) triggered message (StartNextStateExecutionRTApp)
    $ |E=Information|TM=13:35:40 (7271154714327563)|o=StateMachine|O=0x000000000247CED0|C=StateMachine|T=|P=x86-linux|f=virtual MARTe::ErrorMManagement::ErrorType MARTe::StateMachine::SendMultipleMessagesAndWaitReply(MARTe::ReferenceContainer, const MARTe::TimeoutType&)|F=StateMachine.cpp:340|D=In state (WAVEFORM_REF) triggered message (InformStatusUpdate)
    $ |E=Information|TM=13:35:41 (7271155716506427)|o=|O=0x0000000002540BE0|C=LoggerBroker|T=|P=x86-linux|f=virtual bool MARTe::LoggerBroker::Execute()|F=LoggerBroker.cpp:152|D=Time [0:0]:1000000
    $ |E=Information|TM=13:35:41 (7271155716512697)|o=|O=0x0000000002540BE0|C=LoggerBroker|T=|P=x86-linux|f=virtual bool MARTe::LoggerBroker::Execute()|F=LoggerBroker.cpp:152|D=ReferencePosition [0:0]:0.400000
    ...

On another terminal change the state of the application by writing to the corresponding PV:

.. code-block:: bash

    caput MARTE2-TUTORIAL-USERNAME-APP-COMMAND 0 #Replace with your username in capital letters
    caput MARTE2-TUTORIAL-USERNAME-APP-COMMAND 1 #Replace with your username in capital letters

Check that the state of the application has changed by monitoring the corresponding PV:

.. code-block:: bash

    camonitor MARTE2-TUTORIAL-USERNAME-APP-STATUS #Replace with your username in capital letters

Which should read ``1``. The ``ReferencePosition`` should also be updated to a constant value in the log output

.. code-block:: console

    |E=Information|TM=13:40:35 (7271449679705588)|o=|O=0x0000000001C6DBE0|C=LoggerBroker|T=|P=x86-linux|f=virtual bool MARTe::LoggerBroker::Execute()|F=LoggerBroker.cpp:152|D=ReferencePosition [0:0]:2.000000
