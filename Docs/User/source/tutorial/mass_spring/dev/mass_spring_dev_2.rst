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

The objective of this exercise is to add to the DataSource the CPU load, reading and parsing from the ``/proc/stat`` file .

Edit the file ``Resources/system_monitor_datasource_template.json``, and add a signal named ``CPULoad`` with ``number_of_elements=8`` and ``number_of_elements_fixed=false``. This will prevent the boilerplate code from forcing the number of elements to be fixed to the value set by the template.

1. Run the manager command to update the boilerplate code for the DataSource.

.. code-block:: bash

   python3 -m marte2_manager.cli --project_name tutorial --project_path ~/Projects/MARTe2/Docs/User/source/_static/ modify --cpt_type datasources --cpt_name SystemMonitor --cpt_template ~/Projects/MARTe2/Docs/User/source/_static/tutorial/Resources/system_monitor_datasource_template.json 

2. Modify the generated code to read from the ``/proc/stat`` file and extract the CPU load information.

3. The ``/proc/stat`` file contains a line starting with ``cpu`` followed by several numbers representing the time spent by the CPU in different states (user, nice, system, idle, iowait, irq, softirq). The CPU load can be calculated as the percentage of time spent in non-idle states over the total time:

.. math:: \text{CPULoad} = 1 - \frac{\text{idle} + \text{iowait}}{\text{user} + \text{nice} + \text{system} + \text{iowait} + \text{irq} + \text{softirq} + \text{idle}}

4. You should make use of the :doc:`MARTe2 stream API</core/streams/streams>` to read from the file and parse the contents.

5. Compile the DataSource using the generated Makefiles.

.. code-block:: bash

    export TARGET=x86-linux
    make -C $HOME/MARTe2-training-proj/ -f Makefile.gcc core

6. Execute the application:

.. code-block:: bash

    make -C ../Configurations/MassSpring/ -f Makefile.cfg #The configuration file needs to be regenerated to update with the actual number of elements of the CPULoad signal. 
    ./MARTeApp.sh -f ../Configurations/MassSpring/RTApp-MassSpring-60_Gen.cfg -l RealTimeLoader -m StateMachine::START

7. Inspect the ``screen`` output and verify that the application is running without issues. The log should show entries similar to the following:

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

    The solution is to add the signal to be template file as required.

    .. literalinclude:: /_static/tutorial/Resources/system_monitor_datasource_template_sol.json
        :language: json
        :caption: Updated json file.
        :linenos:

    Then implement the functionality of the system monitor data source.

    .. literalinclude:: /_static/tutorial/Source/DataSources/SystemMonitorSol/SystemMonitorSol.cpp
        :language: c++
        :lines: 195,212-232,283-323
        :caption: Reading of the CPU load.
        :linenos:
