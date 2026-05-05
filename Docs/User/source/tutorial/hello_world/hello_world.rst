.. date: 19/03/2026
   author: Andre' Neto
   copyright: Copyright 2017 F4E | European Joint Undertaking for ITER and
   the Development of Fusion Energy ('Fusion for Energy').
   Licensed under the EUPL, Version 1.1 or - as soon as they will be approved
   by the European Commission - subsequent versions of the EUPL (the "Licence")
   You may not use this work except in compliance with the Licence.
   You may obtain a copy of the Licence at: http://ec.europa.eu/idabc/eupl
   warning: Unless required by applicable law or agreed to in writing, 
   software distributed under the Licence is distributed on an "AS IS"
   basis, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
   or implied. See the Licence permissions and limitations under the Licence.

Hello World
===========

.. important::

   Two implicit rules:

   - All commands are assumed to be run from the ``$MARTE_TRAINING_PARENT_DIR/MARTe2-training-proj/Startup`` directory, unless otherwise specified.
   - When more than one application must be executed in parallel, a new terminal needs to be opened for each application.

Run the application
-------------------

Start the application with:

.. code-block:: bash

   cd $MARTE_TRAINING_PARENT_DIR/MARTe2-training-proj/Startup
    ./MARTeApp.sh -f ../Configurations/HelloWorld/RTApp-HelloWorld-1.cfg -l RealTimeLoader -s State1

Once the application is running, inspect the ``screen`` output and verify that the log shows the counter increasing:

.. code-block:: console

    $ [Information - LoggerBroker.cpp:152]: Counter [0:0]:1
    $ [Information - LoggerBroker.cpp:152]: Counter [0:0]:2
    ...

If these messages appear, the Hello World MARTe2 application has started successfully.

To stop the application, press ``Ctrl+C`` in the terminal where it is running.

Notes on common warning messages
--------------------------------

.. warning::

    While running the application, you may observe messages such as:

    .. code-block:: console

        $ [Warning - Threads.cpp:169]: Requested a thread priority that is higher ...
        $ [Warning - Threads.cpp:181]: Failed to change the thread priority ...

    These warnings are caused by insufficient user permissions to set real-time thread priorities.

    For development purposes, they can be safely ignored and do not affect the functional behaviour of the application.

    In a production environment, however, running without the appropriate permissions may impact real-time performance.

Understanding what just ran
---------------------------

The HelloWorld application that was launched in the previous step is fully described by the file:

.. code-block:: text

    ../Configurations/HelloWorld/RTApp-HelloWorld-1.cfg

Rather than hard-coding the application structure in C++, MARTe2 uses a :ref:`configuration-based approach <MARTeConfigurationLanguage>`. 
This means that the execution :ref:`states <MARTeRealTimeState>`, :ref:`scheduling <MARTeRealTimeApplication>` behaviour, :ref:`functional blocks <MARTeGAMs>`, and signal connections are all described in the configuration.

When the command below was executed:

.. code-block:: bash

    ./MARTeApp.sh -f ../Configurations/HelloWorld/RTApp-HelloWorld-1.cfg -l RealTimeLoader -s State1

MARTe2 loaded this configuration, created the :ref:`RealTimeApplication <MARTeRealTimeApplication>`, selected the ``State1`` :ref:`state <MARTeRealTimeState>`, and started :ref:`executing <MARTeScheduler>` it.


RealTimeApplication
~~~~~~~~~~~~~~~~~~~

At the top level (but not necessarily at the beginning of the file) of every MARTe2 configuration there is a :ref:`RealTimeApplication <MARTeRealTimeApplication>`. This component defines the overall structure and behaviour of the application.

.. note::

     A MARTe configuration file may contain multiple RealTimeApplication instances.


The ``RealTimeApplication`` is always composed of the following sections:

- :ref:`+Functions <MARTeGAMs>`: defines the processing components (GAMs)
- :ref:`+Data <MARTeDataSources>`: defines the data sources 
- :ref:`+States <MARTeRealTimeState>`: defines the execution states (e.g. ``State1``) and their ``RealTimeThread`` instances
- :ref:`+Scheduler <MARTeScheduler>`: controls how the application is executed

.. literalinclude:: /_static/tutorial/Configurations/HelloWorld/RTApp-HelloWorld-1.cfg
   :language: c++
   :lines: 1-4, 45-46, 70-71, 84-85
   :caption: MARTe RealTimeApplication.
   :linenos:
   :emphasize-lines: 3, 5, 7, 9

 
Functions
+++++++++

Two functional blocks (GAMs) are executed in the thread: ``GAMTimer`` and ``GAMDisplay``. 

The first is an :vcisdoxygenmccl:`IOGAM` responsible for getting a counter and a timer signal from a DataSource named ``Timer`` and copying these signals to a :ref:`GAMDataSource <GAMDataSource>`.

.. literalinclude:: /_static/tutorial/Configurations/HelloWorld/RTApp-HelloWorld-1.cfg
   :language: c++
   :lines: 5-28
   :caption: IOGAM named GAMTimer
   :linenos: 
   :emphasize-lines: 2, 5, 9, 16

The ``Frequency`` is a special :ref:`signal property <MARTeSignalRules>` that defines the rate at which the thread executes. The actual pacing of the thread is determined by the DataSource that provides the signal with the ``Frequency`` property. In this case, the ``Timer`` DataSource is configured to execute at a frequency of ``1 Hz``.

The second is an ``IOGAM`` that takes care of copying the signals from the :ref:`GAMDataSource <GAMDataSource>` and offering them to a DataSource named ``Display``.

.. literalinclude:: /_static/tutorial/Configurations/HelloWorld/RTApp-HelloWorld-1.cfg
   :language: c++
   :lines: 29-43
   :caption: GAMDisplay IOGAM
   :linenos: 
   :emphasize-lines: 2, 5, 11

DataSources
+++++++++++

The MARTe :ref:`+Data <MARTeDataSources>` provide an interface between the functional blocks (GAMs) and the outside world. They are responsible for acquiring data from external sources (e.g. hardware, files, network) and making it available to the GAMs, as well as for taking data from the GAMs and sending it to external sinks.
Two DataSources are defined in the configuration: ``Timer`` (of type :vcisdoxygenmccl:`LinuxTimer`) and ``Display`` (of type :vcisdoxygenmccl:`LoggerDataSource`).

.. literalinclude:: /_static/tutorial/Configurations/HelloWorld/RTApp-HelloWorld-1.cfg
   :language: c++
   :lines: 57-68
   :caption: LinuxTimer DataSource 
   :linenos: 
   :emphasize-lines: 1, 5, 8

.. literalinclude:: /_static/tutorial/Configurations/HelloWorld/RTApp-HelloWorld-1.cfg
   :language: c++
   :lines: 51-53
   :caption: LoggerDataSource 
   :linenos: 
   :emphasize-lines: 2

States and Threads
++++++++++++++++++

In the HelloWorld example, a single state (``State1``), with a single thread (``Thread``), is defined. The thread executes the two GAMs described in the previous section.

.. literalinclude:: /_static/tutorial/Configurations/HelloWorld/RTApp-HelloWorld-1.cfg
   :language: c++
   :lines: 70-83
   :caption: MARTe RealTimeState definition.
   :linenos:
   :emphasize-lines: 3, 7, 10

Scheduler
+++++++++

MARTe allows the default :ref:`GAMScheduler <MARTeScheduler>` component to be modified, but this is meant only for special applications (e.g. in embedded systems). For most applications, the default scheduler is sufficient.