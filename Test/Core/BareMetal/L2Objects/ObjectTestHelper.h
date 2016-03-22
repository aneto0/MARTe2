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

#include <StreamString.h>

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
    void SetInteger(int32 inumber) {
        oneInteger.SetVariable(inumber);
    }

    /**
     * @brief Sets the value of the second object.
     * @param[in] inumber is the desired value associated to the second object.
     */
    void SetSpecialInteger(int32 inumber) {
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

/**
 * @brief Helper class to support the testing of Object's features for
 * extracting its own data and metadata as a StructuredDataI object.
 */
class NonRegisteredIntegerObject: public Object {
public:

    /**
     * @brief Default constructor
     */
    NonRegisteredIntegerObject() {
        member = 0;
    }

    /**
     * @brief Destructor
     */
    virtual ~NonRegisteredIntegerObject() {
    }

    /**
     * Member attribute
     */
    int32 member;

};

/**
 * @brief Helper class to support the testing of Object's features for
 * extracting its own data and metadata as a StructuredDataI object.
 */
class NonIntrospectableIntegerObject: public Object {
public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Default constructor
     */
    NonIntrospectableIntegerObject() {
        member = 0;
    }

    /**
     * @brief Destructor
     */
    virtual ~NonIntrospectableIntegerObject() {
    }

    /**
     * Member attribute
     */
    int32 member;

};

/**
 * @brief Helper class to support the testing of Object's features for
 * extracting its own data and metadata as a StructuredDataI object.
 */
class IntrospectableIntegerObject: public Object {
public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Default constructor
     */
    IntrospectableIntegerObject() {
        member = 0;
    }

    /**
     * @brief Destructor
     */
    virtual ~IntrospectableIntegerObject() {
    }

    /**
     * Member attribute
     */
    int32 member;

};

/**
 * @brief Helper class to support the testing of Object's features for
 * extracting its own data and metadata as a StructuredDataI object.
 */
class IntrospectableObjectWith2Members: public Object {
public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Default constructor
     */
    IntrospectableObjectWith2Members() {
        member1 = 0;
        member2 = 0;
    }

    /**
     * @brief Destructor
     */
    virtual ~IntrospectableObjectWith2Members() {
    }

    /**
     * Member 1 attribute
     */
    int32 member1;

    /**
     * Member 2 attribute
     */
    uint64 member2;

};

/**
 * @brief Helper class to support the testing of Object's features for
 * extracting its own data and metadata as a StructuredDataI object.
 */
class IntrospectableObjectWith3Members: public Object {
public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Default constructor
     */
    IntrospectableObjectWith3Members() {
        member1 = 0;
        member2 = 0;
    }

    /**
     * @brief Destructor
     */
    virtual ~IntrospectableObjectWith3Members() {
    }

    /**
     * Member 1 attribute
     */
    int32 member1;

    /**
     * Member 2 attribute
     */
    uint64 member2;

    /**
     * Member 3 atribute
     */
    IntrospectableIntegerObject member3;

};

/**
 * @brief Helper class to support the testing of Object's features for
 * extracting its own data and metadata as a StructuredDataI object.
 */
class IntrospectableObjectWithVector: public Object {
public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Default constructor
     */
    IntrospectableObjectWithVector() :
            member1(0),
            member2(0),
            member3(),
            member4(0.0f),
            member5(0.0) {
    }

    /**
     * @brief Destructor
     */
    virtual ~IntrospectableObjectWithVector() {
    }

    /**
     * Member 1 attribute
     */
    int32 member1;

    /**
     * Member 2 attribute
     */
    uint64 member2;

    /**
     * Member 3 atribute
     */
    int32 member3[5];

    /**
     * Member 4 attribute
     */
    float32 member4;

    /**
     * Member 5 attribute
     */
    float64 member5;

};

/**
 * @brief Helper class to support the testing of Object's features for
 * extracting its own data and metadata as a StructuredDataI object.
 */
class IntrospectableObjectWithMatrix: public Object {
public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Default constructor
     */
    IntrospectableObjectWithMatrix() :
            member1(0),
            member2(0),
            member3() {
    }

    /**
     * @brief Destructor
     */
    virtual ~IntrospectableObjectWithMatrix() {
    }

    /**
     * Member 1 attribute
     */
    int32 member1;

    /**
     * Member 2 attribute
     */
    uint64 member2;

    /**
     * Member 3 atribute
     */
    int32 member3[5][3];

};

class TestObjectsDataSet {
public:
    IntrospectableIntegerObject& GetTest3Object();
    IntrospectableObjectWith2Members& GetTest4Object();
    IntrospectableObjectWith3Members& GetTest5Object();
    IntrospectableObjectWithVector& GetTest6Object();
    IntrospectableObjectWithMatrix& GetTest7Object();
private:
};

class JsonStreamsDataSet {
public:
    const StreamString& GetTest3DataAsJson();
    const StreamString& GetTest3MetadataAsJson();
    const StreamString& GetTest4DataAsJson();
    const StreamString& GetTest4MetadataAsJson();
    const StreamString& GetTest5DataAsJson();
    const StreamString& GetTest5MetadataAsJson();
    const StreamString& GetTest6DataAsJson();
    const StreamString& GetTest6MetadataAsJson();
    const StreamString& GetTest7DataAsJson();
    const StreamString& GetTest7MetadataAsJson();
private:
};

#endif /* OBJECTTESTHELPER_H_ */

