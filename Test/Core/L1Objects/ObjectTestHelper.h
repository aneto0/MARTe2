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
     * Default constructor
     */
    IntegerObject() {
        dummyVariable = 0;
    }

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

class SpecialIntegerObject: public IntegerObject {
public:
    CLASS_REGISTER_DECLARATION()

    /**
     * Default constructor
     */
    SpecialIntegerObject() {
    }

    virtual ~SpecialIntegerObject() {
    }


    int32 saturate(int32 threshold) {

        return dummyVariable > threshold ? threshold : dummyVariable;
    }

private:

};


class FloatObject: public Object {
public:
    CLASS_REGISTER_DECLARATION()


    /**
     * Default constructor
     */
    FloatObject() {
        dummyFVariable = 0;
        deleted = false;
    }

    virtual ~FloatObject() {
        deleted = true;
    }

    /**
     * @return the value of the dummy variable.
     */
    uint32 GetVariable() const {
        return dummyFVariable;
    }

    /**
     * @brief Updates the value of the dummy variable.
     * @param dummyVariable the new value to set.
     */
    void SetVariable(int32 dummyVariable) {
        this->dummyFVariable = dummyVariable;
    }

    bool Deleted() {
        return deleted;
    }
private:
    /**
     * Dummy variable
     */
    int32 dummyFVariable;

    bool deleted;
};

#endif /* OBJECTTESTHELPER_H_ */

