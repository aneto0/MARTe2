.. date: 24/03/2026
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

Statistics 
==========

This section continues to build upon the previous example and adds features that allow computing runtime statistics on the MARTe2 signals.

The MARTe2-components repository offers two GAMs that can be used to implement statistics:

- :vcisdoxygenmccl:`StatisticsGAM`, provides average, standard deviation, minimum and maximum of its input signal over a moving time window.

- :vcisdoxygenmccl:`HistogramGAM`, computes histograms from the input signal values.

In this example, the ``StatisticsGAM`` is used to compute the average and standard deviation of the cycle time, while the ``HistogramGAM`` is used to compute a histogram of the cycle time values.

.. note::
    The number of bins of the HistogramGAM is computed using the output signal number of elements. The size of the bin is automatically computed as :math:`(MaxValue - MinValue) / (NumberOfElements - 2)`. This size is cast to the input signal type and thus may lead to rounding errors.

    In case the input signal is an integer, it is recommended to satisfy the equation above such that the size of the bin is also an exact integer.

   .. literalinclude:: /_static/tutorial/Configurations/MassSpring/RTApp-MassSpring-5.cfg
      :language: c++
      :lines: 285-303
      :caption: HistogramGAM configuration.
      :linenos:
      :emphasize-lines: 8-9, 16

Running the application
-----------------------

Start the application with:

.. code-block:: bash

    ./MARTeApp.sh -f ../Configurations/MassSpring/RTApp-MassSpring-5.cfg -l RealTimeLoader -s State1

Once the application is running, inspect the ``screen`` output and verify that the log shows the cycle time average, maximum, and minimum values, as well as the standard deviation and the histogram values. The log should show entries similar to the following:

.. code-block:: console

    $ [Information - LoggerBroker.cpp:152]: Thread1CycleTimeAverage [0:0]:10002
    $ [Information - LoggerBroker.cpp:152]: Thread1CycleTimeStdDev [0:0]:61
    $ [Information - LoggerBroker.cpp:152]: Thread1CycleTimeMax [0:0]:9859
    $ [Information - LoggerBroker.cpp:152]: Thread1CycleTimeMin [0:0]:10131
    $ [Information - LoggerBroker.cpp:152]: Thread1CycleTimeHistogram [0:10]:{ 5 0 0 0 2 888 1 0 0 1 4 }

Exercises
---------

Ex. 1: Add the force statistics 
-------------------------------

1. Edit the file ``../Configurations/MassSpring/RTApp-MassSpring-6.cfg`` and add a ``StatisticsGAM`` to compute information about the ``Force`` signal.

.. warning::

    MARTe2 does not distinguish between a scalar signal (``NumberOfDimensions = 0``, ``NumberOfElements = 1``) and a single-element vector signal (``NumberOfDimensions = 1``, ``NumberOfElements = 1``).

    However, some components impose specific requirements. For example, the ``PIDGAM`` requires its output to be a single-element vector, while the ``StatisticsGAM`` requires its input to be a scalar.

    .. literalinclude:: /_static/tutorial/Configurations/MassSpring/RTApp-MassSpring-6.cfg
      :language: c++
      :lines: 39, 62-69
      :caption: PIDGAM output signal requirements.
      :linenos:
      :emphasize-lines: 6

    Consequently, connecting the output of the ``PIDGAM`` to the input of the ``StatisticsGAM`` requires an intermediate GAM to convert between these definitions. Otherwise, the application will report conflicting signal requirements.

    .. literalinclude:: /_static/tutorial/Configurations/MassSpring/RTApp-MassSpring-6.cfg
      :language: c++
      :lines: 285-303
      :caption: GAM to convert between scalar and single-element vector signals.
      :linenos:
      :emphasize-lines: 8, 16

    Note that the IOGAM performs a memcpy of all the input signals memory into the output signals memory, only requiring size compatibility between the input and output signals. This allows the IOGAM to be used as a simple pass-through GAM to convert between different signal definitions, without the need to implement a custom GAM for this purpose.

.. warning::

  Because the IOGAM performs a ``memcpy`` of the input signal memory into the output signal memory, it does not perform any transformation of the signal values and will copy the signals in the same order as they are defined in the configuration file.
  
  This means that if the order is not maintained consistently, the output signals will not match the expected input signal order and/or be cast to the incorrect type.

  For example:

  .. code-block:: c++

      +MyIOGAM = {
          Class = IOGAM
          InputSignals = {
            InputReference = {
                DataSource = Driver1
                Type = float64
            }
            Measurement = {
                DataSource = Driver2
                Type = float64
            }
          }
          OutputSignals = {
            Measurement = {
                DataSource = DDB1
                Type = float64
            }
            InputReference = {
                DataSource = DDB1
                Type = float64
            }
          }
       }

  Would swap the ``InputReference`` and ``Measurement`` signals.

.. code-block:: bash

    ./MARTeApp.sh -f ../Configurations/MassSpring/RTApp-MassSpring-6.cfg -l RealTimeLoader -s State1

.. dropdown:: Solution
   :icon: key

   The solution is to add a new GAM and connect all the signals to the ``GAMDisplay``.

   .. literalinclude:: /_static/tutorial/Configurations/MassSpring/RTApp-MassSpring-6-solution.cfg
      :language: c++
      :lines: 302-330
      :caption: Updated configuration with the Force statistics.
      :linenos:

Ex. 2: Add an additional histogram for the GAMTimer read signal
---------------------------------------------------------------

The GAMTimer Read signal provides a good indication of the amount of free time in the thread and thus of the system load. Adding a histogram of this signal can help to identify if the system is overloaded and whether the cycle time is sufficient for the application.

1. Edit the file ``../Configurations/MassSpring/RTApp-MassSpring-7.cfg`` and add a signal to the HistogramGAM to compute a histogram of the ``GAMTimer_ReadTime`` signal.
2. Rename the signal in the output as ``Thread1FreeTimeHistogram``.
3. Use as ``MinLim`` the value ``2600`` and as ``MaxLim`` the value ``9800``.

  1. Why are the Min and Max limits of the histogram set to these values? What is the expected range of values for the ``GAMTimer_ReadTime`` signal?
  2. Why were these two exact numbers chosen (and not e.g. ``2000`` and ``10000``)? What is the bin size of the histogram?


.. dropdown:: Solution
   :icon: key

   The solution is to add the signal to the HistogramGAM and connect it to the ``GAMDisplay``.

   .. literalinclude:: /_static/tutorial/Configurations/MassSpring/RTApp-MassSpring-7-solution.cfg
      :language: c++
      :lines: 331-360
      :caption: Updated configuration with the Force statistics.
      :linenos: 
      :emphasize-lines: 11, 24