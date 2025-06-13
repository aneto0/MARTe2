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

Introspection
=============

The MARTe2 :vcisdoxygencl:`Introspection` and :vcisdoxygencl:`IntrospectionT` allow to register in the :vcisdoxygencl:`ClassRegistryDatabase` members of a class or a ``C struct``. 

This allows to develop components that query and adapt in runtime to the properties of a given class or structure. Examples are the  :doc:`configuration of Objects </core/configuration/objects>` using structures and the interchange of real-time data between :doc:`GAMs </core/app/gams/gam>`.   

Class/struct register
---------------------

.. note::

   See :doc:`the following link </core/configuration/objectregistrydatabase>` for a full data-driven alternative on how to create and register structures.
    

In order to automatically register a MARTe Object or a ``C struct`` into the :vcisdoxygencl:`ClassRegistryDatabase`, the following macros must be used:  

- ``DECLARE_CLASS_MEMBER(className, memberName, type, modifierString, attributeString )``, where:
   - ``className`` is the name of the class or structure;
   - ``memberName`` is the name of the member to be registered;
   - ``type`` is the member type (a basic type or a structure);
   - ``modifierStrubg`` are the possible member modifiers as described in the documentation of the method ``GetMemberModifiers`` of :vcisdoxygencl:`IntrospectionEntry`;
   - ``attributeString`` is a list of attributes as specified in the documentation of the method ``GetMemberAtributes`` of :vcisdoxygencl:`IntrospectionEntry`.       
- ``DECLARE_CLASS_INTROSPECTION(className, introEntryArray)`` or, in the case of ``C structs``, ``DECLARE_STRUCT_INTROSPECTION(structName, introEntryArray)``, where:
   - ``className`` is the name of the class and ``structName`` is the name of the struct;
   - ``introEntryArray`` is a zero terminated array of all the previously declared :vcisdoxygencl:`IntrospectionEntry` elements for this class or structure. The entries in the array shall be named ``&className_memberName_introspectionEntry``, where ``className`` is the name of the struct of class and ``memberName`` is the name of the member to be registered.

.. code-block:: c++   

    struct A {
       MARTe::float32 f1;
       MARTe::float32 f2;    
    };
    struct B {
       struct A a1;
       struct A a2;
    };
    ...
    DECLARE_CLASS_MEMBER(A, f1, float32, "", "");
    DECLARE_CLASS_MEMBER(A, f2, float32, "", "");
    static const MARTe::IntrospectionEntry* AStructEntries[] = { &A_f1_introspectionEntry, &A_f2_introspectionEntry, 0 };
    DECLARE_STRUCT_INTROSPECTION(A, AStructEntries)
    ...
    DECLARE_CLASS_MEMBER(B, a1, A, "", "");
    DECLARE_CLASS_MEMBER(B, a2, A, "", "");
    static const MARTe::IntrospectionEntry* BStructEntries[] = { &B_a1_introspectionEntry, &B_a2_introspectionEntry, 0 };
    DECLARE_STRUCT_INTROSPECTION(B, BStructEntries)    
   
.. note::

   This introspection infrastructure is expected to be mostly used by higher level tools, which will automatically generate the required code. 
   
      
Query instrospection
--------------------

The introspection information is accessible by querying the :vcisdoxygencl:`ClassRegistryDatabase` against the relevant struct/class name and by calling the method ``GetIntrospection`` on the retrieved :vcisdoxygencl:`ClassRegistryItem` (see example below).   

Examples
--------
   
The following example show how to register a structure, a MARTe object and to how to query their properties in runtime.  

.. literalinclude:: /_static/examples/Core/ObjectsExample3.cpp
   :language: c++
   :emphasize-lines: 77-78,80-81,83,86-93,95-102,110,113,120
   :caption: Introspection example (ObjectsExample3)
   :linenos:

Instructions on how to compile and execute the examples can be found :doc:`here </core/examples>`.   

