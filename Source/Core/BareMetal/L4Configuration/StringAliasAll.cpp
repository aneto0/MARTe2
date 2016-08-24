/**
 * @file StringAliasAll.cpp
 * @brief Source file for class StringAliasAll
 * @date 12/ago/2016
 * @author pc
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
 * the class StringAliasAll (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "StringAliasAll.h"
#include "stdio.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

StringAliasAll::StringAliasAll() :
        AliasConverterI() {

}

StringAliasAll::~StringAliasAll() {

}

bool StringAliasAll::Initialise(StructuredDataI & data) {
    bool ret = ReferenceContainer::Initialise(data);
    if (ret) {
        data.Link(parameters);
    }
    return ret;
}

bool StringAliasAll::SourceToDestination(const AnyType &source,
                                         AnyObject &destination) {

    StreamString buff;
    bool ret = TypeConvert(buff, source);

    if (ret) {
        AnyType toSerialise = parameters.GetType(buff.Buffer());
        if (ret) {
            destination.Serialise(toSerialise);
        }
        else {
            REPORT_ERROR(ErrorManagement::FatalError, "Alias not found or incompatible destination");
        }
    }
    else {
        REPORT_ERROR(ErrorManagement::FatalError, "The source must be a one-dimensional string");
    }
    return ret;
}

bool StringAliasAll::DestinationToSource(const AnyType &destination,
                                         AnyObject &source) {

    TypeDescriptor desttd = destination.GetTypeDescriptor();
    bool isString = (desttd.type == SString);
//    bool isStructured = (desttd.type == StructuredDataNode);
    StreamString destPrint;

    //allocates the memory for the destination's values
    AnyObject temp;
    bool ret = temp.Serialise(destination);

    //if SString becomes a CString and the seek is done automatically (great trick!)
    AnyType tempAt = temp.GetType();

    destPrint.Printf("%!", tempAt);

    uint32 numberOfAliases = parameters.GetNumberOfChildren();

    for (uint32 i = 0u; (i < numberOfAliases) && ret; i++) {
        StreamString alias;
        const char8 * childName = parameters.GetChildName(i);
        if (StringHelper::Compare(childName, "Class") != 0) {

            //printing two equal values we should obtain the same string in result!
            //need to do this for seek problem (in cdb is always a const char*)
            if (isString) {
                alias.Printf("%!", parameters.GetType(childName));
            }
            else {
                ret = parameters.Read(childName, destination);
                alias.Printf("%!", destination);
            }

            if (ret) {
                if (alias == destPrint) {
                    source.Serialise(childName);
                    break;
                }
            }
        }
    }
    TypeConvert(destination, tempAt);
    return ret;
}
CLASS_REGISTER(StringAliasAll, "1.0");

}
