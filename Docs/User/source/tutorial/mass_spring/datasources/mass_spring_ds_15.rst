.. date: 10/04/2026
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

.. _RealTimeThreadAsyncBridgeTutorial:

RealTimeThreadAsyncBridge 
=========================

The :vcisdoxygenmccl:`RealTimeThreadAsyncBridge` DataSource allows to asynchronously collect data from RealTimeThreads. 

It is typicaly used to collect data from a RealTimeThread and offer it to a non-real-time thread, for example for logging purposes. The data is collected asynchronously, which means that the RealTimeThread is not blocked while the data is being collected. The data is then offered to the non-real-time thread with a certain frequency (e.g. 100 Hz).

.. note::

    Many DataSource components already offer a decoupling mechanism to collect data asynchronously, for example the :vcisdoxygenmccl:`FileWriterGAM`.

.. important::

    Since the data is collected asynchronously, it is good practice to use techniques that mitigate aliasing, such as latching, tracking minimum and maximum values, or collecting histograms. 

In this example a monitor thread asynchronously logs to the screen at a frequency of 1 Hz, without impacting the performance of the real-time thread, which is running at 100 Hz. 

.. literalinclude:: /_static/tutorial/Configurations/MassSpring/RTApp-MassSpring-49.cfg
    :language: c++
    :lines: 791-810
    :caption: Thread configuration. Note that the ``CPUs`` could have been allocated to different cores.
    :linenos: 
    :emphasize-lines: 7, 10, 12, 15

.. literalinclude:: /_static/tutorial/Configurations/MassSpring/RTApp-MassSpring-49.cfg
    :language: c++
    :lines: 763-765
    :caption: The ``RealTimeThreadAsyncBridge`` expects one thread to write data to it and one or more threads to read data from it.
    :linenos: 

.. literalinclude:: /_static/tutorial/Configurations/MassSpring/RTApp-MassSpring-49.cfg
    :language: c++
    :lines: 664-675,706-715
    :caption: The ``GAMWriter`` writes data into the ``RTThreadAsynch``.
    :linenos: 
    :emphasize-lines: 15,19

.. literalinclude:: /_static/tutorial/Configurations/MassSpring/RTApp-MassSpring-49.cfg
    :language: c++
    :lines: 581-592,623-632
    :caption: The ``GAMDisplay`` reads data directly from into the ``RTThreadAsynch``. Note that the ``GAMDisplay`` is executed by the ``ThreadMonitor``, which is running at 1 Hz, while the ``GAMWriter`` is executed by the ``ThreadProcess``, which is running at 100 Hz.
    :linenos: 
    :emphasize-lines: 5,9

Running the application
-----------------------

Start the receiver application with:

.. code-block:: bash

    ./MARTeApp.sh -f ../Configurations/MassSpring/RTApp-MassSpring-49.cfg -l RealTimeLoader -s State1

Once the application is running, inspect the ``screen`` output and verify that the application is running without any issues. The log should show entries similar to the following:

.. code-block:: console

    $ [Warning - Threads.cpp:185]: Failed to change the thread priority (likely due to insufficient permissions)                                 [0/1887]
    $ [Warning - Threads.cpp:173]: Requested a thread priority that is higher than the one supported by the selected policy - clipping to the maximum value supported by the policy.
    $ [Warning - Threads.cpp:181]: Failed to change the thread priority (likely due to insufficient permissions)
    $ [Information - RealTimeLoader.cpp:111]: Started application in state State1
    $ [Information - MARTeApp.cpp:135]: Application starting
    $ [Information - LoggerBroker.cpp:152]: Time [0:0]:1000000
    $ [Information - LoggerBroker.cpp:152]: Thread1CycleTimeAverage [0:0]:9999
    $ [Information - LoggerBroker.cpp:152]: Thread1CycleTimeMovingAverage [0:0]:9999.400138
    $ [Information - LoggerBroker.cpp:152]: Thread1CycleTimeMax [0:0]:10974
    $ [Information - LoggerBroker.cpp:152]: Thread1CycleTimeMin [0:0]:9027
    $ [Information - LoggerBroker.cpp:152]: Thread1CycleTimeHistogram [0:10]:{ 0 0 0 0 0 0 0 0 0 0 0 }
    $ [Information - LoggerBroker.cpp:152]: Thread1FreeTimeHistogram [0:10]:{ 0 0 0 0 0 0 0 0 0 0 0 }
    $ [Information - LoggerBroker.cpp:152]: ForceMax [0:0]:30.000000
    $ [Information - LoggerBroker.cpp:152]: ForceMin [0:0]:3.420744
    $ [Information - LoggerBroker.cpp:152]: Time [0:0]:2000000
    $ [Information - LoggerBroker.cpp:152]: Thread1CycleTimeAverage [0:0]:9999
    $ [Information - LoggerBroker.cpp:152]: Thread1CycleTimeMovingAverage [0:0]:9999.599456
    $ [Information - LoggerBroker.cpp:152]: Thread1CycleTimeMax [0:0]:10889
    $ [Information - LoggerBroker.cpp:152]: Thread1CycleTimeMin [0:0]:9112
    $ [Information - LoggerBroker.cpp:152]: Thread1CycleTimeHistogram [0:10]:{ 0 0 0 0 0 0 0 0 0 0 0 }
    $ [Information - LoggerBroker.cpp:152]: Thread1FreeTimeHistogram [0:10]:{ 0 0 0 0 0 0 0 0 0 0 0 }
    $ [Information - LoggerBroker.cpp:152]: ForceMax [0:0]:30.000000
    $ [Information - LoggerBroker.cpp:152]: ForceMin [0:0]:3.420744

