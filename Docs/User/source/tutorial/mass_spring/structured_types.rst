.. date: 01/04/2026
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

.. _MassSpringStructuredTypes:

Structured types
================

This section describes how to define structured types in MARTe2 configuration files, given that some components require structured types to represent complex data structures. A structured type is a user-defined data type that can contain multiple fields of different types. This allows for more complex data representations and can be used to model real-world entities more accurately.

Note that this section only focus on the use of structured types in the configuration files, and does not cover the implementation of structured types in the code. For more information on how to implement structured types in the code, please refer to the :ref:`GAM documentation <StructuredGAM>`.

Structured types are very similar to ``C structs`` (defined with packed alignment), meaning that no padding is added between the fields. This allows for a straightforward mapping between the structured types defined in the configuration files and the corresponding ``C structures`` used in the code (which need to be defined with ``__attribute__((__packed__))``).

In this section, the control signals are grouped into a struct to facilitate the monitoring of the application.

.. literalinclude:: /_static/tutorial/Configurations/MassSpring/RTApp-MassSpring-34.cfg
    :language: c++
    :lines: 1-54
    :caption: Application structures definition. Note the use of structures of structures.
    :linenos:
    :emphasize-lines: 4, 46, 51

.. literalinclude:: /_static/tutorial/Configurations/MassSpring/RTApp-MassSpring-34.cfg
    :language: c++
    :lines: 637-644,811-816
    :caption: Use of structured signal in GAMDisplay.
    :linenos:
    :emphasize-lines: 12

.. important::

   MARTe2 always expands the structured types into their individual fields when the application is run. This means that the structured types are only a convenient way to group related signals together in the configuration file, but they do not have any impact on the actual data representation in memory nor on the way the signals are accessed in the components (GAMs and DataSource) code.

.. note::

   The IOGAM is typically used to convert between types that are memory compatible. In this case the flat list of the input signals of the ``GAMDisplay`` is converted into a single structured signal that is then used as an output to the ``GAMDisplay``. 

.. note::

   The use of structured types is not limited to the GAMDisplay. It can be used in any component that supports structured types (e.g. FileWriter, Logger, ...).  GAMs can also access individual fields of the structured types, in a very similar way to ``C``. The access to the individual fields of the structured types is done using the dot notation in the ``Alias`` field (e.g. ``Alias=Monitor.Control.Position``).



Running the application
-----------------------

Start the application with:

.. code-block:: bash

    ./MARTeApp.sh -f ../Configurations/MassSpring/RTApp-MassSpring-34.cfg -l RealTimeLoader -s State1

Once the application is running, inspect the ``screen`` output and verify that the log shows the structured data as expected. The log should show entries similar to the following:

.. code-block:: console

   $ [Information - LoggerBroker.cpp:152]: Monitor.Time [0:0]:1810000
   $ [Information - LoggerBroker.cpp:152]: Monitor.Control.ReferencePosition [0:0]:2.000000
   $ [Information - LoggerBroker.cpp:152]: Monitor.Control.Position [0:0]:2.184821
   $ [Information - LoggerBroker.cpp:152]: Monitor.Control.PositionDisturbed [0:0]:2.192160
   $ [Information - LoggerBroker.cpp:152]: Monitor.Control.PositionFiltered [0:0]:2.186666
   $ [Information - LoggerBroker.cpp:152]: Monitor.Control.PositionM [0:0]:2.153957
   $ [Information - LoggerBroker.cpp:152]: Monitor.Control.Velocity [0:0]:-0.147657
   $ [Information - LoggerBroker.cpp:152]: Monitor.Control.VelocityM [0:0]:-0.185259
   $ [Information - LoggerBroker.cpp:152]: Monitor.Control.PositionErr [0:0]:0.186666
   $ [Information - LoggerBroker.cpp:152]: Monitor.Control.Force [0:0]:21.471177
   $ [Information - LoggerBroker.cpp:152]: Monitor.Control.ForceAverage [0:0]:15.088476
   $ [Information - LoggerBroker.cpp:152]: Monitor.Control.ForceStdDev [0:0]:7.428618
   $ [Information - LoggerBroker.cpp:152]: Monitor.Control.ForceMax [0:0]:3.420744
   $ [Information - LoggerBroker.cpp:152]: Monitor.Control.ForceMin [0:0]:30.000000
   $ [Information - LoggerBroker.cpp:152]: Thread1CycleTime [0:0]:9992
   ...

Exercices
---------

Ex. 1: Structured statistics
----------------------------

1. Edit the file ``../Configurations/MassSpring/RTApp-MassSpring-35.cfg`` and add structured types that match the following C structures:

   .. code-block:: c++

      struct ControlParameters {
         float64 ReferencePosition;
         float64 Position;
         float64 PositionDisturbed;
         float64 PositionFiltered;
         float64 PositionM;
         float64 Velocity;
         float64 VelocityM;
         float64 PositionErr;
         float64 Force;
         float64 ForceAverage;
         float64 ForceStdDev;
         float64 ForceMax;
         float64 ForceMin;
      };

      struct GAMPerformance {
         uint32 ReadTime;
         uint32 ExecTime;
         uint32 WriteTime;
      };

      struct ThreadPerformance {
         uint32 CycleTime;
         uint32 CycleTimeAverage;
         float32 CycleTimeMovingAverage;
         uint32 CycleTimeStdDev;
         uint32 CycleTimeMax;
         uint32 CycleTimeMin;
         uint32 CycleTimeHistogram[11];
         uint32 FreeTimeHistogram[11];
         uint32 GAMsExecutionTime;
      };

      struct AppPerformance {
         GAMPerformance GAMTimer;
         GAMPerformance GAMReference;
         GAMPerformance GAMController;
         GAMPerformance GAMSpringMass;
         GAMPerformance GAMPerfMonitor;
         GAMPerformance GAMDisplay;
         ThreadPerformance Thread1;
      };

      struct Monitor {
         uint32 Time;
         ControlParameters Control;
         AppPerformance Performance;
      };


2. Modify the GAMDisplay to output the full structure.

.. code-block:: bash

    ./MARTeApp.sh -f ../Configurations/MassSpring/RTApp-MassSpring-35.cfg -l RealTimeLoader -s State1

.. dropdown:: Solution
   :icon: key

   The solution is to modify the add the types to the configuration file.

   .. literalinclude:: /_static/tutorial/Configurations/MassSpring/RTApp-MassSpring-35-solution.cfg
      :language: c++
      :lines: 1-125
      :caption: Structured type definition. 
      :linenos: 

   Add add the structured type to the GAMDisplay configuration.

   .. literalinclude:: /_static/tutorial/Configurations/MassSpring/RTApp-MassSpring-35-solution.cfg
      :language: c++
      :lines: 708-715, 882-887
      :caption: GAMDisplay configuration. 
      :linenos: 
      :emphasize-lines: 12

