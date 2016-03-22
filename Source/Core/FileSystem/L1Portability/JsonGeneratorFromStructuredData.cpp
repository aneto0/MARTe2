/**
 * @file JsonGeneratorFromStructuredData.cpp
 * @brief Source file for class JsonGeneratorFromStructuredData
 * @date 08/03/2016
 * @author Ivan Herrero
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
 * the class JsonGeneratorFromStructuredData (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "JsonGeneratorFromStructuredData.h"
#include "Vector.h"
#include "Matrix.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

namespace {

using namespace MARTe;

void generateJsonScalarValue(const StreamString& value,
               const TypeDescriptor descriptor,
               StreamString& result) {
    if (descriptor == VoidType || descriptor == InvalidType) {
        result = "null";
    }
    else if (descriptor == PointerType || descriptor == Character8Bit || descriptor == ConstCharString || descriptor == CharString) {
        result = "\"";
        result += value;
        result += "\"";
    }
    else {
        result = value;
    }
}

}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

JsonGeneratorFromStructuredData::JsonGeneratorFromStructuredData() {
}

JsonGeneratorFromStructuredData::~JsonGeneratorFromStructuredData() {
}

bool JsonGeneratorFromStructuredData::Execute(StructuredDataI& input,
                                                 StreamString& output) {
    bool ret = true;

    static const AnyType BEGIN_OBJECT = "{";
    static const AnyType END_OBJECT = "}";
    static const AnyType BEGIN_ARRAY = "[";
    static const AnyType END_ARRAY = "]";
    static const AnyType NAME_SEPARATOR = ":";
    static const AnyType VALUE_SEPARATOR = ",";
    static const AnyType NULL_VALUE = "null";
    static const AnyType TRUE_VALUE = "true";
    static const AnyType FALSE_VALUE = "false";

    const uint32 numberOfChildren = input.GetNumberOfChildren();

    if (numberOfChildren == 0) {
        //There is no children, so it is a null value
        ret = output.Printf("%s", NULL_VALUE);
    }
    else {
        //There is at least 1 child, so it is an object value
        ret = output.Printf("%s", BEGIN_OBJECT);

        for (uint32 childNumber = 0u; (childNumber < numberOfChildren) && (ret); childNumber++) {
            const char8 * childName = input.GetChildName(childNumber);
            //value separator
            if (childNumber > 0) {
                ret = output.Printf("%s", VALUE_SEPARATOR);
            }
            //key name
            if (ret) {
                ret = output.Printf("\"%s\"", childName);
            }
            //name separator
            if (ret) {
                ret = output.Printf("%s", NAME_SEPARATOR);
            }
            //value
            if (ret) {
                const TypeDescriptor descriptor = input.GetType(childName).GetTypeDescriptor();
                const uint8 dimensions = input.GetType(childName).GetNumberOfDimensions();
                if (dimensions == 0) {
                    StreamString value;
                    bool isRead = input.Read(childName, value);
                    if (isRead) {
                        //The child is a leaf
                        StreamString cookedValue;
                        generateJsonScalarValue(value, descriptor, cookedValue);
                        ret = output.Printf("%s", cookedValue.Buffer());
                    }
                    else {
                        //The child is a node
                        if (ret) {
                            ret = input.MoveRelative(childName);
                        }
                        if (ret) {
                            ret = Execute(input, output);
                        }
                        if (ret) {
                            ret = input.MoveToAncestor(1u);
                        }

                    }
                }
                else if (dimensions == 1) {
                    ret = output.Printf("%s", BEGIN_ARRAY);
                    if (ret) {
                        const uint8 elements = input.GetType(childName).GetNumberOfElements(0u);
                        Vector<StreamString> values(elements);
                        ret = input.Read(childName, values);
                        uint8 i = 0;
                        while (ret && i < elements) {
                            if (ret && (i > 0)) {
                                ret = output.Printf("%s", VALUE_SEPARATOR);
                            }
                            if (ret) {
                                StreamString cookedValue;
                                generateJsonScalarValue(values[i], descriptor, cookedValue);
                                ret = output.Printf("%s", cookedValue.Buffer());
                            }
                            i++;
                        }
                    }
                    if (ret) {
                        ret = output.Printf("%s", END_ARRAY);
                    }
                }
                else if (dimensions == 2) {
                    ret = output.Printf("%s", BEGIN_ARRAY);
                    if (ret) {
                        const uint8 rows = input.GetType(childName).GetNumberOfElements(1u);
                        const uint8 cols = input.GetType(childName).GetNumberOfElements(0u);
                        Matrix<StreamString> values(rows, cols);
                        ret = input.Read(childName, values);
                        uint8 i = 0;
                        while (ret && i < rows) {
                            if (ret && (i > 0)) {
                                ret = output.Printf("%s", VALUE_SEPARATOR);
                            }
                            if (ret) {
                                ret = output.Printf("%s", BEGIN_ARRAY);
                            }
                            if (ret) {
                                uint8 j = 0;
                                while (ret && j < cols) {
                                    if (ret && (j > 0)) {
                                        ret = output.Printf("%s", VALUE_SEPARATOR);
                                    }
                                    if (ret) {
                                        StreamString cookedValue;
                                        generateJsonScalarValue(values[i][j], descriptor, cookedValue);
                                        ret = output.Printf("%s", cookedValue.Buffer());
                                    }
                                    j++;
                                }
                            }
                            if (ret) {
                                ret = output.Printf("%s", END_ARRAY);
                            }
                            i++;
                        }
                    }
                    if (ret) {
                        ret = output.Printf("%s", END_ARRAY);
                    }
                }
                else {
                    ret = false;
                }
            }
        }

        if (ret) {
            ret = output.Printf("%s", END_OBJECT);
        }

    }

    return ret;

}

}
