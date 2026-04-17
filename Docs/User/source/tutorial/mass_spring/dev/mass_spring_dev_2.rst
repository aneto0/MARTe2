.. date: 17/04/2026
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

DataSources
===========

The development of DataSource components is discussed in the :doc:`DataSource development section </core/app/gams/datasource>`.

The development of DataSource components requires a good understanding of the MARTe2 framework, (in particular the concept of Brokers - see also :vcisdoxygencl:`BrokerI`), as well as a good knowledge of the C++ programming language and the real-time programming concepts. 

The most important methods in DataSource developments are:

- ``Initialise``, which is called once at the beginning of the application execution, is used to load the static parameters.
- ``SetConfiguredDatabase`` is called after the application is set up, when the characteristics of all input signals are known. It can be used to validate the signal characteristics and allocate any private memory that depends on them.
- ``Synchronise``, called by synchroning Broker components (e.g. :vcisdoxygencl:`MemoryMapSynchronisedInputBroker`), to update the memory before (or after) a copy.
- ``AllocateMemory``, requiring the allocation of memory for the input and output signals.
- ``GetSignalMemoryBuffer``, to return the pointer to the memory buffer of a given signal.
- ``GetBrokerName``, to return the name of the Broker(s) that can be used to interact with the DataSource.

.. literalinclude:: /_static/tutorial/Source/DataSources/SystemMonitor/NetworkMonitor.h
    :language: c++
    :lines: 163-166
    :caption: Static parameters definition.
    :linenos:

.. literalinclude:: /_static/tutorial/Source/DataSources/SystemMonitor/NetworkMonitor.cpp
    :language: c++
    :lines: 81-86
    :caption: DataSource Initialise method. 
    :linenos:

As for the GAMs, DataSources are divided into two broad categories: generic and application-specific, with the further caveat that they tend to also be platform (hardware and operating system) specific.

For application-specific DataSource components, the use of the manager to create the boilerplate code is recommended, as it allows developers to quickly create a new DataSource and focus on implementing the functionality. 

.. warning::

    Great care shall be taken as this approach greatly limits the flexibility of the design: for example, the type of Broker is imposed (e.g. :vcisdoxygencl:`MemoryMapSynchronisedInputBroker` or :vcisdoxygencl:`MemoryMapSynchronisedOutputBroker`); the implementation is run directly on the context of the RealTimeThread, where typically the DataSource implementation is expected to be executed in a different thread.

In order to use the manager to create the boilerplate code for the DataSource, the following command can be used:

.. code-block:: bash

    python3 -m marte2_manager.cli --project_name MARTe2-training-proj --project_path $HOME -l DEBUG add --cpt_type datasources --cpt_name MyDataSource --cpt_namespace Tutorial # Modify the project name and path if needed

This will create a new DataSource component named ``MyDataSource`` in the namespace ``Tutorial``, together with all the required Makefiles and tests.

To facilitate the generation of boilerplate code, a JSON template can be used to specify the characteristics of the DataSource, including the number of signals, their data types, and the static parameters. An example of such a template is shown in the following listings:

.. literalinclude:: /_static/tutorial/Resources/system_monitor_datasource_template.json
    :language: json
    :caption: JSON definition for the generation of boilerplate code.
    :linenos:

In this example a DataSource named ``SystemMonitor`` is defined and monitors several statistics by reading from ``/sys/class/net/lo/statistics/`` and from ``/proc/stat``.

To update the DataSource boilerplate code based on the JSON template, the following command can be used:

.. code-block:: bash

    python3 -m marte2_manager.cli --project_name MARTe2-training-proj --project_path $HOME -l DEBUG modify --cpt_type datasources --cpt_name SystemMonitor --cpt_template $HOME/MARTe2-training-proj/Resources/system_monitor_datasource_template.json # Modify the project name and path if needed

The code can be immediately compiled and tested using the generated Makefiles.

.. code-block:: bash

    export TARGET=x86-linux
    make -C $HOME/MARTe2-training-proj/ -f Makefile.gcc core

Compile the DataSource:

.. code-block:: bash

    export TARGET=x86-linux
    make -C $HOME/MARTe2-training-proj/ -f Makefile.gcc core

Note that the DataSource path was automatically added to the ``MARTeApp.sh`` script.

Running the application
-----------------------

The DataSource is already added to the configuration file ``../Configurations/MassSpring/RTApp-MassSpring-59.cfg``, so the application can be run directly after compiling the GAM.

.. code-block:: bash

    ./MARTeApp.sh -f ../Configurations/MassSpring/RTApp-MassSpring-59.cfg -l RealTimeLoader -m StateMachine::START

Once the application is running, inspect the ``screen`` output and verify that the application is running without issues. The log should show entries similar to the following:

.. code-block:: console

    $ [Information - LoggerBroker.cpp:152]: TimeNet [0:0]:1000000
    $ [Information - LoggerBroker.cpp:152]: RXPackets [0:0]:5207657751
    $ [Information - LoggerBroker.cpp:152]: TXPackets [0:0]:5207657751
    $ [Information - LoggerBroker.cpp:152]: RXDropped [0:0]:0
    $ [Information - LoggerBroker.cpp:152]: TXDropped [0:0]:0
    ...

Exercise
--------

The objective of this exercise is to implement the MassSpring model in a GAM.

The configurable parameters are:

- Mass (m) [kg] – must be set and greater than zero
- Spring constant (k) [N/m] – default value: 10.0
- Damping coefficient (c) [Ns/m] – default value: 0.5
- Initial position (x0) [m] – default value: 0.0
- Initial velocity (v0) [m/s] – default value: 0.0

The input signals are:

- Time (t) [microseconds]
- Force (F) [N]

The output signals are:

- Position (x) [m]
- Velocity (v) [m/s]

Based on the file ``Resources/any_gam_template.json``, create a new JSON template for the MassSpring GAM that implements the requirements above.

1. Run the manager command to generate the boilerplate code for the MassSpring GAM based on the JSON template created in the previous step.

.. code-block:: bash

    python3 -m marte2_manager.cli --project_name MARTe2-training-proj --project_path $HOME -l DEBUG add --cpt_type gams --cpt_name MassSpringModel --cpt_namespace Tutorial --cpt_template $HOME/MARTe2-training-proj/Resources/mass_spring_gam_template.json # Modify the project name and path if needed

2. Modify the generated code to implement the functionality of the MassSpring model based on the equations of motion of the system. The equations of motion are described in the :doc:`MathExpressionGAM section</tutorial/mass_spring/gams/mass_spring_gams_3>` of the tutorial.

3. Compile the GAM using the generated Makefiles.

.. code-block:: bash

    export TARGET=x86-linux
    make -C $HOME/MARTe2-training-proj/ -f Makefile.gcc core

4. Execute the application:

.. code-block:: bash

    ./MARTeApp.sh -f ../Configurations/MassSpring/RTApp-MassSpring-58.cfg -l RealTimeLoader -m StateMachine::START

5. Inspect the ``screen`` output and verify that the application is running without issues. The log should show entries similar to the following:

.. code-block:: bash

    ...
    $ [Information - LoggerBroker.cpp:152]: Time [0:0]:7010000
    $ [Information - LoggerBroker.cpp:152]: Position [0:0]:2.000131
    $ [Information - LoggerBroker.cpp:152]: PositionM [0:0]:2.010475
    $ [Information - LoggerBroker.cpp:152]: PositionSPM [0:0]:2.002166
    $ [Information - LoggerBroker.cpp:152]: Velocity [0:0]:-0.000294
    $ [Information - LoggerBroker.cpp:152]: VelocityM [0:0]:-0.012081
    $ [Information - LoggerBroker.cpp:152]: VelocitySPM [0:0]:0.048822

Where ``PositionSPM`` and ``VelocitySPM`` are the position and velocity calculated by the MassSpringModel GAM, while ``Position``, ``PositionM``, ``Velocity`` and ``VelocityM`` are calculated by the ``MathExpressionGAM`` and the ``SSMGAM``. The values should be similar across the three GAMs, with minor differences due to numerical errors and differences in the integration methods used.

.. dropdown:: Solution
   :icon: key

    The solution is to create a new JSON template for the MassSpring GAM based on the requirements specified above, and then use the manager to generate the boilerplate code.

    .. literalinclude:: /_static/tutorial/Resources/mass_spring_gam_template_sol.json
        :language: json
        :caption: JSON definition for the generation of boilerplate code for the ``MassSpringModel`` GAM example.
        :linenos:

    Then implement the functionality of the MassSpring model based on the equations of motion.

    .. literalinclude:: /_static/tutorial/Source/GAMs/MassSpringModelSol/MassSpringModelSol.cpp
        :language: c++
        :lines: 165-181
        :caption: ``Execute`` method definition.
        :linenos:

    Note that the damping coefficient and the spring constant are divided by the mass in the ``Setup`` method.

    .. literalinclude:: /_static/tutorial/Source/GAMs/MassSpringModelSol/MassSpringModelSol.cpp
        :language: c++
        :lines: 99-104, 158-162
        :caption: ``Setup`` method definition (snippet).
        :linenos: