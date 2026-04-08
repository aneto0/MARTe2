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
    
    The OPCUADataSource is only available in distribution where `open62541 <https://open62541.org//>`__ is installed.

The :vcisdoxygenmccl:`OPCUADSOutput` DataSource can be used to stream application data over the OPC UA protocol. This allows to interface the application with any OPC UA based monitoring or control applications.

.. warning:: 

    This DataSource writes to OPCUA **synchronously** on the same thread of the MARTe ReadTimeThread, which may negatively impact the real-time performance of the application. Decoupling mechanisms are discussed later in the tutorial.

The DataSource only supports structured types as defined in the :ref:`MassSpringStructuredTypes`. 

Given that the OPCUA server ports need to be unique, in order to avoid port clashes,  the configuration file ``../Configurations/MassSpring/RTApp-MassSpring-40.cfg`` will be automatically updated from a ``Makefile.cfg``. 

The OPCUA types are hosted using a MARTe2 :vcisdoxygenmccl:`OPCUAServer`, which in this example is instantiated in the same configuration file of the application. 

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

Open another terminal and check that the OPCUA records are being updated with the application data, by running another MARTe2 application.

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
      :lines: 1586-1600
      :caption: New OPCUADSOutput DataSource configuration.
      :linenos:
      :emphasize-lines: 4,10

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
