.. date: 24/03/2018
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


Parsers
=======

The core of the framework offers four parsers. 

All the parsers implement the :vcisdoxygencl:`ParserI` interface. Three of the parsers implement the :vcisdoxygencl:`ConfigurationParserI` interface and are capable of transforming an input stream, encoded in any of the supported languages, into a :vcisdoxygencl:`ConfigurationDatabase`. 
One parser is capable of transforming a mathematical expression into stack machine instruction to be used by :vcisdoxygencl:`RuntimeEvaluator`.

====================================== =======
Type                                   Meaning
====================================== =======
:vcisdoxygencl:`StandardParser`        The :ref:`MARTeConfigurationLanguage`.
:vcisdoxygencl:`XmlParser`             `XML <https://www.w3.org/TR/xml>`_.
:vcisdoxygencl:`JsonParser`            `JSON <https://www.json.org/>`_.
:vcisdoxygencl:`MathExpressionParser`  Mathematical expressions.
====================================== =======  

.. _MARTeConfigurationLanguage:

MARTe configuration language
----------------------------

The MARTe configuration language has a syntax similar to JSON and is parsed using the :vcisdoxygencl:`StandardParser`. 

The syntax is composed by a tree of name/value pairs separated by an **=** sign. Curly braces are used to define multi-dimensional arrays and to create new named nodes in the tree.

An example of a configuration file using the MARTe language:

.. code-block:: c++

   A = {
      B = 1
      C = "ABCD"
      D = {1, 2, 3, 4}
      E = {"A", "B"}
      F = {
         G = 3.34
         H = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}}
      }
   }

   
Examples
--------

The following example shows how to parse configuration files in all the supported languages. 

.. literalinclude:: /_static/examples/Core/ConfigurationExample3.cpp
   :language: c++
   :caption: Object configuration example (ConfigurationExample3)
   :linenos:

Instructions on how to compile and execute the example can be found :doc:`here </core/examples>`.
