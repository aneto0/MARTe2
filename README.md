# MARTe2

The MARTe software framework is C++ modular and multi-platform framework for the development of real-time control system applications. 

The previous version of the framework (aka [MARTe1](https://vcis-gitlab.f4e.europa.eu/aneto/MARTe1)) was deployed in many fusion real-time control systems, particularly in the [JET tokamak](https://www.euro-fusion.org/jet). One of the main advantages of the MARTe architecture is the bold separation between the platform specific implementation, the environment details and the real-time algorithms (i.e. the user code). 

The platform is defined by the target processor and the operating system, while the environment encapsulates all the interfacing details which are related to the peculiarities of the location where the final system is to be deployed. This includes both the interfacing with the hardware plaform and the binding to the services that allow to configure and retrieve data from the system. This clear separation of concern has allowed to reuse many components inside the same environment (e.g. all the systems deployed at JET share the same services for parameter configuration and data retrieval) and to develop and test the user algorithms in non-real-time operating systems and to later deploy the same exact code in a previously tested platform.

As more systems started to use MARTe1, the number of supported environments and platforms has considerably grown. This has leveraged the exposure of the same core code
to different environment configurations, thus increasing the confidence on its quality and robustness. Having the same infrastructure being used inside a community has also had the advantage of sharing and recycling knowledge about the framework and its architecture.

One of the main objectives of the MARTe2 version was to develop a software Quality Assurance (QA) strategy that is appropriate for the development of real-time
applications that required the integration of contributions from a large and heterogeneous development community, which includes developer profiles both from the
scientific community and from the industry. 

More details about the QA process (and in particular on the MISRAC++:2008 compliance) can be found in this [folder](https://vcis-gitlab.f4e.europa.eu/aneto/MARTe2-doc/tree/bdcbf8ee457e7a463169f259ed70b790cf4fe1e3/Documents).

## Glossary

## Requirements 

[requirements](https://vcis-gitlab.f4e.europa.eu/aneto/MARTe2-doc/blob/bdcbf8ee457e7a463169f259ed70b790cf4fe1e3/Reports/MARTe2/MARTe2%20-%20Requirements%20report.docx)

## Architecture


The Multi-threaded Application Real-Time executor (MARTe) is a C++ framework for the development of real-time applications.

MARTe is built upon a C++ multi-platform real-time library named BaseLib2. The first key feature of the library is the ability to run the same code in different Operating Systems (OS). It is organized in consecutive layers where the lowest level implements the different calls for each of the available systems. In order to guarantee portability, the remaining layers and the end-user code must use the functionalities provided by the library, avoiding OS dependent calls. This layered scheme provides a logical organization on how the code is distributed. Top layers have a broader view over the library functionality and tend to accommodate code that is less critical for the operation of a system, granting at the same time a set of tools that greatly eases the development of applications. When porting the library to a new OS only the lowest layer needs to be adjusted.

## License

Copyright 2015 F4E | European Joint Undertaking for ITER and the Development of Fusion Energy ('Fusion for Energy').
Licensed under the EUPL, Version 1.1 or - as soon they will be approved by the European Commission - subsequent versions of the EUPL (the "Licence"). You may not use this work except in compliance with the Licence. You may obtain a copy of the Licence at: http://ec.europa.eu/idabc/eupl

Unless required by applicable law or agreed to in writing, software distributed under the Licence is distributed on an "AS IS" basis, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the Licence permissions and limitations under the Licence.
