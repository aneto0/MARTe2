.. date: 27/03/2026
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

Other GAMs
==========

This section concludes the introduction to the GAMs used in the mass-spring-damper example, listing other commonly used GAMs that are not covered in the previous sections.

- :vcisdoxygenmccl:`CRCGAM`: computes the CRC checksum of the input signals.
- :vcisdoxygenmccl:`MessageGAM`: sends :vcisdoxygencl:`Message` events on the basis of commands received in the input signals.
- :vcisdoxygenmccl:`MuxGAM`: selects the signal to be output based on a selector signal.
- :vcisdoxygenmccl:`SimulinkWrapperGAM`: encapsulates a shared library generated from a Simulink(r) model (requires the Simulink Coder(r) toolbox to generate the model, but not to execute the GAM).
- :vcisdoxygenmccl:`TriggeredIOGAM`: writes to an output DataSource only when a trigger condition is met.

The ``TriggeredIOGAM`` is commonly used to write to an output DataSource (e.g. logger, file, ...) only when a trigger condition is met. In the mass-spring-damper example, the ``TriggeredIOGAM`` is used to log the system state only until the position converges to its final value.

.. literalinclude:: /_static/tutorial/Configurations/MassSpring/RTApp-MassSpring-16.cfg
    :language: c++
    :lines: 557-573
    :caption: MathExpressionGAM configuration to trigger while the position error is greater than a given value.
    :linenos:
    :emphasize-lines: 3-5

.. literalinclude:: /_static/tutorial/Configurations/MassSpring/RTApp-MassSpring-16.cfg
    :language: c++
    :lines: 575-581
    :caption: GAMDisplay ``Class`` modified to ``TriggeredIOGAM`` and addition of the required ``Trigger`` signal.
    :linenos:
    :emphasize-lines: 2

The complete list of GAMs is available in the GitLab repository at :vcisgl:`MARTe2-components/Source/Components/GAMs`.

Running the application
-----------------------

Start the application with:

.. code-block:: bash

    ./MARTeApp.sh -f ../Configurations/MassSpring/RTApp-MassSpring-16.cfg -l RealTimeLoader -s State1

Once the application is running, inspect the ``screen`` output and verify that the log shows the ``Position`` converging to the ``ReferencePosition`` value and stops logging once the position converges. The log should show entries similar to the following:

.. code-block:: console

   $ [Information - LoggerBroker.cpp:152]: Velocity [0:0]:-0.041142
   $ [Information - LoggerBroker.cpp:152]: VelocityM [0:0]:-0.096766
   $ [Information - LoggerBroker.cpp:152]: PositionErr [0:0]:0.001040
   $ [Information - LoggerBroker.cpp:152]: Force [0:0]:20.093487

Exercises
---------

Ex. 1: Time-based logging
-------------------------

1. Edit the file ``../Configurations/MassSpring/RTApp-MassSpring-17.cfg`` and modify the trigger signal to log only the first 10 seconds of the application.

.. dropdown:: Solution
   :icon: key

   The solution is to modify the ``GAMMathTrigger`` to implement the time-based expression.

   .. literalinclude:: /_static/tutorial/Configurations/MassSpring/RTApp-MassSpring-17-solution.cfg
      :language: c++
      :lines: 557-574
      :caption: Updated MathExpressionGAM configuration to trigger while the time is less than 10 seconds. Note the required cast type for the constant value.
      :linenos:
      :emphasize-lines: 3-5