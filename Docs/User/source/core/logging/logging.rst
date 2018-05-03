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

Logging
=======

MARTe offers a logging mechanism that is both :ref:`thread and interrupt safe <loggerservice>`. 

.. note::
   The number of logging messages is expected to be maximised in any MARTe component.

ErrorType
---------

The framework offers the following ErrorTypes (i.e. severities, see :vcisdoxygenclem:`ErrorType`).

=================== =======
Type                Meaning
=================== =======
FatalError          An error which is not recoverable.
RecoverableError    The component can recover from this error.
InitialisationError Error which occurred during the initialisation of the component.
OSError             Operating system error (typically used when developing interfaces to hardware).
ParametersError     Initialisation error due to misconfiguration of parameters.
IllegalOperation    Operation not possible in a given state.
ErrorSharing        Given resource cannot be shared.
ErrorAccessDenied   Access denied to a given resource.
Exception           Runtime exception triggered.
Timeout             Timeout waiting for a given resource.
CommunicationError  Communication error (sockets, files, ...).
SyntaxError         Invalid syntax.
UnsupportedFeature  Request for a feature that is not available (possibly available on other states).
InternalSetupError  Error due 
Debug               Debug messages.
Information         Information messages.
Warning             Warning messages.
Completed           ErrorType to be used when a given operation has completed.
NotCompleted        ErrorType to be used when a given operation has not yet completed. Usually a retry is expected.
=================== =======  

Log macros
----------

The :vciscorebml3:`AdvancedErrorManagement` defines two main error logging macros. 

The ``REPORT_ERROR_STATIC`` is to be called by classes that do not inherit from :vcisdoxygencl:`Object` while the ``REPORT_ERROR`` should be called by classes that inherit from Object, since this will automatically add the object name, class name and object pointer to the log message.  

Both macros expect a compulsory :vcisdoxygenclem:`ErrorType`, followed by a compulsory error/information string and a list of optional parameters that will be ``Printf`` in the String using the rules described in the :doc:`/core/streams/streams` section. 

.. code-block:: c++   

   ...
   REPORT_ERROR(ErrorManagement::Information, "Array set to %f", readVector);
   ...
   
Callback
--------

The messages are processed by a user registered callback function. 

This is set by calling the global function ``SetErrorProcessFunction`` with a pointer to user error handling function.

.. code-block:: c++

   typedef void (*ErrorProcessFunctionType)(const ErrorInformation &errorInfo, const char8 * const errorDescription);   
   
The ``errorDescription`` already contains the *printfed* message while the :vcisdoxygenstem:`ErrorInformation` provides a list of properties related to the conditions at the time of the logging (e.g. time, line number, ...).

.. code-block:: c++
   
   void ErrorProcessExampleFunction(const MARTe::ErrorManagement::ErrorInformation &errorInfo, const char * const errorDescription) {
      ...
      printf("[%s - %s:%d]: %s\n", errorCodeStr.Buffer(), errorInfo.fileName, errorInfo.header.lineNumber, errorDescription);
      printf(RST);
      ...
   
.. _loggerservice:

LoggerService
-------------   

The framework also offers a data-driven :vcisdoxygencl:`LoggerService` which decouples the log production from the log consumption using a decoupling queue.

The :vcisdoxygencl:`LoggerService` allows to register one or more :vcisdoxygencl:`LoggerConsumerI` components, which will consume the logging message accordingly. Note that the ``LoggerService`` will override any callback function previously set by the user. 

The framework currently offers the following consumers: :vcisdoxygencl:`ConsoleLogger`, :vcisdoxygencl:`UDPLogger` and :vcisdoxygenmccl:`SysLogger` (from the :vcisgl:`MARTe2-components`).  

Examples
--------

The following is an example of a LoggerService instance which prints the logging messages in the console. 

.. literalinclude:: /_static/examples/Core/LoggerServiceExample1.cpp
   :language: c++
   :caption: LoggerService example (LoggerServiceExample1)
   :linenos:
   :emphasize-lines: 107-109,112

Instructions on how to compile and execute the example can be found :doc:`here </core/examples>`.  

