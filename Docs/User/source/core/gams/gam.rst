.. date: 06/04/2018
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

GAM
===

The Generic Application Module (:vcisdoxygencl:`GAM`) is the component where user-algorithms are to be implemented.

The GAM offers one interface for configuration and two interfaces for real-time input and output processing.

.. image:: GAM-1.png

.. warning::
   No interface with operating system (e.g. reading from files/sockets) shall be implemented in the GAMs. The only exception is memory allocation during configuration.   

GAMs can be conceptually divided in two sets: one where the inputs and outputs signals (number, type and dimensions) are fixed by design; and another where the algorithm behaviour varies with the signal characteristics of a given real-time application (see examples below).  

Configuration
-------------

A GAM is initialised just like any other :doc:`MARTe Object </core/configuration/objects>`.

The properties related to the input and output signals are available when the ``Setup`` method is called. At this stage any of the signal related methods described in the GAM API can be used to query the signal properties.

.. code-block:: c++   

   virtual bool MyGAM::Setup () {
      ...
      uint32 numberOfInputSignals = GetNumberOfInputSignals();
      ...
      for (i = 0u; i<numberOfInputSignals; i++) {
         TypeDescriptor td = GetSignalType(InputSignals, i);
         ...
      }
      ...
   }

Typical use cases of the Setup method are to validate and/or configure the algorithm against the input/output signal requirements of the specific application (e.g. the number, type and dimensions);   

.. code-block:: c++   

   virtual bool MyGAM::Setup () {
      ...
      uint32 numberOfInputSignals = GetNumberOfInputSignals();
      ...
      //Only accept one input signal
      bool ok = (numberOfInputSignals == 1u);
      ...
   }
   
In the configuration stream the input signals shall be placed inside a node named ``InputSignals`` and the output signals inside a node named ``OutputSignals``. 

.. code-block:: c++   

   ...
   InputSignals = {
      Counter = {
         DataSource = DDB1
         Type = uint32
      }                
   }
   OutputSignals = {
      GainCounter = {
         DataSource = DDB1
         Type = uint32
      }                
   }
   ...

Signal properties
~~~~~~~~~~~~~~~~~

The signal name (in the context of the GAM) is the name of the node. Other properties that can be set for any signal are:

==================== =======
Property             Meaning
==================== =======
Type                 The signal type as any of the supported :doc:`/core/types/types`
DataSource           The name of the DataSource from where the signal will read/written from/to.
Frequency            Only meaningful for input signals. The frequency at which the signal is expected to be produced (at most one signal per real-time thread) may have this property set.
Trigger              Only meaningful for output signals. Trigger the DataSource when this signal is written.
NumberOfElements     The number of elements (1 if the signal is a scalar).
NumberOfDimensions   The number of dimensions (0 if scalar, 1 if vector, 2 if matrix).
Samples              The number of samples to read from a DataSource.  This number defines the number of samples that the DataSource shall acquire for each control cycle. Note that each sample may contain an array. Indeed, the amount of memory required to hold a signal of type T, with M samples and N elements is: sizeof(T) x M x N. Typical use cases: i) ADC: M samples, 1 element; ii) Image: 1 sample, N elements; iii) Video: M samples, N elements.   
Ranges               In the case of a vector read/write only a subset. The format is a matrix, indexed to zero, of the ranges that are to be read (e.g. ``{{0, 1}, {3, 5}}`` would read elements 0, 1, 3, 4 and 5 of the array).
Alias                The name of the signal in the DataSource (which can be different from the name of the signal in the GAM).
==================== =======  
   
   
   
Real-time execution
-------------------

The ``Execute`` method is called at every real-time cycle. 

When the method is called, the input signals will be ready to be processed by the GAM. After the method is called the output signals will be propagated accordingly. 


