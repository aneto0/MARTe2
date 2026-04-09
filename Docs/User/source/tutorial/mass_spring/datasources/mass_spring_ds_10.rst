.. date: 02/04/2026
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

EPICSPVAInput
=============

.. warning::
    
    The EPICSPVAInput DataSource is only available in distribution where `EPICS Channel Access <https://epics-controls.org/>`__ is installed.

The :vcisdoxygenmccl:`EPICSPVAInput` DataSource can be used to receive application data using the EPICS pvAccess protocol. This allows to interface the application with any EPICS based monitoring or control application (e.g  `EPICS Control System Studio <https://controlsystemstudio.org/>`__).

This DataSource reads from the PVA records asynchronously on a separate thread (which may be allocated to a different CPU core). This means that it is not intended to be used for real-time control, but rather for live parameter configuration.

The name of the PVs needs to be defined for every signal in the DataSource configuration. 

Given that the PVA records names needs to be unique, in order to avoid name clashes,  the configuration file ``../Configurations/MassSpring/RTApp-MassSpring-36.cfg`` will be automatically updated from a ``Makefile.cfg``. The name of the PV records are based on the username.

The PV records are hosted using a MARTe2 :vcisdoxygenmccl:`EPICSPVADatabase`, which in this example is instantiated in the same configuration file of the application. 

.. literalinclude:: /_static/tutorial/Configurations/MassSpring/RTApp-MassSpring-38.cfg
    :language: c++
    :lines: 186-192,211-220
    :caption: EPICSPVADatabase configuration. Note that the PV record names are automatically replaced by the ``Makefile.cfg``.
    :linenos:
    :emphasize-lines: 2,8,10

In this example the ``ReferencePosition`` signal is read from a PVA record and used as reference for the control of the mass-spring system.

.. literalinclude:: /_static/tutorial/Configurations/MassSpring/RTApp-MassSpring-38.cfg
    :language: c++
    :lines: 250-265
    :caption: IOGAM to copy from the DataSource. Note of the use of the ``Alias`` field to access directly the signal from the DataSource without the need to copy the full structure.
    :linenos:
    :emphasize-lines: 7

.. literalinclude:: /_static/tutorial/Configurations/MassSpring/RTApp-MassSpring-38.cfg
    :language: c++
    :lines: 1573-1584
    :caption: EPICSPVAInput configuration. Note that the PV record name is automatically replaced by the ``Makefile.cfg``.
    :linenos: 
    :emphasize-lines: 2,6-10

Running the application
-----------------------

.. code-block:: bash

    make -C ../Configurations/MassSpring/ -f Makefile.cfg
    ./MARTeApp.sh -f ../Configurations/MassSpring/RTApp-MassSpring-38_Gen.cfg -l RealTimeLoader -s State1

Once the application is running, inspect the ``screen`` output and verify that the application is running without any issues. The log should show entries similar to the following:

.. code-block:: bash

    ...
    $ [Debug - EPICSPVAChannelWrapper.cpp:362]: Resolving scalar (or array of) [value.Control.Thread1.GAMsExecutionTime]
    $ [Debug - EPICSPVAChannelWrapper.cpp:373]: Assigned PV to signal with name [value.Control.Thread1.GAMsExecutionTime]
    $ [Information - LoggerBroker.cpp:152]: Time [0:0]:1000000
    $ [Information - LoggerBroker.cpp:152]: Time [0:0]:2000000
    ...

Open another terminal and check that the PVA records are being updated with the application data using the command:

.. code-block:: bash

    pvget -m MARTE2-TUTORIAL-USERNAME-MASS-SPRING-CONTROL #Replace with your username in capital letters

The output should be similar to the following:

.. code-block:: bash

    $ MARTE2-TUTORIAL-USERNAME-MASS-SPRING-CONTROL structure
    $   MonitorControl value
    $       uint Time 38650000
    $       ControlParameters Control
    $           double ReferencePosition 0
    $           double Position 3.15283e-14
    $           double PositionDisturbed 5.46623e-14
    $           double PositionFiltered 2.90153e-13
    $           double PositionM 3.99788e-09
    $           double Velocity -2.95341e-12
    $           double VelocityM 1.85333e-09
    $           double PositionErr 2.90153e-13
    $           double Force -6.71745e-11
    $           double ForceAverage -6.60828e-17
    $           double ForceStdDev 2.22174e-08
    $           double ForceMax 2.23554e-10
    $           double ForceMin -2.23302e-10

   ...

Open another terminal and modify the value of the ``ReferencePosition`` PVA record using the command:

.. code-block:: bash

    pvput MARTE2-TUTORIAL-USERNAME-MASS-SPRING-CONTROL-INPUT value='{"ReferencePosition":1.0}' #Replace with your username in capital letters

Open another terminal and check that the PVA record is  being updated with the application data using the command:

.. code-block:: bash

    pvget -m MARTE2-TUTORIAL-USERNAME-MASS-SPRING-CONTROL #Replace with your username in capital letters

The output should be similar to the following:

.. code-block:: bash

    $ MARTE2-TUTORIAL-USERNAME-MASS-SPRING-CONTROL structure
    $   MonitorControl value
    $       uint Time 177230000
    $       ControlParameters Control
    $           double ReferencePosition 1
    $           double Position 0.996746
    $           double PositionDisturbed 1.00621
    $           double PositionFiltered 0.996685
    $           double PositionM 0.993831
    $           double Velocity 0.00478432
    $           double VelocityM 0.0349763
    $           double PositionErr -0.00331458
    $           double Force 9.9693
    $           double ForceAverage 10.0853
    $           double ForceStdDev 0.152168
    $           double ForceMax 10.489
    $           double ForceMin 9.96202

Exercices
---------

Ex. 1: Switch on/off
--------------------

The objective of this exercise is to use the EPICSPVAInput DataSource to switch on/off the force output of the mass-spring system.

1. Edit the file ``../Configurations/MassSpring/RTApp-MassSpring-39.cfg`` and add a :vcisdoxygenmccl:`MuxGAM` that selects between the signals ``ForceZero`` and ``ForceFromController`` to produce an output signal named ``Force`` in ``DDB1``. Name this GAM ``GAMMuxSwitchOn``.
2. Modify the ``ControllerGAM`` to output the signal named ``ForceFromController`` instead of ``Force``.
3. Modify the ``ReferenceGAM`` to read the full structure from the EPICSCAInput DataSource and to copy both the ``ReferencePosition`` and the ``SwitchOn`` signal to the ``DDB1``.
4. Add the MuxGAM to the execution list.
5. Execute the application:

.. code-block:: bash

    make -C ../Configurations/MassSpring/ -f Makefile.cfg
    ./MARTeApp.sh -f ../Configurations/MassSpring/RTApp-MassSpring-39_Gen.cfg -l RealTimeLoader -s State1

6. Set a new target ``ReferencePosition`` using the ``pvput`` command.

.. code-block:: bash

    pvput MARTE2-TUTORIAL-USERNAME-MASS-SPRING-CONTROL-INPUT value='{"ReferencePosition":1.0}' #Replace with your username in capital letters

7. Observe that the ``Position`` is not changing and that the system is not responding to the new target ``ReferencePosition``.

.. code-block:: bash

    pvget -m MARTE2-TUTORIAL-USERNAME-MASS-SPRING-CONTROL #Replace with your username in capital letters

8. Switch on the system by setting the switch on-off PV to 1 using the command: 

.. code-block:: bash

    pvput MARTE2-TUTORIAL-USERNAME-MASS-SPRING-CONTROL-INPUT value='{"SwitchOn":1}' #Replace with your username in capital letters

9. Observe that the ``Position`` is now changing and that the system is responding to the new target ``ReferencePosition``.

.. code-block:: bash

    pvget -m MARTE2-TUTORIAL-USERNAME-MASS-SPRING-CONTROL #Replace with your username in capital letters

.. dropdown:: Solution
   :icon: key

   The solution is to add a ``MuxGAM`` as specified above.

   .. literalinclude:: /_static/tutorial/Configurations/MassSpring/RTApp-MassSpring-39-solution.cfg
      :language: c++
      :lines: 345-375
      :caption: MuxGAM configuration.
      :linenos:
      :emphasize-lines: 2,4

   Update the ``GAMReference`` to read both signals from the DataSource.

   .. literalinclude:: /_static/tutorial/Configurations/MassSpring/RTApp-MassSpring-39-solution.cfg
      :language: c++
      :lines: 253-271
      :caption: GAMReference configuration
      :linenos:

   .. note::

        The ``Alias`` could also have been used to access directly the signals from the DataSource, in the ``MuxGAM``, without the need to copy the full structure in the ``ReferenceGAM``. 
        
        However, the MuxGAM requires the signal to have ``NumberOfDimensions=1``, while the structured signal read from the DataSource has ``NumberOfDimensions=0``, and even if they are binary compatible (array of one element and scalar), another GAM to covert between them would be needed, which would make the configuration more complex.

   Add the GAM to the execution list.

   .. literalinclude:: /_static/tutorial/Configurations/MassSpring/RTApp-MassSpring-39-solution.cfg
      :language: c++
      :lines: 1664-1677
      :caption: GAM execution list. 
      :linenos:
      :emphasize-lines: 10
