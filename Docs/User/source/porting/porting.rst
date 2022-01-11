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
direct binding and leave room for code re-utilisation will be further explained (see Architecture/Environment unbinding section). This is done to avoid
ARCH x ENV combinations proliferation of (mostly) copy-paste code.

Bundled Architecture and Environment follow this principle, although specific platforms have some exceptions which are handled in a further descripted
section (Architecture/Environment unbinding).

Bundled Architecture/Environment folders must reside in a specific directory, located inside MARTe2 directory tree (e.g. Source/Core/BareMetal/L0Types/Architecture/x86_gcc).
In order to avoid cluttering, have clear separation and allow own "unofficial" portings, their placement can be customised by using environment settings.

Aforementioned mechanism works leveraging a Makefile-based structure where 
- The MakeDefaults directory contains definitions (MakeStdLibDefs) and rules (MakeStdLibRules) for each couple of Architecture and Environment
- The MakeDefaults directory can be placed inside the MARTe2 one for bundled configuration or moved outside for customisation
- Each top-level Makefile knows its own MARTe2 tier (BareMetal/FileSystem/Scheduler) and level (L0, L1, ..., L6)
- The TARGET environment variable is used to derive the Architecture and Environment couple (i.e. their directory name)
- Makefile are defaulting to MARTe2 Architecture/Environment path but two environment variables allow them to be placed elsewhere

For instance, a complete workflow for an existing target configuration is (Ultrascale+ porting running with FreeRTOS)
TARGET=armv8_gcc_a53-freertos_us
This translates Architecture to armv8_gcc_a53 and Environment to freertos_us which means going into the MakeDefaults directory and search for 
the following two files:

MakeStdLibDefs.armv8_gcc_a53-freertos_us
MakeStdLibRules.armv8_gcc_a53-freertos_us

The MakeStdLibDefs file defines two new variables (i.e. ENVIRONMENT=FreeRTOS and ARCHITECTURE=armv8_gcc) which are used for the subsequent Makefiles to 
search and find the right Environment and Architecture subdirectories.


Base folder customisation
--------------------------

In order to have own porting, without affecting cloned MARTe2 repository, MakeDefaults directory can be customised.
The MakeDefaults directory can be configured by means of the MARTe2_MAKEDEFAULT_DIR

Once the TARGET environment variable is set, inside that folder you must have (at least) MakeStdLibDefs.arch-env and MakeStdLibRules.arch-env files.

At this point, these two files may combine already existing Architecture/Environment or specify new ones. Using the MARTe2_PORTABLE_ENV_DIR and
MARTe2_PORTABLE_ARCH_DIR directories, you can also move outside the MARTe2 directory tree your own porting implementation, leaving unaffected the original
folder content and structure.

