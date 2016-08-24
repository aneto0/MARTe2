/**
 * @file Validate.cpp
 * @brief Source file for class Validate
 * @date 25/gen/2016
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
 * the class Validate (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "Validate.h"
#include "TypeDescriptor.h"
#include "TypeConversion.h"
#include "StringHelper.h"
#include "StandardParser.h"
#include "ConfigurationDatabase.h"
#include "AdvancedErrorManagement.h"
#include "ValidatorI.h"
#include "AliasConverterI.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

bool Validate(const AnyType &value,
              const char8* const attributes) {

    ConfigurationDatabase cdb;

    StreamString configString = attributes;
    bool ret = true;
    if (configString.Size() > 0u) {
        ret = (configString.Seek(0ULL));

        StandardParser parser(configString, cdb);
        if (ret) {
            ret = parser.Parse();
        }
        if (ret) {
            ReferenceContainer validatorContainer;
            ret = validatorContainer.Initialise(cdb);
            if (ret) {
                uint32 numberOfValidators = validatorContainer.Size();
                for (uint32 i = 0u; (i < numberOfValidators) && (ret); i++) {
                    ReferenceT<ValidatorI> validator = validatorContainer.Get(i);
                    if (validator.IsValid()) {
                        ret = validator->Validate(value);
                    }
                }
            }
        }
    }
    return ret;
}

bool Alias(StructuredDataI& data,
           const char8* const memberName,
           const char8* const attributes) {
    ConfigurationDatabase attribCdb;

    StreamString configString = attributes;
    bool ret = true;
    if (configString.Size() > 0u) {
        ret = (configString.Seek(0ULL));

        StandardParser parser(configString, attribCdb);
        if (ret) {
            ret = parser.Parse();
        }
        if (ret) {

            ReferenceContainer aliasConverterContainer;
            ret = aliasConverterContainer.Initialise(attribCdb);

            if (ret) {
                uint32 numberOfAliasConverters = aliasConverterContainer.Size();
                for (uint32 i = 0u; (i < numberOfAliasConverters) && (ret); i++) {
                    ReferenceT<AliasConverterI> aliasConverter = aliasConverterContainer.Get(i);
                    if (aliasConverter.IsValid()) {
                        AnyType source = data.GetType(memberName);
                        ret = (!source.IsVoid());
                        if (ret) {
                            AnyObject destinationOut;

                            ret = aliasConverter->SourceToDestination(source, destinationOut);
                            AnyType destination = destinationOut.GetType();
                            if (ret) {
                                ret = !destination.IsVoid();
                            }
                            if (ret) {
                                ret = data.Delete(memberName);
                            }
                            if (ret) {
                                data.Write(memberName, destination);
                            }
                        }
                        else{
                            REPORT_ERROR_PARAMETERS(ErrorManagement::FatalError, "Required member %s not found", memberName)
                        }
                    }
                }
            }

        }
    }
    return ret;

}

bool InvertAlias(StructuredDataI& data,
                 const char8* const memberName,
                 const char8* const attributes) {
    ConfigurationDatabase attribCdb;

    StreamString configString = attributes;
    bool ret = true;
    if (configString.Size() > 0u) {
        ret = (configString.Seek(0ULL));

        StandardParser parser(configString, attribCdb);
        if (ret) {
            ret = parser.Parse();
        }
        if (ret) {

            ReferenceContainer aliasConverterContainer;
            ret = aliasConverterContainer.Initialise(attribCdb);

            if (ret) {
                uint32 numberOfAliasConverters = aliasConverterContainer.Size();
                for (uint32 i = 0u; (i < numberOfAliasConverters) && (ret); i++) {
                    ReferenceT<AliasConverterI> aliasConverter = aliasConverterContainer.Get(i);
                    if (aliasConverter.IsValid()) {
                        AnyType destination = data.GetType(memberName);
                        ret = (!destination.IsVoid());
                        if (ret) {
                            AnyObject sourceOut;

                            ret = aliasConverter->DestinationToSource(destination, sourceOut);
                            AnyType source = sourceOut.GetType();
                            if (ret) {
                                ret = !source.IsVoid();
                            }
                            if (ret) {
                                ret = data.Delete(memberName);
                            }
                            if (ret) {
                                data.Write(memberName, source);
                            }
                        }
                        else{
                            REPORT_ERROR_PARAMETERS(ErrorManagement::FatalError, "Required member %s not found", memberName)
                        }
                    }
                }
            }

        }
    }
    return ret;

}
}
