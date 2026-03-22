.. date: 23/03/2026
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

Mass-spring-damper Example - Simple Control
===========================================

In this section, you will implement a simple control strategy for the mass-spring-damper system using MARTe2 components. The control strategy will be based on a proportional controller that adjusts the external force (:math:`F`) applied to the mass based on the error between the desired position and the actual position of the mass.

.. figure:: images/mass_spring_control.svg
   :align: center
   :alt: Mass-spring-damper control system
   :width: 400px

   Schematic representation of the mass-spring-damper system control system. 

The first is to identify the main components required for the control of the mass-spring-damper system and map them to existing MARTe2 components. The main components are:

- Desired position: This component represents the target position for the mass. 
- Controller: This component implements the control logic that calculates the external force (:math:`F`) based on the error between the desired position and the actual position of the mass.
- Mass-spring: This component represents the physical system of the mass-spring-damper and simulates its dynamics based on the applied force.
- Monitoring: This component allows monitoring the system's behavior, such as the actual position of the mass and the applied force.

The following parameters are common to all the examples in this tutorial:

- Mass (:math:`m`): 1 kg
- Damping coefficient (:math:`c`): 0.5 Ns/m
- Spring stiffness (:math:`k`): 10 N/m
- Initial position of the mass: 0 m
- Initial velocity of the mass: 0 m/s
- Sampling time: 0.01 s

Constant position
-----------------

In this example the system will be controlled to maintain a constant position. The desired position will be set to a fixed value, and the controller will adjust the force to keep the mass at that position.

The selected components are:

- Desired position: :vcisdoxygenmccl:`ConstantGAM`
- Controller: :vcisdoxygenmccl:`PIDGAM`
