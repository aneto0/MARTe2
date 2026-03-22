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

FAQ
===

.. dropdown:: Error message: `[FatalError - RealTimeApplication.cpp:73]: Could not stop the RealTimeApplication. Was it ever started?`?
   :icon: question

   The destruction of the RealTimeApplication object is being attempted without the application having been started. This can be caused by a failure during the startup phase of the application, which prevents the application from reaching the state where it is considered "started".  It rarely is the root cause of the issue, but rather a consequence of an earlier failure during startup. To identify the root cause, it is necessary to look at the log messages that appear before this error message in the application log.

.. dropdown:: What are the rules for signal naming in the configuration file?
   :icon: question

   Typically GAMs and DataSources do not force signals to have specific names, and the user is free to choose the signal names in the configuration file. 
   
   Users should however check on the component documentation if there are any specific requirements or conventions for signal names, as some components may impose signal names or signal conventions.

   For example, in the HelloWorld application, the signal produced by the ``GAMTimer`` is named ``Counter``. This name is not fixed and can be changed to any other valid name without affecting the functionality of the application, as long as the signal connections are correctly updated in the configuration file.

   For example the configuration files ``RTApp-HelloWorld-1.cfg`` and ``RTApp-HelloWorld-11.cfg`` are totally equivalent.

   .. grid:: 2

      .. grid-item-card:: Before

         .. literalinclude:: /_static/tutorial/Configurations/HelloWorld/RTApp-HelloWorld-1.cfg
            :language: c++
            :lines: 5-12,60-64
            :linenos: 
            :emphasize-lines: 4,10

      .. grid-item-card:: After

         .. literalinclude:: /_static/tutorial/Configurations/HelloWorld/RTApp-HelloWorld-11.cfg
            :language: c++
            :lines: 5-12,60-64
            :linenos: 
            :emphasize-lines: 4,10

.. dropdown:: What are the available signal properties?
   :icon: question

   See the :ref:`MARTeSignalProperties` documentation for a complete list of available signal properties.

.. dropdown:: What are the available signal types?

   See the :ref:`MARTe Types <MARTeTypes>` documentation for a complete list of available signal types.

.. dropdown:: Can I use structured types for the signals?

   Yes, see the :ref:`GAM documentation <StructuredGAM>` .

.. dropdown:: What are the available GAM properties?
   :icon: question

   All the MARTe component properties, including those of GAMs are specific to the component and are documented in the component API documentation.

   .. warning::

      The component properties are processed by the component itself and properties that are not recognised by the component will be ignored. 
      
      This means that if a property is misspelled or if a property is used that does not exist for the component, the application will still run without any error message, but the property will not have any effect on the behaviour of the component. 

      In the following example the property Frequency in the GAMTimer outside the InputSignals is ignored:

      .. literalinclude:: /_static/tutorial/Configurations/HelloWorld/RTApp-HelloWorld-12.cfg
         :language: c++
         :lines: 1-18
         :linenos: 
         :emphasize-lines: 7,14

.. dropdown:: What are the available DataSource properties?
   :icon: question

   All the MARTe component properties, including those of DataSources are specific to the component and are documented in the component API documentation.

.. dropdown:: Why do I see the acronym "DDB" in many configuration files?
   :icon: question

   The DDB is the name of the original implementation of the GAMDataSource in MARTe1. It is used interchangeably with GAMDataSource in MARTe2, and it is still present in many configuration files for historical reasons. The DDB is a simple DataSource that provides an interface for GAMs to exchange data between each other, without any connection to external hardware or software. 

