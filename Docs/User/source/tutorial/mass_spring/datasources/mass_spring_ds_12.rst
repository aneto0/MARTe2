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

OPCUADSInput
============

.. warning::

    The OPCUADataSource is only available in distributions where `open62541 <https://open62541.org//>`__ is installed.

The :vcisdoxygenmccl:`OPCUADSInput` DataSource can be used to receive application data using OPCUA.

This DataSource can read both in synchronous and asynchronous mode. In the first case, the data is read from the OPCUA server in the same thread as the application, while in the second case, the data is read asynchronously on a separate thread (which may be allocated to a different CPU core). The asynchronous mode is not intended to be used for real-time control, but rather for live parameter configuration.

Given that the OPCUA server ports need to be unique in order to avoid port clashes, the configuration file ``../Configurations/MassSpring/RTApp-MassSpring-43.cfg`` will be automatically updated from a ``Makefile.cfg``.

The OPCUA types are hosted using a MARTe2 :vcisdoxygenmccl:`OPCUAServer`, which in this example is instantiated in the same configuration file as the application.

.. literalinclude:: /_static/tutorial/Configurations/MassSpring/RTApp-MassSpring-43.cfg
    :language: c++
    :lines: 186-207
    :caption: OPCUAServer configuration. Note that the OPCUA server port is automatically replaced by the ``Makefile.cfg``.
    :linenos:
    :emphasize-lines: 2,8,17

In this example, the ``ReferencePosition`` signal is read from an OPCUA signal and used as the reference for the control of the mass-spring system.

.. literalinclude:: /_static/tutorial/Configurations/MassSpring/RTApp-MassSpring-43.cfg
    :language: c++
    :lines: 236-251
    :caption: IOGAM to copy from the DataSource. Note the use of the ``Alias`` field to access the signal directly from the DataSource without the need to copy the full structure.
    :linenos:
    :emphasize-lines: 7

.. literalinclude:: /_static/tutorial/Configurations/MassSpring/RTApp-MassSpring-43.cfg
    :language: c++
    :lines: 1569-1582
    :caption: OPCUADSInput DataSource configuration. Note that the OPCUA address is automatically replaced by the ``Makefile.cfg``.
    :linenos:
    :emphasize-lines: 2,6,8-12

Running the application
-----------------------

.. code-block:: bash

    make -C ../Configurations/MassSpring/ -f Makefile.cfg
    ./MARTeApp.sh -f ../Configurations/MassSpring/RTApp-MassSpring-43_Gen.cfg -l RealTimeLoader -s State1

Once the application is running, inspect the ``screen`` output and verify that the application is running without any issues. The log should show entries similar to the following:

.. code-block:: bash

    ...
    $ [Warning - Threads.cpp:185]: Failed to change the thread priority (likely due to insufficient permissions)
    $ [Information - RealTimeLoader.cpp:111]: Started application in state State1
    $ [Information - MARTeApp.cpp:135]: Application starting
    $ [Information - LoggerBroker.cpp:152]: Time [0:0]:0
    $ [Information - LoggerBroker.cpp:152]: Time [0:0]:1000000
    ...

Open another terminal and check that the OPCUA records are being updated with the application data by running another MARTe2 application.

.. code-block:: bash

    ./MARTeApp.sh -f ../Configurations/MassSpring/RTApp-MassSpring-40-Monitor_Gen.cfg -l RealTimeLoader -s State1

The output should be similar to the following:

.. code-block:: bash

    $ [Information - LoggerBroker.cpp:152]: Monitor.Time [0:0]:9540000
    $ [Information - LoggerBroker.cpp:152]: Monitor.Control.ReferencePosition [0:0]:0
    $ [Information - LoggerBroker.cpp:152]: Monitor.Control.Position [0:0]:0
    $ [Information - LoggerBroker.cpp:152]: Monitor.Control.PositionDisturbed [0:0]:0.005878
    $ [Information - LoggerBroker.cpp:152]: Monitor.Control.PositionFiltered [0:0]:0
    $ [Information - LoggerBroker.cpp:152]: Monitor.Control.PositionM [0:0]:-0.000001
    $ [Information - LoggerBroker.cpp:152]: Monitor.Control.Velocity [0:0]:0
    $ [Information - LoggerBroker.cpp:152]: Monitor.Control.VelocityM [0:0]:-0.000004
    $ [Information - LoggerBroker.cpp:152]: Monitor.Control.PositionErr [0:0]:0
    $ [Information - LoggerBroker.cpp:152]: Monitor.Control.Force [0:0]:0
    $ [Information - LoggerBroker.cpp:152]: Monitor.Control.ForceAverage [0:0]:0
    $ [Information - LoggerBroker.cpp:152]: Monitor.Control.ForceStdDev [0:0]:0
    $ [Information - LoggerBroker.cpp:152]: Monitor.Control.ForceMax [0:0]:0
    $ [Information - LoggerBroker.cpp:152]: Monitor.Control.ForceMin [0:0]:0

.. note::

    You can also use any OPCUA client to connect to the MARTe2 OPCUA server and monitor the application data.

    For example, if the Python ``opcua`` library is installed on your system, you can use the Python script located in ``../Test/Integrated/opcua_monitor.py`` to connect to the server and print the values of the nodes being written by the application.

    .. code-block:: bash

        OPCUA_PORT=`awk '/\+OPCUAServer/,/}/ {if ($1=="Port") print $3}' ../Configurations/MassSpring/RTApp-MassSpring-43_Gen.cfg`;echo "OPCUA_PORT=$OPCUA_PORT"
        python3.6 ../Test/Integrated/opcua_monitor.py -p $OPCUA_PORT -s MassSpringDemo 

Open another terminal and modify the value of the ``ReferencePosition`` OPCUA signal using another MARTe2 application.

.. code-block:: bash

    ./MARTeApp.sh -f ../Configurations/MassSpring/RTApp-MassSpring-43-Control_Gen.cfg -l RealTimeLoader -s State1

.. note::

    You can also use any OPCUA client to connect to the MARTe2 OPCUA server and modify the application data.

    For example, if the Python ``opcua`` library is installed on your system, you can use the Python script located in ``../Test/Integrated/opcua_writer.py`` to connect to the server and modify the value of ``ReferencePosition``.

    .. code-block:: bash

        OPCUA_PORT=`awk '/\+OPCUAServer/,/}/ {if ($1=="Port") print $3}' ../Configurations/MassSpring/RTApp-MassSpring-43_Gen.cfg`;echo "OPCUA_PORT=$OPCUA_PORT"
        python3.6 ../Test/Integrated/opcua_writer.py -p $OPCUA_PORT -s MassSpringDemoControlInput.ReferencePosition -v 1.3 

The output should be similar to the following:

.. code-block:: bash

    ...
    $ [Warning - Threads.cpp:185]: Failed to change the thread priority (likely due to insufficient permissions)
    $ [Information - RealTimeLoader.cpp:111]: Started application in state State1
    $ [Information - MARTeApp.cpp:135]: Application starting

Open another terminal and, using one of the commands above, check that the ``ReferencePosition`` is being updated and that the system is responding to the new target:

.. code-block:: bash

    ./MARTeApp.sh -f ../Configurations/MassSpring/RTApp-MassSpring-40-Monitor_Gen.cfg -l RealTimeLoader -s State1

Or:

.. code-block:: bash

    python3.6 ../Test/Integrated/opcua_monitor.py -p OPCUA_PORT -s MassSpringDemo #Replace OPCUA_PORT with the actual port number being used in the configuration file.

Exercises
---------

Ex. 1: Switch on/off
--------------------

The objective of this exercise is to use the ``OPCUADSInput`` DataSource to switch the force output of the mass-spring system on and off.

1. Edit the file ``../Configurations/MassSpring/RTApp-MassSpring-44.cfg`` and add a :vcisdoxygenmccl:`MuxGAM` that selects between the signals ``ForceZero`` and ``ForceFromController`` to produce an output signal named ``Force`` in ``DDB1``. Name this GAM ``GAMMuxSwitchOn``.
2. Modify the ``ControllerGAM`` to output the signal named ``ForceFromController`` instead of ``Force``.
3. Modify the ``ReferenceGAM`` to read the full structure from the ``OPCUADSInput`` DataSource and copy both the ``ReferencePosition`` and the ``SwitchOn`` signal to ``DDB1``.
4. Add the MuxGAM to the execution list.
5. Execute the application:

.. code-block:: bash

    make -C ../Configurations/MassSpring/ -f Makefile.cfg
    ./MARTeApp.sh -f ../Configurations/MassSpring/RTApp-MassSpring-44_Gen.cfg -l RealTimeLoader -s State1

6. Set a new target ``ReferencePosition`` by running the following MARTe2 application.

.. code-block:: bash

    ./MARTeApp.sh -f ../Configurations/MassSpring/RTApp-MassSpring-43-Control_Gen.cfg -l RealTimeLoader -s State1 #Alternatively, use the Python script described above to update the OPCUA signal.

7. Observe that the ``Position`` is not changing and that the system is not responding to the new target ``ReferencePosition``.

.. code-block:: bash

    ./MARTeApp.sh -f ../Configurations/MassSpring/RTApp-MassSpring-40-Monitor_Gen.cfg -l RealTimeLoader -s State1 #Alternatively, use the Python script described above to monitor the OPCUA signal.

8. Switch on the system by setting the switch on/off signal to 1 using the command:

.. code-block:: bash

    ./MARTeApp.sh -f ../Configurations/MassSpring/RTApp-MassSpring-44-Control-On_Gen.cfg -l RealTimeLoader -s State1

8.1. Alternatively, use the Python script described above to update the OPCUA signal.

.. code-block:: bash

        python3.6 ../Test/Integrated/opcua_writer.py -p OPCUA_PORT -s MassSpringDemoControlInput.SwitchOn -v 1 #Replace OPCUA_PORT with the actual port number being used in the configuration file.

9. Observe that the ``Position`` is now changing and that the system is responding to the new target ``ReferencePosition``, using one of the commands described above.

.. dropdown:: Solution
   :icon: key

   The solution is to add a ``MuxGAM`` as specified above.

   .. literalinclude:: /_static/tutorial/Configurations/MassSpring/RTApp-MassSpring-44-solution.cfg
      :language: c++
      :lines: 331-361
      :caption: MuxGAM configuration.
      :linenos:
      :emphasize-lines: 2,4

   Update the ``GAMReference`` to read both signals from the DataSource.

   .. literalinclude:: /_static/tutorial/Configurations/MassSpring/RTApp-MassSpring-44-solution.cfg
      :language: c++
      :lines: 239-257
      :caption: GAMReference configuration
      :linenos:

   .. note::

        The ``Alias`` could also have been used to access the signals directly from the DataSource in the ``MuxGAM``, without the need to copy the full structure in the ``ReferenceGAM``.

        However, the MuxGAM requires the signal to have ``NumberOfDimensions=1``, while the structured signal read from the DataSource has ``NumberOfDimensions=0``. Even if they are binary compatible (array of one element and scalar), another GAM to convert between them would be needed, which would make the configuration more complex.

   Add the GAM to the execution list.

   .. literalinclude:: /_static/tutorial/Configurations/MassSpring/RTApp-MassSpring-44-solution.cfg
      :language: c++
      :lines: 1667-1685
      :caption: GAM execution list.
      :linenos:
      :emphasize-lines: 10