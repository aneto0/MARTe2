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

OPCUADSOutput
=============

.. warning::

    The OPCUADataSource is only available in distributions where `open62541 <https://open62541.org//>`__ is installed.

The :vcisdoxygenmccl:`OPCUADSOutput` DataSource can be used to stream application data over the OPC UA protocol. This allows interfacing the application with any OPC UA-based monitoring or control applications.

.. warning::

    This DataSource writes to OPCUA **synchronously** on the same thread as the MARTe RealTimeThread, which may negatively impact the real-time performance of the application. Decoupling mechanisms are discussed later in the tutorial (see the sections on :ref:`RealTimeThreadAsyncBridgeTutorial` and :ref:`RealTimeThreadSynchronisationTutorial`).

The DataSource only supports structured types as defined in :ref:`MassSpringStructuredTypes`.

Given that the OPCUA server ports need to be unique in order to avoid port clashes, the configuration file ``../Configurations/MassSpring/RTApp-MassSpring-40.cfg`` will be automatically updated from a ``Makefile.cfg``.

The OPCUA types are hosted using a MARTe2 :vcisdoxygenmccl:`OPCUAServer`, which in this example is instantiated in the same configuration file as the application.

The objective of this example is to monitor the application data using OPCUA.

.. literalinclude:: /_static/tutorial/Configurations/MassSpring/RTApp-MassSpring-40.cfg
    :language: c++
    :lines: 126-141
    :caption: OPCUAServer configuration. Note that the OPCUA server port is automatically replaced by the ``Makefile.cfg``.
    :linenos:
    :emphasize-lines: 2, 7-9

.. literalinclude:: /_static/tutorial/Configurations/MassSpring/RTApp-MassSpring-40.cfg
    :language: c++
    :lines: 756-765, 818-824
    :caption: IOGAM to copy from the flattened signal type definition into a structured type. Note that the OPCUADSOutput requires a Trigger signal, which in this case is the Time signal.
    :linenos:
    :emphasize-lines: 12-14

.. literalinclude:: /_static/tutorial/Configurations/MassSpring/RTApp-MassSpring-40.cfg
    :language: c++
    :lines: 838-849
    :caption: OPCUADSOutput DataSource configuration. Note that the OPCUA address is automatically replaced by the ``Makefile.cfg``.
    :linenos:
    :emphasize-lines: 2,3,6,9


Running the application
-----------------------

Start the application with:

.. code-block:: bash

    make -C ../Configurations/MassSpring/ -f Makefile.cfg
    ./MARTeApp.sh -f ../Configurations/MassSpring/RTApp-MassSpring-40_Gen.cfg -l RealTimeLoader -s State1

Once the application is running, inspect the ``screen`` output and verify that the application is running without any issues. The log should show entries similar to the following:

.. code-block:: bash

    ...
    $ [Warning - Threads.cpp:181]: Failed to change the thread priority (likely due to insufficient permissions)
    $ [Information - RealTimeLoader.cpp:111]: Started application in state State1
    $ [Information - MARTeApp.cpp:135]: Application starting
    $ [Information - LoggerBroker.cpp:152]: Time [0:0]: 0
    $ [Information - LoggerBroker.cpp:152]: Time [0:0]:1000000
    ...

Open another terminal and check that the OPCUA records are being updated with the application data by running another MARTe2 application.

.. code-block:: bash

    ./MARTeApp.sh -f ../Configurations/MassSpring/RTApp-MassSpring-40-Monitor_Gen.cfg -l RealTimeLoader -s State1

The output should be similar to the following:

.. code-block:: bash

    $ [Information - LoggerBroker.cpp:152]: Monitor.Time [0:0]:115290000
    $ [Information - LoggerBroker.cpp:152]: Monitor.Control.ReferencePosition [0:0]:1.972000
    $ [Information - LoggerBroker.cpp:152]: Monitor.Control.Position [0:0]:1.916281
    $ [Information - LoggerBroker.cpp:152]: Monitor.Control.PositionDisturbed [0:0]:1.911329
    $ [Information - LoggerBroker.cpp:152]: Monitor.Control.PositionFiltered [0:0]:1.917298
    $ [Information - LoggerBroker.cpp:152]: Monitor.Control.PositionM [0:0]:1.923568
    $ [Information - LoggerBroker.cpp:152]: Monitor.Control.Velocity [0:0]:-0.125965
    $ [Information - LoggerBroker.cpp:152]: Monitor.Control.VelocityM [0:0]:-0.194689
    $ [Information - LoggerBroker.cpp:152]: Monitor.Control.PositionErr [0:0]:-0.054702
    $ [Information - LoggerBroker.cpp:152]: Monitor.Control.Force [0:0]:13.292912
    $ [Information - LoggerBroker.cpp:152]: Monitor.Control.ForceAverage [0:0]:15.260224
    $ [Information - LoggerBroker.cpp:152]: Monitor.Control.ForceStdDev [0:0]:2.372243
    $ [Information - LoggerBroker.cpp:152]: Monitor.Control.ForceMax [0:0]:19.272639
    $ [Information - LoggerBroker.cpp:152]: Monitor.Control.ForceMin [0:0]:9.151042

.. note::

    You can also use any OPCUA client to connect to the MARTe2 OPCUA server and monitor the application data.

    For example, if the Python ``opcua`` library is installed on your system, you can use the Python script located in ``../Test/Integrated/opcua_monitor.py`` to connect to the server and print the values of the nodes being written by the application.

    .. code-block:: bash

        OPCUA_PORT=`awk '/\+OPCUAServer/,/}/ {if ($1=="Port") print $3}' ../Configurations/MassSpring/RTApp-MassSpring-40_Gen.cfg`;echo "OPCUA_PORT=$OPCUA_PORT"
        python3.6 ../Test/Integrated/opcua_monitor.py -p $OPCUA_PORT -s MassSpringDemo

As explained in the :vcisdoxygenmccl:`OPCUADSOutput` documentation, it is possible to associate a timestamp signal with one or more output signals. When a timestamp signal is configured, its value is used to populate the ``OPCUA`` ``SourceTimestamp`` field when writing the associated nodes.

Exercises
---------

Ex. 1: Statistics monitoring
----------------------------

Monitor the statistics signals as part of another OPCUA structure.

1. Edit the file ``../Configurations/MassSpring/RTApp-MassSpring-41.cfg`` and add a new DataSource instance to write the signal ``MonitorPerf`` from the ``GAMWriterPerf`` IOGAM.

.. important::

    The ``OPCUADSOutput`` does not allow writing multiple structured signals, so a new DataSource needs to be added to the configuration file.

2. Start the sender application.

.. code-block:: bash

    make -C ../Configurations/MassSpring/ -f Makefile.cfg
    ./MARTeApp.sh -f ../Configurations/MassSpring/RTApp-MassSpring-41_Gen.cfg -l RealTimeLoader -s State1

3. Open another terminal and check that the OPCUA records are being updated with the application data by running another MARTe2 application.

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

The objective of this exercise is to use another signal to timestamp OPCUA against a given signal instead of the timestamp that is automatically set by the library.

1. Edit the file ``../Configurations/MassSpring/RTApp-MassSpring-42.cfg`` and add a new signal named ``AbsoluteTimeStamp`` to the ``OPCUAWriter`` DataSource.
2. Make sure that the signal contains the property ``DefaultTimestampSignal`` set to ``1``.
3. Modify the IOGAM ``GAMWriter`` to copy the ``AbsoluteTimeStamp`` signal from the DDB1 into the new signal in the OPCUAWriter DataSource.

.. note::

    As explained in the :vcisdoxygenmccl:`LinuxTimer` documentation, the ``AbsoluteTimeStamp`` signal is the counter multiplied by the period (counting from the Epoch). If you are running on a CODAC Core System, you may use the :vcisdoxygenmccl:`TcnTimeProvider` to obtain the absolute time.

4. Start the sender application.

.. code-block:: bash

    make -C ../Configurations/MassSpring/ -f Makefile.cfg
    ./MARTeApp.sh -f ../Configurations/MassSpring/RTApp-MassSpring-42_Gen.cfg -l RealTimeLoader -s State1

5. Open another terminal and check that the OPCUA records are being updated with the application data by running the following Python application (assuming that the Python ``opcua`` library is installed on your system):

.. code-block:: bash

    OPCUA_PORT=`awk '/\+OPCUAServer/,/}/ {if ($1=="Port") print $3}' ../Configurations/MassSpring/RTApp-MassSpring-42_Gen.cfg`;echo "OPCUA_PORT=$OPCUA_PORT"
    python3.6 ../Test/Integrated/opcua_monitor.py -p $OPCUA_PORT -s MassSpringDemo --print_time_stamps

.. dropdown:: Solution
   :icon: key

   The solution is to add the ``AbsoluteTime`` to the DataSource and set the ``DefaultTimestampSignal`` property.

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