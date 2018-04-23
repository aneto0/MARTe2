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

Threads and semaphores
======================

The MARTe framework offers a portable API for the management and synchronisation of multi-threaded applications.

.. note::

   The preferred way of using threads is through the :doc:`Services </core/scheduling/services>` interface.

Threads
-------

The :vcisdoxygenns:`Threads` API allows to launch named threads bounded by to a specific CPU affinity (see :vcisdoxygenns:`ProcessorType`) and with a defined amount of stack memory.
The thread priority class and priority level can also be changed for every individual thread.

.. warning::
   
   The change of some of the thread parameters (e.g. priorities) might fail if the user does not have the appropriate operating system permissions. 

.. code-block:: c++

   ...
   void IncrementDecrementFunction(const void * const parameters) {
      ...
   }
   ...
   Threads::BeginThread(&IncrementDecrementFunction, (int64 *)i, THREADS_DEFAULT_STACKSIZE, "NamedThread", ExceptionHandler::NotHandled, 0x1);

.. note::

   When starting a thread, if the affinity parameter is not set, the thread will be allowed to run in the *default* CPU set. The application default CPU set can be changed with  ``ProcessorType::SetDefaultCPUs(...)``

The framework also allows to query the number of threads and its associated properties (see :vcisdoxygencl:`ThreadInformation` and ``Threads::GetThreadInfoCopy``).

Atomic operations
-----------------

The :vcisdoxygenns:`Atomic` API offers a set of portable functions that allow to perform atomic operations, i.e. operations that are guaranteed to be uninterruptible in a multi-threaded application environment. 

.. code-block:: c++
   
   ...
   b = Atomic::Exchange(&a, b);
   ...
   Atomic::Increment(&a);
   ...
   if (!Atomic::TestAndSet(&locked)) {
   
In particular, the ``Atomic::TestAndSet`` functions are used to implement fast polling mutex semaphores (see below) that can be deployed in environments that do not have an operating system scheduler. 

High Resolution Timer
---------------------

The :vcisdoxygenns:`HighResolutionTimer` provides a portable abstraction of a timer and allows to measure *ticks* with high resolution. 

These *ticks* (see ``HighResolutionTimer::Counter()``) can be converted from/into time using the ``HighResolutionTimer::Frequency()`` and ``HighResolutionTimer::Period()`` function.  

.. code-block:: c++

   uint64 countStart = HighResolutionTimer::Counter();
   ...
   while (HighResolutionTimer::Counter() < countEnd);

Sleep
-----

The :vcisdoxygenns:`Sleep` API allows to voluntarily return the control back to the scheduler. The only exception is the ``Sleep::Busy`` which waits for a given time to ellapse without yielding the CPU.

.. code-block:: c++

   ...
   MARTe::Sleep::Sec(1e-3);
   ...

Semaphores
----------

The framework offers three distinct sets of portables semaphores:
 
- :vcisdoxygencl:`MutexSem`: mutex semaphore that offers exclusive access to a critical area;
- :vcisdoxygencl:`EventSem`: barrier semaphore that, after being released, allow shared access to a critical area;
- :vcisdoxygencl:`FastPollingMutexSem` and :vcisdoxygencl:`FastPollingEventSem`: mutex and event semaphores that do not require an operating system scheduler. Both work by polling, with atomic operations, a given memory region.

.. code-block:: c++

   ...
   MARTe::MutexSem mutexSem;
   mutexSem.Create(false);
   if(!mutexSem.Lock()){
   ...
   if(!mutexSem.UnLock()){
   ...
   
The event semaphores need the ``Event::Reset`` method to be called in order to lower the barrier. 

.. warning::

   The ``Event::Reset`` operation is not atomic. As consequence, depending on the specific use-case, it may require a mutex semaphore to protect access to a given shared resource while this operating is being performed.
   
.. code-block:: c++

   ...
   MARTe::EventSem eventSem;
   eventSem.Create();
   ...
   mutexSem.Lock();
   if (needsReset) {
      needsReset = false;
      eventSem.Reset();
   }
   mutexSem.UnLock();
   eventSem.Wait();
   
Examples
--------

Atomic
~~~~~~

The following example demonstrates how to use the Atomic API.

.. literalinclude:: /_static/examples/Core/AtomicExample1.cpp
   :language: c++
   :emphasize-lines: 63, 66, 71, 75, 85
   :caption: Atomic example (AtomicExample1)
   :linenos:

   
HighResolutionTimer
~~~~~~~~~~~~~~~~~~~

The following is an example of a possible use of the HighResolutionTimer.

.. literalinclude:: /_static/examples/Core/HighResolutionTimerExample1.cpp
   :language: c++
   :emphasize-lines: 49, 52, 54-56
   :caption: HighResolutionTimer example (HighResolutionTimerExample1)
   :linenos:
   
   
Instructions on how to compile and execute the examples can be found :doc:`here </core/examples>`.

Threads
~~~~~~~

The following example illustrates how to manage threads.

.. literalinclude:: /_static/examples/Core/ThreadsExample1.cpp
   :language: c++
   :emphasize-lines: 75,87
   :caption: Thread example (ThreadsExample1)
   :linenos:
   
Semaphores
~~~~~~~~~~

The following is an example of the use of mutex and event semaphores and on how to query the thread properties.

.. literalinclude:: /_static/examples/Core/ThreadsExample2.cpp
   :language: c++
   :emphasize-lines: 81, 87, 94, 109-111, 118, 122, 127-133
   :caption: Event and mutex semaphores (ThreadsExample2)
   :linenos:
   
FastPolling Semaphores
~~~~~~~~~~~~~~~~~~~~~~

The same example as above, but implemented using the FastPolling semaphores.

.. literalinclude:: /_static/examples/Core/ThreadsExample3.cpp
   :language: c++
   :emphasize-lines: 82, 88, 95, 108-110, 117
   :caption: FastPolling event and mutex semaphores (ThreadsExample3)
   :linenos:
   
Instructions on how to compile and execute the examples can be found :doc:`here </core/examples>`.