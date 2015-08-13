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
     * @return true if the object pointer is equal to the input object pointer passed in the contructor.
     */
    bool TestCopyConstructor();

    /**
     * @brief Tests the copy constructor passing a null input.
     * @return true if the object pointer is initialized to null.
     */
    bool TestCopyConstructorNullPtr();

    /**
     * @brief Tests the constructor which instantiates a new object.
     * @details Creates an object and checks that it is effectively created doing some methods. Then checks that the number
     * of references is equal to true.
     * @return true if successful, false otherwise.
     */
    bool TestBuildObjectConstructor();

    /**
     * @brief Tests the constructor passing a wrong class name.
     * @return true if the reference created is invalid.
     */
    bool TestBuildFakeObjectConstructor();

    /**
     * @brief Tests the copy constructor with an object pointer in input.
     * @details Creates an object and calls the copy constructor passing the object pointer. Then checks that the reference pointer points
     * effectively to that object and that the number of references is equal to one.
     * @return true if successful, false otherwise.
     */
    bool TestCopyFromObjPtrConstructor();

    /**
     * @brief Tests the copy constructor passing a null input object as input.
     * @return true if the built object is invalid.
     */
    bool TestCopyFromObjPtrConstructorNullPtr();

    /**
     * @brief Tests the destructor.
     * @return true if after the destructor call the reference is invalid.
     */
    bool TestDestructor();

    //TODO
    bool TestInitialise();

    /**
     * @brief Tests the Reference::RemoveReference function.
     * @details Creates an object and assigns to it a number of references. Then removes them one by one checking that the number of references
     * parameter decreases and the reference objects removed becomes invalid.
     * @return true if successful, false otherwise.
     */
    bool TestRemoveReference();

    /**
     * @brief Tests the = operator with another Reference object.
     * @details Creates a reference to an object and then a second reference using the = operator. Then checks if the second reference points
     * effectively to the created object and if the number of references is correct.
     * @return true if successful, false otherwise.
     */
    bool TestCopyOperatorReference();

    /**
     * @brief Tests the = operator with a Reference with a NULL pointer.
     * @return true if the Reference object copied is invalid.
     */
    bool TestCopyOperatorReferenceNull();

    /**
     * @brief Tests the = operator with an object pointer.
     * @details Creates an object and then a reference using the = operator. Then checks if the reference points
     * effectively to the created object and if the number of references is correct.
     * @return true if successful, false otherwise.
     */
    bool TestCopyOperatorObject();

    /**
     * @brief Tests the = operator with a null object pointer.
     * @return true if the Reference object copied is invalid.
     */
    bool TestCopyOperatorObjectNull();

    /**
     * @brief Tests the Reference::IsValid function.
     * @details Checks if the function returns false in case of null initializations and true in case of right inizializations.
     * @return true if successful, false otherwise.
     */
    bool TestIsValid();

    /**
     * @brief Tests the Reference::NumberOfReference function.
     * @details Creates an object and assigns to it a number of references in different ways checking if the number of reference is correct. Then removes
     * the references and checks if the number of references returned decreases.
     * @return true if successful, false otherwise.
     */
    bool TestNumberOfReferences();

    /**
     * @brief Tests the == operator.
     * @details Creates references to the same object in different ways and checks that the == operator returns true. Then checks it it returns false
     * in case of references which point to different objects.
     * @return true if successful, false otherwise.
     */
    bool TestEqualOperator();

    /**
     * @brief Tests the != operator.
     * @details Creates references to the same object in different ways and checks that the != operator returns false. Then checks it it returns true
     * in case of references which point to different objects.
     * @return true if successful, false otherwise.
     */
    bool TestDifferentOperator();

    /**
     * @brief Tests if the references created in a function are destroyed when the function terminates.
     * @details A thread calls a function which assigns a certain number of references to an object and waits on
     * an EventSem. The main thread checks that if the number of references is correct, then posts the semaphore and when the thread
     * terminates checks that the number of references is decreased by the number of references created in the function.
     * @return true if successful, false otherwise.
     */
    bool TestInFunctionOnStack();

    /**
     * @brief Tests if the references created on the heap in a function are not destroyed when the function terminates.
     * @details A thread calls a function which assigns a certain number of references to an object on the heap and waits on
     * an EventSem. The main thread checks that if the number of references is correct, then posts the semaphore and when the thread
     * terminates checks that the number of references is still the same.
     * @param[in] nRefs is the desired number of reference to create.
     * @return true if successful, false otherwise.
     */
    bool TestInFunctionOnHeap(uint32 nRefs);

    /**
     * @brief Tests if it is possible assign a reference to another for objects on the same inherit branch.
     * @details Creates a parent object and a child object. Assigns the reference to the parent and then assigns it to the child and vice versa.
     * Since are all Object children the reference assignment should not be a problem.
     * @return true if successful, false otherwise.
     */
    bool TestRightInherithance();

    /**
     * @brief Tests if it is possible assign a reference to another for objects on a different inherit branch.
     * @details Creates two separate children of Object. Assigns the reference to the first and then assigns it to the second and vice versa.
     * Since are all Object children the reference assignment should not be a problem.
     * @return true if successful, false otherwise.
     */
    bool TestWrongInherithance();

    /**
     * An event semaphore used for threads synchronization
     */
    EventSem eventSem;

    /**
     * The reference to be passed to the thread callback
     */
    Reference storedRef;

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

