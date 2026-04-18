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

EPICSPVAOutput
==============

.. warning::

    The EPICSPVAOutput DataSource is only available in distributions where `EPICS Channel Access <https://epics-controls.org/>`__ is installed.

The :vcisdoxygenmccl:`EPICSPVAOutput` DataSource can be used to stream application data over the EPICS pvAccess protocol. This allows interfacing the application with any EPICS pvAccess-based monitoring or control applications.

This DataSource writes to the PVA records asynchronously on a separate thread (which may be allocated to a different CPU core).

The DataSource only supports structured types as defined in :ref:`MassSpringStructuredTypes`.

Given that the PVA record names need to be unique, in order to avoid name clashes, the configuration file ``../Configurations/MassSpring/RTApp-MassSpring-36.cfg`` will be automatically updated from a ``Makefile.cfg``. The names of the PV records are based on the username.

The PV records are hosted using a MARTe2 :vcisdoxygenmccl:`EPICSPVADatabase`, which in this example is instantiated in the same configuration file as the application.

The objective of this example is to monitor the application data using EPICS PVA records.

.. literalinclude:: /_static/tutorial/Configurations/MassSpring/RTApp-MassSpring-36.cfg
    :language: c++
    :lines: 126-141
    :caption: EPICSPVADatabase configuration. Note that the PV record names are automatically replaced by the ``Makefile.cfg``.
    :linenos:
    :emphasize-lines: 2, 8, 10-11

.. literalinclude:: /_static/tutorial/Configurations/MassSpring/RTApp-MassSpring-36.cfg
    :language: c++
    :lines: 758-765, 818-823
    :caption: IOGAM to copy from the flattened signal type definition into a structured type.
    :linenos:

.. literalinclude:: /_static/tutorial/Configurations/MassSpring/RTApp-MassSpring-36.cfg
    :language: c++
    :lines: 838-851
    :caption: EPICSPVAOutput DataSource configuration. Note that the PV record names are automatically replaced by the ``Makefile.cfg``.
    :linenos:
    :emphasize-lines: 2,9-11


Running the application
-----------------------

Start the application with:

.. code-block:: bash

    make -C ../Configurations/MassSpring/ -f Makefile.cfg
    ./MARTeApp.sh -f ../Configurations/MassSpring/RTApp-MassSpring-36_Gen.cfg -l RealTimeLoader -s State1

Once the application is running, inspect the ``screen`` output and verify that the application is running without any issues. The log should show entries similar to the following:

.. code-block:: bash

    ...
    $ [Debug - EPICSPVAChannelWrapper.cpp:339]: ResolveStructure -- fields [12 of 13] -- [ForceMin]
    $ [Debug - EPICSPVAChannelWrapper.cpp:362]: Resolving scalar (or array of) [value.Control.ForceMin]
    $ [Debug - EPICSPVAChannelWrapper.cpp:373]: Assigned PV to signal with name [value.Control.ForceMin]
    $  [Information - LoggerBroker.cpp:152]: Time [0:0]:1000000
    $ [Information - LoggerBroker.cpp:152]: Time [0:0]:2000000
    ...

Open another terminal and check that the PVA records are being updated with the application data using the command:

.. code-block:: bash

    pvget -m MARTE2-TUTORIAL-USERNAME-MASS-SPRING-CONTROL #Replace with your username in capital letters

The output should be similar to the following:

.. code-block:: bash

    $ MARTE2-TUTORIAL-USERNAME-MASS-SPRING-CONTROL structure
    $   MonitorControl value
    $       uint Time 93710000
    $       ControlParameters Control
    $           double ReferencePosition 1.412
    $           double Position 1.31519
    $           double PositionDisturbed 1.31702
    $           double PositionFiltered 1.31007
    $           double PositionM 1.31937
    $           double Velocity 0.404339
    $           double VelocityM 0.386377
    $           double PositionErr -0.101934
    $           double Force 13.3695
    $           double ForceAverage 9.42601
    $           double ForceStdDev 2.24461
    $           double ForceMax 13.3695
    $           double ForceMin 5.70393

Exercises
---------

Ex. 1: Statistics monitoring
----------------------------

Monitor the statistics signals as part of another EPICS PVA record.

1. Edit the file ``../Configurations/MassSpring/RTApp-MassSpring-37.cfg`` and add to the ``GAMWriter`` output signals a signal of type ``MonitorPerformance``.
2. Add the corresponding signal to the EPICSPVAOutput DataSource.
3. Check that the PVA record is being written and that the content is updated with the latest values of the statistics signals.

.. code-block:: bash

    make -C ../Configurations/MassSpring/ -f Makefile.cfg
    ./MARTeApp.sh -f ../Configurations/MassSpring/RTApp-MassSpring-37_Gen.cfg -l RealTimeLoader -s State1

.. dropdown:: Solution
   :icon: key

   The solution is to add the structured signal to the ``GAMWriter`` output signals.

   .. note::

        The ``MonitorPerformance`` structure also requires a ``Time`` signal, so this signal must be included among the ``GAMWriter`` input signals.

        Since ``Time`` is already defined as an ``InputSignal`` for ``MonitorControl``, and MARTe2 does not allow duplicate node names, an ``Alias`` node must be added to the configuration file to provide an alias for ``Time``, as shown in the following snippet.

   .. literalinclude:: /_static/tutorial/Configurations/MassSpring/RTApp-MassSpring-37-solution.cfg
      :language: c++
      :lines: 1190-1197, 1249-1253, 1545-1554
      :caption: Updated GAMWriter highlighting the addition of the structured signal and the alias for the Time signal.
      :linenos:
      :emphasize-lines: 4, 9, 12, 19, 21

   Then, the structured signal needs to be added to the EPICSPVAOutput DataSource.

   .. literalinclude:: /_static/tutorial/Configurations/MassSpring/RTApp-MassSpring-37-solution.cfg
      :language: c++
      :lines: 1569-1587
      :caption: Updated EPICSPVAOutput DataSource configuration highlighting the addition of the structured signal. Note that the PV record names are automatically replaced by the ``Makefile.cfg``.
      :linenos:
      :emphasize-lines: 13-17

   Open another terminal and check that the PVA records are being updated with the application data using the command:

   .. code-block:: bash

        pvget -m MARTE2-TUTORIAL-USERNAME-MASS-SPRING-PERF #Replace with your username in capital letters

   The output should be similar to the following:

   .. code-block:: bash

        $ MARTE2-TUTORIAL-USERNAME-MASS-SPRING-PERF structure
        $     MonitorPerformance value
        $         uint Time 3370000
        $         AppPerformance Control
        $             GAMPerformance GAMPerfMonitor
        $                 uint ReadTime 0
        $               uint ExecTime 0
        $               uint WriteTime 0
        $           GAMPerformance GAMTimer
        $               uint ReadTime 9985
        $               uint ExecTime 9985
        $               uint WriteTime 9985
        $           GAMPerformance GAMReferenceWaveform
        $               uint ReadTime 9985
        $               uint ExecTime 9986
        $               uint WriteTime 9986
        $               ...
        $           GAMPerformance GAMWriter
        $               uint ReadTime 9997
        $               uint ExecTime 9997
        $               uint WriteTime 9998
        $           ThreadPerformance Thread1
        $               uint CycleTime 9999
        $               uint CycleTimeAverage 9999
        $               float CycleTimeMovingAverage 9999.5
        $               uint CycleTimeStdDev 93
        $               uint CycleTimeMax 10012
        $               uint CycleTimeMin 9988
        $               uint[] CycleTimeHistogram [0,0,0,0,0,137,0,0,0,0,0]
        $               uint[] FreeTimeHistogram [0,0,0,0,0,0,0,0,0,0,137]
        $               int GAMsExecutionTime 13