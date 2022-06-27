.. date: 28/03/2018
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

Object configuration
====================

:doc:`/core/objects/objects` are configured using inputs from a (tree) database. The access to this database is abstracted by the :vcisdoxygencl:`StructuredDataI` interface.

The StructuredDataI offers methods to navigate the tree, create new nodes and read/write values from/to the tree leafs. Note that the StructuredDataI does not prescribe the configuration language of the database implementation (which can be e.g. XML, JSON, ...).

Using the :vcisdoxygencl:`Object` Initialise method the class implementation is expected to query the database, verify the values and assign them to its members. 

.. code-block:: c++   

   ...
   virtual bool Initialise(MARTe::StructuredDataI &data) {
      bool ok = Object::Initialise(data);
      if (ok) {
          ok = data.Read("Gain1", gain1);
   ...
   
The :vcisdoxygencl:`ConfigurationDatabase` offers a read/write implementation of the StructuredDataI and is used internally in the core of the framework.

.. code-block:: c++   

   ...
   ConfigurationDatabase cdb;
   cdb.Write("Gain1", 2);
   ...
      
Reading values
--------------

The ``StructuredDataI::Read`` method accepts an :vcisdoxygencl:`AnyType` and will automatically convert the value stored in the database into the target type (i.e. the type of the variable passed in the Read).

The return value should always be checked. It will fail if the type cannot be converted or if the variable does not exist in the database.
 
To read multi-dimensional values, the memory must be allocated beforehand. As a consequence, the type and dimension of the variable to be read must be known. This is achieved by querying the type ``StructuredDataI::GetType("NAME_OF_THE_LEAF_TO_QUERY");``.

.. code-block:: c++

   ...
   AnyType arrayDescription = data.GetType("Modes");   
   ok = arrayDescription.GetDataPointer() != NULL_PTR(void *);
   //The leaf exists   
   if (ok) {
      numberOfElements = arrayDescription.GetNumberOfElements(0u);
      ...
   if (ok) {
      ...
      modesArray = new StreamString[numberOfElements];
      Vector<StreamString> readVector(modesArray, numberOfElements);
      ok = data.Read("Modes", readVector);
      ...

.. warning::

   The ``StructuredDataI::GetType`` method does not return any memory/value and shall only be used to query information about the type. 

For matrices a similar strategy applies:

.. code-block:: c++

   ...
   AnyType arrayDescription = data.GetType("Model");
   ok = arrayDescription.GetDataPointer() != NULL_PTR(void *);
   ...
   if (ok) {
      numberOfDimensions = arrayDescription.GetNumberOfDimensions();
      ok = (numberOfDimensions == 2u);
      ...
      numberOfRows = arrayDescription.GetNumberOfElements(1u);
      numberOfCols = arrayDescription.GetNumberOfElements(0u);
   ...	
   if (ok) {
      Matrix<float32> modelMatrix(numberOfRows, numberOfCols);
      ok = data.Read("Model", modelMatrix);
      ...

Reading values with the StructuredDataIHelper
---------------------------------------------

As of v1.7.0 of MARTe2, the :vcisdoxygencl:`StructuredDataIHelper` facilitates the reading and validation of configuration values. In particular, it allows to:

- Read arrays and matrices without having to query for their size:

.. code-block:: c++

    virtual bool Initialise(MARTe::StructuredDataI &data) {
        bool ok = Object::Initialise(data);
        StructuredDataIHelper helper(data, this);
        ...
        MARTe::int32 *referencesArray;
        if (ok) {
            uint32 nOfReferences = 0u;
            ok = helper.ReadArray("References", referencesArray, nOfReferences);
        }
        ...
        MARTe::float32 **model;
        if (ok) {
            uint32 modelCols = 0u;
            ok = helper.ReadMatrix("Model", model, modelRows, modelCols);
        }
        ...

- Automatically assign default values for parameters that are not defined:

.. code-block:: c++

    virtual bool Initialise(MARTe::StructuredDataI &data) {
        bool ok = Object::Initialise(data);
        StructuredDataIHelper helper(data, this);
        ...
        //Assign default of 1.7 to gain2
        ok = helper.Read("Gain2", gain2, 1.7);
        ...

- Read "enum-like" types:

.. code-block:: c++

    virtual bool Initialise(MARTe::StructuredDataI &data) {
        bool ok = Object::Initialise(data);
        StructuredDataIHelper helper(data, this);
        ...
        const char8 * controllerTypesStr[] = {"C1", "C2", "C3"};
        uint32 controllerTypes[] = {1, 2, 3};
        ok = helper.ReadEnum("ControllerType", controllerType, controllerTypesStr, controllerTypes);
        ...

- Validate parameters against simple mathematical expressions:

.. code-block:: c++

    virtual bool Initialise(MARTe::StructuredDataI &data) {
        bool ok = Object::Initialise(data);
        StructuredDataIHelper helper(data, this);
        ...
 
        if (ok) {
            ok = helper.ReadValidated("Gain3", gain3, "(Gain3 > (float32)(-3.0)) && (Gain3 <= (float32)(0.0))");
        }

Navigating the database
-----------------------
   
The StructuredDataI can be navigated using the Move methods. Paths in the tree are separated using dots (**.**). This allows to create configurations that map into nested structures of any complexity. 

.. code-block:: c++

	data.CreateAbsolute("A.B.C")
	data.CreateAbsolute("D.E.F")
	data.MoveToRoot()
	data.MoveAbsolute("A.B.C")
	data.MoveAbsolute("D")
	data.MoveRelative("E.F")
   
Examples
--------

The following example shows how to read parameters (including arrays and matrices). 

.. literalinclude:: /_static/examples/Core/ConfigurationExample1.cpp
   :language: c++   
   :caption: Object configuration example (ConfigurationExample1)
   :linenos:
   
This is a similar example, highlighting structured configurations. 

.. literalinclude:: /_static/examples/Core/ConfigurationExample2.cpp
   :language: c++
   :caption: Object configuration example (ConfigurationExample2)
   :linenos:

The following example shows how the reading of parameters can be simplified with the StructuredDataIHelper. 

.. literalinclude:: /_static/examples/Core/ConfigurationExample8.cpp
   :language: c++   
   :caption: Object configuration example using the StructuredDataIHelper (ConfigurationExample8)
   :linenos:

Instructions on how to compile and execute the example can be found :doc:`here </core/examples>`.

