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
MARTe2_PORTABLE_ARCH_DIR environment variables, you can also move outside the MARTe2 directory tree your own porting implementation, leaving unaffected the original
folder content and structure, given however they remain hierarchically structured as their bundled counterpart.


Architecture/Environment unbinding
----------------------------------

In some scenarios it may be difficult a total separation between architecture and environment. When this happens, it is advisable to use the following
pattern. This approach can be already found inside some of the actual MARTe2 porting.

- Create a standard porting file, where instead of the real implementation you only put a #define guarded include
- Only in the included file, use the needed "mixed" approach
- In your MakeStdLibDefs.arch-env add the ENV_ARCH_CUSTOMIZATION variable

For example, the CompilerTypes.h file for the Xilinx Ultrascale+ implementation has a non-generic armv8 gcc porting. To achieve that, a standard
CompilerTypes.h is produced:
.. code-block:: bash
    #ifdef XILINX_ULTRASCALE
    #include "CompilerTypesXil.h"
    #define CUSTOMIZED_COMPILERTYPES_HEADER
    #endif

    #ifndef CUSTOMIZED_COMPILERTYPES_HEADER
    #include "CompilerTypesDefault.h"
    #endif

As shown above, the ENV_ARCH_CUSTOMIZATION is brought inside the CompilerTypes.h file and its value is used to distinguish between "clean" and
targeted implementation. Note also the approach allows multiple implementations and a fallback CompilerTypesDefault.h implementation. The fallback
implementation is, effectively, the default clean one.

Portable test environment
-------------------------

MARTe2 "standard" test suite is based on GTest. Another option, which comes handy when porting MARTe2 is the availability of a portable test environment.
The portable test environment is based on a mechanism that offers an interface compatible to the one offered by GTest but way simpler 
in terms of implementation. Standard makefiles default to the GTest suite, the setting of the MARTe2_TEST_ENVIRONMENT variable to "Portable" instead,
switches to the Portable. The portable environment, used in conjunction with the static test mode, produces a whole test executable which can be
executed on the ported environment.

To reduce the size of the executable or the scope of the tests, a test link filter is available. It is enabled only when MARTe2_TEST_ENVIRONMENT is set to 
"Portable" and enables another two options: MARTe2_TEST_LINK_FILTER_MODE and MARTe2_FILTER_ITEMS

The MARTe2_TEST_LINK_FILTER_MODE environment variable controls the level at which the filter has to operate while the MARTe2_FILTER_ITEMS controls what,
also according to the mode, has to be included or excluded from the linking step in the executable generation.

The MARTe2_TEST_LINK_FILTER_MODE can be:
- Layer, the filtering happens at whole Tier/Layer level, meaning that the MARTe2_FILTER_ITEMS will contain the name of the layers which have to be included/excluded;
- Library, the filtering happens at single test object level, meaning that the MARTe2_FILTER_ITEMS will contain the name of the test objects that have to be included/excluded;

The MARTe2_FILTER_ITEMS environment variable must contain a colon (:) separated list of the names of the layers/library to include or exclude.
When only the name is specified (e.g. L1PortabilityBGTest.a) the linking operates in whitelist mode and includes only the specified library.
When the name is preceded by a minus sign (-) (e.g. -L1PortabilityBGTest.a) the linking operates in blacklist mode, a whole list of the available
layers will be built and, if and only if the specified name is found, it will be removed from the list of the linked objects.

When operating in Layer mode, the MARTe2 naming convention helps filtering in/out the right tier as a capital letter (B for BareMetal, 
S for Scheduler and F for FileSystem) is appended to the layer name itself (e.g. L1PortabilityBGTest.a)

Note that the filter cannot operate in mixed black/whitelist mode (e.g. you cannot mix L1PortabilityBGTest.a:-L2ObjectsBGTest.a). If you both include and exclude
an error will be generated.
