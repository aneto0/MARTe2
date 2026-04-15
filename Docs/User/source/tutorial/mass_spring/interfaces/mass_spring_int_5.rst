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

This section concludes the introduction to the Interface components used in the mass-spring-damper example, listing other commonly used Interface components that are not covered in the previous sections.

- :vcisdoxygenmccl:`ApplicationKiller`: terminates an application based on a Message event.
- :vcisdoxygenmccl:`SysLogger`: to be used with a :vcisdoxygencl:`LoggerService`. Sends log messages to the configured ``syslog``.
- :vcisdoxygenmccl:`TcnTimeProvider`: to be used with a :vcisdoxygenmccl:`LinuxTimer`. Provides TCN synchronised information to the system.