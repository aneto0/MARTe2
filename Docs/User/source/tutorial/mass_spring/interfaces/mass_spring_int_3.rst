.. date: 14/04/2026
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

LoggerService
=============

The MARTe2 :vcisdoxygencl:`LoggerService` (see also :ref:`MARTeLogging`) enables to decouple (real-time safe) and propagate the logging messages to different consumers.

Examples of LoggerService consumers include the :vcisdoxygencl:`ConsoleLogger` (which prints the log messages on the console), the :vcisdoxygencl:`UDPLogger` (which sends the log messages over UDP) and the :vcisdoxygenmccl:`SysLogger` (which sends the log messages to the machine ``syslog``).

All the :vcisdoxygencl:`LoggerConsumerI` accept a ``Format`` string which specifies what log properties are to be included in the log message.

In this example two LoggerConsumers are used: the ``ConsoleLogger`` and the ``UDPLogger``.  In order to avoid mixing messages from different applications the destination  ``UDP`` port number needs to be unique. In order to avoid clashes,  the configuration file ``../Configurations/MassSpring/RTApp-MassSpring-55.cfg`` will be automatically updated from a ``Makefile.cfg``.

.. literalinclude:: /_static/tutorial/Configurations/MassSpring/RTApp-MassSpring-55.cfg
    :language: c++
    :lines: 4-19
    :caption: LoggerService configuration. Note that destination UDP port will be automatically updated by the ``Makefile.cfg``.
    :linenos:
    :emphasize-lines: 6, 8, 13

The tools to handle the reception of the log messages are outside the scope of this tutorial, but a simple example of how to receive the log messages using the `marte2-log <https://vcis-gitlab.f4e.europa.eu/common/marte2-logview>`__ application is shown below.

Running the application
-----------------------

Start the first application with:

.. code-block:: bash

    make -C ../Configurations/MassSpring/ -f Makefile.cfg
    ./MARTeApp.sh -f ../Configurations/MassSpring/RTApp-MassSpring-55_Gen.cfg -l RealTimeLoader -m StateMachine::START

Once the application is running, inspect the ``screen`` output and verify that the application is running without any issues. The log should show entries similar to the following:

.. code-block:: console

    $ |E=Warning|TM=16:55:55 (7196770772734895)|o=|O=|C=|T=|P=x86-linux|f=void MARTe::Threads::SetPriority(const ThreadIdentifier&, const MARTe::Threads::PriorityClassType&, const uint8&)|F=Threads.cpp:185|D=Failed to change the thread priority (likely due to insufficient permissions)
    $ |E=Information|TM=16:55:55 (7196770772750327)|o=StateMachine|O=0x0000000001D490F0|C=StateMachine|T=|P=x86-linux|f=virtual MARTe::ErrorManagement::ErrorType MARTe::StateMachine::SendMultipleMessagesAndWaitReply(MARTe::ReferenceContainer, const MARTe::TimeoutType&)|F=StateMachine.cpp:340|D=In state (INITIAL) triggered message (StartNextStateExecutionRTApp)
    $ |E=Information|TM=16:55:56 (7196771773805898)|o=|O=0x0000000001DFCAB0|C=LoggerBroker|T=|P=x86-linux|f=virtual bool MARTe::LoggerBroker::Execute()|F=LoggerBroker.cpp:152|D=Time [0:0]:990000
    ...

1. Open the file ``../Configurations/MassSpring/RTApp-MassSpring-55_Gen.cfg`` and take note of the allocated ``UDPLogger`` ``Port`` number. (Alternatively, run the command ``UDP_LOGGER_DEST_PORT=`awk '/\+UDPLogger/,/}/ {if ($1=="Port") print $3}' ../Configurations/MassSpring/RTApp-MassSpring-55_Gen.cfg`;echo "UDP_LOGGER_DEST_PORT=$UDP_LOGGER_DEST_PORT"`` to extract the port number from the configuration file).:
2. Clone the `marte2-log <https://vcis-gitlab.f4e.europa.eu/common/marte2-logview>`__ application and start it with the same ``UDP`` port number as the one allocated to the ``UDPLogger`` in the configuration file:
3. Use the following command to find an available port number:

.. code-block:: bash

    LOG_HTTP_PORT=`PORT=8080;while netstat -tln | grep -q ":$PORT"; do PORT=$((PORT + 1)); done; echo $PORT
    echo $LOG_HTTP_PORT

4. Start the ``marte2-log`` application in web mode with the following command:

.. code-block:: bash

    python3.6 logapp.py -t WS -p $UDP_LOGGER_DEST_PORT -r $LOG_HTTP_PORT #Replace the $UDP_LOGGER_DEST_PORT with the port number allocated to the UDPLogger in the configuration file

5. Open a web browser and navigate to ``http://localhost:$LOG_HTTP_PORT/index.html`` (replace the $LOG_HTTP_PORT with the port number allocated in the previous step) to view the log messages in real-time.
