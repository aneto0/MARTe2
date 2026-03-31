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

SDNSubscriber
=============

.. warning::
    
    The SDNSubscriber DataSource is only available in `CODAC Core System <https://www.iter.org/machine/supporting-systems/codac/codac-core-system>`__ distributions. 

The :vcisdoxygenmccl:`SDNSubscriber` DataSource can be used to receive application data over the ITER SDN network. 

Typicaly use-cases include subscribing to data from a remote application. This data can be used for monitoring or control purposes. In the latter case, the data is used to synchronise the ``RealTimeThread``.

In this section, the ``LinuxTimer`` will be replaced by a ``SDNSubscriber`` and the ``RealTimeThread`` will execute everytime a SDN topic arrives.

The publisher will be the application developed as part of the exercise from the previous section, which is streaming application data and statistics over SDN. The configuration file for the sender is ``../Configurations/MassSpring/RTApp-MassSpring-28-Sender.cfg``.

The ``SDNSubscriber`` configuration is similar to the ``UDPReceiver``, with the main difference being that the topic name is set from the variable ``TUTORIAL_TOPIC_NAME``, which is replaced by the ``Makefile.cfg``. The ExecutionMode is set to ``RealTimeThread``, which means that the pace of the ``RealTimeThread`` will be determined by the arrival of SDN topics.

Given that some application GAMs need to use a relative time reference, the ``SDNSubscriber`` also offers its ``Header`` as a signal to be further processed by other GAMs.

.. literalinclude:: /_static/tutorial/Configurations/MassSpring/RTApp-MassSpring-28-Subscriber.cfg
    :language: c++
    :lines: 1271-1285
    :caption: SDNSubscriber configuration. 
    :linenos: 
    :emphasize-lines: 3-5,7-10

The ``GAMSDNSubscriber`` is an excellent example on how an IOGAM can be used to expand and cast signals. This strategy is used to expand the binary definition of the SDN header into individual signals that can be used by other GAMs.

.. literalinclude:: /_static/tutorial/Configurations/MassSpring/RTApp-MassSpring-28-Subscriber.cfg
    :language: c++
    :lines: 5-63
    :caption: ``GAMTimer`` replaced with ``GAMSDNSubscriber``. The ``Frequency`` parameter still needs to be set to a non-zero value, but the actual pace of the ``RealTimeThread`` will be provided by the arrival of SDN packets.
    :linenos: 

Given that some GAMs require a relative time reference, a ``MathExpressionGAM`` is used to compute and maintain a relative time based on the timestamp received in the SDN header. Since this signal is of type ``uint64``, and because the ``MathExpressionGAM`` supports neither conditional statements nor the mod operator, a fairly complex expression is needed to compute the relative time in microseconds, as shown in the following code snippet. This provides a good example of why a custom GAM is often preferable for complex signal processing tasks, as discussed later in the tutorial.

.. literalinclude:: /_static/tutorial/Configurations/MassSpring/RTApp-MassSpring-28-Subscriber.cfg
    :language: c++
    :lines: 64-78
    :caption: ``MathExpressionGAM`` to compute relative time in microseconds (``uint32``).
    :linenos: 

Running the application
-----------------------

TODO HERE!!

Generate the configuration files:

.. code-block:: bash

    make -C ../Configurations/MassSpring/ -f Makefile.cfg

Start the sender application with:

.. code-block:: bash

    ./MARTeApp.sh -f ../Configurations/MassSpring/RTApp-MassSpring-24-Sender_Gen.cfg -l RealTimeLoader -s State1

Once the application is running, inspect the ``screen`` output and verify that the application is running without any issues. The log should show entries similar to the following:

.. code-block:: console

    $ [Warning - Threads.cpp:181]: Failed to change the thread priority (likely due to insufficient permissions)
    $ [Information - RealTimeLoader.cpp:111]: Started application in state State1
    $ [Information - MARTeApp.cpp:135]: Application starting
    $ [Information - LoggerBroker.cpp:152]: Time [0:0]:0
    $ [Information - LoggerBroker.cpp:152]: Time [0:0]:1000000
    ...

Open another terminal and start the receiver application with:

.. code-block:: bash

    ./MARTeApp.sh -f ../Configurations/MassSpring/RTApp-MassSpring-24-Receiver_Gen.cfg -l RealTimeLoader -s State1

Once the application is running, inspect the ``screen`` output and verify that the application is running without any issues. The log should show entries similar to the following:

.. code-block:: console

    $ [Warning - Threads.cpp:181]: Failed to change the thread priority (likely due to insufficient permissions)
    $ [Information - RealTimeLoader.cpp:111]: Started application in state State1
    $ [Information - MARTeApp.cpp:135]: Application starting
    $ [Information - LoggerBroker.cpp:152]: Time [0:0]:14000000
    ...

.. note:: The actual time values will depend on the arrival of UDP packets, which in turn depends on the execution of the sender application. If the receiver application is started before the sender, the first ``Time`` value will not be zero, but will instead correspond to the ``Time`` from the first received UDP packet.

Exercices
---------

Ex. 1: Detecting stalled data
-----------------------------

In the example above, the receiver application waits forever for UDP packets to arrive. In some cases, it may be desirable to detect if the data stream has stalled (e.g. due to a network issue or because the sender application has stopped). This can be achieved by using the ``Timeout`` parameter of the ``UDPReceiver`` DataSource, which specifies a timeout in milliseconds after which the ``UDPReceiver`` will return with a failure.

.. warning::

    A DataSource failure propagates to the interfacing GAM and from there to the ``GAMScheduler``. 
    
    This means that all the subsequent GAMs in the execution list **will not be executed**. 
    
    Consequently, if a GAM needs to gracefully handle a failure, it must be executed before the GAM that will fail (which means that it will use data from the previous cycle).

1. Edit the file ``../Configurations/MassSpring/RTApp-MassSpring-25-Receiver.cfg`` and modify the DataSource ``UDPWriterReference`` to have a ``Timeout`` of ``1.0 s``.
2. Add a ``MathExpressionGAM`` to detect stalled data based on the ``Time`` signal and name it ``UDPStalled``.
3. Add the  ``UDPStalled`` signal to the ``GAMWriterStats`` GAM and to the ``UDPWriterStats`` DataSource.
4. Add the new ``MathExpressionGAM`` to the execution list, before the ``GAMUDPReceiver`` and move the ``GAMWriterStats`` also before the ``GAMUDPReceiver``.
5. Run ``make -C ../Configurations/MassSpring/ -f Makefile.cfg`` to generate the configuration file with the correct UDP port numbers.

.. code-block:: bash

    ./MARTeApp.sh -f ../Configurations/MassSpring/RTApp-MassSpring-25-Receiver_Gen.cfg -l RealTimeLoader -s State1

.. code-block:: bash

    python ../Test/Integrated/udp_monitor.py -p TUTORIAL_UDP_PORT_4 -s 3 #Replace TUTORIAL_UDP_PORT_4 with the actual port number being used by the application. Note that the signal index is set to 3, which corresponds to expect a packet with the ``UDPStalled`` signal in the payload.

.. dropdown:: Solution
   :icon: key

   The solution is to modify the configuration file ``../Configurations/MassSpring/RTApp-MassSpring-25-Receiver.cfg`` and to add a ``MathExpressionGAM``.

   .. literalinclude:: /_static/tutorial/Configurations/MassSpring/RTApp-MassSpring-25-Receiver-solution.cfg
      :language: c++
      :caption: MathExpressionGAM to detect stalled data based on the ``Time`` signal.
      :linenos:
      :lines: 5-30

   Add the ``UDPStalled`` signal to the ``GAMWriterStats`` GAM and to the ``UDPWriterStats`` DataSource.

   .. literalinclude:: /_static/tutorial/Configurations/MassSpring/RTApp-MassSpring-25-Receiver-solution.cfg
      :language: c++
      :caption: Updated ``GAMWriterStats`` with the ``UDPStalled`` signal.
      :linenos:
      :lines: 950-957,992-997

   .. literalinclude:: /_static/tutorial/Configurations/MassSpring/RTApp-MassSpring-25-Receiver-solution.cfg
      :language: c++
      :caption: Updated ``UDPWriterStats`` with the ``UDPStalled`` signal.
      :linenos:
      :lines: 1184-1193

   Add the new ``MathExpressionGAM`` to the execution list, before the ``GAMUDPReceiver`` and move the ``GAMWriterStats`` also before the ``GAMUDPReceiver``.
    
    .. literalinclude:: /_static/tutorial/Configurations/MassSpring/RTApp-MassSpring-25-Receiver-solution.cfg
      :language: c++
      :caption: Updated execution list.
      :linenos:
      :lines: 1238-1251
      :emphasize-lines: 10


