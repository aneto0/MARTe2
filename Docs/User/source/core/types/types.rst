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
uint8    8 bit signed integer.
uint16   16 bit signed integer.
uint32   32 bit signed integer.
uint64   64 bit signed integer.
char8    8 character.
float32  IEEE 754 single precision float.
float64  IEEE 754 double precision float.
uintp    Sufficiently large to hold a pointer address in the target architecture.
CString  A C string.
CCString A C constant string.
======== =======

Other types
-----------

=================================== =======
Type                                Meaning
=================================== =======
:vciscorebml0:`BitBoolean`          When used in an union allows to have the same effect of a struct with a 1 bit boolean (e.g. :vciscorebml1:`TypeDescriptor`).
:vciscorebml0:`BitRange`            When used in an union allows to have the same effect of a struct with bit fielded attributes (e.g. :vciscorebml1:`TypeDescriptor`).
:vciscorebml0:`FractionalInteger`   An helper class that is used to define integer types with non-standard bit sizes, such as uint3 or uint63.
:vciscorebml0:`Matrix`              Fixed size matrix of values.
:vciscorebml0:`Vector`              Fixed size array of values.
:vciscorebml0:`ZeroTerminatedArray` Describes a zero terminated array.
=================================== =======


