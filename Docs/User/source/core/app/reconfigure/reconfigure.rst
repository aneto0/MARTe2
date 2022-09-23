.. date: 23/09/2022
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

Reconfiguration
===============

As discussed in the :doc:`application kick-starting </core/app/kickstart/kickstart>` section, the :vcisdoxygencl:`Loader` component (or any component inheriting from it) is responsible for configuring and starting a standard MARTe applicaton (i.e. one that relies on the `MARTeApp.cpp <https://vcis-gitlab.f4e.europa.eu/aneto/MARTe2/blob/master/Source/App/MARTeApp.cpp>`_).

The :vcisdoxygencl:`Loader` component also offers a reconfiguration interface (see ``Loader::Reconfigure``) that can be used in runtime to modify the configuration of an application that is already in execution.

Any component inheriting from the ``Loader`` class can implement the ``PostInit`` method in order to receive runtime configuration parameters (e.g. a TCP port where to listen for new configurations). 

The ``Loader`` also offers a set of pre-defined message triggers that are sent: before the configuration is modified; after the configuration is (successfully) applied; in case a configuration error occurs; and if a configuration occurs and the user has configured the option to reload the last valid configuration.

Finally, the ``Loader`` components also allows to query the currently loaded configuration (see ``Loader::GetLastValidConfiguration``).

The :vcisdoxygencl:`ConfigurationLoaderTCP` in an implementation that allows changing the runtime configuration over a TCP connection.

Examples
--------

Valid reconfiguration
~~~~~~~~~~~~~~~~~~~~~

This is an example of a ``MARTeApp`` that uses a ``ConfigurationLoaderTCP``. 

**Start the application with the -m parameter**.

To modify a configuration, start the application and, on another console, type ``nc localhost 24680 < RTApp-9-reload.cfg``. 

The new configuration has a higher cycle time so that the frequency at which the signals are printed into console should significantly increase.

To return to a configuration running at a lower frequency, on another console, type ``nc localhost 24680 < RTApp-9-reload-2.cfg``. 

.. literalinclude:: /_static/examples/Configurations/RTApp-9.cfg
   :language: bash	
   :caption: ConfigurationLoaderTCP application (Run with NAME_OF_THE_MESSAGE=StateMachine:START and NAME_OF_THE_CONFIGURATION_FILE=RTApp-9.cfg)
   :linenos:
   :emphasize-lines: 1-22

Invalid reconfiguration
~~~~~~~~~~~~~~~~~~~~~~~

Given that the application was launched with the parameter `ReloadLast=true`, a failed reconfiguration attempt will trigger the reloading of the last valid configuration.

To load an invalid configuration, start the application and, on another console, type ``nc localhost 24680 < RTApp-9-reload-fail.cfg``. Notice that the application will continue to run at the original frequency.
