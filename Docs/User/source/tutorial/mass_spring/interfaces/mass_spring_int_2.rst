.. date: 14/04/2026
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

HttpService
===========

The MARTe2 :vcisdoxygencl:`HttpService` (see also :ref:`MARTeHTTPService`) allows interaction with a MARTe2 application over ``HTTP``.

It enables querying the internal state of any component using ``JSON``. By default, ``GAMs`` and ``DataSources`` expose their signal values, which can be useful for building dashboards with external tools. Another useful feature is the ability to browse and navigate the application structure.

In this example, the ``HttpService`` is used to query the internal state of the application and to implement a simple HTML-based dashboard for the MassSpring system. The :vcisdoxygencl:`HttpMessageInterface` component is used to send messages to the application, allowing the StateMachine to change state and thus modify the reference position.

Since the ``HttpService`` port must be unique to avoid conflicts, the configuration file ``../Configurations/MassSpring/RTApp-MassSpring-53.cfg`` is automatically updated by ``Makefile.cfg``.

.. literalinclude:: /_static/tutorial/Configurations/MassSpring/RTApp-MassSpring-53.cfg
    :language: c++
    :lines: 4-50
    :caption: HttpService configuration.
    :linenos:
    :emphasize-lines: 10, 16, 40

Running the application
-----------------------

Start the application (note that ``HTTP_PORT`` is automatically assigned and printed):

.. code-block:: bash

    make -C ../Configurations/MassSpring/ -f Makefile.cfg
    HTTP_PORT=`awk '/\+WebServer/,/}/ {if ($1=="Port") print $3}' ../Configurations/MassSpring/RTApp-MassSpring-53_Gen.cfg`;echo "HTTP_PORT=$HTTP_PORT"
    ./MARTeApp.sh -f ../Configurations/MassSpring/RTApp-MassSpring-53_Gen.cfg -l RealTimeLoader -m StateMachine::START

Verify that the application is running correctly:

.. code-block:: console

    $ [Information - LoggerBroker.cpp:152]: Time [0:0]:990000
    $ [Information - LoggerBroker.cpp:152]: ReferencePosition [0:0]:0.396000

1. Open a web browser and navigate to:

   ``http://localhost:HTTP_PORT``

2. Browse the application structure:

::

    [-] ObjectBrowse (HttpObjectBrowser) 
        [-] MassSpringApp (RealTimeApplication)
            [-] Functions (ReferenceContainer)
                [-] GAMWriter (IOGAM)

3. Verify that signal values are updating.
4. Open:

   ``http://localhost:HTTP_PORT/ObjectBrowse/MassSpringApp/Functions/GAMWriter?TextMode=0``

   and confirm that JSON data is returned.

5. Open:

   ``http://localhost:HTTP_PORT/?path=MassSpring-1.html``

   to view the dashboard.

6. Navigate to the ``HttpMessageInterface``:

::

    [-] ObjectBrowse
        [-] HttpMessageInterface (HttpMessageInterface)

7. Click ``GOTO_CONSTANT_REF`` and observe the system settling at a constant reference.
8. Click ``GOTO_SWITCH_OFF`` and observe the position decaying to zero.

Exercises
---------

Ex. 1: Change the reference position via HttpMessageInterface
-------------------------------------------------------------

The objective is to add a message that directly sets a new reference value in the :vcisdoxygenmccl:`ConstantGAM` ``GAMReference``.

1. Edit ``../Configurations/MassSpring/RTApp-MassSpring-54.cfg`` and add a new message to set the value ``3`` (an example for value ``1`` is already provided).

2. Start the application:

.. code-block:: bash

    make -C ../Configurations/MassSpring/ -f Makefile.cfg
    HTTP_PORT=`awk '/\+WebServer/,/}/ {if ($1=="Port") print $3}' ../Configurations/MassSpring/RTApp-MassSpring-54_Gen.cfg`;echo "HTTP_PORT=$HTTP_PORT"
    ./MARTeApp.sh -f ../Configurations/MassSpring/RTApp-MassSpring-54_Gen.cfg -l RealTimeLoader -m StateMachine::START

3. Verify output:

.. code-block:: console

    $ [Warning - Threads.cpp:185]: Failed to change the thread priority (likely due to insufficient permissions)
    $ [Information - StateMachine.cpp:340]: In state (INITIAL) triggered message (StartNextStateExecutionRTApp)
    $ [Information - LoggerBroker.cpp:152]: Time [0:0]:990000
    $ [Information - LoggerBroker.cpp:152]: ReferencePosition [0:0]:2.000000

4. Open the ``HttpMessageInterface`` in the browser:

::

    [-] ObjectBrowse
        [-] HttpMessageInterface (HttpMessageInterface)

5. Click the new message and verify that ``ReferencePosition`` changes to ``3``.

.. dropdown:: Solution
   :icon: key

   Add the new message:

   .. literalinclude:: /_static/tutorial/Configurations/MassSpring/RTApp-MassSpring-54-solution.cfg
      :language: c++
      :lines: 19-61
      :caption: Updated HttpMessageInterface configuration.
      :linenos:
      :emphasize-lines: 32-42