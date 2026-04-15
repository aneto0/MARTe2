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

.. _RealTimeThreadSynchronisationTutorial:

RealTimeThreadSynchronisation
=============================

The :vcisdoxygenmccl:`RealTimeThreadSynchronisation` DataSource allows to synchronise multiple RealTimeThreads.

Typicaly use-cases include (see also the section on :ref:`MARTeMultipleThreads`):

- Sharing of workload between multiple threads.
- Synchronisation of threads with different execution frequencies (e.g. a thread executing at 1 kHz and another thread executing at 10 kHz, where the 1 kHz thread needs to be synchronised with the 10 kHz thread).

In this example one thread is used to receive data from a UDP stream and to synchronise the execution of the RealTimeThread with the arrival of UDP packets. This data will then be offered to a separate thread which will be responsible for processing the data at 100 Hz and store it in a file.

.. literalinclude:: /_static/tutorial/Configurations/MassSpring/RTApp-MassSpring-47-Receiver.cfg
    :language: c++
    :lines: 1259-1277
    :caption: Thread configuration. Note that the ``CPUs`` could have been allocated to different cores.
    :linenos: 
    :emphasize-lines: 7, 10, 12, 15

.. literalinclude:: /_static/tutorial/Configurations/MassSpring/RTApp-MassSpring-47-Receiver.cfg
    :language: c++
    :lines: 1254-1257
    :caption: The ``RealTimeThreadSynchronisation`` expects one thread to write data to it and one or more threads to read data from it.
    :linenos: 

.. literalinclude:: /_static/tutorial/Configurations/MassSpring/RTApp-MassSpring-47-Receiver.cfg
    :language: c++
    :lines: 54-92
    :caption: The ``GAMThreadSyncOut`` writes data into the ``RTThreadSynch``.
    :linenos: 
    :emphasize-lines: 23

.. literalinclude:: /_static/tutorial/Configurations/MassSpring/RTApp-MassSpring-47-Receiver.cfg
    :language: c++
    :lines: 93-138
    :caption: The ``GAMThreadSyncIn`` reads data into the ``RTThreadSynch``. Note that by requesting ``Samples=10`` the GAM will read 10 samples from the ``RTThreadSynch`` and will only execute when 10 samples are available, thus lowering to process data at a lower frequency (e.g. 100 Hz). The 10 collected samples will then be offered as an array of 10 elements.
    :linenos: 
    :emphasize-lines: 7,12,17,22,29,34,39,44

.. literalinclude:: /_static/tutorial/Configurations/MassSpring/RTApp-MassSpring-47-Receiver.cfg
    :language: c++
    :lines: 140-186
    :caption: The data needs to downsampled. Ideally, it should have been decimated with a proper filter, but for simplicity in this example the data is just downsampled by taking one sample every 10.
    :linenos: 
    :emphasize-lines: 8,14,20,26

Running the application
-----------------------

Start the receiver application with:

.. code-block:: bash

    make -C ../Configurations/MassSpring/ -f Makefile.cfg
    ./MARTeApp.sh -f ../Configurations/MassSpring/RTApp-MassSpring-47-Receiver_Gen.cfg -l RealTimeLoader -s State1

Once the application is running, inspect the ``screen`` output and verify that the application is running without any issues. The log should show entries similar to the following:

.. code-block:: console

    $ [Warning - Threads.cpp:181]: Failed to change the thread priority (likely due to insufficient permissions)
    $ [Information - RealTimeLoader.cpp:111]: Started application in state State1
    $ [Information - MARTeApp.cpp:135]: Application starting

.. note::

    The ``RTThreadSynch`` is configured with a timeout of 5 seconds, which means that if the sender application is not started within 5 seconds, the receiver application will log a warning message and will continue to wait for data to arrive. 

    .. code-block:: console

        $ [Warning - GAMSchedulerI.cpp:429]: BrokerI GAMThreadSyncIn.InputBroker.RealTimeThreadSynchBroker failed, owner function: GAMThreadSyncIn, owner DataSource: RTThreadSynch
        $ [FatalError - GAMScheduler.cpp:233]: Failed to ExecuteSingleCycle().
        $ [Warning - GAMSchedulerI.cpp:429]: BrokerI GAMThreadSyncI

Open another terminal and start the sender application (which will be sending at 1 kHz) with:

.. code-block:: bash

    ./MARTeApp.sh -f ../Configurations/MassSpring/RTApp-MassSpring-47-Sender_Gen.cfg -l RealTimeLoader -s State1

Once the application is running, inspect the ``screen`` output and verify that the application is running without any issues. The log should show entries similar to the following:

.. code-block:: console

    $ [Warning - Threads.cpp:181]: Failed to change the thread priority (likely due to insufficient permissions)
    $ [Information - RealTimeLoader.cpp:111]: Started application in state State1
    $ [Information - MARTeApp.cpp:135]: Application starting
    $ [Information - LoggerBroker.cpp:152]: Time [0:0]:0
    $ [Information - LoggerBroker.cpp:152]: Time [0:0]:1000000
    ...

Wait some seconds and inspect the collected data:

.. code-block:: bash

    python ../Test/Integrated/plot_mass_spring_csv.py -f ../Test/Integrated/MassSpring-47.csv -s ReferencePosition Position
    python ../Test/Integrated/plot_mass_spring_csv.py -f ../Test/Integrated/MassSpring-47.csv -s ThreadReceiverCycleTime ThreadProcessCycleTime

Check that the cycle-times are 1000 microseconds for the receiver thread and 10000 microseconds for the processing thread, which confirms that the synchronisation is working correctly.

Exercises
---------

Ex. 1: Running two threads with different models
------------------------------------------------

The objective of this exercice is to synchronise two threads with the same data producer. They will both run at the same frequency (100 Hz) but will implement different models for the mass-spring system (as shown in previous examples).

This use-case could be used to compare the performance of two different models and implement an 1oo2 mechanism to switch between the two models in case of a failure.

1. Edit the file ``../Configurations/MassSpring/RTApp-MassSpring-48-Receiver.cfg`` and add an IOGAM named ``GAMThreadSyncIn2``. This GAM shall read the signals ``Counter``, ``Time``, ``ReferencePosition`` and ``ThreadReceiverCycleTime`` from the ``RTThreadSynch`` DataSource, with 10 samples each, and offer them to the rest of the RealTimeThread as arrays of 10 elements. 
2. Add another thread to the ``RealTimeState`` and name it ``+ThreadProcess2``. Make sure that the following GAMs are executed in the correct order: ``GAMPerfMonitorProcess2``, ``GAMThreadSyncIn2``, ``GAMDownsample2``, ``GAMController2``, ``GAMMathModel``,  ``GAMMathTrigger2`` and ``GAMWriter2``.
3. Run ``make -C ../Configurations/MassSpring/ -f Makefile.cfg`` to generate the configuration file with the correct UDP port numbers.

.. code-block:: bash

    make -C ../Configurations/MassSpring/ -f Makefile.cfg
    ./MARTeApp.sh -f ../Configurations/MassSpring/RTApp-MassSpring-48-Receiver_Gen.cfg -l RealTimeLoader -s State1

4. Open another terminal and start the sender application (which will be sending at 1 kHz) with:

.. code-block:: bash

    ./MARTeApp.sh -f ../Configurations/MassSpring/RTApp-MassSpring-47-Sender_Gen.cfg -l RealTimeLoader -s State1

Wait some seconds and inspect the collected data:

.. code-block:: bash

    python ../Test/Integrated/plot_mass_spring_csv.py -f ../Test/Integrated/MassSpring-48-1.csv -s ReferencePosition Position
    python ../Test/Integrated/plot_mass_spring_csv.py -f ../Test/Integrated/MassSpring-48-1.csv -s ThreadReceiverCycleTime ThreadProcess1CycleTime
    python ../Test/Integrated/plot_mass_spring_csv.py -f ../Test/Integrated/MassSpring-48-2.csv -s ReferencePosition Position
    python ../Test/Integrated/plot_mass_spring_csv.py -f ../Test/Integrated/MassSpring-48-2.csv -s ThreadReceiverCycleTime ThreadProcess2CycleTime

.. dropdown:: Solution
   :icon: key

   The solution is to modify the configuration file ``../Configurations/MassSpring/RTApp-MassSpring-48-Receiver.cfg`` and to add the IOGAM as described above.

   .. literalinclude:: /_static/tutorial/Configurations/MassSpring/RTApp-MassSpring-48-Receiver-solution.cfg
      :language: c++
      :caption: GAM to synchronise the second thread.
      :linenos:
      :lines: 140-186

   The ThreadProcess2 is then added to the RealTimeState and the GAMs are added in the correct order.

   .. literalinclude:: /_static/tutorial/Configurations/MassSpring/RTApp-MassSpring-48-Receiver-solution.cfg
       :language: c++
       :caption: Updated execution list.
       :linenos:
       :lines: 745-768
       :emphasize-lines: 17-21
