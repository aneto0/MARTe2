/**
 * @file StreamStructuredDataITest.cpp
 * @brief Source file for class StreamStructuredDataITest
 * @date 03/10/2018
 * @author Giuseppe Ferro
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

 * @details This source file contains the definition of all the methods for
 * the class StreamStructuredDataITest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "Reference.h"
#include "StreamString.h"
#include "StreamStructuredDataITest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

class StreamStructuredDataITestSD: public StreamStructuredDataI {

    virtual bool Read(const char8 * const name,
                      const AnyType &value);

    virtual AnyType GetType(const char8 * const name);

    virtual bool Write(const char8 * const name,
                       const AnyType &value);

    virtual bool Copy(StructuredDataI &destination);

    virtual bool AddToCurrentNode(Reference node);

    virtual bool MoveToRoot();

    virtual bool MoveToAncestor(uint32 generations);

    virtual bool MoveAbsolute(const char8 * const path);

    virtual bool MoveRelative(const char8 * const path);

    virtual bool MoveToChild(const uint32 childIdx);

    virtual bool CreateAbsolute(const char8 * const path);

    virtual bool CreateRelative(const char8 * const path);

    virtual bool Delete(const char8 * const name);

    virtual const char8 *GetName();

    virtual const char8 *GetChildName(const uint32 index);

    virtual uint32 GetNumberOfChildren();

};

bool StreamStructuredDataITestSD::Read(const char8 * const name,
          const AnyType &value) {
    return true;
}

AnyType StreamStructuredDataITestSD::GetType(const char8 * const name) {
    return true;
}

bool StreamStructuredDataITestSD::Write(const char8 * const name,
           const AnyType &value) {
    return true;
}

bool StreamStructuredDataITestSD::Copy(StructuredDataI &destination) {
    return true;
}

bool StreamStructuredDataITestSD::AddToCurrentNode(Reference node) {
    return true;
}

bool StreamStructuredDataITestSD::MoveToRoot() {
    return true;
}

bool StreamStructuredDataITestSD::MoveToAncestor(uint32 generations) {
    return true;
}

bool StreamStructuredDataITestSD::MoveAbsolute(const char8 * const path) {
    return true;
}

bool StreamStructuredDataITestSD::MoveRelative(const char8 * const path) {
    return true;
}

bool StreamStructuredDataITestSD::MoveToChild(const uint32 childIdx) {
    return true;
}

bool StreamStructuredDataITestSD::CreateAbsolute(const char8 * const path) {
    return true;
}

bool StreamStructuredDataITestSD::CreateRelative(const char8 * const path) {
    return true;
}

bool StreamStructuredDataITestSD::Delete(const char8 * const name) {
    return true;
}

const char8 *StreamStructuredDataITestSD::GetName() {
    return NULL;
}

const char8 *StreamStructuredDataITestSD::GetChildName(const uint32 index) {
    return NULL;
}

uint32 StreamStructuredDataITestSD::GetNumberOfChildren() {
    return 0u;
}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

StreamStructuredDataITest::StreamStructuredDataITest() {
}

StreamStructuredDataITest::~StreamStructuredDataITest() {
}

bool StreamStructuredDataITest::TestConstructor() {
    StreamStructuredDataITestSD test;
    return test.GetStream() == NULL;
}

bool StreamStructuredDataITest::TestSetStream() {
    StreamString stream;
    StreamStructuredDataITestSD test;
    test.SetStream(stream);
    return test.GetStream() == &stream;
}

bool StreamStructuredDataITest::TestGetStream() {
    return TestSetStream();
}
