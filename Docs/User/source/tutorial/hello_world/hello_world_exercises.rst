.. date: 21/03/2026
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

Exercices
=========

Ex. 1: Modify the thread frequency
----------------------------------

1. Edit the file ``../Configurations/HelloWorld/RTApp-HelloWorld-2.cfg`` and modify the frequency of the real-time thread to ``10 Hz``.
2. Check that the counter increases at the new rate.

.. code-block:: bash

    ./MARTeApp.sh -f ../Configurations/HelloWorld/RTApp-HelloWorld-2.cfg -l RealTimeLoader -s State1

.. dropdown:: Solution
   :icon: key

   The solution is to modify the ``Frequency`` property of the signal that is used to pace the thread. A modified configuration file with the correct frequency is shown below.

   .. literalinclude:: /_static/tutorial/Configurations/HelloWorld/RTApp-HelloWorld-2-solution.cfg
      :language: c++
      :lines: 5-15
      :caption: Modified configuration with 10 Hz frequency
      :linenos: 
      :emphasize-lines: 9

Ex. 2: Find the typo
--------------------

1. Run the following application.

.. code-block:: bash

    ./MARTeApp.sh -f ../Configurations/HelloWorld/RTApp-HelloWorld-3.cfg -l RealTimeLoader -s State1

2. Identify the typo in the configuration file that prevents the application from running.

.. dropdown:: Solution
   :icon: key

   The State name is misspelled as ``Idle`` instead of ``State1`` in the configuration file.

   .. code-block:: console

      $ [InitialisationEr - RealTimeLoader.cpp:118]: Failed to PrepareNextState for state State1

   .. literalinclude:: /_static/tutorial/Configurations/HelloWorld/RTApp-HelloWorld-3.cfg
      :language: c++
      :lines: 70-83
      :caption: Modified state name in the configuration file
      :linenos: 
      :emphasize-lines: 3

3. Is there a way of launching the application without modifying the configuration file to fix the typo?

.. dropdown:: Solution
   :icon: key

   Yes, the state name can be overridden at launch time by using the ``-s`` option of the ``MARTeApp.sh`` script. For example, the following command will launch the application with the correct state name:

   .. code-block:: bash

       ./MARTeApp.sh -f ../Configurations/HelloWorld/RTApp-HelloWorld-3.cfg -l RealTimeLoader -s Idle

Ex. 3: Modify the logged signal
-------------------------------

1. Edit the file ``../Configurations/HelloWorld/RTApp-HelloWorld-4.cfg`` and modify the configuration to log the ``Timer`` signal instead of the ``Counter`` signal.

.. dropdown:: Solution
   :icon: key

   The solution is to modify the signals in the configuration file.

   .. literalinclude:: /_static/tutorial/Configurations/HelloWorld/RTApp-HelloWorld-4-solution.cfg
      :language: c++
      :lines: 29-43
      :caption: Modified configuration with Time signal
      :linenos: 
      :emphasize-lines: 4, 10

Ex. 4: Signal names
--------------------

1. Run the following application.

.. code-block:: bash

    ./MARTeApp.sh -f ../Configurations/HelloWorld/RTApp-HelloWorld-5.cfg -l RealTimeLoader -s State1

2. Identify the typo in the configuration file that prevents the application from running.

.. dropdown:: Solution
   :icon: key

   MARTe signal names are case-sensitive. The signal name ``counter`` is misspelled as ``Counter`` in the configuration file.

   The GAMDataSource complains that the ``GAMDisplay`` GAM is requesting for a signal named ``Counter``, which does not exist and the application fails to start.

   .. code-block:: console

      $ [FatalError - GAMDataSource.cpp:400]: In GAMDataSource DDB1, state State1, signal Counter has an invalid number of producers. Should be > 0 but is 0

   .. literalinclude:: /_static/tutorial/Configurations/HelloWorld/RTApp-HelloWorld-5.cfg
      :language: c++
      :lines: 18-27
      :caption: Typo in the configuration file with the signal name
      :linenos: 
      :emphasize-lines: 2

Ex. 5: Signal alias
-------------------

1. Run the following application.

.. code-block:: bash

    ./MARTeApp.sh -f ../Configurations/HelloWorld/RTApp-HelloWorld-6.cfg -l RealTimeLoader -s State1

2. The application will fail to start with the following error message:

.. code-block:: console

   $ [FatalError - GAMDataSource.cpp:400]: In GAMDataSource DDB1, state State1, signal CounterIn has an invalid number of producers. Should be > 0 but is 0

3. Modify the configuration file to fix the issue by using the signal :ref:`Alias <MARTeSignalProperties>` property and make the application run.

.. dropdown:: Solution
   :icon: key

   MARTe allows the GAMs to rename signals with different names than the ones provided by the DataSources. 

   The scope of these signal names is private to the GAM.

   This is useful when the same signal is provided by different DataSources or when the signal name in the DataSource is not suitable for the GAM. 

   The solution is to use the :ref:`Alias <MARTeSignalProperties>` property to create an alias for the signal name. In this case, the signal ``Counter`` is aliased as ``CounterIn`` in the configuration file.

   .. literalinclude:: /_static/tutorial/Configurations/HelloWorld/RTApp-HelloWorld-6-solution.cfg
      :language: c++
      :lines: 29-44
      :caption: Signal alias
      :linenos: 
      :emphasize-lines: 5

4. Note that the OutputSignal name was also modified to ``CounterOut`` to keep the same naming convention. Why is this not an issue?

.. dropdown:: Answer
   :icon: key

   Some DataSources, such as the LoggerDataSource, allow to add signals in runtime. In this case, the LoggerDataSource will create a new signal named ``CounterOut`` and log its value.

Ex. 6: Signal types
--------------------

1. Run the following application.

.. code-block:: bash

    ./MARTeApp.sh -f ../Configurations/HelloWorld/RTApp-HelloWorld-7.cfg -l RealTimeLoader -s State1

2. Identify the typo in the configuration file that prevents the application from running.

.. dropdown:: Solution
   :icon: key

   A typo in the ``Time`` signal type is preventing the application from running. The signal type is misspelled as ``uint22`` instead of ``uint32`` in the configuration file. 

   .. code-block:: console

      $ [InitialisationEr - RealTimeLoader.cpp:118]: Failed to PrepareNextState for state State1

   .. literalinclude:: /_static/tutorial/Configurations/HelloWorld/RTApp-HelloWorld-7.cfg
      :language: c++
      :lines: 5-28
      :caption: Signal type typo in the configuration file
      :linenos: 
      :emphasize-lines: 6

3. Run the following application.

.. code-block:: bash

    ./MARTeApp.sh -f ../Configurations/HelloWorld/RTApp-HelloWorld-8.cfg -l RealTimeLoader -s State1

4. Identify the typo in the configuration file that prevents the application from running.

.. dropdown:: Solution
   :icon: key

   A typo in the ``Counter`` signal type in the ``Display`` GAM is preventing the application from running. The signal is produced and offered to the ``GAMDataSource`` as an ``uint32`` and is trying to be consumed as an ``uint16`` by the ``DisplayGAM``. 

   .. code-block:: console

      $ [InitialisationError - RealTimeApplicationConfigurationBuilder.cpp:1160]: The data source assigned to the signal Counter in GAMDisplay is incompatible
      $ [FatalError - RealTimeApplicationConfigurationBuilder.cpp:1037]: Failed to resolve for data source DDB1 and function GAMDisplay

   .. literalinclude:: /_static/tutorial/Configurations/HelloWorld/RTApp-HelloWorld-8.cfg
      :language: c++
      :lines: 5-43
      :caption: Signal type typo in the configuration file
      :linenos: 
      :emphasize-lines: 6, 30

Ex. 7: Component instantiation
------------------------------

1. Run the following application.

.. code-block:: bash

    ./MARTeApp.sh -f ../Configurations/HelloWorld/RTApp-HelloWorld-9.cfg -l RealTimeLoader -s State1

2. Identify and correct the typo in the configuration file that prevents the application from running.

.. dropdown:: Solution
   :icon: key

   A typo in the ``Class`` name of the GAMTimer does not allow MARTe to find the class to be automatically instantiated.

   .. code-block:: console

      $ [Warning - LoadableLibrary.cpp:78]: LoadableLibrary: Failed dlopen(): iogam.so: cannot open shared object file: No such file or directory
      $ [Warning - LoadableLibrary.cpp:78]: LoadableLibrary: Failed dlopen(): iogam.gam: cannot open shared object file: No such file or directory
      $ [Warning - LoadableLibrary.cpp:78]: LoadableLibrary: Failed dlopen(): iogam.drv: cannot open shared object file: No such file or directory
      $ [FatalError - Reference.cpp:129]: Reference: Failed CreateByName() in constructor
      $ [FatalError - ReferenceContainer.cpp:421]: Failed to Initialise object with name +GAMTimer

   All MARTe components are automatically :ref:`instantiated by the framework <MARTeDataDrivenApplications>` based on the configuration file. The ``Class`` property of each component specifies the type of the component to be instantiated. 
   
   If there is a typo in the class name, MARTe will not be able to find the class and will fail to instantiate the component.

   .. literalinclude:: /_static/tutorial/Configurations/HelloWorld/RTApp-HelloWorld-9.cfg
      :language: c++
      :lines: 5-29
      :caption: Class name typo in the configuration file
      :linenos: 
      :emphasize-lines: 2

1. Run the following application.

.. code-block:: bash

    ./MARTeApp.sh -f ../Configurations/HelloWorld/RTApp-HelloWorld-10.cfg -l RealTimeLoader -s State1

2. Identify the typo in the configuration file that prevents the application from running.

.. dropdown:: Solution
   :icon: key

   The name of the component to be instantiated does not start with a ``+`` character, which is required for components that are to be :ref:`instantiated by the framework <MARTeDataDrivenApplications>` in runtime. In this case the name of the component is ``GAMTimer`` instead of ``+GAMTimer``.

   .. code-block:: console

      $ [FatalError - RealTimeThread.cpp:156]: Undefined GAMTimer
      $ [ParametersError - RealTimeApplicationConfigurationBuilder.cpp:104]: Failed to ResolveStates

   .. literalinclude:: /_static/tutorial/Configurations/HelloWorld/RTApp-HelloWorld-10.cfg
      :language: c++
      :lines: 5-29
      :caption: Class name typo in the configuration file
      :linenos: 
      :emphasize-lines: 1

