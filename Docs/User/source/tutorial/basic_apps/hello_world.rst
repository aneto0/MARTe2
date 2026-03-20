.. date: 19/03/2026
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

Hello World
===========

Run the application
-------------------

Start the application with:

.. code-block:: bash

    /MARTeApp.sh -f ../Configurations/HelloWorld/RTApp-HelloWorld-1.cfg -l RealTimeLoader -s State1

Once the application is running, inspect the ``screen`` output and verify that the log shows the counter increasing:

.. code-block:: text

    [Information - LoggerBroker.cpp:152]: Counter [0:0]:1
    [Information - LoggerBroker.cpp:152]: Counter [0:0]:2
    ...

If these messages appear, the Hello World MARTe2 application has started successfully.

To stop the application, press ``Ctrl+C`` in the terminal where it is running.

Notes on common warning messages
--------------------------------

.. warning::

    While running the application, you may observe messages such as:

    .. code-block:: text

        [Warning - Threads.cpp:169]: Requested a thread priority that is higher ...
        [Warning - Threads.cpp:181]: Failed to change the thread priority ...

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

    /MARTeApp.sh -f ../Configurations/HelloWorld/RTApp-HelloWorld-1.cfg -l RealTimeLoader -s State1

MARTe2 loaded this configuration, created the :ref:`RealTimeApplication <MARTeRealTimeApplication>`, selected the ``State1`` :ref:`state <MARTeRealTimeState>`, and started :ref:`executing <MARTeScheduler>` it:.


RealTimeApplication
~~~~~~~~~~~~~~~~~~~~

At the top level (but not necessarily at the beginning of the file) of every MARTe2 configuration there is a :ref:`RealTimeApplication <MARTeRealTimeApplication>`. This component defines the overall structure and behaviour of the application.

.. note::

     A MARTe configuration file may contain multiple RealTimeApplication instances.


The ``RealTimeApplication`` is always composed by the following sections:

- :ref:`+Functions <MARTeGAMs>`: defines the processing components (GAMs)
- :ref:`+Data <MARTeDataSources>`: defines the data sources and signals
- :ref:`+States <MARTeRealTimeState>`: defines the execution states (e.g. ``State1``)
- :ref:`+Scheduler <MARTeScheduler>`: controls how the application is executed

.. literalinclude:: /_static/tutorial/Configurations/HelloWorld/RTApp-HelloWorld-1.cfg
   :language: c++
   :lines: 1-4, 45-46, 70-71, 84-85
   :caption: MARTe RealTimeApplication.
   :linenos:
   :emphasize-lines: 3, 5, 7, 9

In the HelloWorld example, these elements are configured to execute a single state (``State1``), with a single thread (``Thread``).

.. literalinclude:: /_static/tutorial/Configurations/HelloWorld/RTApp-HelloWorld-1.cfg
   :language: c++
   :lines: 70-83
   :caption: MARTe RealTimeState definition.
   :linenos:
   :emphasize-lines: 3, 7, 10
 
Two functional blocks (GAMs) are executed in the thread: ``GAMTimer`` and ``GAMDisplay``. 

The first is an :vcisdoxygenmccl:`IOGAM` responsible for getting a counter and a timer signal from a DataSource named ``Timer`` and copying these signals to a :ref:`GAMDataSource <GAMDataSource>`.

.. literalinclude:: /_static/tutorial/Configurations/HelloWorld/RTApp-HelloWorld-1.cfg
   :language: c++
   :lines: 5-28
   :caption: IOGAM named GAMTimer
   :linenos: 
   :emphasize-lines: 2, 5, 16
 