.. date: 24/03/2026
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

FileWriter
==========

On the previous sections the application monitoring was based on the use of the Logger, which is a powerful tool to log any information from the application, but it has some obvious limitations. Namely it does not store the files and the output can quicly become overwhelming if the application is running for a long time. 

In this section, data will be stored into a file using the ``FileWriter`` DataSource. This DataSource can be configured to store the data in a binary or text CSV format. 

Many  MARTe2 data sources, including the ``FileWriter`` data source, have decoupling mechanisms so that the writing of data does not interfere with the real-time execution of the application.

These mechanisms typically involve buffering the data in memory and writing it to the file asynchronously on a separate thread (which may be allocated to a different CPU core).

.. literalinclude:: /_static/tutorial/Configurations/MassSpring/RTApp-MassSpring-18.cfg
    :language: c++
    :lines: 919-934
    :caption: FileWriter configuration.
    :linenos:
    :emphasize-lines: 6,8

.. note::

    Many of these data-sources also have the built-in capability of the writing the data based on an external trigger, also allowing to specify the number of pre and post-samples to be stored when the trigger occurs.


Running the application
-----------------------

Start the application with:

.. code-block:: bash

    ./MARTeApp.sh -f ../Configurations/MassSpring/RTApp-MassSpring-18.cfg -l RealTimeLoader -s State1

Once the application is running, inspect the ``screen`` output and verify that the application is running without any issues. The log should show entries similar to the following:

.. code-block:: console

    $ [Warning - Threads.cpp:181]: Failed to change the thread priority (likely due to insufficient permissions)
    $ [Information - RealTimeLoader.cpp:111]: Started application in state State1
    $ [Information - MARTeApp.cpp:135]: Application starting
    ...

Open another console and check the content of the log file (e.g. ``../Test/Integrated/MassSpring-18.csv``) and verify that the data is being stored in the file as expected. The log should show entries similar to the following:

.. code-block:: console

   less ../Test/Integrated/MassSpring-18.csv

   #Time (uint32)[1],ReferencePosition (float64)[1],Position (float64)[1],PositionDisturbed (float64)[1],PositionFiltered (float64)[1],PositionM (float64)[1],Velocity (float64)[1],VelocityM (float64)[1],PositionErr (float64)[1],Force (float64)[1],Thread1CycleTime (uint32)[1],GAMTimer_ReadTime (uint32)[1],GAMTimer_ExecTime (uint32)[1],GAMTimer_WriteTime (uint32)[1],GAMReference_ExecTime (uint32)[1],GAMReference_WriteTime (uint32)[1],GAMController_ReadTime (uint32)[1],GAMController_ExecTime (uint32)[1],GAMController_WriteTime (uint32)[1],GAMSpringMass_ReadTime (uint32)[1],GAMSpringMass_ExecTime (uint32)[1],GAMSpringMass_WriteTime (uint32)[1],GAMPerfMonitor_ReadTime (uint32)[1],GAMPerfMonitor_ExecTime (uint32)[1],GAMPerfMonitor_WriteTime (uint32)[1],GAMFileWriter_ReadTime (uint32)[1],GAMFileWriter_ExecTime (uint32)[1],GAMFileWriter_WriteTime (uint32)[1],Thread1CycleTimeAverage (uint32)[1],Thread1CycleTimeMovingAverage (float32)[1],Thread1CycleTimeStdDev (uint32)[1],Thread1CycleTimeMax (uint32)[1],Thread1CycleTimeMin (uint32)[1],Thread1CycleTimeHistogram (uint32)[11],Thread1FreeTimeHistogram (uint32)[11],GAMsExecutionTime (uint32)[1],ForceAverage (float64)[1],ForceStdDev (float64)[1],ForceMax (float64)[1],ForceMin (float64)[1]
    0,2.000000,0,0,0,0,0,0.300000,-2.000000,30.000000,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,{ 0 0 0 0 0 0 0 0 0 0 0 } ,{ 0 0 0 0 0 0 0 0 0 0 0 } ,0,0.150000,2.116010,30.000000,30.000000
    20000,2.000000,0.001496,0.009511,0.009097,0.003000,0.299177,0.598500,-1.990903,30.000000,0,9849,9850,9851,9852,9853,9874,9875,9876,9876,9883,9883,5,7,10,9932,9933,9941,0,0,0,0,0,{ 0 0 0 0 0 0 0 0 0 0 0 } ,{ 0 0 0 0 0 0 0 0 0 0 0 } ,92,0.300000,2.984962,30.000000,30.000000
    ...

The data can be plot using any plotting tool (e.g. ``gnuplot``) and should show the expected behaviour of the system.

A python based plotting script is also available in ``../Test/Integrated/plot_mass_spring_csv.py``. To run the script, execute the following command:

.. code-block:: bash

    python ../Test/Integrated/plot_mass_spring_csv.py -f ../Test/Integrated/MassSpring-18.csv -s ReferencePosition Position

Exercices
---------

Ex. 1: Log execution every second
---------------------------------

As it can be seen when executing the application the log when is now totally silent. Add a GAMDisplay to log the time once per second.

Also make sure that data is only written to the file for the first 10 seconds of the application execution, using the DataSource built-in triggering mechanism.

1. Edit the file ``../Configurations/MassSpring/RTApp-MassSpring-19.cfg`` and add a GAM to compute the display trigger. Note that the ``MathExpressionGAM`` does not implement the ``mod`` operator. A possible solution is to divide the ``Time`` by the desired period (in ``uint32``) and then multiply the result by the period again. If the result is equal to the original time, then the trigger condition is met.
2. Add a GAM to trigger the Time into the ``LoggerDataSource``.
3. Modify the ``FileWriter`` DataSource to use the trigger signal to only write data for the first 10 seconds of the application execution.
4. Modify the ``FileWriter`` DataSource to write into a file named ``../Test/Integrated/MassSpring-19.csv``.
5. Check that the application is only writing to the display once per second and that data is only being written to the file for the first 10 seconds of the application execution.

.. code-block:: bash

    ./MARTeApp.sh -f ../Configurations/MassSpring/RTApp-MassSpring-19.cfg -l RealTimeLoader -s State1

.. dropdown:: Solution
   :icon: key

   The solution is to add a ``MathExpressionGAM`` implemeting the equation defined above.

   .. literalinclude:: /_static/tutorial/Configurations/MassSpring/RTApp-MassSpring-19-solution.cfg
      :language: c++
      :lines: 575-597
      :caption: MathExpressionGAM to compute the display trigger.
      :linenos:

   To modify the ``FileWriter`` DataSource to use the trigger signal, the ``Trigger`` parameter needs to be added to the DataSource configuration.

   .. literalinclude:: /_static/tutorial/Configurations/MassSpring/RTApp-MassSpring-19-solution.cfg
      :language: c++
      :lines: 969-988
      :caption: Updated FileWriter with Trigger signal.
      :linenos:
      :emphasize-lines: 15-17

Ex. 2: Use the Refresh for live monitoring
------------------------------------------

The ``FileWriter`` DataSource has a built-in capability to refresh the file content instead of appending to it. This can be used to monitor the application behaviour in real-time using an external tool (e.g. ``watch -n 1 cat ../Test/Integrated/MassSpring-20-stats.csv``).

1. Edit the file ``../Configurations/MassSpring/RTApp-MassSpring-20.cfg`` and add a ``FileWriter`` DataSource to store the signals ``Thread1CycleTime``, ``Thread1CycleTimeAverage``, ``Thread1CycleTimeMovingAverage``, ``Thread1CycleTimeStdDev``, ``Thread1CycleTimeMax``, ``Thread1CycleTimeMin``, ``Thread1CycleTimeHistogram``, ``Thread1FreeTimeHistogram``, ``GAMsExecutionTime``.
2. Make sure that the property ``RefreshContent`` is set to ``1`` in the DataSource configuration.
3. Write the data into a file named ``../Test/Integrated/MassSpring-20-stats.csv``.
4. Add a GAM write the signals to the DataSource.
5. Add the GAM to the execution list.
6. Check that the file is being refreshed and that the content is updated with the latest values of the signals.

.. code-block:: bash

    ./MARTeApp.sh -f ../Configurations/MassSpring/RTApp-MassSpring-20.cfg -l RealTimeLoader -s State1

.. dropdown:: Solution
   :icon: key

   The solution is to add a ``FileWriterStats`` with the signals.

   .. literalinclude:: /_static/tutorial/Configurations/MassSpring/RTApp-MassSpring-20-solution.cfg
      :language: c++
      :lines: 1193-1237
      :caption: FileWriterStats to store the signals.
      :linenos:
      :emphasize-lines: 11

   Add the GAM to write the signals to the DataSource.

   .. literalinclude:: /_static/tutorial/Configurations/MassSpring/RTApp-MassSpring-20-solution.cfg
      :language: c++
      :lines: 956-963, 998-1003
      :caption: GAM to write the signals to the DataSource. 
      :linenos:

