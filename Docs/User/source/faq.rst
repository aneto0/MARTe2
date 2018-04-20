.. date: 26/03/2018
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

FAQ
===

Where do I start from?
----------------------

If you wish to understand how to build a MARTe real-time application, you should start from the concepts explained :doc:`here </core/gams/rtapp>` (top-down approach).

If you would like to understand the foundations of the library, the best place to start is from the :doc:`code structure </core/structure/code>` and to follow the help (bottom-up approach).

Deployment of new applications is detailed :doc:`here </deploying/deploying>`.

How does MARTe guarantee real-time?
-----------------------------------

The framework is designed to ensure a fully predictable behaviour of the real-time execution loops. The real-time performance will, nevertheless, always depend on the underlying infrastructure (including the operating system) setup. 
