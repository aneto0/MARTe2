.. date: 27/03/2018
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

Deploying in Linux
==================

The MARTe real-time performance is as good as the operating system real-time performance.

In Linux, the best strategy is to isolate the CPUs from the Linux scheduler (using the ``isolcpus`` parameter) and to assign the :doc:`MARTe real-time threads </core/gams/rtapp>` to these CPUs.  

Best practices from existent projects
-------------------------------------

1. Switch off all the services which are not required for the project and in particular any services that might interfere with the CPU performance and interrupt allocation (e.g. ``cpuspeed`` and ``irqbalance``).

2. Boot with isolated CPUs

.. code-block:: bash

   isolcpus=1,2,3 intel_idle.max_cstate=0 processor.max_cstate=0 idle=poll selinux=0 maxcpus=4
