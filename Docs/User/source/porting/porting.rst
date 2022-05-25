.. date: 10/01/2022
   author: Giuseppe Avon
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
Starting from fundamentals, the MARTe2 structure will be again represented from the porting point-of-view.
MARTe2 porting consists essentially in adaptations on the outer layers (lowermost and uppermost) of its layered structure. Specifically:

   - Core/BareMetal
     
     - L0Types *(Architecture)*
     - L1Portability *(Architecture/Environment)*
     - L6App *(Environment)*

   - Core/FileSystem
     
     - L1Portability *(Environment)*
     - L6App *(Environment)*


   - Core/Scheduler
     
     - L1Portability *(Environment)*

As described above, layers may contain one or both *Architecture* and *Environment* subdirectories. While the first contains **hardware 
related porting code**, the latter contains **software and "operating system"** code. The term **"operating system"** is intentionally quoted. Actually the term is abused as MARTe2 can also be ported on bare metal, **directly** on hardware without an operating system but with an hardware abstraction layer of software instead, which wraps calls to the underlying platform.

Each of the Architecture and Environment subfolders may contain other folders in turn, which represent the specific platform and operating system for the porting couple. Their naming must follow a convention and must encourage code reusation across combinations. One fundamental advice is to avoid binding Architectures to Environment and to maintain clean and streamlined versions of the two. Should this be impossible, a scheme to avoid
direct binding and leave room for code re-utilisation will be further explained (see Architecture/Environment unbinding section). This is done to avoid ``ARCH x ENV`` combinations proliferation of (mostly) copy-paste code.

Bundled Architecture and Environment follow this principle, although specific platforms have some exceptions which are handled in a further descripted section (Architecture/Environment unbinding).

Bundled Architecture/Environment folders **must** reside in a specific directory, located inside MARTe2 directory tree (e.g. Source/Core/BareMetal/L0Types/Architecture/x86_gcc).
In order to avoid cluttering, have clear separation and allow independent "unofficial" portings, their placement can be customised by using environment settings.

The aforementioned mechanism works leveraging a Makefile-based structure where 
   - The **MakeDefaults** directory contains definitions (**MakeStdLibDefs**) and rules (**MakeStdLibRules**) for each couple of Architecture and Environment
   - The **MakeDefaults** directory can be placed inside the MARTe2 one for bundled configuration or moved outside for customisation
   - Each top-level Makefile knows its own MARTe2 tier (BareMetal/FileSystem/Scheduler) and level (L0, L1, ..., L6). This is particulary true in L0, where ``TIER`` and ``LAYER`` symbols are needed to build include paths
   - The **TARGET** environment variable is used to derive the Architecture and Environment couple (i.e. their directory name)
   - Makefile are defaulting to MARTe2 *Architecture/Environment* path but two environment variables allow them to be placed elsewhere

For instance, a complete workflow for an existing target configuration is (e.g. Ultrascale+ porting running with FreeRTOS)

.. code-block:: bash

   TARGET=armv8_gcc_a53-freertos_us

This translates Architecture to armv8_gcc_a53 and Environment to freertos_us which means going into the MakeDefaults directory and search for 
the following two files:

.. code-block:: c++
 
   MakeStdLibDefs.armv8_gcc_a53-freertos_us
   MakeStdLibRules.armv8_gcc_a53-freertos_us

The **MakeStdLibDefs** file defines two new variables (i.e. ``ENVIRONMENT=FreeRTOS`` and ``ARCHITECTURE=armv8_gcc``) which are used for the subsequent Makefiles to search and find the right *Environment* and *Architecture* subdirectories.
Please **note** that the files may have further naming convention, which can be used to do subdivisions. In the specific example, the ``_a53`` suffix stands for the eventual CPU specialisation and ``_us`` stands for Ultrascale+ environment.

Base folder customisation
----------------------------

In order to have independent portings, without affecting the cloned MARTe2 repository, the **MakeDefaults** directory can be customised.
The **MakeDefaults** directory can be configured by means of the ``MARTe2_MAKEDEFAULT_DIR``

Once the ``TARGET`` environment variable is set, inside that folder you must have (at least) the **MakeStdLibDefs.arch-env** and the **MakeStdLibRules.arch-env** files.

At this point, these two files may combine already existing *Architecture/Environment* or specify new ones. Using the ``MARTe2_PORTABLE_ENV_DIR`` and ``MARTe2_PORTABLE_ARCH_DIR`` environment variables, you can also move outside the MARTe2 directory tree your own porting implementation, leaving unaffected the original folder content and structure, given however they remain hierarchically structured as their bundled counterpart.


Architecture/Environment unbinding
------------------------------------

In some scenarios it may be difficult a total separation between architecture and environment. When this happens, it is advisable to use the following pattern. This approach can be already found inside some of the actual MARTe2 portings.

   - Create a standard porting file, where instead of the real implementation you only put a ``#define`` guarded include
   - Only in the included file, use the needed *"mixed"* approach
   - In your **MakeStdLibDefs.arch-env** add the ``ENV_ARCH_CUSTOMIZATION`` variable

For example, the ``CompilerTypes.h`` file for the Xilinx Ultrascale+ implementation has a non-generic armv8 gcc porting. To achieve that, a standard
CompilerTypes.h is produced:

.. code-block:: c++

   #ifdef XILINX_ULTRASCALE
   #include "CompilerTypesXil.h"
   #define CUSTOMIZED_COMPILERTYPES_HEADER
   #endif
   #ifndef CUSTOMIZED_COMPILERTYPES_HEADER
   #include "CompilerTypesDefault.h"
   #endif
   
As shown above, the ``ENV_ARCH_CUSTOMIZATION`` is brought inside the ``CompilerTypes.h`` file and its value is used to distinguish between *clean* and *targeted* implementation. Note also the approach allows multiple implementations and a fallback ``CompilerTypesDefault.h`` implementation. The fallback implementation is, effectively, the default clean one.

Portable test environment
---------------------------

MARTe2 *default* test suite is based on **GTest**. Another option, which comes handy when porting MARTe2 is the availability of a portable test environment.
The portable test environment is based on a mechanism that offers an interface compatible to the one offered by GTest but way simpler
in terms of implementation and dependencies. Standard makefiles default to the **GTest** suite, the setting of the ``MARTe2_TEST_ENVIRONMENT`` variable to ``"Portable"`` instead, switches to the Portable. The **portable** environment, used in conjunction with the static test mode, produces a whole test executable which can be executed on the ported environment.

To reduce the size of the executable or the scope of the tests, a test link filter is available. It is enabled only when ``MARTe2_TEST_ENVIRONMENT`` is set to ``"Portable"`` and enables another two options: ``MARTe2_TEST_LINK_FILTER_MODE`` and ``MARTe2_FILTER_ITEMS``.

The ``MARTe2_TEST_LINK_FILTER_MODE`` environment variable controls the level at which the filter has to operate while the ``MARTe2_FILTER_ITEMS`` controls what, also according to the mode, has to be included or excluded from the linking step in the executable generation.

The ``MARTe2_TEST_LINK_FILTER_MODE`` can be:
- ``Layer``, the filtering happens at whole *Tier/Layer* level, meaning that the ``MARTe2_FILTER_ITEMS`` will contain the name of the layers which have to be included/excluded;
- ``Library``, the filtering happens at single test object level, meaning that the ``MARTe2_FILTER_ITEMS`` will contain the name of the test objects that have to be included/excluded;

The ``MARTe2_FILTER_ITEMS`` environment variable **must** contain a **colon (:) separated list** of the names of the layers/library to include or exclude.
When **only the name** is specified (e.g. L1PortabilityBGTest.a) the linking operates in *whitelist* mode and includes only the specified library.
When the name is preceded by a **minus sign (-)** (e.g. -L1PortabilityBGTest.a) the linking operates in *blacklist* mode, a whole list of the available layers will be built and, if and only if the specified name is found, it will be removed from the list of the linked objects.

When operating in Layer mode, the MARTe2 naming convention helps filtering in/out the right tier as a capital letter (B for BareMetal, 
S for Scheduler and F for FileSystem) is appended to the layer name itself (e.g. L1PortabilityBGTest.a or L2ObjectsBGTest.a)

Note that the filter **cannot** operate in **mixed black/whitelist mode** (e.g. you **cannot** mix L1PortabilityBGTest.a:-L2ObjectsBGTest.a). If you both include and exclude an **error** will be generated.


MARTe2 startup hooks
----------------------

Some embedded deployment scenarios require that hardware initialisation functions start before any other code is executed. This is particularly useful when deploying the full MARTe2 Application, which owns already a main() entry point.
MARTe2 has currently two (2) hooks which are called during the startup phase, before the GlobalObjectsDatabase constructor begins its initialisation loop:
 
 .. code-block:: c++

   InitHardware() {[...]}
   InitPlatform() {[...]}

Their declaration stays inside ``BareMetal/L1Portability/HardwareI.h`` and you **should** provide an implementation (**at least an empty one** if not needed).
Implementation of these methods are intended to initialise the underlying hardware platform **before** the MARTe initialisation process begins, like in embedded or microcontroller-based platforms, where peripherals (e.g. U[S]ART) should be **initialised beforehand** (e.g. console output functions).
They are called subsequently, one after the other without any other intermediate call (e.g. ``InitHardware(); InitPlatform();``).

Bootstrapping advices
-----------------------

If the platform initialisation **does not require to be executed before** the ``GlobalObjectsDatabase`` construction, it can be
accomplished at a later stage in the ported ``Bootstrap.cpp``. This is the case of the current FreeRTOS implementation, where a ``MARTe2HardwareInitialise()``
extern function is expected to be implemented from the hardware platform side.

In the specific aforementioned FreeRTOS implementation, as the platform hardware initialisation may be required to run in a task execution environment, MARTe2 waits for a ``vTaskNotification`` to resume its operations. In that scenario, the minimal ``MARTe2HardwareInitialise()`` call must notify the caller task to start MARTe2.


Walkthrough
-----------

The porting guide is supported by the following walkthrough, in order to speedup the boilerplate operations needed to begin the MARTe2 porting process.

Step 1 - Makefile Rules and Definitions
-----------------------------------------

In the first step you will need to create a suitable directory to hold the two Makefile Rules and Definitions for your architecture.
Makefile naming convention needs for these two to be named with the following scheme [architecture]-[environment] where

   - **[architecture]** can also be split into subarchitectures (e.g. armv8_gcc or x86_gcc or x86_cl)
   - **[environment]** can also be split into subenvironments (e.g. freertos_us or bare_us)

Step 2 - Change MakeStdLibDefs file
-------------------------------------
Inherit an existing MakeStdLibDefs with default needed compiler defines by changing the first inclusion. You can use a shipped one or start from
one of them to adapt to the ported platform.

Update the ``ENVIRONMENT`` and ``ARCHITECTURE`` (see above) with the names your architecture and environment combination should have. Note that
these two names will match the two *Architecture/Environment* subdirectories under each ported layer
For tidyness, their name should be given accordingly to their internally set ``ARCHITECTURE``and ``ENVIRONMENT`` variables.

Update the ``MARTe2_LINK_MODE`` variable to suit your environment. Allowed values are **Static** or **anything else** (also none or undefined). This value will
affect the MARTeApp **linking** step.

Update the ``MARTe2_TEST_ENVIRONMENT`` variable to suit your needs. Allowed values are ``Portable`` and ``GTest``. This value will affect the MARTe2 test suite 
generation. Undefined **defaults to GTest**.

Append, by using ``INCLUDES+=``, eventual additional directories which are needed during the compilation process. Keep in mind that these are
the ones where both ported architecture and environment code will source. Usually here goes the **BSP (Board Support Package)** path where all the includable
headers for the platform reside. To keep the code clean and independent from the path, you can use your own environment variable to point the includes to the right place.

Update the (a) ``COMPILERPREFIX``, (b) ``COMPILER``, (c) ``CCOMPILER`` and (d) ``LINKER`` with your 
   (a) **prefix** for all the names (e.g. ``aarch64-none-elf``); 
   (b) **C++** compiler (e.g. ``-g++``);
   (c) **C** compiler (e.g. ``-gcc``) and (d) linker (e.g. ``-ld``). The ``COMPILERPREFIX`` could be omitted, as its purpose is solely aestethic, avoiding repetition.

Update the ``CPUSPECIFICFLAG``, ``ADDITIONALFLAGS``, ``OPTIM`` and ``CFLAGS`` according to your own platform specifications. Note that the only significant variable
is ``CFLAGS``, which is built appending the others.


Step 3 - Change MakeStdLibRules file
--------------------------------------

Inherit an existing **MakeStdLibRules** with default needed compiler defines by changing the first inclusion. You can use a shipped one or start from one
of them to adapt to the ported platform. Please note here that rules are also named with a convention and they *inherit/override* settings (e.g. ``gcc-dynamic`` vs ``gcc-static``).


Step 4 - Set the right environment variables
----------------------------------------------

In contrast to standard shipped MARTe2, the ``MARTe2_DIR`` environment variable **must be set** in order for the makefiles chain to become able to pick
MARTe2 and own ported files. This becomes true especially if using the external **Makedefaults/Architecture/Environment** directories configuration.

At a bare minimum, the following environment variables must be set:

   - ``MARTe2_MAKEDEFAULT_DIR`` (where aforementioned **MakeStdLibDefs** and **MakeStdLibRules** reside)
   - ``MARTe2_PORTABLE_ENV_DIR`` (where the **environment**-related porting files reside)
   - ``MARTe2_PORTABLE_ARCH_DIR`` (where the **architecture**-related porting files reside)

Plus, setting the environment variable ``MARTe2_TEST_ENVIRONMENT``, sets the right test environment to be compiled for MARTe. As above stated, this variable
can be ``GTest`` or ``Portable``. If planning to port for embedded architecture, **prefer the Portable environment**, unless sure that the GTest suite is able to
be compiled. Also take into account the link filter previously explained.
Note that the ``MARTe2_TEST_ENVIRONMENT`` **defaults to GTest**. To avoid test compilation, which is useful in the first development phases, you can disable
their compilation by issuing 

 .. code-block:: c++

   "make -f Makefile.gcc core"

After the bare minimum variables, you must also consider the BSP files location which will be needed for your own platform porting. This variable
will be used in the aforementioned ``INCLUDES+=`` line inside the *MakeStdLibDefs* file previously written.


Step 5 - Create the architecture and environment files
--------------------------------------------------------

Use the provided stubs to speedup the development process or take them from an existing implementation. Consider that the stubs or, 
alternatively, your own **must** provide an implementation for every function defined (**no __weaks** symbols) eventually returning a failure
or fail-safe value.

Also consider that, given that the *Architecture* and *Environment* directories are different, porting may be only oriented to just one of them.
This could be achieved by setting accordingly the ``MARTe2_PORTABLE_ARCH_DIR`` and ``MARTe2_PORTABLE_ENV_DIR`` variables.

.. warning::  If other ancillary files are needed, aside the mandatory porting ones, keep in mind to add their ``.x`` line in the ``Makefile.inc``.


Quick reference on files to change
------------------------------------

Files which are needed are (1)

   - BareMetal - L0Types - Architecture 
     
     - CompilerTypes.h

   - BareMetal - L1Portability - Architecture 
     
     - AtomicA.h **(2)**
     - EndianityA.h **(3)**
     - FastMathA.h
     - HardwareIA.h **(4)**
     - HighResolutionTimerA.h **(5)**
     - ProcessorA.h **(6)**

   - BareMetal - L1Portability - Environment 
     
     - BasicConsole.cpp **(7)**
     - GeneralDefinitions.h **(8)**
     - HardwareI.cpp **(9)**
     - HighResolutionTimer.h **(10)** + HighResolutionTimerCalibrator.h/.cpp
     - LoadableLibrary.cpp **(11)**
     - Sleep.cpp **(12)**
     - StandardHeap.cpp **(13)**
     - Timestamp.cpp **(14)**

   - BareMetal - L6App - Environment 
     
     - Bootstrap.cpp **(15)**

   - FileSystem - L1Portability - Environment 
     
     - BasicFile.cpp **(16)**
     - BasicFileProperties.h **(17)**
     - BasicSocket.cpp **(18)**
     - BasicTCPSocket.cpp
     - BasicUART.cpp **(19)**
     - BasicUARTProperties.h
     - Directory.cpp **(20)**
     - DirectoryCore.h
     - DirectoryScanner.cpp
     - InternetHost.cpp **(21)**
     - InternetHostCore.h
     - InternetMulticastCore.h
     - InternetService.cpp **(22)**
     - InternetServiceCore.h
     - Select.cpp **(23)**
     - SelectProperties.h
     - SocketCore.h **(24)**

   - Scheduler - L1Portability - Environment 
     
     - EventSem.cpp
     - MutexSem.cpp
     - Processor.cpp
     - Threads.cpp **(25)**
     - ThreadDatabase.h **(26)**
  
Notes
-------

Based on the file listing above, some notes are provided about specific files porting instructions.

   - **(1)**: provided stubs are empty do-nothing implementation of the described methods.
   - **(2)**: when porting AtomicA.h, prefer intrinsics over inline assembly where possible (e.g. __atomic_test_and_set)
   - **(3)**: when porting EndianityA.h, ensure your platform endianess and implement accordingly the To/From functions. 
     Some of them may result in do-nothing functions (e.g. the ToLittleEndian in a little endian architecture).
     Consider that some architectures may be both configured as big and little.
     Prefer intrinsics to achieve endiannes swap (e.g. __bswap/16/32/64).
   - **(4)**: HardwareIA.h holds the function that will be called at startup, which is intended to be called as hardware initialisation.
   - **(5)**: Native microseconds-granular counters are preferred, to suit internal calculations (delays, etc).
   - **(6)**: Can also be implemented to return a fixed compile-time value if cpuid mechanism is unwanted/unnecessary as not fundamental.
   - **(7)**: Can also be empty-implemented like the stub, if the console is unavailable (e.g. headless or unsupervised) or unneeded.
   - **(8)**: Catch all container for all baseline stuff (e.g. handles and constants).
   - **(9)**: Contains the code which must be run before MARTe2 GlobalObjectsDatabase construction, see HardwareIA.h and notes above.
   - **(10)**: If the vanilla 32/64 bit counter is available without further initialisations, the implementation can be straightforwardly
     included in HighResolutionTimer.h (stateless, no class, direct inline methods). Else, if the HRT needs a stateful implementation
     or a peripheral initialisation beforehand, the HRT calibrator approach can be used. This means that you have an instance of the 
     calibrator, which is a class in charge of the initialisation and preparation of the timer/counter which lies statically and externally
     referenced from the HighResolutionTimer.h file inside the inlined methods. This approach can be translated anywhere else, to suit
     similar needs.
   - **(11)**: If the porting is based on dynamic linking and runtime module loading, place here suitable functions (see Linux dlopen())
   - **(12)**: Sleep functions here are meant to be, if available, a non-busy sleep alternative from the OS (e.g. yielding like the FreeRTOS vTaskDelay()).
   - **(13)**: Must implement own heap management function (alloc/free/realloc/...). Use OS-aware primitives when available (e.g. FreeRTOS vPortMalloc/vPortFree)
   - **(14)**: Timestamp functions can be left empty if the platform has not RTC
   - **(15)**: Functions can be semantically split between BareMetal/L6App and FileSystem/L6App bootstraps
   - **(16)**: Handle variable, used across FileSystem implementations, stays in GeneralDefinitions.h. It assumes a POSIX style handle.
   - **(17)**: Place here convenient structure to hold basic file information (see Linux implementation).
   - **(18)**: Needs aformentioned Handle and InternetHost class to be defined, see below.
   - **(19)**: Use BasicUARTProperties.h to define a suitable structure for UART configuration data (bps, data, stop, parity bits, ...)
   - **(20)**: Use DirectoryCore.h to define a suitable structure to hold directory stat.
   - **(21)**: Use InternetHostCore.h to define a suitable structure to hold IP/host data.
   - **(22)**: Use InternetServiceCore.h to define a suitable structure to hold IP/service data.
   - **(23)**: Use SelectProperties.h to define a suitable structure to hold select data (POSIX fd_set style).
   - **(24)**: Used by BasicSocket as base handle.
   - **(25)**: If the OS/BM does not include a thread database, the ThreadDatabase can be used instead, see ThreadDatabase.h
   - **(26)**: Defaults to a MARTe2 based GenericThreadDatabase, which can be used to store/retrieve currently spawned threads.
     Be aware that, if not using the database, that some informations may be different between the real number of running threads
     and the MARTe2 perceived one. For example, if bypassing the ThreadsDatabase, asking directly to the OS the number of threads
     returns a number that may be inconsistent for the MARTe2 point-of-view. This is especially true in some tests, which expect
     a zero (0) number of threads still running after the completion. The zero (0) value has to be intended as "MARTe2 has 0 threads running"
     and not as "The system has 0 threads running". (See socket test suite).
