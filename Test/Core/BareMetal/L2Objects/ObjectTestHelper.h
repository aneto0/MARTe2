/**
 * @file ObjectTestHelper.h
 * @brief Header file for class ObjectTestHelper
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

 * @details This header file contains the declaration of the class ObjectTestHelper
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef OBJECTTESTHELPER_H_
#define OBJECTTESTHELPER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "Object.h"

using namespace MARTe;
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
/**
 * @brief Helper class to support the testing of references.
 */
class IntegerObject: public Object {
public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Default constructor
     */
    IntegerObject() {
        dummyVariable = 0;
    }

    /**
     * @brief Destructor
     */
    virtual ~IntegerObject() {
    }

    /**
     * @return the value of the dummy variable.
     */
    uint32 GetVariable() const {
        return dummyVariable;
    }

    /**
     * @brief Updates the value of the dummy variable.
     * @param dummyVariable the new value to set.
     */
    void SetVariable(int32 dummyVariable) {
        this->dummyVariable = dummyVariable;
    }

protected:
    /**
     * Dummy variable
     */
    int32 dummyVariable;

};


/**
 * @brief An object which inherits from integer object.
 */
class SpecialIntegerObject: public IntegerObject {
public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Default constructor
     */
    SpecialIntegerObject() {
    }

    /**
     * @brief Destructor.
     */
    virtual ~SpecialIntegerObject() {
    }

    /**
     * @brief Saturates the integer value whithin the specified thereshold.
     * @param[in] threshold is the maximum value allowed.
     * @return the number saturated.
     */
    int32 saturate(int32 threshold) {

        return dummyVariable > threshold ? threshold : dummyVariable;
    }

private:

};


/**
 * @brief An object which inherits from Object.
 */
class FloatObject: public Object {
public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Default constructor
     */
    FloatObject() {
        dummyFVariable = 0;
    }

    /**
     * @brief Destructor
     */
    virtual ~FloatObject() {
    }

    /**
     * @brief Return the float number.
     * @return the value of the dummy variable.
     */
    float32 GetVariable() const {
        return dummyFVariable;
    }

    /**
     * @brief Updates the value of the dummy variable.
     * @param dummyVariable the new value to set.
     */
    void SetVariable(float32 dummyVariable) {
        this->dummyFVariable = dummyVariable;
    }

private:
    /**
     * Dummy variable
     */
    float32 dummyFVariable;

};


/**
 * @brief A class which contains two different Object types.
 */
class CollectInts: public Object {

    /**
     * First object.
     */
    IntegerObject oneInteger;

    /**
     * Second object
     */
    SpecialIntegerObject oneSpecialInteger;

public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Constructor.
     */
    CollectInts() {
        oneInteger.SetVariable(0);
        oneSpecialInteger.SetVariable(0);
    }

    /**
     * @brief Sets the value of the first object.
     * @param[in] inumber is the desired value associated to the first object.
     */
    void SetInteger(int32 inumber){
        oneInteger.SetVariable(inumber);
    }

    /**
     * @brief Sets the value of the second object.
     * @param[in] inumber is the desired value associated to the second object.
     */
    void SetSpecialInteger(int32 inumber){
        oneSpecialInteger.SetVariable(inumber);
    }

    /**
     * @brief Returns the value of the first object.
     * @return the value of the first object.
     */
    int32 GetInteger() {
        return oneInteger.GetVariable();
    }


    /**
     * @brief Returns the value of the second object.
     * @return the value of the second object.
     */
    int32 GetSpecialInteger() {
        return oneSpecialInteger.GetVariable();
    }
};


#endif /* OBJECTTESTHELPER_H_ */

