.. date: 12/04/2018
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

RealTimeApplication
===================

A MARTe :vcisdoxygencl:`RealTimeApplication` is assembled by connecting together :doc:`GAMs </core/gams/gam>` and :doc:`DataSources </core/gams/datasource>`.

GAMs are grouped in real-time threads which are executed in the context of specific states. A :vcisdoxygencl:`RealTimeApplication` shall be in one (and only one) given state at any given time.

The orchestration of the states is performed by means of :doc:`messages  </core/messages/messages>` (e.g. by a component that relays HTTP requests into MARTe messages). 

.. image:: RTApp-1.png

Configuration
-------------

The configuration of a RealTimeApplication has to follow a set of strict rules. 

In particular, the nodes ``Functions``, ``Data``, ``States`` and ``Scheduler`` shall exist and shall be configured using the rules defined below:

Functions
---------

GAMs are listed inside a node named ``Functions`` of type ``ReferenceContainer``.

.. code-block:: bash

   $App1 = {
      Class = RealTimeApplication
      +Functions = {
         Class = ReferenceContainer
         +GAM1 = {
            Class = AGAM
            InputSignals = {
               ...
            }
            OutputSignals = {
                ...
            }
         }
         +GAM2 = {
            Class = BGAM
            InputSignals = {
               ...
            }
            OutputSignals = {
               ...
            }
         }
         ...
      }
      ...
   }

GAMs can also be grouped inside reference containers (in order to improve the readability of a configuration stream) and inside :doc:`GAMGroups </core/gams/gam>`.

.. code-block:: bash

   $App1 = {
      Class = RealTimeApplication
      +Functions = {
         Class = ReferenceContainer
         +Controllers = {
            Class = ReferenceContainer
            +PID1 = {
               Class = PIDGAM
               InputSignals = {
                  ...
               }
               OutputSignals = {
                  ...
               }
            }
            +PID2 = {
               Class = PIDGAM
               InputSignals = {
                  ...
               }
               OutputSignals = {
                  ...
               }
            }
         }
         +GAM2 = {
            Class = GAMGroup
            +GAM3 = {
               InputSignals = {
                  ...
               }
               OutputSignals = {
                  ...
               }
            }
            +GAM4 = {
               InputSignals = {
                  ...
               }
               OutputSignals = {
                  ...
               }
            }
         ...
      }
      ...
   }

Data
----

DataSources are grouped inside a node named ``Data`` of type ``ReferenceContainer``.

.. code-block:: bash

   $App1 = {
      Class = RealTimeApplication
      +Functions = {
         Class = ReferenceContainer
         ...
      }
      +Data = {
         Class = ReferenceContainer
         DefaultDataSource = DDB1
         +DDB1 = {
            Class = GAMDataSource
         }        
         +LoggerDataSource = {
            Class = LoggerDataSource
         }
         +Timings = {
            Class = TimingDataSource
         }
         +Timer = {
            Class = LinuxTimer
            SleepNature = "Default"
            Signals = {
                Counter = {
                    Type = uint32
                }
                Time = {
                    Type = uint32
                }
            }
        }
        ...
      ...
   }
   
A component of class type ``TimingDataSource`` shall exist.

The property ``DefaultDataSource`` shall define what is the DataSource to be used when the :doc:`GAM </core/gams/gam>` signals do not specify the ``DataSource`` property.

TimingDataSource
~~~~~~~~~~~~~~~~

The RealTimeApplication will automatically add to the TimingDataSource the following signals: 

- For every RealTimeThread, it will generate a signal named ``STATE_NAME.THREAD_NAME_CycleTime``, where ``STATE_NAME`` is the name of the state where the thread is running and ``THREAD_NAME`` is the name of the RealTimeThread object instance. The type of this signal is uint32 and it holds the thread cycle time.
- For every GAM, it will generate three signals named: ``GAM_NAME_ReadTime``, ``GAM_NAME_WriteTime`` and ``GAM_NAME_ExecTime`` where `GAM_NAME`` is the object name of the GAM instance. The type of these signals is uint32.
   - The ``GAM_NAME_ReadTime`` holds the time elapsed from the beginning of the cycle until all the input brokers for this ``GAM_NAME`` have been executed;
   - The ``GAM_NAME_WriteTime`` holds the time elapsed from the beginning of the cycle until all the output brokers for this ``GAM_NAME`` have been executed;
   - The ``GAM_NAME_ExecTime`` holds the time elapsed from the beginning of the cycle until this ``GAM_NAME`` has finished its execution.

.. code-block:: bash

   $App1 = {
      Class = RealTimeApplication
      +Functions = {
         Class = ReferenceContainer
         +GAMDisplay = {
            Class = IOGAM            
            InputSignals = {
               State1_Thread1_CycleTime = {
                   Alias = State1.Thread1_CycleTime
                   DataSource = Timings
                   Type = uint32
               }
               GAMFixed1_ReadTime = {
                   DataSource = Timings
                   Type = uint32
               }
               GAMFixed1_ExecTime = {
                   DataSource = Timings
                   Type = uint32
               }
               GAMFixed1_WriteTime = {
                   DataSource = Timings
                   Type = uint32
               }
            ...
            }
          ...
          }
      ...
      }
      +Data = {
         Class = ReferenceContainer
         DefaultDataSource = DDB1
         ...
         +Timings = {
            Class = TimingDataSource
         }
         ...
         

.. warning::
   
   Given that a MARTe object name may not contain a ``.`` and given that the cycle time signal produced by the Timings DataSource is named ``STATE_NAME.THREAD_NAME_CycleTime``, an Alias will always have to be used to read the signal.
   
   .. code-block:: bash
   
      InputSignals = {
         State1_Thread1_CycleTime = {
            Alias = State1.Thread1_CycleTime
            DataSource = Timings
         

States
------

The available RealTimeApplication states are listed inside a node named ``States`` of type :vcisdoxygencl:`RealTimeState`.

Each state shall have a node named ``Threads`` with the list of :vcisdoxygencl:`RealTimeThread` components to be executed in that state.

Each RealTimeThread shall contain the name of the ``Functions`` to be executed. Note that if the function is a ReferenceContainer or a GAMGroup, the GAMs contained inside these containers shall not be declared (as they are automatically added for scheduling). 

.. code-block:: bash

   $App1 = {
      Class = RealTimeApplication
      +Functions = {
         Class = ReferenceContainer
         +Controllers1 = {
            Class = ReferenceContainer
            +PID1 = {
               ...
            }
            +PID2 = {
               ...
            }
         }
         +Controllers2 = {
            Class = ReferenceContainer
            +PID3 = {
               ...
            }
            +PID4 = {
               ...
            }
         }
         +GAMGroup1 = {
            Class = GAMGroup
            +GAM3 = {
               ...
            }
            +GAM4 = {
               ...
            }
         ...
         }
         +Reference1 = {
            Class = AGAM
            ...
         }
         +Reference2 = {
            	Class = AGAM
            ...
         }
      }
      +States = {
         Class = ReferenceContainer
         +State1 = {
            Class = RealTimeState
            +Threads = {
               Class = ReferenceContainer
               +Thread1 = {
                  Class = RealTimeThread
                  CPUs = 0x1 //Only meaningful in operating systems that support it
                  Functions = {Reference1 Controllers1 GAMGroup1} // Note that PID1 and PID2, etc are implicitly called.
               }
               +Thread2 = {
                  Class = RealTimeThread
                  CPUs = 0x2 //Only meaningful in operating systems that support it
                  Functions = {Reference2 Controllers2}
               }
           }
        }
        +State2 = {
           Class = RealTimeState
           +Threads = {
              Class = ReferenceContainer
              +Thread1 = {
                  Class = RealTimeThread
                  Functions = {Reference1 GAMGroup1 }
              }
           }
     

Scheduler
---------

The scheduler is defined in a node named ``Scheduler`` and the class shall inherit from :vcisdoxygencl:`GAMSchedulerI`. 

The name of the node containing the TimingDataSource class shall be defined in a parameter named ``TimingDataSource``.

.. code-block:: bash

   $App1 = {
      Class = RealTimeApplication
      ...
      +Scheduler = {
         Class = GAMScheduler
         TimingDataSource = Timings
      }

The framework offers a multi-thread scheduler (see :vcisdoxygencl:`GAMScheduler`). Bare metal applications will typically develop bespoke scheduling mechanisms. 

Initialisation
--------------

A RealTimeApplication is initialised just like any other :doc:`MARTe Object </core/configuration/objects>`. Internally, the RealTimeApplication initialisation is mostly delegated to the :vcisdoxygencl:`RealTimeApplicationBuilder` component and is divided in two stages: first all the signals dependencies, between the GAMs and the DataSources, are completed and verified (see :ref:`Rules` below). This is followed by a stage where the Brokers are instantiated and all the required memory allocated.

The two states are independent and can be performed in separate phases. This allows, for example, to pre-compute (see ``RealTimeApplicationBuilder::ConfigureBeforeInitialisation``) and directly store in the memory of a low-power embedded system the output of the signal verification logic (phase 1 of the initialisation). 

The RealTimeApplication will try to complete the missing signal properties based on the information that is available. If a GAM requires a signal that is produced by a subsequent GAM, an implicit zero-hold is introduced in the cycle and the signal is initialised to its Default value.

.. image:: RTApp-2.png

.. _Rules:

Rules
-----

The following rules have to be met and are verified by the ``RealTimeApplication`` initialisation mechanism:

Global rules
~~~~~~~~~~~~

1. The nodes ``Functions``, ``Data``, ``States`` and ``Scheduler`` shall exist;
2. At least one GAM shall be declared;
3. At least one DataSource shall be declared;
4. Exactly one TimingDataSource shall be declared;
5. At least one state shall be declared;
6. For each state, at least one thread shall be declared;
7. For each thread, at least one function (GAM) shall be declared;

Signal rules
~~~~~~~~~~~~

1. For every thread, the input port of each GAM or DataSource shall be connected to exactly one signal (from another GAM or from a DataSource);
2. For every thread, the output port of a given GAM or DataSource may be connected to zero or more signals (in another GAM or DataSource);
3. The properties (type, number of elements and number of dimensions) of each signal shall be fully consistent between the signal producer and the signal consumer;
4. The signal type shall be defined either by the signal producer or by one of the signal consumers:
   
   a. If the number of elements is not defined, one is assumed;
   b. the number of dimensions is not defined, zero is assumed (scalar signal);
   c. If no Default value is specified, zero is assumed.
5. For every thread, at most one signal shall define the property `Frequency` (i.e. at most one synchronisation point per thread).

The following is a valid example where the GAM resolves the signal type from the DataSource.

.. image:: RTApp-SignalResolution-1.png

In this example the type of the signal *A1* is not defined by the producer nor by the source and thus the configuration is invalid and will fail.

.. image:: RTApp-SignalResolution-2.png

In the following, there is a contradiction between the properties of the signal *A1* and thus the configuration will also fail.

.. image:: RTApp-SignalResolution-3.png

The correct way to access an array of smaller dimensions is to use the parameter ``Ranges``. The GAM parameter will only use (and allocate memory) to the first two elements of the array:

.. image:: RTApp-SignalResolution-4.png 

State change
------------

The state can be changed by calling the methods ``PrepareNextState``, ``StopCurrentStateExecution`` and ``StartNextStateExecution`` on the :vcisdoxygencl:`RealTimeApplication`.

These are methods are registered as RPC functions and thus can be triggered using the :doc:`messaging mechanisms </core/messages/messages>`.

Typically the interface to the state changing mechanism is provided by the :doc:`StateMachine </core/statemachine/statemachine>`.

.. image:: RealTimeStateMachineExampleStateMachine.png

.. code-block:: bash
   
   +StateMachine = {
      Class = StateMachine
      ...
      +STATE1 = {
         Class = ReferenceContainer
         +GOTOSTATE2 = {
            Class = StateMachineEvent
            NextState = "STATE2"
            NextStateError = "ERROR"
            Timeout = 0
            +PrepareChangeToState2Msg = {
               Class = Message
               Destination = TestApp
               Mode = ExpectsReply
               Function = PrepareNextState
               +Parameters = {
                  Class = ConfigurationDatabase
                  param1 = State2
               }
            }
            +StopCurrentStateExecutionMsg = {
               Class = Message
               Destination = TestApp
               Function = StopCurrentStateExecution
               Mode = ExpectsReply
            }
            +StartNextStateExecutionMsg = {
               Class = Message
               Destination = TestApp
               Function = StartNextStateExecution
               Mode = ExpectsReply
            }
            ...

Synchronising multiple threads
------------------------------

The synchronisation between threads is performed using the :vcisdoxygenmccl:`RealTimeThreadSynchronisation` DataSource component.

.. image:: RTApp-3.png

The threads synchronising can run at a frequency which is sub-multiple of the master (i.e. the one with the GAM writing to the DataSource) thread frequency . This is expressed by asking for a number of samples (> 1) to the RealTimeThreadSynchronisation DataSource.

The :vcisdoxygenmccl:`RealTimeThreadAsyncBridge` component also allows to exchange data between threads without an explicit synchronisation point. This means that the consumer threads will use the latest available data.


Examples
--------

GAMGroup and Ranges
~~~~~~~~~~~~~~~~~~~

In the RealTimeApplication example below, note that only the name of the parent GAMGroup has to be set for scheduling (as opposed to individually listing all of its internal GAM members). 

Note also how the ``Ranges`` can be used to only access a subset of an array. 
   
.. literalinclude:: /_static/examples/Configurations/RTApp-4.cfg
   :language: bash   
   :caption: GAMGroups and Ranges (Run with NAME_OF_THE_STATE=State1 and NAME_OF_THE_CONFIGURATION_FILE=RTApp-4.cfg)
   :linenos:
   :emphasize-lines: 45,132-133,161,202

Execution times
~~~~~~~~~~~~~~~

.. image:: RealTimeSynchExampleStateState1.png

The following is an example of a RealTimeApplication which prints the execution times of the real-time thread and of the GAMs (including the brokers' read/write times).
	
.. literalinclude:: /_static/examples/Configurations/RTApp-1.cfg
   :language: bash	
   :caption: Execution times measuremetn (Run with NAME_OF_THE_STATE=State1 and NAME_OF_THE_CONFIGURATION_FILE=RTApp-1.cfg)
   :linenos:
   :emphasize-lines: 56-58,61-63,65-67,69-71

Multiple states
~~~~~~~~~~~~~~~

.. image:: RealTimeStateMachineExampleRTApp.png

This is an example of a RealTimeApplication with two states. The custom component ``TCPSocketMessageProxyExample`` forwards TCP *messages* into MARTe messages. 

**Start the application with the -m parameter**.

In order to change state, start the application and, in another console, type ``echo -e "Destination=StateMachine\nFunction=GOTOSTATE2" | nc 127.0.0.1 24680``.
	
.. literalinclude:: /_static/examples/Configurations/RTApp-3.cfg
   :language: bash	
   :caption: Multiple states configuration (Run with NAME_OF_THE_MESSAGE=StateMachine:START and NAME_OF_THE_CONFIGURATION_FILE=RTApp-3.cfg)
   :linenos:
   :emphasize-lines: 1-3,477,482,490,493,495,498,500,503,511,514

Thread Synchronisation
~~~~~~~~~~~~~~~~~~~~~~

The following is an example of a RealTimeApplication with multiple synchronised threads. Note that the Thread2 and the Thread3 run at a sub-frequency of Thread1.

.. image:: RealTimeSynchExampleStateState1.png
   
.. literalinclude:: /_static/examples/Configurations/RTApp-2.cfg
   :language: bash	
   :caption: Thread synchronisation configuration (Run with NAME_OF_THE_STATE=State1 and NAME_OF_THE_CONFIGURATION_FILE=RTApp-2.cfg)
   :linenos:
   :emphasize-lines: 90,133

   
Instructions on how to compile and execute the examples can be found :doc:`here </core/examples>`.