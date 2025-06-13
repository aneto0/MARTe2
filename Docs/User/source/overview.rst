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

Overview
========

The MARTe software framework is a C++ modular and multi-platform framework for the development of real-time control system :doc:`applications </core/app/gams/rtapp>`. 

The previous version of the framework (aka `MARTe1 <https://github.com/aneto0/MARTe1>`_) was deployed in many fusion real-time control systems, particularly in the `JET tokamak <https://www.euro-fusion.org/devices/jet/>`_. 

One of the main advantages of the MARTe architecture is the :doc:`bold separation </core/structure/code>` between the platform specific implementation, the environment details and the real-time algorithms (i.e. the user code). 

The platform is defined by the target processor and the operating system, while the environment encapsulates all the interfacing details which are related to the peculiarities of the location where the final system is to be deployed. This includes both the interfacing with the hardware platform and the binding to the services that allow to configure and retrieve data from the system. 

This clear separation of concern has allowed to reuse many components inside the same environment (e.g. all the systems deployed at JET share the same services for parameter configuration and data retrieval) and to develop and test the user algorithms in non-real-time operating systems and to later deploy the same exact code in a previously tested platform.

As more systems started to use MARTe1, the number of supported environments and platforms has grown considerably. This has leveraged the exposure of the same core code
to different environment configurations, thus increasing the confidence on its quality and robustness. Having the same infrastructure being used inside a community has also had the advantage of sharing and recycling knowledge about the framework and its architecture.

One of the main objectives of the MARTe2 version was to develop a software Quality Assurance (QA) strategy that is appropriate for the development of real-time
applications that required the integration of contributions from a large and heterogeneous development community, which includes developer profiles both from the
scientific community and from the industry. 

More details about the QA process (and in particular on the MISRAC++:2008 compliance) can be found :doc:`here </contributing/qa/qa>`.
