.. date: 15/04/2026
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

Other Interfaces
================

This section concludes the overview of Interface components used in the mass-spring-damper example, by listing other commonly used interfaces that were not covered previously.

- :vcisdoxygenmccl:`ApplicationKiller`:  
  Terminates the application when a specific Message is received.  
  Useful for implementing controlled shutdowns or safety mechanisms.

- :vcisdoxygenmccl:`SysLogger`:  
  Designed to be used with the :vcisdoxygencl:`LoggerService`.  
  Forwards log messages to the system ``syslog``, enabling integration with standard OS logging tools and monitoring systems.

- :vcisdoxygenmccl:`TcnTimeProvider`:  
  Typically used together with a :vcisdoxygenmccl:`LinuxTimer`.  
  Provides time synchronisation based on TCN (Timing Communication Network), ensuring consistent and accurate timing across distributed systems.