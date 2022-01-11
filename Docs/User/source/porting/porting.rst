.. date: 10/01/2022
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

Porting
=======

MARTe2 has a friendly structure which allows easy porting on different platforms. 
Essentially, porting comes down to a sequence of basic steps, some of them are mandatory while others are optional, to accomplish the whole porting process.


Porting fundamentals
--------------------
Starting from fundamentals, MARTe2 structure will be again represented from the porting point-of-view.
MARTe2 porting consists essentially in adaptations on the outer layers (lowermost and uppermost) of its layered structure. Specifically:

- Core/BareMetal

 - L0Types (Architecture)
 - L1Portability (Architecture/Environment)
 - L6App (Environment)


- Core/FileSystem
 
 - L1Portability (Environment)
 - L6App (Environment)


- Core/Scheduler

 - L1Portability (Environment)

As you may have noticed, layers may contain one or both Architecture and Environment subdirectories. While the first contains hardware 
related porting code, the latter contains "operating system" code. The term "operating system" is intentionally quoted. Actually the term is abused
as MARTe2 can also be ported on bare metal, directly on hardware without an operating system but with an hardware abstraction layer of software instead,
which wraps calls to the underlying platform.

Each of the Architecture and Environment subfolders contains other folders in turn, which represent the specific platform and operating system for the
porting couple. Their naming must follow a convention and must encourage code reusation across combinations. One fundamental advice is to avoid
binding Architectures to Environment and to maintain clean and streamlined versions of the two. Should this be impossible, a scheme to avoid
direct binding and leave room for code re-utilisation will be further explained.