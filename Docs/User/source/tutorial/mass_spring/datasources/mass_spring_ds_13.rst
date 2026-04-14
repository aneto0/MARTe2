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

Generate the DAN configuration file from the template:

.. code-block:: bash

    ../Test/Integrated/GenerateDANConfig.sh

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

Ex. 1: Statistics archiving
---------------------------

Archive the following statistics signals using a separate ``DANSource``.

1. Edit the file ``../Configurations/MassSpring/RTApp-MassSpring-46.cfg`` and a new DataSource instance to archive the following signals: ``Time``, 
``Thread1CycleTime``,  ``Thread1CycleTimeMax``, ``Thread1CycleTimeMin``, ``Thread1CycleTimeHistogram``, ``Thread1FreeTimeHistogram``, and ``GAMsExecutionTime``; with a period of 0.01 seconds.

2. Open the file ``../Test/Integrated/dan_mass_spring_demo_template.xml`` to check how the DataSource needs to be named, and name it accordingly.

.. note::

    As explained in the :vcisdoxygenmccl:`DANSource` documentation, arrays need to follow a specific naming convention in the ``xml`` file. 
    
    No action is required on the MARTe2 configuration file, but the arrays in the DAN configuration file need to be named following the convention ``<SignalName>_N_T``, where ``N`` is the number of elements in the array and ``T`` is the period in nanoseconds. 
    
    For example, for the signal ``Thread1CycleTimeHistogram`` with 11 elements and a period of 0.01 seconds, the element in the DAN configuration file needs to be named ``DANWriterPerf_T10000000_N11_uint32``.

    .. literalinclude:: /_static/tutorial/Test/Integrated/dan_mass_spring_demo_template.xml
        :language: xml
        :lines: 131-132,153-157
        :caption: DAN configuration file, highighting the naming convention for arrays. Note that ``N`` denotes the number of elements in the array and ``T`` the period in nanoseconds.
        :linenos:
        :emphasize-lines: 1

3. Add the corresponding IOGAM and add it to the execution list.

4. Note that the StateMachine already contains the necessary messages to open the DAN stream, so no modification is needed in this case.

5. Start the archiving application.

.. code-block:: bash

    make -C ../Configurations/MassSpring/ -f Makefile.cfg
    ./MARTeApp.sh -f ../Configurations/MassSpring/RTApp-MassSpring-46_Gen.cfg -l RealTimeLoader -m StateMachine::START

6. Wait a few seconds and stop the application.

7. Open another terminal and check that the performance signals have been archived.

.. code-block:: bash

    dan-plot VT /tmp/data/ICMassSpringDemoAppUSERNAME_DANSTREAMER0_DANWriterPerf_uint32_NUMBER.h5 Thread1CycleTime,Thread1CycleTimeMin,Thread1CycleTimeMax #Replace USERNAME with your username with the first letter in capital letters and the NUMBER with the actual number of the file generated in the ``/tmp/data/`` directory.

.. dropdown:: Solution
   :icon: key

   The solution is to add a new ``DAN::DANSource`` instance, named ``DANSourcePerf`` to the configuration file and set the corresponding properties.
   
   .. literalinclude:: /_static/tutorial/Configurations/MassSpring/RTApp-MassSpring-46-solution.cfg
      :language: c++
      :lines: 1490-1531
      :caption: New DANSource configuration.
      :linenos:
      :emphasize-lines: 1, 12

   Add a new IOGAM to read the performance signals and add it to the execution list.

   .. literalinclude:: /_static/tutorial/Configurations/MassSpring/RTApp-MassSpring-46-solution.cfg
      :language: c++
      :lines: 1342-1349,1376-1381
      :caption: IOGAM to copy the performance signals.
      :linenos:

   .. literalinclude:: /_static/tutorial/Configurations/MassSpring/RTApp-MassSpring-46-solution.cfg
      :language: c++
      :lines: 1547-1557
      :caption: Updated execution list.
      :linenos:
      :emphasize-lines: 8
