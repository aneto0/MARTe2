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

The ``Initialise`` method of the :vcisdoxygencl:`ReferenceContainer` will look for any name starting with the character ``+``. 

When the ``+`` character is found, the property ``Class=LIB::CLASS`` shall exist in the subtree. ``LIB`` is the name of the shared library holding the compiled ``CLASS``.  

The ``CLASS`` shall inherit from ``Object`` and shall implement the macros defined in the :doc:`/core/objects/objects` section.  

If ``LIB`` is not defined, it is assumed that the class was already registered (e.g. because it was statically linked) or that the name of the library is equal to the name of the class. 

This process is recursively repeated and a tree of Objects is built. The name of the Object is automatically set to the name of the node.

In the following example, an object named ``A``, of type ``AClass``, would be created, together with another object of type ``AClass``, named ``B``, and with an object of type ``CClass`` named ``C``. It is assumed that the ``AClass`` inherits from ``ReferenceContainer`` and that, as a consequence, calling ``Get(0)`` on the instance named ``B`` would return a ``Reference`` to ``C``. 

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
      
The :vcisdoxygencl:`ObjectRegistryDatabase` is a ``ReferenceContainer`` which offers a database to hold and link all the ``Objects`` defined in a given configuration stream.    

The ``Find`` method can be used to find any of the instantiated ``Objects`` in the tree. 

A dot **.** is used as the path separator.

.. code-block:: c++   

   ...
   ObjectRegistryDatabase *ord = ObjectRegistryDatabase::Instance();
   ReferenceT<CClass> obj = ord->Find("B.C");
   if (obj.IsValid()) {
      ...
   
If the node name starts with a ``$``, besides implementing the same behaviour described before for the ``+``, it will also set the node as a search *domain*. 

This means that when using the :vcisdoxygencl:`ObjectRegistryDatabase` ``Find`` method, the ``:`` symbol will allow to perform searches related to a given root domain. For example:

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
   
Would allow to Find ``C``, ``C.E``, ``D`` and ``D.F``, using ``B`` as the root domain (see the example below).

.. note::

    Other characters can also be set as a identifiers for the new Object creation and for the setting of a given Object as the root domain. See the methods ``AddBuildToken``, ``RemoveBuildToken``, ``IsBuildToken``, ``AddDomainToken``, ``RemoveDomainToken`` and ``IsDomainToken`` in  :vcisdoxygencl:`ReferenceContainer`. 

Reading C structures
--------------------

If a ``C struct`` has been :doc:`registered </core/objects/introspection>` in the ClassRegistryDatabase it is possible to directly map the contents of a configuration node to the registered structure (see example below).

To read/write a registered structure an AnyType which describes the registered type must be created:

.. code-block:: c++
   
   struct AStruct {
      float32 f1;
      float32 f2;
   };
   ///Register the struct with the required macros.
   AStruct aStruct1;
   AStruct aStruct2;
   ClassRegistryItem * registeredStructClassProperties = ClassRegistryDatabase::Instance()->Find("AStruct");
   ...
   ClassUID registeredStructUID = registeredStructClassProperties->GetUniqueIdentifier();
   TypeDescriptor registeredStructTypeDescriptor(false, registeredStructUID);
   AnyType registeredStructAnyType1 = AnyType(registeredStructTypeDescriptor, 0u, &aStruct1);
   AnyType registeredStructAnyType2 = AnyType(registeredStructTypeDescriptor, 0u, &aStruct2);
   ...
   data.Read("AStruct1", registeredStructAnyType1);
   data.Read("AStruct2", registeredStructAnyType2);
   ...
   if (aStruct1.f1 == aStruct2.f1) {
   ...  

.. note::

   Only the types and structure of the configuration tree must match with the types and structure of the ``C struct``, i.e. the names are ignored. 

Reading data-driven C structures
--------------------------------

The :vcisdoxygencl:`IntrospectionStructure` class can also be used to dynamically create and register structures using a standard configuration file.

.. code-block:: c++
   
   +Types = {
       Class = ReferenceContainer
       +GainFromIntroStructure = {
           Class = IntrospectionStructure
           gain1 = {
               Type = float32
               NumberOfElements = {1}
           }
           gain2 = {
               Type = float32
               NumberOfElements = {1}
           }
           gain3 = {
               Type = float32
               NumberOfElements = {6}
           }
       }
       +GainsFromIntroStructure = {
            Class = IntrospectionStructure
            lowGains = {
            Type = GainFromIntroStructure
       }
       ...

Preprocessing
-------------

The MARTe configuration files can be preprocessed using the ``C`` preprocessor directives (e.g. ``#include`` and ``#define``). 

This strategy can be used to write modular configuration settings (see example below). Note that the files must first be processed with a ``Makefile.inc`` and a ``Makefile.cfg``, following the MARTe Makefile structure:

.. code-block:: makefile
   :caption: Makefile.inc 

    #Named of the unit files to be compiled. It will generate files named *_Gen.cfg (e.g. RTApp-6_Gen.cfg)
    OBJSX=RTApp-6.x

    #Location of the Build directory where the configuration file will be written to
    BUILD_DIR?=.

    #Location of the MakeDefaults directory.
    #Note that the MARTe2_DIR environment variable
    #must have been exported before
    MAKEDEFAULTDIR=$(MARTe2_DIR)/MakeDefaults

    include $(MAKEDEFAULTDIR)/MakeStdLibDefs.$(TARGET)

    CFLAGS += -DENABLE_WEB_BROWSING

    all: $(OBJS) 
        echo  $(OBJS)

    include $(MAKEDEFAULTDIR)/MakeStdLibRules.$(TARGET)

.. code-block:: makefile
   :caption: Makefile.cfg

    export TARGET=cfg

    include Makefile.inc

Postprocessing
--------------

As of v1.7.0, the :vcisdoxygencl:`StandardParser` also allows to post-process and assign simple mathematical expressions to nodes in the tree.
The syntax is `NODE=(OUTPUT_VAR_TYPE|EXPRESSION)`, where `OUTPUT_VAR_TYPE` is the output variable type (e.g. `uint32`) and `EXPRESSION` is the mathematical expression to be executed. 

The variables in the expression can reference to other nodes in the configuration tree, noting that the expressions are executed in order (top to bottom) and that the names in the node path cannot include the `+` sign.

.. code-block:: c++

    int a =3;
    Parameters = {
        T1_FREQUENCY = (uint32)10000
        T2_FREQUENCY = (uint32)1
        SAMPLES = (uint32|"Parameters.T1_FREQUENCY / Parameters.T2_FREQUENCY")
    }
    +TestApp = {
        Class = RealTimeApplication
        +Functions = {
            Class = ReferenceContainer
            +GAMTimer = {
                Class = IOGAM
                InputSignals = {
                    Time = {
                        Frequency = (uint32|"(uint32)2 * Parameters.T1_FREQUENCY")
                        ...


Examples
--------

The following example shows how to load an application from a configuration file. 

Note that Objects can be nested inside other Objects (provided that the container Object inherits from ReferenceContainer and that it calls ReferenceContainer::Initialise on its Initialise method).

.. literalinclude:: /_static/examples/Core/ConfigurationExample4.cpp
   :language: c++   
   :caption: Data driven application example (ConfigurationExample4)
   :linenos:  
   :emphasize-lines: 195,297-299,309-310,314-315,332
   
This example highlights how the Find method can be used to search for Objects in the ObjectRegistryDatabase. Note how the reference **G.H** is found with respect to **B**.    

.. literalinclude:: /_static/examples/Core/ConfigurationExample5.cpp
   :language: c++   
   :caption: ObjectRegistryDatabase Find example (ConfigurationExample5)
   :linenos:  
   :emphasize-lines: 143
   
This example shows how to read and write directly from a registered ``C struct``:

.. literalinclude:: /_static/examples/Core/ConfigurationExample6.cpp
   :language: c++   
   :caption: Reading registered structures (ConfigurationExample6)
   :linenos:  
   :emphasize-lines: 73,80,149,152,280-281
   
Similar to the example below but the structures are registered using the configuration file:

.. literalinclude:: /_static/examples/Core/ConfigurationExample7.cpp
   :language: c++   
   :caption: Reading registered structures (ConfigurationExample7)
   :linenos:  
   :emphasize-lines: 202-215,218-225
   
Instructions on how to compile and execute the example can be found :doc:`here </core/examples>`.

Example on how to preprocess complex configuration files:

.. literalinclude:: /_static/examples/Configurations/RTApp-6.cfg
    :language: c++ 
    :caption: Example of a configuration file that includes other configuration files.
    :linenos:  

.. literalinclude:: /_static/examples/Configurations/Makefile.inc
    :language: makefile
    :linenos:  

Example with post-processing of variables:

.. literalinclude:: /_static/examples/Configurations/RTApp-8.cfg
    :caption: Example of a configuration file with variable post-processing.
    :linenos:  

