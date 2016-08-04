/**
 * @file ReferenceTest.h
 * @brief Header file for class ReferenceTest
 * @date 07/08/2015
 * @author Giuseppe Ferrò
 *
 * @copyright Copyright 2015 F4E | European Joint Undertaking for ITER and
 * the Development of Fusion Energy ('Fusion for Energy').
 * Licensed under the EUPL, Version 1.1 or - as soon they will be approved
 * by the European Commission - subsequent versions of the EUPL (the "Licence")
 * You may not use this work except in compliance with the Licence.
 * You may obtain a copy of the Licence at: http://ec.europa.eu/idabc/eupl
 *
 * @warning Unless required by applicable law or agreed to in writing, 
 * software distributed under the Licence is distributed on an "AS IS"
 * basis, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
 * or implied. See the Licence permissions and limitations under the Licence.

 * @details This header file contains the declaration of the class ReferenceTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef REFERENCETEST_H_
#define REFERENCETEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "Reference.h"
#include "EventSem.h"
using namespace MARTe;

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
/**
 * Tests the Reference public methods.
 */
class ReferenceTest {
public:

    ReferenceTest();

    /**
     * @brief Tests the default constructor
     * @return true if the internal object pointer is null after the reference constructor call.
     */
    bool TestDefaultConstructor();

    /**
     * @brief Tests the copy constructor.
     * @return true if the object pointer is equal to the input object pointer passed in the constructor.
     */
    bool TestCopyConstructor();

    /**
     * @brief Tests the copy constructor passing a null input.
     * @return true if the object pointer is initialised to NULL.
     */
    bool TestCopyConstructorNullPtr();

    /**
     * @brief Tests the constructor which instantiates a new object.
     * @details Creates an IntegerObject and checks that it is effectively by calling some of its methods.
     * Finally it is checked that the number of references is equal to one.
     * @return true if an object of type IntegerObject can be successfully created by the Reference constructor.
     */
    bool TestBuildObjectConstructor();

    /**
     * @brief Tests the constructor passing an invalid class name.
     * @return true if the reference created is invalid.
     */
    bool TestBuildFakeObjectConstructor();

    /**
     * @brief Tests the copy constructor with an object pointer in input.
     * @details Creates an object and calls the copy constructor passing the object pointer.
     * Then checks that the reference pointer effectively points to that object and that the number
     * of references is equal to one.
     * @return true if after calling the copy constructor the Reference points to the new object pointer.
     */
    bool TestCopyFromObjPtrConstructor();

    /**
     * @brief Tests the copy constructor passing a NULL input object as input.
     * @return true if the built object is invalid.
     */
    bool TestCopyFromObjPtrConstructorNullPtr();

    /**
     * @brief Tests the destructor.
     * @return true if after the destructor call the reference is invalid.
     */
    bool TestDestructor();

    /**
     * @brief Tests if the function initialises an already created object
     */
    bool TestInitialiseNoCreation();

    /**
     * @brief Tests if the function creates and initialises the object
     * reading the class name from the StructuredDataI in input
     */
    bool TestInitialiseCreation();

    /**
     * @brief Tests if the function fails trying initialise an object not created
     */
    bool TestInitialiseNoObject();

    /**
     * @brief Tests if the function fails creating the object without the class name
     * specified in the StructuredDataI
     */
    bool TestInitialiseNoClassName();


    /**
     * @brief Tests the Reference::RemoveReference function.
     * @details Creates an object and assigns to it several references.
     * Then removes each reference one by one checking that the number of references
     * parameter decreases and that the removed references become invalid.
     * @return true if calling Reference::RemoveReference correctly decreases the number of references and
     * asserts that the removed reference becomes invalid .
     */
    bool TestRemoveReference();

    /**
     * @brief Tests the operator = with another Reference object.
     * @details Creates a reference to an object and then a second reference using the = operator.
     * @return true if the second reference effectively points to the created object and
     * if the number of references is correct.
     */
    bool TestCopyOperatorReference();

    /**
     * @brief Tests the = operator with a Reference with a NULL pointer.
     * @return true if the copied Reference object is invalid.
     */
    bool TestCopyOperatorReferenceNull();

    /**
     * @brief Tests the = operator with an object pointer.
     * @details Creates an object and then a reference using the = operator.
     * @return true if if the reference points effectively to the created
     * object and if the number of references is correct.
     */
    bool TestCopyOperatorObject();

    /**
     * @brief Tests the = operator with a NULL object pointer.
     * @return true if the Reference object copied is invalid.
     */
    bool TestCopyOperatorObjectNull();

    /**
     * @brief Tests the Reference::IsValid function.
     * @return true if Reference::IsValid returns false in case of NULL initialisation and true in case of a correct initialisation.
     */
    bool TestIsValid();

    /**
     * @brief Tests the Reference::NumberOfReferences function.
     * @details Creates an object and assigns to it a number of references in different ways checking
     *  if the number of reference is correct. Then removes the references and checks if the number of references returned decreases.
     * @return true if the number of references holding the Object is always kept consistent with the value returned by Reference::NumberOfReferences.
     */
    bool TestNumberOfReferences();

    /**
     * @brief Tests the == operator.
     * @details Creates several references to the same object and checks that the == operator returns true.
     * Then checks if it returns false for the case of references which point to different objects.
     * @return true if the operator == returns true for references pointing at the same object, false otherwise.
     */
    bool TestEqualOperator();

    /**
     * @brief Tests the != operator.
     * @details Creates several references to the same object and checks that the != operator returns false.
     * Then checks if it returns true for the case of references which point to different objects.
     * @return true if the operator != returns false for references pointing at the same object, true otherwise.
     */
    bool TestDifferentOperator();

    /**
     * @brief Tests if the references created in a function are destroyed when the function terminates.
     * @details A thread calls a function which assigns a certain number of references to an object and waits on
     * an EventSem. The main thread checks that if the number of references is correct, then posts the semaphore and when the thread
     * terminates checks that the number of references is decreased by the number of references created in the function.
     * @return true if the number of references before and after calling a function is kept constant.
     */
    bool TestInFunctionOnStack();

    /**
     * @brief Tests if the references created on the heap in a function are not destroyed when the function terminates.
     * @details A thread calls a function which assigns a certain number of references to an object on the heap and waits on
     * an EventSem. The main thread checks that if the number of references is correct, then posts the semaphore and when the thread
     * terminates checks that the number of references is still the same.
     * @param[in] nRefs is the desired number of reference to create.
     * @return true if the references created in the heap is kept constant after being used inside functions.
     */
    bool TestInFunctionOnHeap(uint32 nRefs);

    /**
     * @brief Tests if it is possible assign a reference to another for objects on the same inheritance branch.
     * @details Creates a parent and a child Object. Assigns the reference to the parent and then assigns it to the child and vice versa.
     * @return true if a reference assignment parent=child and a reference assignment child=parent are valid.
     */
    bool TestRightInherithance();

    /**
     * @brief Tests if it is possible assign a reference to objects on a different inherit branch.
     * @details Creates two separate children of Object (child1 and child2).
     * Assigns the reference to the first and then assigns it to the second and vice versa.
     * Since both inherit from Object the reference assignment should work.
     * @return true if a reference assignment child1=child2 and a reference assignment child2=child1 are valid.
     */
    bool TestWrongInherithance();




    /**
     * @brief Tests the atomicity of the IncrementReferences and DecrementReferences functions.
     * @details Calls two threads which increments and decrements the references associated to two different objects.
     * @param[in] nRefs is the desired number of references for the objects.
     * @return true if the number of references is always consistent with the expected value, false otherwise.
     */
    bool HugeTest(uint32 nRefs);

    /**
     * An event semaphore used for threads synchronisation
     */
    EventSem eventSem;

    /**
     * A reference passed to the thread callback
     */
    Reference storedRef;

    /**
     * A reference passed to the thread callback
     */
    Reference storedRef2;

    /**
     * The number of references to be assigned in the thread callback function
     */
    uint32 nRefs;

    /**
     * An array of references pointers to allocate references on the heap.
     */
    Reference **arrayRefs;

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* REFERENCETEST_H_ */

