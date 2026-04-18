.. date: 01/04/2026
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

EPICSCAOutput
=============

.. warning::

    The EPICSCAOutput DataSource is only available in distributions where `EPICS Channel Access <https://epics-controls.org/>`__ is installed.

The :vcisdoxygenmccl:`EPICSCAOutput` DataSource can be used to stream application data over the EPICS Channel Access protocol. This allows interfacing the application with any EPICS-based monitoring or control application (e.g. `EPICS Archiver Appliance <https://slacmshankar.github.io/epicsarchiver_docs/>`__, `EPICS Control System Studio <https://controlsystemstudio.org/>`__).

This DataSource writes to the PVs asynchronously on a separate thread (which may be allocated to a different CPU core).

The names of the PVs need to be defined for every signal in the DataSource configuration.

Given that the PV name needs to be unique, in order to avoid name clashes, the configuration file ``../Configurations/MassSpring/RTApp-MassSpring-30.cfg`` will be automatically updated from a ``Makefile.cfg``. The names of the PVs are based on the username.

The PVs are hosted on a ``softIOC`` which needs to be started before running the application. The ``softIOC`` configuration file is generated from a template by running the command ``../Test/Integrated/GenerateEPICSSoftIOCDb.sh``, which will also automatically generate the PV names to match the ones defined in the configuration file ``../Configurations/MassSpring/RTApp-MassSpring-30.cfg``.

In this example, the main application data is written into the corresponding PVs.

.. literalinclude:: /_static/tutorial/Configurations/MassSpring/RTApp-MassSpring-30.cfg
    :language: c++
    :lines: 896-915
    :caption: EPICSCAOutput configuration. Note that the PV names are automatically replaced by the ``Makefile.cfg``.
    :linenos:
    :emphasize-lines: 9, 13, 17

Running the application
-----------------------

Start the ``softIOC`` in a separate terminal:

.. code-block:: bash

    ../Test/Integrated/GenerateEPICSSoftIOCDb.sh
    softIoc -d ../Test/Integrated/MassSpring-1.db

Start the application with:

.. code-block:: bash

    ./MARTeApp.sh -f ../Configurations/MassSpring/RTApp-MassSpring-30_Gen.cfg -l RealTimeLoader -s State1

Once the application is running, inspect the ``screen`` output and verify that the application is running without any issues. The log should show entries similar to the following:

.. code-block:: bash

    $ [Warning - Threads.cpp:181]: Failed to change the thread priority (likely due to insufficient permissions)
    $ [Information - RealTimeLoader.cpp:111]: Started application in state State1
    $ [Information - MARTeApp.cpp:135]: Application starting
    $ [Information - LoggerBroker.cpp:152]: Time [0:0]:1000000
    ...

Open another terminal and check that the PVs are being updated with the application data using the command:

.. code-block:: bash

    camonitor MARTE2-TUTORIAL-USERNAME-REFERENCE-POSITION MARTE2-TUTORIAL-USERNAME-POSITION #Replace with your username in capital letters

The output should be similar to the following:

.. code-block:: bash

    $ MARTE2-TUTORIAL-USERNAME-REFERENCE-POSITION 2026-04-01 10:07:25.733591 1.324
    $ MARTE2-TUTORIAL-USERNAME-POSITION 2026-04-01 10:07:25.733595 1.22757
    $ MARTE2-TUTORIAL-USERNAME-REFERENCE-POSITION 2026-04-01 10:07:25.748061 1.328
    $ MARTE2-TUTORIAL-USERNAME-POSITION 2026-04-01 10:07:25.748076 1.2316
    $ MARTE2-TUTORIAL-USERNAME-REFERENCE-POSITION 2026-04-01 10:07:25.753653 1.332
    $ MARTE2-TUTORIAL-USERNAME-POSITION 2026-04-01 10:07:25.753660 1.23562

Exercises
---------

Ex. 1: Statistics monitoring
----------------------------

Add a GAM and another DataSource to monitor the statistics of the execution of the application. The statistics should include ``Thread1CycleTime``, ``Thread1CycleTimeAverage``, ``Thread1CycleTimeMovingAverage``, ``Thread1CycleTimeStdDev``, ``Thread1CycleTimeMax``, ``Thread1CycleTimeMin``, ``Thread1CycleTimeHistogram``, ``Thread1FreeTimeHistogram``, ``GAMsExecutionTime``.

1. Edit the file ``../Configurations/MassSpring/RTApp-MassSpring-31.cfg`` and add an ``EPICSCAOutput`` DataSource to store the signals ``Thread1CycleTime``, ``Thread1CycleTimeAverage``, ``Thread1CycleTimeMovingAverage``, ``Thread1CycleTimeStdDev``, ``Thread1CycleTimeMax``, ``Thread1CycleTimeMin``, ``Thread1CycleTimeHistogram``, ``Thread1FreeTimeHistogram``, ``GAMsExecutionTime``.
2. The names of the PVs can be found in the file ``../Test/Integrated/MassSpring-1.db``. 
3. Add a GAM to write the signals to the DataSource.
4. Add the GAM to the execution list.
5. Check that the PVs are being written and that the content is updated with the latest values of the signals.

.. code-block:: bash

    make -C ../Configurations/MassSpring/ -f Makefile.cfg
    ./MARTeApp.sh -f ../Configurations/MassSpring/RTApp-MassSpring-31_Gen.cfg -l RealTimeLoader -s State1

.. dropdown:: Solution
   :icon: key

   The solution is to add an ``EPICSCAWriterStats`` with the signals.

   .. literalinclude:: /_static/tutorial/Configurations/MassSpring/RTApp-MassSpring-31-solution.cfg
      :language: c++
      :lines: 1115-1133
      :caption: EPICSCAWriterStats to write the statistics signals.
      :linenos:
      :emphasize-lines: 9, 13, 17

   Add the GAM to write the signals to the DataSource.

   .. literalinclude:: /_static/tutorial/Configurations/MassSpring/RTApp-MassSpring-31-solution.cfg
      :language: c++
      :lines: 883-894, 925-934
      :caption: GAM to write the signals to the DataSource.
      :linenos:

   Open another terminal and check that the PVs are being updated with the application data using the command:

   .. code-block:: bash

       camonitor MARTE2-TUTORIAL-USERNAME-THREAD1-CYCLE-TIME MARTE2-TUTORIAL-USERNAME-THREAD1-CYCLE-TIME-HISTOGRAM #Replace with your username in capital letters

   The output should be similar to the following:

   .. code-block:: bash

       $ MARTE2-TUTORIAL-USERNAME-THREAD1-CYCLE-TIME 2026-04-01 10:17:06.617292 9990
       $ MARTE2-TUTORIAL-USERNAME-THREAD1-CYCLE-TIME-HISTOGRAM 2026-04-01 10:17:06.617297 11 0 0 0 2 0 1362 0 2 0 0 0
       $ MARTE2-TUTORIAL-USERNAME-THREAD1-CYCLE-TIME 2026-04-01 10:17:06.627271 10009
       $ MARTE2-TUTORIAL-USERNAME-THREAD1-CYCLE-TIME-HISTOGRAM 2026-04-01 10:17:06.627276 11 0 0 0 2 0 1363 0 2 0 0 0
