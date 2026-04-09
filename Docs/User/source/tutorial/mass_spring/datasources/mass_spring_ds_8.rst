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

EPICSCAInput
=============

.. warning::
    
    The EPICSCAInput DataSource is only available in distribution where `EPICS Channel Access <https://epics-controls.org/>`__ is installed.

The :vcisdoxygenmccl:`EPICSCAInput` DataSource can be used to receive application data using the EPICS Channel Access protocol. This allows to interface the application with any EPICS based monitoring or control application (e.g  `EPICS Control System Studio <https://controlsystemstudio.org/>`__).

This DataSource reads from the PVs asynchronously on a separate thread (which may be allocated to a different CPU core). This means that it is not intended to be used for real-time control, but rather for live parameter configuration.

The name of the PVs needs to be defined for every signal in the DataSource configuration. 

Given that the PV name needs to be unique, in order to avoid name clashes,  the configuration file ``../Configurations/MassSpring/RTApp-MassSpring-32.cfg`` will be automatically updated from a ``Makefile.cfg``. The name of the PVs are based on the username.

The PVs are hosted on a ``softIOC`` which needs to be started before running the application. The ``softIOC`` configuration file is generated from a template by running the command ``../Test/Integrated/GenerateEPICSSoftIOCDb.sh``, which will also automatically generate the PV names to match the ones defined in the configuration file ``../Configurations/MassSpring/RTApp-MassSpring-32.cfg``.

In this example the ``ReferencePosition`` signal is read from the corresponding PV and used as reference for the control of the mass-spring system.

.. literalinclude:: /_static/tutorial/Configurations/MassSpring/RTApp-MassSpring-32.cfg
    :language: c++
    :lines: 986-996
    :caption: EPICSCAInput configuration. Note that the PVs names are automatically replaced by the ``Makefile.cfg``.
    :linenos: 
    :emphasize-lines: 8

Running the application
-----------------------

Start the ``softIOC`` in a separate terminal:

.. code-block:: bash

    ../Test/Integrated/GenerateEPICSSoftIOCDb.sh
    softIoc -d ../Test/Integrated/MassSpring-1.db

Start the application with:

.. code-block:: bash

    ./MARTeApp.sh -f ../Configurations/MassSpring/RTApp-MassSpring-32_Gen.cfg -l RealTimeLoader -s State1

Once the application is running, inspect the ``screen`` output and verify that the application is running without any issues. The log should show entries similar to the following:

.. code-block:: bash

    $ [Warning - Threads.cpp:181]: Failed to change the thread priority (likely due to insufficient permissions)
    $ [Information - RealTimeLoader.cpp:111]: Started application in state State1
    $ [Information - MARTeApp.cpp:135]: Application starting
    $ [Information - LoggerBroker.cpp:152]: Time [0:0]:0
    $ [Information - LoggerBroker.cpp:152]: Time [0:0]:1000000
    ...

Open another terminal and modify the value of the ``ReferencePosition`` PV using the command:

.. code-block:: bash

    caput MARTE2-TUTORIAL-USERNAME-REFERENCE-POSITION 1.0 #Replace with your username in capital letters

Open another terminal and check that the PVs are being updated with the application data using the command:

.. code-block:: bash

    camonitor MARTE2-TUTORIAL-USERNAME-REFERENCE-POSITION MARTE2-TUTORIAL-USERNAME-POSITION #Replace with your username in capital letters

The output should be similar to the following:

.. code-block:: bash

    $ MARTE2-TUTORIAL-USERNAME-POSITION 2026-04-01 13:22:03.381297 1.00076
    $ MARTE2-TUTORIAL-USERNAME-POSITION 2026-04-01 13:22:03.391297 1.00071
    $ MARTE2-TUTORIAL-USERNAME-POSITION 2026-04-01 13:22:03.401303 1.00065
    $ MARTE2-TUTORIAL-USERNAME-POSITION 2026-04-01 13:22:03.411300 1.0006
    $ MARTE2-TUTORIAL-USERNAME-POSITION 2026-04-01 13:22:03.421299 1.00055
    $ MARTE2-TUTORIAL-USERNAME-POSITION 2026-04-01 13:22:03.431300 1.0005

Exercices
---------

Ex. 1: Switch on/off
--------------------

The objective of this exercise is to use the EPICSCAInput DataSource to switch on/off the force output of the mass-spring system.

1. Edit the file ``../Configurations/MassSpring/RTApp-MassSpring-33.cfg`` and add a :vcisdoxygenmccl:`MuxGAM` that selects between a signal named ``ForceZero`` and the signal ``ForceFromController`` to produce an output signal named ``Force`` in ``DDB1``. Name the GAM ``GAMMuxSwitchOn``.
2. Modify the ``ControllerGAM`` to output the signal named ``ForceFromController`` instead of ``Force``.
3. The name of the switch on-off PV is ``MARTE2-TUTORIAL-USERNAME-SWITCH-ON``. 
4. Add the MuxGAM to the execution list.
5. Execute the application:

.. code-block:: bash

    make -C ../Configurations/MassSpring/ -f Makefile.cfg
    ./MARTeApp.sh -f ../Configurations/MassSpring/RTApp-MassSpring-33_Gen.cfg -l RealTimeLoader -s State1

5. Set a new target ``ReferencePosition`` using the ``caput`` command.

.. code-block:: bash

    caput MARTE2-TUTORIAL-USERNAME-REFERENCE-POSITION 1.0 #Replace with your username in capital letters

6. Observe that the ``Position`` is not changing and that the system is not responding to the new target ``ReferencePosition``.

.. code-block:: bash

    camonitor MARTE2-TUTORIAL-USERNAME-REFERENCE-POSITION MARTE2-TUTORIAL-USERNAME-POSITION #Replace with your username in capital letters  

7. Switch on the system by setting the switch on-off PV to 1 using the command: 

.. code-block:: bash

    caput MARTE2-TUTORIAL-USERNAME-SWITCH-ON 1 #Replace with your username in capital letters

8. Observe that the ``Position`` is now changing and that the system is responding to the new target ``ReferencePosition``.

.. code-block:: bash

    camonitor MARTE2-TUTORIAL-USERNAME-REFERENCE-POSITION MARTE2-TUTORIAL-USERNAME-POSITION #Replace with your username in capital letters  

.. dropdown:: Solution
   :icon: key

   The solution is to add a ``MuxGAM`` as specified above.

   .. literalinclude:: /_static/tutorial/Configurations/MassSpring/RTApp-MassSpring-33-solution.cfg
      :language: c++
      :lines: 148-178
      :caption: MuxGAM configuration.
      :linenos:
      :emphasize-lines: 2,4

   Add the signal to the EPICSCAInput DataSource.

   .. literalinclude:: /_static/tutorial/Configurations/MassSpring/RTApp-MassSpring-33-solution.cfg
      :language: c++
      :lines: 1027-1041
      :caption: DataSource 
      :linenos:
      :emphasize-lines: 2,12

   Add the GAM to the execution list.

   .. literalinclude:: /_static/tutorial/Configurations/MassSpring/RTApp-MassSpring-33-solution.cfg
      :language: c++
      :lines: 1233-1246
      :caption: GAM execution list. 
      :linenos:
      :emphasize-lines: 10
