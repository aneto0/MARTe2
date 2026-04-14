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

The MARTe2 :vcisdoxygencl:`HttpService` (see also :ref:`MARTeHTTPService`) allows to interact with MARTe2 application over ``HTTP``.

Is allows to query using ``JSON`` the internal state of any component in the application. By default, the ``GAMs`` and ``DataSources`` expose the values of their signals, which can be useful to implement dashboards using other tools. Another interesting feature is to browse and navigate through the application structure. 

In this example the HTTPService is used to query the internal state of the application and to implement a simple HTML based dashboard of the MassSpring. The :vcisdoxygencl:`HttpMessageInterface` component is used to send messages to the application to change the state of the StateMachine and thus change the reference position of the system.

Given that the ``HTTPService`` port number needs to be unique, in order to avoid clashes,  the configuration file ``../Configurations/MassSpring/RTApp-MassSpring-53.cfg`` will be automatically updated from a ``Makefile.cfg``. 

.. literalinclude:: /_static/tutorial/Configurations/MassSpring/RTApp-MassSpring-53.cfg
    :language: c++
    :lines: 4-50
    :caption: HttpService configuration.
    :linenos:
    :emphasize-lines: 10, 16, 40

Running the application
-----------------------

Start the first application with:

.. code-block:: bash

    make -C ../Configurations/MassSpring/ -f Makefile.cfg
    ./MARTeApp.sh -f ../Configurations/MassSpring/RTApp-MassSpring-53_Gen.cfg -l RealTimeLoader -m StateMachine::START

Once the application is running, inspect the ``screen`` output and verify that the application is running without any issues. The log should show entries similar to the following:

.. code-block:: console

    $ [Warning - Threads.cpp:185]: Failed to change the thread priority (likely due to insufficient permissions)
    $ [Information - StateMachine.cpp:340]: In state (INITIAL) triggered message (StartNextStateExecutionRTApp)
    $ [Information - LoggerBroker.cpp:152]: Time [0:0]:990000
    $ [Information - LoggerBroker.cpp:152]: ReferencePosition [0:0]:0.396000
    ...

1. Open a web browser and navigate to ``http://localhost:HTTP_PORT``, where ``HTTP_PORT`` is the port number assigned to the ``HttpService``. 
2. Navigate to (by clicking on the ``[+]`` button):

::

    [-] ObjectBrowse (HttpObjectBrowser) 
        [-] MassSpringApp (RealTimeApplication)
            [-] Functions (ReferenceContainer)
                [-] GAMWriter (IOGAM) [>] 0x0 
                    
3. Check that value of all the signals are being updated.
4. Open another browser tab and navigate to ``http://localhost:HTTP_PORT/ObjectBrowse/MassSpringApp/Functions/GAMWriter?TextMode=0``. Verify that the JSON is being retrived.
5. Open another browser tab and navigate to ``http://localhost:HTTP_PORT/?path=MassSpring-1.html``. Verify that a simple dashboard is shown and updated.
6. Open another browser tab and navigate to the ``HttpMessageInterface``:

::

    [-] ObjectBrowse (HttpObjectBrowser) 
        [-] HttpMessageInterface (HttpMessageInterface) [>] 0x0

7. Click on ``GOTO_CONSTANT_REF`` and observe on the dashboard that the position settles at the constant reference.
8. Click on ``GOTO_SWITCH_OFF`` and observe on the dashboard that the position decays to zero.
 

