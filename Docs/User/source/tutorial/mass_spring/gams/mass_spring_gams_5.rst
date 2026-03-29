.. date: 27/03/2026
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

Filters and type conversion
===========================

The last exercise on the previous section introduced a disturbance to the system. 

In this section we will add a filter before the controller to mitigate the effect of the disturbance.

The filter is implemented using the :vcisdoxygenmccl:`FilterGAM`, and is configured as a notch filter filtered at at 10 Hz.

.. literalinclude:: /_static/tutorial/Configurations/MassSpring/RTApp-MassSpring-14.cfg
    :language: c++
    :lines: 80-101
    :caption: FilterGAM configuration.
    :linenos:
    :emphasize-lines: 3-4

Running the application
-----------------------

Start the application with:

.. code-block:: bash

    ./MARTeApp.sh -f ../Configurations/MassSpring/RTApp-MassSpring-14.cfg -l RealTimeLoader -s State1

Once the application is running, inspect the ``screen`` output and verify that the log shows the ``Position`` converging to the ``ReferencePosition`` value despite the ``PositionDisturbed`` (which is the input to the filter) varying over time. The log should show entries similar to the following:

.. code-block:: console

   $ [Information - LoggerBroker.cpp:152]: Time [0:0]:151070000
   $ [Information - LoggerBroker.cpp:152]: ReferencePosition [0:0]:2.000000
   $ [Information - LoggerBroker.cpp:152]: Position [0:0]:2.000000
   $ [Information - LoggerBroker.cpp:152]: PositionDisturbed [0:0]:1.990489
   $ [Information - LoggerBroker.cpp:152]: PositionFiltered [0:0]:2.000000

Exercices
---------

Ex. 1: Simple statistics 
------------------------

1. Edit the file ``../Configurations/MassSpring/RTApp-MassSpring-15.cfg`` and add a ``FilterGAM`` to implement a moving average of the ``Thread1CycleTime`` and compare it with the one being computed by the ``StatisticsGAM``. The moving average should be computed over a window of 100 ms.
2. Given that the ``FilterGAM`` does not accept ``uint32`` signals, use a :vcisdoxygenmccl:`ConversionGAM` to convert the ``Thread1CycleTime`` signal to a ``float32`` before feeding it to the ``FilterGAM``.

.. dropdown:: Solution
   :icon: key

   The solution is to add a ``ConversionGAM`` to convert the signal.

   .. literalinclude:: /_static/tutorial/Configurations/MassSpring/RTApp-MassSpring-15-solution.cfg
      :language: c++
      :lines: 444-458
      :caption: Type conversion.
      :linenos:
      :emphasize-lines: 6,12

   And to add a ``FilterGAM`` to compute the moving average.

   .. literalinclude:: /_static/tutorial/Configurations/MassSpring/RTApp-MassSpring-15-solution.cfg
      :language: c++
      :lines: 459-478
      :caption: Moving average filter.
      :linenos:
      :emphasize-lines: 3-4

   Finally, add the output signal of the FilterGAM to the ``GAMDisplay`` and the GAMs to the execution list.
