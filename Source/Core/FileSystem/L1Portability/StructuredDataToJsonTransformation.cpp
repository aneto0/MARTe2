/**
 * @file StructuredDataToJsonTransformation.cpp
 * @brief Source file for class StructuredDataToJsonTransformation
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
 * the class StructuredDataToJsonTransformation (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "StructuredDataToJsonTransformation.h"
#include "StreamString.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

namespace {

}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

StructuredDataToJsonTransformation::StructuredDataToJsonTransformation() {
//Auto-generated constructor stub for StructuredDataToJsonTransformation

//TODO Verify if manual additions are needed here
}

StructuredDataToJsonTransformation::~StructuredDataToJsonTransformation() {
//Auto-generated destructor stub for StructuredDataToJsonTransformation

//TODO Verify if manual additions are needed here
}

bool StructuredDataToJsonTransformation::Execute(StructuredDataI& input,
                                                 IOBuffer& output) {
    bool ret = true;

    static const AnyType BEGIN_OBJECT[] = { "{", voidAnyType };
    static const AnyType END_OBJECT[] = { "}", voidAnyType };
    //static const AnyType BEGIN_ARRAY[] = { "[", voidAnyType };
    //static const AnyType END_ARRAY[] = { "]", voidAnyType };
    static const AnyType NAME_SEPARATOR[] = { ":", voidAnyType };
    static const AnyType VALUE_SEPARATOR[] = { ",", voidAnyType };
    static const AnyType NULL_VALUE[] = { "null", voidAnyType };
    //static const AnyType TRUE_VALUE[] = { "true", voidAnyType };
    //static const AnyType FALSE_VALUE[] = { "false", voidAnyType };

    //TODO: Arrays and matrices!!

    uint32 numberOfChildren = input.GetNumberOfChildren();

    if (numberOfChildren == 0) {
        //There is no children, so it is a null value
        if (!output.PrintFormatted("%s", &NULL_VALUE[0])) {
            ret = false;
        }
    }
    else {
        //There is at least 1 child, so it is an object value
        if (!output.PrintFormatted("%s", &BEGIN_OBJECT[0])) {
            ret = false;
        }

        for (uint32 i = 0u; (i < numberOfChildren) && (ret); i++) {
            const char8 * childName = input.GetChildName(i);
            //value separator
            if (i > 0) {
                if (!output.PrintFormatted("%s", &VALUE_SEPARATOR[0])) {
                    ret = false;
                }
            }
            //key name
            {
                AnyType printChildName[] = { childName, voidAnyType };
                if (!output.PrintFormatted("\"%s\"", &printChildName[0])) {
                    ret = false;
                }
            }
            //name separator
            if (!output.PrintFormatted("%s", &NAME_SEPARATOR[0])) {
                ret = false;
            }
            //value
            {
                StreamString rawValue;
                if (input.Read(childName, rawValue)) {
                    //The child is a leaf
                    StreamString cookedValue;
                    {
                        TypeDescriptor descriptor;
                        descriptor = input.GetType(childName).GetTypeDescriptor();
                        if (descriptor == VoidType || descriptor == InvalidType) {
                            cookedValue = "null";
                        }
                        else if (descriptor == PointerType || descriptor == Character8Bit || descriptor == ConstCharString || descriptor == CharString) {
                            cookedValue = "\"";
                            cookedValue += rawValue;
                            cookedValue += "\"";
                        }
                        else {
                            cookedValue = rawValue;
                        }
                    }
                    {
                        AnyType printLeaf[] = { cookedValue.Buffer(), voidAnyType };
                        if (!output.PrintFormatted("%s", &printLeaf[0])) {
                            ret = false;
                        }
                    }
                }
                else {
                    //The child is a node
                    if (ret) {
                        if (input.MoveRelative(childName)) {
                            ret = Execute(input, output);
                            if (!input.MoveToAncestor(1u)) {
                                ret = false;
                            }
                        }
                        else {
                            ret = false;
                        }
                    }

                }
            }
        }

        if (!output.PrintFormatted("%s", &END_OBJECT[0])) {
            ret = false;
        }

    }

    return ret;

}

bool StructuredDataToJsonTransformation::FormatJson(IOBuffer& input,
                                                    IOBuffer& output) {
    bool ret = true;
    char8 c;
    while (input.GetC(c)) {
        StreamString str;
        uint32 size;
        switch (c) {
        case '{':
            str = "{\n";
            size = 2;
            break;
        case ':':
            str = ": ";
            size = 2;
            break;
        case ',':
            str = ",\n";
            size = 2;
            break;
        case '}':
            str = "\n}";
            size = 2;
            break;
        default:
            str = c;
            size = 1;
            break;
        }
        output.WriteAll(str.Buffer(), size);
    }
    return ret;  //TODO: Check ret!!
}

}

