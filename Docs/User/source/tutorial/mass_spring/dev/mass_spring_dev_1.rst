.. date: 17/04/2026
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

GAMs
====

The development of GAM components is discussed in the :doc:`GAM development section </core/app/gams/gam>`.

The most important methods in GAM development are:

- ``Initialise``, which is called once at the beginning of the application execution, is used to load the static parameters.
- ``Setup`` is called after the application is set up, when the characteristics of all input and output signals are known. It can be used to validate the signal characteristics and allocate any private memory that depends on them.
- ``Execute`` is called at each cycle of the application execution and is used to implement the actual functionality of the GAM. It is important to note that this method is called in a real-time thread and thus it should not contain any blocking calls or any calls to the operating system that might cause the thread to be preempted. It should also not contain any calls to the standard library that might cause the thread to be preempted (e.g. memory allocation, file I/O, etc.). The use of the standard library is not forbidden, but it should be used with caution and only for operations that are known to be safe in a real-time context (e.g. mathematical operations, string manipulation, etc.).

.. literalinclude:: /_static/tutorial/Source/GAMs/DeltaTSol/DeltaTSol.h
    :language: c++
    :lines: 106-112
    :caption: Static parameters definition.
    :linenos:

.. literalinclude:: /_static/tutorial/Source/GAMs/DeltaTSol/DeltaTSol.cpp
    :language: c++
    :lines: 65-78
    :caption: GAM Initialise method. Note the use of ``StructuredDataIHelper`` to validate the parameter.
    :linenos:

GAMs are divided into two broad categories:

- GAMs that are generic and adapt to the application requirements (e.g. the number of input and output signals, their data types, etc.). These GAMs are typically transversal to several applications and are therefore good candidates to be included in the MARTe2-components library. They usually require more extensive development, as they demand careful design and testing to cover edge cases.
- GAMs that are specific to a particular application.

For application-specific GAMs, the use of the ``marte2_manager`` to create the boilerplate code is recommended, as it allows developers to quickly create a new GAM and focus on implementing the functionality.

.. note::

    For generic GAMs, the use of the manager is not recommended, as it might not provide all the necessary flexibility for the design. In this case, it is recommended to start from an existing GAM with similar characteristics and modify it accordingly.

In this example, a GAM will be developed to generate a relative time from an absolute time signal. The user shall be able to specify a divider value named ``DeltaTDiv``, which will be used to divide the absolute time signal and generate the relative time signal.

In order to use the ``marte2_manager`` to create the boilerplate code for the GAM, the following command can be used:

.. code-block:: bash

    python3 -m marte2_manager.cli --project_name MARTe2-training-proj --project_path $HOME -l DEBUG add --cpt_type gams --cpt_name DeltaT --cpt_namespace Tutorial # Modify the project name and path if needed

This will create a new GAM component named ``DeltaT`` in the namespace ``Tutorial``, together with all the required Makefiles and tests.

To facilitate the generation of boilerplate code, a JSON template can be used to specify the characteristics of the GAM, including the number of input and output signals, their data types, and the static parameters. An example of such a template is shown in the following listings:

.. literalinclude:: /_static/tutorial/Resources/any_gam_template.json
    :language: json
    :caption: JSON definition for the generation of boilerplate code.
    :linenos:

.. literalinclude:: /_static/tutorial/Resources/delta_t_gam_template.json
    :language: json
    :caption: JSON definition for the generation of boilerplate code for the ``DeltaT`` GAM example.
    :linenos:

To update the GAM boilerplate code based on the JSON template, the following command can be used:

.. code-block:: bash

    python3 -m marte2_manager.cli --project_name MARTe2-training-proj --project_path $HOME -l DEBUG modify --cpt_type gams --cpt_name DeltaT --cpt_template $HOME/MARTe2-training-proj/Resources/delta_t_gam_template.json # Modify the project name and path if needed

The code can be immediately compiled and tested using the generated Makefiles.

.. code-block:: bash

    export TARGET=x86-linux
    make -C $HOME/MARTe2-training-proj/ -f Makefile.gcc core

The code must then be modified to add the missing variables and implement the ``Execute`` method. The implementation can be copied from the ``DeltaTSol`` GAM.

.. literalinclude:: /_static/tutorial/Source/GAMs/DeltaTSol/DeltaTSol.h
    :language: c++
    :lines: 128-131
    :caption: Add the ``lastAbsoluteTime`` variable to the GAM header file.
    :linenos:

.. literalinclude:: /_static/tutorial/Source/GAMs/DeltaTSol/DeltaTSol.cpp
    :language: c++
    :lines: 44-57
    :caption: Initialise the ``lastAbsoluteTime`` variable.
    :linenos:
    :emphasize-lines: 13

.. literalinclude:: /_static/tutorial/Source/GAMs/DeltaTSol/DeltaTSol.cpp
    :language: c++
    :lines: 122-135
    :caption: Implement the ``Execute`` method.
    :linenos:

Compile the GAM:

.. code-block:: bash

    export TARGET=x86-linux
    make -C $HOME/MARTe2-training-proj/ -f Makefile.gcc core

Note that the GAM path was automatically added to the ``MARTeApp.sh`` script.

Running the application
-----------------------

The GAM is already added to the configuration file ``../Configurations/MassSpring/RTApp-MassSpring-57.cfg``, so the application can be run directly after compiling the GAM.

.. code-block:: bash

    ./MARTeApp.sh -f ../Configurations/MassSpring/RTApp-MassSpring-57.cfg -l RealTimeLoader -m StateMachine::START

Once the application is running, inspect the ``screen`` output and verify that the application is running without issues. The log should show entries similar to the following:

.. code-block:: console

    $ [Information - LoggerBroker.cpp:152]: Time [0:0]:3830000
    $ [Information - LoggerBroker.cpp:152]: AbsoluteTime [0:0]:7437436439108
    $ [Information - LoggerBroker.cpp:152]: DeltaTime [0:0]:3820000
    ...

The ``marte2_manager`` also allows to print a snippet of the configuration file that corresponds to the GAM being developed.

.. code-block:: bash

    python3 -m marte2_manager.cli --project_name MARTe2-training-proj --project_path $HOME -l DEBUG print --cpt_type gams --cpt_name DeltaT --cpt_template $HOME/MARTe2-training-proj/Resources/delta_t_gam_template.json # Modify the project name and path if needed

Exercise
--------

The objective of this exercise is to implement the MassSpring model in a GAM.

The configurable parameters are:

- Mass (m) [kg] – must be set and greater than zero
- Spring constant (k) [N/m] – default value: 10.0
- Damping coefficient (c) [Ns/m] – default value: 0.5
- Initial position (x0) [m] – default value: 0.0
- Initial velocity (v0) [m/s] – default value: 0.0

The input signals are:

- Time (t) [microseconds]
- Force (F) [N]

The output signals are:

- Position (x) [m]
- Velocity (v) [m/s]

Based on the file ``Resources/any_gam_template.json``, create a new JSON template for the MassSpring GAM that implements the requirements above.

1. Run the manager command to generate the boilerplate code for the MassSpring GAM based on the JSON template created in the previous step.

.. code-block:: bash

    python3 -m marte2_manager.cli --project_name MARTe2-training-proj --project_path $HOME -l DEBUG add --cpt_type gams --cpt_name MassSpringModel --cpt_namespace Tutorial --cpt_template $HOME/MARTe2-training-proj/Resources/mass_spring_gam_template.json # Modify the project name and path if needed

2. Modify the generated code to implement the functionality of the MassSpring model based on the equations of motion of the system. The equations of motion are described in the :doc:`MathExpressionGAM section</tutorial/mass_spring/gams/mass_spring_gams_3>` of the tutorial.

3. Compile the GAM using the generated Makefiles.

.. code-block:: bash

    export TARGET=x86-linux
    make -C $HOME/MARTe2-training-proj/ -f Makefile.gcc core

4. Execute the application:

.. code-block:: bash

    ./MARTeApp.sh -f ../Configurations/MassSpring/RTApp-MassSpring-58.cfg -l RealTimeLoader -m StateMachine::START

5. Inspect the ``screen`` output and verify that the application is running without issues. The log should show entries similar to the following:

.. code-block:: bash

    ...
    $ [Information - LoggerBroker.cpp:152]: Time [0:0]:7010000
    $ [Information - LoggerBroker.cpp:152]: Position [0:0]:2.000131
    $ [Information - LoggerBroker.cpp:152]: PositionM [0:0]:2.010475
    $ [Information - LoggerBroker.cpp:152]: PositionSPM [0:0]:2.002166
    $ [Information - LoggerBroker.cpp:152]: Velocity [0:0]:-0.000294
    $ [Information - LoggerBroker.cpp:152]: VelocityM [0:0]:-0.012081
    $ [Information - LoggerBroker.cpp:152]: VelocitySPM [0:0]:0.048822

Where ``PositionSPM`` and ``VelocitySPM`` are the position and velocity calculated by the MassSpringModel GAM, while ``Position``, ``PositionM``, ``Velocity`` and ``VelocityM`` are calculated by the ``MathExpressionGAM`` and the ``SSMGAM``. The values should be similar across the three GAMs, with minor differences due to numerical errors and differences in the integration methods used.

.. dropdown:: Solution
   :icon: key

    The solution is to create a new JSON template for the MassSpring GAM based on the requirements specified above, and then use the manager to generate the boilerplate code.

    .. literalinclude:: /_static/tutorial/Resources/mass_spring_gam_template_sol.json
        :language: json
        :caption: JSON definition for the generation of boilerplate code for the ``MassSpringModel`` GAM example.
        :linenos:

    Then implement the functionality of the MassSpring model based on the equations of motion.

    .. literalinclude:: /_static/tutorial/Source/GAMs/MassSpringModelSol/MassSpringModelSol.cpp
        :language: c++
        :lines: 165-181
        :caption: ``Execute`` method definition.
        :linenos:

    Note that the damping coefficient and the spring constant are divided by the mass in the ``Setup`` method.

    .. literalinclude:: /_static/tutorial/Source/GAMs/MassSpringModelSol/MassSpringModelSol.cpp
        :language: c++
        :lines: 99-104, 158-162
        :caption: ``Setup`` method definition (snippet).
        :linenos: