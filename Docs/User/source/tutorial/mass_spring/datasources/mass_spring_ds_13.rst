.. date: 09/04/2026
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

DAN
===

.. warning::
    
    The DAN DataSource is only available in `CODAC Core System <https://www.iter.org/machine/supporting-systems/codac/codac-core-system>`__ distributions. 

The :vcisdoxygenmccl:`DANSource` DataSource can be used to archive application data to the ITER Data Archiving Network (DAN). 

Given that the property ``ICProgramName`` needs to be unique, in order to avoid clashes,  the configuration file ``../Configurations/MassSpring/RTApp-MassSpring-45.cfg`` will be automatically updated from a ``Makefile.cfg``. 

The signals to be archived are defined in a DAN configuration file, which is generated from a template by running the command ``../Test/Integrated/GenerateDANConfig.sh``. 

.. literalinclude:: /_static/tutorial/Test/Integrated/dan_mass_spring_demo_template.xml
    :language: xml
    :lines: 9-38
    :caption: DAN configuration file. 
    :linenos:
    :emphasize-lines: 2-3

The objective of this example is to archive the application data using the DAN DataSource.

.. literalinclude:: /_static/tutorial/Configurations/MassSpring/RTApp-MassSpring-45.cfg
    :language: c++
    :lines: 1349-1371
    :caption: ``DAN::DANSource`` configuration. Note that the ``ICProgramName`` is automatically replaced by the ``Makefile.cfg``.
    :linenos:
    :emphasize-lines: 6, 14, 18

.. important::

    As explained in the :vcisdoxygenmccl:`DANSource` documentation, the DAN DataSource requires that the ``OpenStream`` is called using the MARTe2 messaging system (see also the section :ref:`MARTeMessages`) before writing any signal to the DataSource. In this example, this is implemented using the :vcisdoxygencl:`StateMachine` (see also the section :ref:`MARTeStateMachine`).

    Examples with the StateMachine component are covered later in the tutorial.

The DataSource also supports the archiving of structures (see the :vcisdoxygencl:`DANSource` documentation), but in this example only simple signals are archived for simplicity.

Running the application
-----------------------

Start the DAN archiver:

.. code-block:: bash

    sudo dan_archiver_writer start

Use the ``danApiTool`` to configure and initialise the archiver:

.. code-block:: bash

    /opt/codac/bin/danApiTool api init ../Test/Integrated/dan_mass_spring_demo_0.xml

Start the application with:

.. code-block:: bash

    make -C ../Configurations/MassSpring/ -f Makefile.cfg
    ./MARTeApp.sh -f ../Configurations/MassSpring/RTApp-MassSpring-45_Gen.cfg -l RealTimeLoader -m StateMachine::START #Note that the command is different from the previous examples.

Once the application is running, inspect the ``screen`` output and verify that the application is running without any issues. The log should show entries similar to the following:

.. code-block:: bash

    ...
    $ [Warning - Threads.cpp:185]: Failed to change the thread priority (likely due to insufficient permissions)
    $ [Information - StateMachine.cpp:340]: In state (INITIAL) triggered message (StartNextStateExecutionRTApp)
    $ [Information - LoggerBroker.cpp:152]: Time [0:0]:1000000
    $ [Information - LoggerBroker.cpp:152]: Time [0:0]:2000000
    ...

Wait some seconds and stop the application. 

The files will be stored in ``/tmp/data/``. 
Open another terminal and plot the archived data using the ``dan-tool``:

.. code-block:: bash

    dan-plot VT /tmp/data/ICMassSpringDemoAppUSERNAME_DANSTREAMER0_DANWriter_float64_NUMBER.h5 ReferencePosition,Position #Replace USERNAME with your username with the first letter in capital letters and the NUMBER with the actual number of the file generated in the ``/tmp/data/`` directory.

Exercices
---------

Ex. 1: Statistics monitoring
----------------------------

Monitor the statistics signals as part of another OPCUA structure.

1. Edit the file ``../Configurations/MassSpring/RTApp-MassSpring-41.cfg`` and a new DataSource instance to write the signal ``MonitorPerf`` from the ``GAMWriterPerf`` IOGAM.

.. important::

    The ``OPCUADSOutput`` does not allow to write multiple structured signals, so a new DataSource needs to be added to the configuration file.

2. Start the sender application.

.. code-block:: bash

    make -C ../Configurations/MassSpring/ -f Makefile.cfg
    ./MARTeApp.sh -f ../Configurations/MassSpring/RTApp-MassSpring-41_Gen.cfg -l RealTimeLoader -s State1

3. Open another terminal and check that the OPCUA records are being updated with the application data, by running another MARTe2 application.

.. code-block:: bash

    ./MARTeApp.sh -f ../Configurations/MassSpring/RTApp-MassSpring-41-Monitor_Gen.cfg -l RealTimeLoader -s State1

.. dropdown:: Solution
   :icon: key

   The solution is to add a new ``OPCUADSOutput`` instance. 
   
   .. literalinclude:: /_static/tutorial/Configurations/MassSpring/RTApp-MassSpring-41-solution.cfg
      :language: c++
      :lines: 1583-1594
      :caption: New OPCUADSOutput DataSource configuration.
      :linenos:
      :emphasize-lines: 3,9

   The outputs from the monitor application (running on the other terminal) should be similar to the following:

   .. code-block:: bash

        $ [Information - LoggerBroker.cpp:152]: MonitorPerf.Perf.GAMHist.ReadTime [0:0]:9848
        $ [Information - LoggerBroker.cpp:152]: MonitorPerf.Perf.GAMHist.ExecTime [0:0]:9849
        $ [Information - LoggerBroker.cpp:152]: MonitorPerf.Perf.GAMHist.WriteTime [0:0]:9849
        $ [Information - LoggerBroker.cpp:152]: MonitorPerf.Perf.GAMMathExpr.ReadTime [0:0]:9849
        $ [Information - LoggerBroker.cpp:152]: MonitorPerf.Perf.GAMMathExpr.ExecTime [0:0]:9849
        $ [Information - LoggerBroker.cpp:152]: MonitorPerf.Perf.GAMMathExpr.WriteTime [0:0]:9849
        $ [Information - LoggerBroker.cpp:152]: MonitorPerf.Perf.GAMConversion.ReadTime [0:0]:9849
        $ [Information - LoggerBroker.cpp:152]: MonitorPerf.Perf.GAMConversion.ExecTime [0:0]:9849
        $ [Information - LoggerBroker.cpp:152]: MonitorPerf.Perf.GAMConversion.WriteTime [0:0]:9849
        $ [Information - LoggerBroker.cpp:152]: MonitorPerf.Perf.GAMFilterMovingAvg.ReadTime [0:0]:9849
        $ [Information - LoggerBroker.cpp:152]: MonitorPerf.Perf.GAMFilterMovingAvg.ExecTime [0:0]:9849
        $ [Information - LoggerBroker.cpp:152]: MonitorPerf.Perf.GAMFilterMovingAvg.WriteTime [0:0]:9850
        $ [Information - LoggerBroker.cpp:152]: MonitorPerf.Perf.GAMMathTrigger.ReadTime [0:0]:9850
        $ [Information - LoggerBroker.cpp:152]: MonitorPerf.Perf.GAMMathTrigger.ExecTime [0:0]:9850
        $ [Information - LoggerBroker.cpp:152]: MonitorPerf.Perf.GAMMathTrigger.WriteTime [0:0]:9850
        $ [Information - LoggerBroker.cpp:152]: MonitorPerf.Perf.GAMMathTriggerSecond.ReadTime [0:0]:9850
        $ [Information - LoggerBroker.cpp:152]: MonitorPerf.Perf.GAMMathTriggerSecond.ExecTime [0:0]:9850
        $ [Information - LoggerBroker.cpp:152]: MonitorPerf.Perf.GAMMathTriggerSecond.WriteTime [0:0]:9850
        $ [Information - LoggerBroker.cpp:152]: MonitorPerf.Perf.GAMDisplay.ReadTime [0:0]:9850
        $ [Information - LoggerBroker.cpp:152]: MonitorPerf.Perf.GAMDisplay.ExecTime [0:0]:9850
        $ [Information - LoggerBroker.cpp:152]: MonitorPerf.Perf.GAMDisplay.WriteTime [0:0]:9850
        $ [Information - LoggerBroker.cpp:152]: MonitorPerf.Perf.GAMWriter.ReadTime [0:0]:9851
        $ [Information - LoggerBroker.cpp:152]: MonitorPerf.Perf.GAMWriter.ExecTime [0:0]:9851
        $ [Information - LoggerBroker.cpp:152]: MonitorPerf.Perf.GAMWriter.WriteTime [0:0]:9896
        $ [Information - LoggerBroker.cpp:152]: MonitorPerf.Perf.Thread1.CycleTime [0:0]:10001
        $ [Information - LoggerBroker.cpp:152]: MonitorPerf.Perf.Thread1.CycleTimeAverage [0:0]:9998
        $ [Information - LoggerBroker.cpp:152]: MonitorPerf.Perf.Thread1.CycleTimeMovingAverage [0:0]:9998.599052
        $ [Information - LoggerBroker.cpp:152]: MonitorPerf.Perf.Thread1.CycleTimeStdDev [0:0]:141
        $ [Information - LoggerBroker.cpp:152]: MonitorPerf.Perf.Thread1.CycleTimeMax [0:0]:10076
        $ [Information - LoggerBroker.cpp:152]: MonitorPerf.Perf.Thread1.CycleTimeMin [0:0]:9918
        $ [Information - LoggerBroker.cpp:152]: MonitorPerf.Perf.Thread1.CycleTimeHistogram [0:10]:{ 2 1 0 0 0 137 0 0 1 2 1 }
        $ [Information - LoggerBroker.cpp:152]: MonitorPerf.Perf.Thread1.FreeTimeHistogram [0:10]:{ 0 0 1 0 0 1 0 1 0 29 112 }
        $ [Information - LoggerBroker.cpp:152]: MonitorPerf.Perf.Thread1.GAMsExecutionTime [0:0]:58

Ex. 2: Timestamping 
-------------------

The objective of this exercise is to use another signal to timestamp the OPCUA against a given signal instead of the timestamp that is automatically set by the library.

1. Edit the file ``../Configurations/MassSpring/RTApp-MassSpring-42.cfg`` and a new signal named ``AbsoluteTimeStamp`` to the ``OPCUAWriter`` DataSource.
2. Make sure that the signal contains the property ``DefaultTimestampSignal`` set to ``1``.
3. Modify the IOGAM ``GAMWriter`` to copy the ``AbsoluteTimeStamp`` signal from the DDB1 into the new signal in the OPCUAWriter DataSource.

.. note::

    As explained in the :vcisdoxygenmccl:`LinuxTimer` documentation the ``AbsoluteTimeStamp`` signal is the counter multiplied by the period (counting from the Epoch). If you are running from a CODAC Core System, you may use the :vcisdoxygenmccl:`TcnTimeProvider` to obtain the absolute time.

4. Start the sender application.

.. code-block:: bash

    make -C ../Configurations/MassSpring/ -f Makefile.cfg
    ./MARTeApp.sh -f ../Configurations/MassSpring/RTApp-MassSpring-42_Gen.cfg -l RealTimeLoader -s State1

5. Open another terminal and check that the OPCUA records are being updated with the application data, by running the following python application (assuming that the python ``opcua`` library is installed in your system):

.. code-block:: bash

    python3.6 ../Test/Integrated/opcua_monitor.py -p -s MassSpringDemo --print_time_stamps #Replace the OPCUA_PORT with the actual port number being used in the configuration file.

.. dropdown:: Solution
   :icon: key

   The solution is to add the ``AbsoluteTime`` to the ``DataSource`` and set the ``DefaultTimestampSignal`` property.

   .. literalinclude:: /_static/tutorial/Configurations/MassSpring/RTApp-MassSpring-42-solution.cfg
      :language: c++
      :lines: 1587-1602
      :caption: New OPCUADSOutput DataSource configuration.
      :linenos:
      :emphasize-lines: 6-9

   The outputs from the monitor application (running on the other terminal) should be similar to the following (note that the timestamps are near the Epoch):

   .. code-block:: bash

        - MassSpringDemo
        - Time:22100000 | src_ts:1970-01-01 01:51:12.614091 | srv_ts:None
        - Control
            - ReferencePosition:0.8280000000000002 | src_ts:1970-01-01 01:51:12.614101 | srv_ts:None
            - Position:0.7308889950318065 | src_ts:1970-01-01 01:51:12.614101 | srv_ts:None
            - PositionDisturbed:0.7329734151785503 | src_ts:1970-01-01 01:51:12.614101 | srv_ts:None
            - PositionFiltered:0.726084634618206 | src_ts:1970-01-01 01:51:12.614101 | srv_ts:None
            - PositionM:0.7292609341046103 | src_ts:1970-01-01 01:51:12.614101 | srv_ts:None
            - Velocity:0.3796677587971925 | src_ts:1970-01-01 01:51:12.614101 | srv_ts:None
            - VelocityM:0.3941233025599467 | src_ts:1970-01-01 01:51:12.614111 | srv_ts:None
            - PositionErr:-0.10212367380322629 | src_ts:1970-01-01 01:51:12.614111 | srv_ts:None
            - Force:7.6191469671596765 | src_ts:1970-01-01 01:51:12.614111 | srv_ts:None
            - ForceAverage:4.3079819265028485 | src_ts:1970-01-01 01:51:12.614111 | srv_ts:None
            - ForceStdDev:1.8327930024983468 | src_ts:1970-01-01 01:51:12.614111 | srv_ts:None
            - ForceMax:7.6191469671596765 | src_ts:1970-01-01 01:51:12.614111 | srv_ts:None
            - ForceMin:1.325131643231978 | src_ts:1970-01-01 01:51:12.614121 | srv_ts:None
