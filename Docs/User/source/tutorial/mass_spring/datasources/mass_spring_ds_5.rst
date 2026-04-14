.. date: 31/03/2026
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

SDNPublisher
============

.. warning::
    
    The SDNPublisher DataSource is only available in `CODAC Core System <https://www.iter.org/machine/supporting-systems/codac/codac-core-system>`__ distributions. 

The :vcisdoxygenmccl:`SDNPublisher` ` DataSource can be used to stream application data over the ITER SDN network (based on UDP multicast).

Typicaly use-cases include streaming data to a remote monitoring or control application.

One of the main differences between the ``SDNPublisher`` and the ``UDPSender`` DataSources is that the former is that the ``SDN`` imposes a common header and offers the possibility of streaming over multicast. The multicast addresses are encoded in the so called ``Topic`` names. 

The SDN header includes built-in fields such as an received and sent absolute time-stamps, counter, and expected topic size and **version**. These features can be used by the receiving application to implement features such as data synchronisation, data integrity checks and to detect stalled data.

In this section, the ``UDPSender`` from the previous section will be replaced by a ``SDNPublisher``.

Given that the topic name is also dependent on the system configuration, the configuration file ``../Configurations/MassSpring/RTApp-MassSpring-26-Publisher.cfg`` will be automatically updated from a ``Makefile.cfg``, as in the previous example.

The ``Makefile`` will automatically call the script ``./GenerateTopicNames.sh`` to generate a unique ``Topic`` name, which will then be used by the ``Makefile`` to update the variables ``TUTORIAL_TOPIC_NAME`` in the generated configuration file ``../Configurations/MassSpring/RTApp-MassSpring-26-Publisher_Gen.cfg``.

.. literalinclude:: /_static/tutorial/Configurations/MassSpring/RTApp-MassSpring-26-Publisher.cfg
    :language: c++
    :lines: 448-458
    :caption: SDNPublisher configuration. Note that the topic name is set from the variable ``TUTORIAL_TOPIC_NAME``, which is replaced by the ``Makefile.cfg``.
    :linenos: 
    :emphasize-lines: 3-4

.. note::

    Some Output DataSources require for a ``Trigger`` signal (see :ref:`MARTeSignalProperties`) to be explicitly declared in the GAM that interfaces with the DataSource. This is the case for the ``SDNPublisher`` DataSource. 

    .. literalinclude:: /_static/tutorial/Configurations/MassSpring/RTApp-MassSpring-26-Publisher.cfg
        :language: c++
        :lines: 344-359
        :caption: Trigger property needs to be explicitly set by at least on signal.
        :linenos: 
        :emphasize-lines: 13


Running the application
-----------------------

Start the application with:

.. code-block:: bash

    make -C ../Configurations/MassSpring/ -f Makefile.cfg
    ./MARTeApp.sh -f ../Configurations/MassSpring/RTApp-MassSpring-26-Publisher_Gen.cfg -l RealTimeLoader -s State1

Once the application is running, inspect the ``screen`` output and verify that the application is running without any issues. The log should show entries similar to the following:

.. code-block:: console

    $ [Warning - Threads.cpp:181]: Failed to change the thread priority (likely due to insufficient permissions)
    $ [Information - RealTimeLoader.cpp:111]: Started application in state State1
    $ [Information - MARTeApp.cpp:135]: Application starting
    $ [Information - LoggerBroker.cpp:152]: Time [0:0]:0
    $ [Information - LoggerBroker.cpp:152]: Time [0:0]:1000000
    ...

The ``CCS`` tool ``sdn-print`` will be used to monitor the SDN traffic and verify that the applications is streaming data over the SDN network. 

Open another console and generate the required ``.xml`` configuration files for the ``sdn-print``. 

.. code-block:: bash

    ../Test/Integrated/GenerateSDNTopicFiles.sh

Export the required environment variables for the ``sdn-print`` tool and run the tool.

.. code-block:: bash

    export SDN_TOPIC_PATH=../Test/Integrated
    sdn-print -i lo -t mass-spring-0 -c -1

Notice that both MARTe2 and the ``sdn-print`` tool are receiving from the SDN topic source using multicast.

If everything is working correctly, you should see the SDN packets being printed to the console:

.. code-block:: console

    $ Using topic definition - <topic name="mass-spring-cabrian-1" version="1" size="8" mapping="239.0.50.191:52544"><dataType format="json">{"type":"mass-spring-cabrian-1","attributes":[{"ReferencePosition":{"type":"float64"}}]}</dataType></topic>
    $ 2026-03-31T16:15:11.716842934 {"ReferencePosition":1.592}
    $ 2026-03-31T16:15:11.726838572 {"ReferencePosition":1.588}

    ...


Exercices
---------

Ex. 1: Stream the ``Stats`` signals over SDN. 
---------------------------------------------

1. Edit the file ``../Configurations/MassSpring/RTApp-MassSpring-27-Publisher.cfg`` and modify the DataSource ``UDPWriterStats`` to stream the signals to the topic identified as ``TUTORIAL_TOPIC_NAME_1``.
2. Run ``make -C ../Configurations/MassSpring/ -f Makefile.cfg`` to generate the configuration file with the correct topic names and UDP port numbers.
3. Check that the application statistics are still being streamed over SDN to the topics identified as ``TUTORIAL_TOPIC_NAME`` and ``TUTORIAL_TOPIC_NAME_1``.

.. code-block:: bash

    make -C ../Configurations/MassSpring/ -f Makefile.cfg
    ./MARTeApp.sh -f ../Configurations/MassSpring/RTApp-MassSpring-27-Publisher_Gen.cfg -l RealTimeLoader -s State1

.. code-block:: bash

    ../Test/Integrated/GenerateSDNTopicFiles.sh
    export SDN_TOPIC_PATH=../Test/Integrated
    sdn-print -i lo -t mass-spring-1 -c -1

.. code-block:: bash

    export SDN_TOPIC_PATH=../Test/Integrated
    sdn-print -i lo -t mass-spring-2 -c -1

.. dropdown:: Solution
   :icon: key

   The solution is to modify the configuration file ``../Configurations/MassSpring/RTApp-MassSpring-27-Publisher.cfg`` and complete the necessary changes on the ``UDPWriterStats`` DataSource, making sure that a ``Trigger=1`` is also explicitly declared in one of the signals interfacing with the ``UDPWriterStats`` DataSource. 
   
   For the sake of clarity the DataSource name should also be updated to reflect that it is now a ``SDNPublisher``.

   .. literalinclude:: /_static/tutorial/Configurations/MassSpring/RTApp-MassSpring-27-Publisher-solution.cfg
      :language: c++
      :caption: Updated SDNPublisher configuration.
      :linenos:
      :lines: 459-491

   .. literalinclude:: /_static/tutorial/Configurations/MassSpring/RTApp-MassSpring-27-Publisher-solution.cfg
      :language: c++
      :caption: IOGAM to with ``Trigger=1`` property set in one of the signals interfacing with the DataSource.
      :linenos:
      :lines: 360-367,398-404
      :emphasize-lines: 13
