.. date: 30/03/2026
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

UDPSender
=========

The :vcisdoxygenmccl:`UDPSender` ` DataSource can be used to stream application data over UDP.

Typicaly use-cases include streaming data to a remote monitoring or control application.

In this section, the ``FileWriter`` named ``FileWriterStats`` will be replaced by a ``UDPSender``.

Given that the UDP port number is extremely dependent on the system configuration, the configuration file ``../Configurations/MassSpring/RTApp-MassSpring-23-Sender.cfg`` will be automatically updated from a ``Makefile.cfg``, as explained in the section :ref:`MARTeConfigurationPreProcessing` of the documentation.

.. literalinclude:: /_static/tutorial/Configurations/MassSpring/Makefile.inc
    :language: Makefile
    :lines: 25-50
    :caption: Makefile configuration to set the UDP port number.
    :linenos:
    :emphasize-lines: 12-13

The ``Makefile`` will automatically call the script ``../Test/Integrated/FindFreeUDPPort.sh`` to find a free UDP port number, which will then be used by the ``Makefile`` to update variable ``TUTORIAL_UDP_PORT`` in the generated configuration file ``../Configurations/MassSpring/RTApp-MassSpring-23-Sender_Gen.cfg``.

.. literalinclude:: /_static/tutorial/Configurations/MassSpring/RTApp-MassSpring-23-Sender.cfg
    :language: c++
    :lines: 1198-1234
    :caption: UDPSender configuration. Note that the port number is set from the variable ``TUTORIAL_UDP_PORT``, which is replaced by the ``Makefile.cfg``.
    :linenos: 
    :emphasize-lines: 4


Running the application
-----------------------

Generate the configuration file:

.. code-block:: bash

    make -C ../Configurations/MassSpring/ -f Makefile.cfg

Note the UDP port number being used in the generated configuration file ``../Configurations/MassSpring/RTApp-MassSpring-23-Sender_Gen.cfg`` or by checking the output of the Makefile command, which should show a line similar to the following (look for the line with ``-DTUTORIAL_UDP_PORT``):

.. code-block:: bash

   gcc -x c -P -E -DTUTORIAL_UDP_PORT=42700 RTApp-MassSpring-23-Sender.cfg -o ./RTApp-MassSpring-23-Sender_Gen.cfg
   RTApp-MassSpring-23-Sender.cfg:364:26: warning: missing terminating " character
             Expression = "m=1.0;
   RTApp-MassSpring-23-Sender.cfg:370:51: warning: missing terminating " character
                           PositionM_1 = PositionM;"
   RTApp-MassSpring-23-Sender.cfg:564:26: warning: missing terminating " character
             Expression = "
   RTApp-MassSpring-23-Sender.cfg:566:77: warning: missing terminating " character
                 TriggerWriter = (uint8)(Time < (uint32)MaxAppTimeMicroSecs);"
   RTApp-MassSpring-23-Sender.cfg:582:26: warning: missing terminating " character
             Expression = "
   RTApp-MassSpring-23-Sender.cfg:585:65: warning: missing terminating " character
                 TriggerDisplay = (uint8)(Time == TruncatedTime);"

Start the application with:

.. code-block:: bash

    ./MARTeApp.sh -f ../Configurations/MassSpring/RTApp-MassSpring-23-Sender.cfg -l RealTimeLoader -s State1

Once the application is running, inspect the ``screen`` output and verify that the application is running without any issues. The log should show entries similar to the following:

.. code-block:: console

    $ [Warning - Threads.cpp:181]: Failed to change the thread priority (likely due to insufficient permissions)
    $ [Information - RealTimeLoader.cpp:111]: Started application in state State1
    $ [Information - MARTeApp.cpp:135]: Application starting
    $ [Information - LoggerBroker.cpp:152]: Time [0:0]:1000000
    ...

A python script to receive the UDP data and print it to the console is available in the file ``../Test/Integrated/UDPReceiver.py``. Run the script with:

.. code-block:: bash

    python ../Test/Integrated/udp_monitor.py -p TUTORIAL_UDP_PORT #Replace TUTORIAL_UDP_PORT with the actual port number being used by the application

Exercices
---------

Ex. 1: Stream the ``ReferencePosition`` signal over UDP. 
--------------------------------------------------------

TODO FINISH ME!!

1. Edit the file ``../Configurations/MassSpring/RTApp-MassSpring-22.cfg`` and modity the FileReader to read from the ``../Test/Integrated/MassSpring-22-Reference.csv`` file.
2. Make sure that the ``FileReader`` interpolates the data between the samples in the file and rewinds and reloops forever.
3. Check that the output file ``../Test/Integrated/MassSpring-22.csv`` is being created and that the ``ReferencePosition`` is set to the expected ramp values.

.. code-block:: bash

    ./MARTeApp.sh -f ../Configurations/MassSpring/RTApp-MassSpring-22.cfg -l RealTimeLoader -s State1

.. dropdown:: Solution
   :icon: key

   The solution is to add the relevant points to the ``../Test/Integrated/MassSpring-22-Reference.csv``.

   .. literalinclude:: /_static/tutorial/Test/Integrated/MassSpring-22-Reference-solution.csv
      :language: console
      :caption: CSV file with ``ReferencePosition`` ramp values.
      :linenos:

   Modify the ``FileReader`` DataSource to read from the new file and to interpolate the data between the samples.

   .. literalinclude:: /_static/tutorial/Configurations/MassSpring/RTApp-MassSpring-22-solution.cfg
      :language: c++
      :lines: 1243-1253
      :caption: Updated FileReader configuration to read the ramp values.
      :linenos:

   Verify that the output file ``../Test/Integrated/MassSpring-22.csv`` is being created and that the ``ReferencePosition`` is set to the expected ramp values.
