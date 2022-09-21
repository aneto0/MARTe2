.. date: 26/03/2018
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

Types
=====

In order to guarantee portability in any of the supported operating systems and architectures, the MARTe2 basic types shall always be used. 
   
Basic types
-----------

======== =======
Type     Meaning
======== =======
uint8    8 bit unsigned integer.
uint16   16 bit unsigned integer.
uint32   32 bit unsigned integer.
uint64   64 bit unsigned integer.
int8     8 bit signed integer.
int16    16 bit signed integer.
int32    32 bit signed integer.
int64    64 bit signed integer.
char8    8 character.
bool     Boolean (size depends on the architecture).
float32  IEEE 754 single precision float.
float64  IEEE 754 double precision float.
uintp    Sufficiently large to hold a pointer address in the target architecture.
CString  A C string.
CCString A C constant string.
======== =======

TypeDescriptor
--------------
The :vcisdoxygencl:`TypeDescriptor` is used to describe any of the MARTe basic types. In particular it can be used to describe a type using a string (e.g. "uint32" => UnsignedInteger32).
It is also a core component of the :vcisdoxygencl:`AnyType` described here below. 

AnyType
-------
The :vcisdoxygencl:`AnyType` allows to encapsulate the value and to describe any of the MARTe2 variables. The AnyType is capable of constructing itself from any of the BasicTypes and can be used as a generic interface to a given function.

.. warning:: The AnyType has no internal memory! The instances of AnyType do not hold a copy of the actual value, but a pointer to it, so the value must be accessible during the life of the AnyType instance (be particularly careful when returning AnyTypes that represent automatic variables or variables that get out of scope!

.. code-block:: c++
   
   void PrintType(const MARTe::AnyType &input) {
      using namespace MARTe;
      //Get the TypeDescriptor
      TypeDescriptor td = input.GetTypeDescriptor();
      uint32 numberOfBits = td.numberOfBits;
      //Automatically retrieve the name
      const char8 *const typeName = TypeDescriptor::GetTypeNameFromTypeDescriptor(td);
      //The logging mechanism will automatically print the value (note the %!) based on the TypeDescriptor.
      REPORT_ERROR_STATIC(MARTe::ErrorManagement::Information, "Type: %s; NumberOfBits: %d; Constant? :%d, "
          "Value: %!", typeName, numberOfBits, isConstant, input);
   }
   
   ...
   
   uint16 anUInt16 = 116;
   //Note that the AnyType is automatically constructed.
   PrintType(anUInt16);
   PrintType(32.0);

The :vcisdoxygencl:`AnyType` is a key design element on all the components that accept many types and also require type interpretation (i.e. need to adapt their behaviour based on the type). Examples are the :vcisdoxygencl:`StructuredDataI` Read and Write functions and the :vcisdoxygencl:`IOBuffer` Printf.   

Other types
-----------

==================================== =======
Type                                 Meaning
==================================== =======
:vcisdoxygencl:`BitBoolean`          When used in an union allows to have the same effect of a struct with a 1 bit boolean (e.g. :vcisdoxygencl:`TypeDescriptor`).
:vcisdoxygencl:`BitRange`            When used in an union allows to have the same effect of a struct with bit fielded attributes (e.g. :vcisdoxygencl:`TypeDescriptor`).
:vcisdoxygencl:`FractionalInteger`   An helper class that is used to define integer types with non-standard bit sizes, such as uint3 or uint63.
:vcisdoxygencl:`Matrix`              Fixed size matrix of values.
:vcisdoxygencl:`Vector`              Fixed size array of values.
:vcisdoxygencl:`ZeroTerminatedArray` Describes a zero terminated array.
==================================== =======

Lists
~~~~~

==================================== =======
Type                                 Meaning
==================================== =======
:vcisdoxygencl:`LinkedListable`      Singly linked list.
:vcisdoxygencl:`StaticList`          Templated version of the StaticListHolder.
:vcisdoxygencl:`StaticListHolder`    Abstract Data Type (ADT) for an unbounded list.
:vcisdoxygencl:`CircularStaticList`  Circular buffer implementation of the StaticList.
==================================== =======

Example
-------

The following example highlights the usage of the AnyType and of the TypeDescriptor.  

.. literalinclude:: /_static/examples/Core/TypesExample1.cpp
   :language: c++
   :emphasize-lines: 59,73-75,88,92
   :caption: AnyType example (TypesExample1)
   :linenos:
   
Instructions on how to compile and execute the example can be found :doc:`here </core/examples>`.
