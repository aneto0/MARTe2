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

Developing DataSource components requires a good understanding of the MARTe2 framework (in particular the concept of Brokers – see also :vcisdoxygencl:`BrokerI`), as well as solid knowledge of the C++ programming language and real-time programming concepts. 

The most important methods in DataSource development are:

- ``Initialise``: called once at the beginning of the application execution; used to load static parameters.
- ``SetConfiguredDatabase``: called after the application is set up, when the characteristics of all input signals are known. It can be used to validate signal characteristics and allocate any private memory that depends on them.
- ``Synchronise``: called by synchronising Broker components (e.g. :vcisdoxygencl:`MemoryMapSynchronisedInputBroker`) to update the memory before (or after) a copy.
- ``AllocateMemory``: responsible for allocating memory for input and output signals.
- ``GetSignalMemoryBuffer``: returns the pointer to the memory buffer of a given signal.
- ``GetBrokerName``: returns the name of the Broker(s) that can be used to interact with the DataSource.

.. literalinclude:: /_static/tutorial/Source/DataSources/SystemMonitor/SystemMonitor.h
    :language: c++
    :lines: 163-166
    :caption: Static parameters definition.
    :linenos:

.. literalinclude:: /_static/tutorial/Source/DataSources/SystemMonitor/SystemMonitor.cpp
    :language: c++
    :lines: 81-86
    :caption: DataSource Initialise method. 
    :linenos:

As with GAMs, DataSources are divided into two broad categories: generic and application-specific. Additionally, they are often platform-specific (hardware and operating system dependent).

For application-specific DataSource components, using the ``marte2_manager`` to generate the boilerplate code is recommended, as it allows developers to quickly create a new DataSource and focus on implementing its functionality. 

.. warning::

    Great care must be taken, as this approach significantly limits design flexibility. For example, the type of Broker is imposed (e.g. :vcisdoxygencl:`MemoryMapSynchronisedInputBroker` or :vcisdoxygencl:`MemoryMapSynchronisedOutputBroker`), and the implementation runs directly in the context of the RealTimeThread, whereas DataSource logic is typically expected to run in a separate thread.

To create a new DataSource using the ``marte2_manager``, run:

.. code-block:: bash

    pyhon -m marte2_manager.cli --project_name MARTe2-training-proj --project_path $MARTE_TRAINING_PARENT_DIR -l DEBUG add --cpt_type datasources --cpt_name MyDataSource --cpt_namespace Tutorial

This creates a new DataSource component named ``MyDataSource`` in the ``Tutorial`` namespace, along with the required Makefiles and tests.

To simplify boilerplate generation, a JSON template can be used to define characteristics such as signals, data types, and static parameters. An example is shown below:

.. literalinclude:: /_static/tutorial/Resources/system_monitor_datasource_template.json
    :language: json
    :caption: JSON definition for boilerplate generation.
    :linenos:

In this example, a DataSource named ``SystemMonitor`` is defined, monitoring statistics by reading from ``/sys/class/net/lo/statistics/`` and ``/proc/stat``.

To update the boilerplate code based on the template:

.. code-block:: bash

    pyhon -m marte2_manager.cli --project_name MARTe2-training-proj --project_path $MARTE_TRAINING_PARENT_DIR -l DEBUG modify --cpt_type datasources --cpt_name SystemMonitor --cpt_template $MARTE_TRAINING_PARENT_DIR/MARTe2-training-proj/Resources/system_monitor_datasource_template.json

The code can then be compiled and tested using the generated Makefiles:

.. code-block:: bash

    export TARGET=x86-linux
    make -C $MARTE_TRAINING_PARENT_DIR/MARTe2-training-proj/ -f Makefile.gcc core

Note that the DataSource path is automatically added to the ``MARTeApp.sh`` script.

Running the application
-----------------------

The DataSource is already included in the configuration file ``../Configurations/MassSpring/RTApp-MassSpring-59.cfg``, so the application can be run directly after compilation.

.. code-block:: bash

    ./MARTeApp.sh -f ../Configurations/MassSpring/RTApp-MassSpring-59.cfg -l RealTimeLoader -m StateMachine::START

Once running, check the output to verify correct behaviour. Example log entries:

.. code-block:: console

    $ [Information - LoggerBroker.cpp:152]: TimeNet [0:0]:1000000
    $ [Information - LoggerBroker.cpp:152]: RXPackets [0:0]:5207657751
    $ [Information - LoggerBroker.cpp:152]: TXPackets [0:0]:5207657751
    $ [Information - LoggerBroker.cpp:152]: RXDropped [0:0]:0
    $ [Information - LoggerBroker.cpp:152]: TXDropped [0:0]:0
    ...

Exercise
--------

The goal of this exercise is to extend the DataSource to compute CPU load by reading and parsing ``/proc/stat``.

Edit ``Resources/system_monitor_datasource_template.json`` and add a signal named ``CPULoad`` with:

- ``number_of_elements=8``
- ``number_of_elements_fixed=false``

This allows dynamic sizing rather than enforcing a fixed number of elements.

1. Update the boilerplate code:

.. code-block:: bash

   pyhon -m marte2_manager.cli --project_name tutorial --project_path ~/Projects/MARTe2/Docs/User/source/_static/ modify --cpt_type datasources --cpt_name SystemMonitor --cpt_template ~/Projects/MARTe2/Docs/User/source/_static/tutorial/Resources/system_monitor_datasource_template.json 

2. Modify the generated code to read and parse ``/proc/stat``.

3. The CPU load is computed from the ``cpu`` line:

.. math::

   \text{CPULoad} = 1 - \frac{\text{idle} + \text{iowait}}{\text{user} + \text{nice} + \text{system} + \text{iowait} + \text{irq} + \text{softirq} + \text{idle}}

4. Use the :doc:`MARTe2 stream API </core/streams/streams>` to read and parse the file.

5. Compile the DataSource:

.. code-block:: bash

    export TARGET=x86-linux
    make -C $MARTE_TRAINING_PARENT_DIR/MARTe2-training-proj/ -f Makefile.gcc core

6. Run the application:

.. code-block:: bash

    make -C ../Configurations/MassSpring/ -f Makefile.cfg #The configuration file needs to be regenerated to update with the actual number of elements of the CPULoad signal. 
    ./MARTeApp.sh -f ../Configurations/MassSpring/RTApp-MassSpring-60_Gen.cfg -l RealTimeLoader -m StateMachine::START

7. Verify the output:

.. code-block:: bash

    ...
    $ [Warning - Threads.cpp:185]: Failed to change the thread priority (likely due to insufficient permissions)
    $ [Information - StateMachine.cpp:340]: In state (INITIAL) triggered message (StartNextStateExecutionRTApp)
    $ [Information - LoggerBroker.cpp:152]: TimeMon [0:0]:500000
    $ [Information - LoggerBroker.cpp:152]: RXPackets [0:0]:5208123795
    $ [Information - LoggerBroker.cpp:152]: TXPackets [0:0]:5208123795
    $ [Information - LoggerBroker.cpp:152]: RXDropped [0:0]:0
    $ [Information - LoggerBroker.cpp:152]: TXDropped [0:0]:0
    $ [Information - LoggerBroker.cpp:152]: CPULoad [0:19]:{ 0.025482 0.041496 0 0 0 0.033175 0.023800 0 0.024148 0.024399 0 0 0 0 0.031807 0.031398 0 0.030271 0.032084 0.031658 }

.. dropdown:: Solution
   :icon: key

    Add the signal to the template:

    .. literalinclude:: /_static/tutorial/Resources/system_monitor_datasource_template_sol.json
        :language: json
        :caption: Updated JSON file.
        :linenos:

    Then implement the CPU load computation:

    .. literalinclude:: /_static/tutorial/Source/DataSources/SystemMonitorSol/SystemMonitorSol.cpp
        :language: c++
        :lines: 195,212-232,283-323
        :caption: CPU load computation.
        :linenos: