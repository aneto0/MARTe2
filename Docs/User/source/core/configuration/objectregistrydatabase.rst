.. date: 03/04/2018
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

Data-driven applications
========================

Using the mechanisms described in the :doc:`/core/objects/objectsintro` and in the :doc:`/core/configuration/configuration` sections, together with a special syntax, the framework is capable of instantiating a tree of objects based on a configuration stream (e.g. a file).

Syntax
------

The Initialise method of the :vcisdoxygencl:`ReferenceContainer` will look for any name starting with the character **+**. 

When the **+** signal is character, the property **Class=LIB::CLASS** shall exist in the subtree. **LIB** is the named of the shared library holding the compiled **CLASS**.  

The **CLASS** shall inherit from Object and shall implement the macros defined in the :doc:`/core/objects/objects` section.  

If **LIB** is not defined, it is assumed that the class was already registered (e.g. because it was statically linked) or that the name of the library is equal to the name of the class. 

This process is recursively repeated and a tree of Objects is built. The name of the Object is automatically set to the name of the node.

In the following example an Object named **A**, of type AClass, would be created, together with another Object of type AClass, named **B**, and with an Object of type CClass named **C**. It is assumed that the AClass inherits from ReferenceContainer and that, as a consequence, calling **Get(0)** on the instance named **B** would return a Reference to **C**. 

.. code-block:: c++   

   +A = {
      Class = AClass
   }
   +B = {
      Class = AClass
      +C = {
         Class = ALIB::CClass
      }
   }
      
The :vcisdoxygencl:`ObjectRegistryDatabase` is a ReferenceContainer which offers a database to hold and link all the Objects defined in a given configuration stream.    

The Find method can be used to find any of the instantiated Objects in the tree. 

A dot **.** is used as the path separator.

.. code-block:: c++   

   ...
   ObjectRegistryDatabase *ord = ObjectRegistryDatabase::Instance();
   ReferenceT<CClass> obj = ord->Find("B.C");
   if (obj.IsValid()) {
      ...
   
If the node name starts with a **$**, besides implementing the same behaviour described before for the **+**, it will also set the node as a search *domain*. 

This means that when using the :vcisdoxygencl:`ObjectRegistryDatabase` *Find* method, the **:** symbol will allow to perform searchs related to a given root domain. For example:

.. code-block:: none

   +A = {
      Class = AClass
      $B = {
         Class = BClass
         +C = {
            Class = CClass
            +E = {
                Class = EClass
            }
         }
         +D = {
            Class = DClass
            +F = {
                Class = CClass
            }
         }
      }
   }
   
Would allow to Find **C**, **C.E**, **D** and **D.F**, using **B** as the root domain (see the example below).

Examples
--------

The following example shows how to load an application from a configuration file. 

Note that Objects can be nested inside other Objects (provided that the container Object inherits from ReferenceContainer and that it calls ReferenceContainer::Initialise on its Initialise method).

.. literalinclude:: /_static/examples/Core/ConfigurationExample4.cpp
   :language: c++   
   :caption: Data driven application example (ConfigurationExample4)
   :linenos:  
   :emphasize-lines: 193,295-296,307-308,312-313,330
   
This example highlights how the Find method can be used to search for Objects in the ObjectRegistryDatabase. Note how the reference **G.H** is found with respect to **B**.    

.. literalinclude:: /_static/examples/Core/ConfigurationExample5.cpp
   :language: c++   
   :caption: ObjectRegistryDatabase Find example (ConfigurationExample5)
   :linenos:  
   :emphasize-lines: 143
   
Instructions on how to compile and execute the example can be found :doc:`here </examples>`.