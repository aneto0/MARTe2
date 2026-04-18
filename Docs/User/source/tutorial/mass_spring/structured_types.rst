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

This section describes how to define structured types in MARTe2 configuration files. Some components require structured types to represent complex data structures. A structured type is a user-defined data type that can contain multiple fields of different types, enabling more expressive and realistic data modelling.

Note that this section focuses only on the use of structured types in configuration files and does not cover their implementation in code. For more details on implementing structured types, refer to the :ref:`GAM documentation <StructuredGAM>`.

Structured types are very similar to ``C structs`` (defined with packed alignment), meaning that no padding is added between fields. This enables a direct mapping between structured types defined in configuration files and the corresponding ``C structures`` in code (which must be defined with ``__attribute__((__packed__))``).

In this section, control signals are grouped into a structure to simplify application monitoring.

.. literalinclude:: /_static/tutorial/Configurations/MassSpring/RTApp-MassSpring-34.cfg
    :language: c++
    :lines: 1-54
    :caption: Application structures definition (including nested structures).
    :linenos:
    :emphasize-lines: 4, 46, 51

.. literalinclude:: /_static/tutorial/Configurations/MassSpring/RTApp-MassSpring-34.cfg
    :language: c++
    :lines: 637-644,811-816
    :caption: Use of structured signal in GAMDisplay.
    :linenos:
    :emphasize-lines: 12

.. important::

   MARTe2 expands structured types into their individual fields at runtime. Structured types are therefore only a convenient way to group related signals in the configuration file; they do not affect the in-memory representation or how signals are accessed in component (GAM and DataSource) code.

.. note::

   The IOGAM is typically used to convert between memory-compatible types. In this example, a flat list of input signals is converted into a single structured signal, which is then passed to the ``GAMDisplay``.

.. note::

   Structured types are not limited to ``GAMDisplay``. They can be used in any component that supports them. GAMs can also access individual fields using a syntax similar to ``C``, via dot notation in the ``Alias`` field (e.g. ``Alias=Monitor.Control.Position``).

.. important::

   When loading registered types, the MARTe2 application may output several warning messages such as:

   .. code-block:: c++

      $ [Warning - LoadableLibrary.cpp:78]: LoadableLibrary: Failed dlopen(): ControlParameters.so: cannot open shared object file: No such file or directory
      $ [Warning - LoadableLibrary.cpp:78]: LoadableLibrary: Failed dlopen(): ControlParameters.gam: cannot open shared object file: No such file or directory
      $ [Warning - LoadableLibrary.cpp:78]: LoadableLibrary: Failed dlopen(): ControlParameters.drv: cannot open shared object file: No such file or directory
      $ [Information - IntrospectionStructure.cpp:270]: Registering type ControlParameters
      $ [Warning - LoadableLibrary.cpp:78]: LoadableLibrary: Failed dlopen(): GAMPerformance.so: cannot open shared object file: No such file or directory
      $ [Warning - LoadableLibrary.cpp:78]: LoadableLibrary: Failed dlopen(): GAMPerformance.gam: cannot open shared object file: No such file or directory
      $ [Warning - LoadableLibrary.cpp:78]: LoadableLibrary: Failed dlopen(): GAMPerformance.drv: cannot open shared object file: No such file or directory
      $ [Information - IntrospectionStructure.cpp:270]: Registering type GAMPerformance
      ...

   These messages are expected and can be safely ignored. They occur because MARTe2 attempts to load structured types as components (e.g. GAMs or DataSources). Since they are not components, loading fails and warnings are issued. However, as long as messages such as:

   ``[Information - IntrospectionStructure.cpp:270]: Registering type <type_name>``

   are present, the structured types are correctly registered and can be used without issues.

Running the application
-----------------------

Start the application with:

.. code-block:: bash

    ./MARTeApp.sh -f ../Configurations/MassSpring/RTApp-MassSpring-34.cfg -l RealTimeLoader -s State1

Once running, verify that structured data is correctly displayed in the logs:

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

Exercises
---------

Ex. 1: Structured statistics
----------------------------

1. Edit ``../Configurations/MassSpring/RTApp-MassSpring-35.cfg`` and define structured types matching the following C structures:

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

2. Modify ``GAMDisplay`` to output the full structured signal.

Run the application:

.. code-block:: bash

    ./MARTeApp.sh -f ../Configurations/MassSpring/RTApp-MassSpring-35.cfg -l RealTimeLoader -s State1

.. dropdown:: Solution
   :icon: key

   Add the structured types to the configuration file:

   .. literalinclude:: /_static/tutorial/Configurations/MassSpring/RTApp-MassSpring-35-solution.cfg
      :language: c++
      :lines: 1-125
      :caption: Structured type definitions.
      :linenos: 

   Then update the ``GAMDisplay`` configuration:

   .. literalinclude:: /_static/tutorial/Configurations/MassSpring/RTApp-MassSpring-35-solution.cfg
      :language: c++
      :lines: 708-715, 882-887
      :caption: GAMDisplay configuration using structured types.
      :linenos: 
      :emphasize-lines: 12