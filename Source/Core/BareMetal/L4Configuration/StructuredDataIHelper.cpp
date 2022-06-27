/**
 * @file StructuredDataIHelper.cpp
 * @brief Source file for class StructuredDataIHelper
 * @date 04/06/2022
 * @author Andre' Neto
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
 * the class StructuredDataIHelper (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#define DLL_API

#include "AdvancedErrorManagement.h"
#include "MathExpressionParser.h"
#include "RuntimeEvaluator.h"
#include "StructuredDataIHelper.h"
#include "StreamString.h"
#include "TypeConversion.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

StructuredDataIHelper::StructuredDataIHelper(StructuredDataI &sdiIn, Reference ownerIn) :
        sdi(sdiIn), owner(ownerIn) {
    hasErrors = !owner.IsValid();
}

StructuredDataIHelper::~StructuredDataIHelper() {
}

void StructuredDataIHelper::ResetErrors() {
    hasErrors = false;
}

bool StructuredDataIHelper::HasErrors() const {
    return hasErrors;
}

bool StructuredDataIHelper::Write(const char8 * const name, const AnyType &value) {
    if (!hasErrors) {
        hasErrors = !sdi.Write(name, value);
    }
    return !hasErrors;
}

bool StructuredDataIHelper::Copy(StructuredDataI &destination) {
    if (!hasErrors) {
        hasErrors = !sdi.Copy(destination);
    }
    return !hasErrors;
}

AnyType StructuredDataIHelper::GetType(const char8 * const name) {
    return sdi.GetType(name);
}

bool StructuredDataIHelper::MoveToRoot() {
    if (!hasErrors) {
        hasErrors = !sdi.MoveToRoot();
    }
    return !hasErrors;
}

bool StructuredDataIHelper::Read(const char8 * const name, const AnyType &value) {
    return Read(name, value, voidAnyType);
}

bool StructuredDataIHelper::Read(const char8 * const name, const AnyType &value, const AnyType &defaultValue) {
    if (!hasErrors) {
        hasErrors = !sdi.Read(name, value);
        if (hasErrors) {
            if (defaultValue.IsVoid()) {
                REPORT_ERROR_PROXY(ErrorManagement::ParametersError, owner, "%s does not exist and no default value specified", name);
            }
            else {
                hasErrors = !TypeConvert(value, defaultValue);
                if (!hasErrors) {
                    REPORT_ERROR_PROXY(ErrorManagement::Information, owner, "%s does not exist. Using default value: %!", name, value);
                }
                else {
                    REPORT_ERROR_PROXY(ErrorManagement::ParametersError, owner, "%s does not exist. Default value specified: %!, but conversion failed.", name, defaultValue);
                }
            }
        }
        else {
            REPORT_ERROR_PROXY(ErrorManagement::Information, owner, "%s successfully read: %!", name, value);
        }
    }
    return !hasErrors;
}

bool StructuredDataIHelper::ReadEnum(const char8 * const name, const AnyType &value, const AnyType &options, const AnyType &optionValues) {
    return ReadEnum(name, value, options, optionValues, voidAnyType);
}

bool StructuredDataIHelper::ReadEnum(const char8 * const name, const AnyType &value, const AnyType &options, const AnyType &optionValues, const AnyType &defaultValue) {
    uint32 numberOfOptions = 0u;
    if (!hasErrors) {
        numberOfOptions = options.GetNumberOfElements(0u);
        hasErrors = (numberOfOptions == 0u);
        if (hasErrors) {
            REPORT_ERROR_PROXY(ErrorManagement::ParametersError, owner, "%s was called with no options specified", name);
        }
    }
    if (!hasErrors) {
        uint32 numberOfOptionValues = optionValues.GetNumberOfElements(0u);
        hasErrors = (numberOfOptions != numberOfOptionValues);
        if (hasErrors) {
            REPORT_ERROR_PROXY(ErrorManagement::ParametersError, owner, "%s was called with sizeof(options) != sizeof(optionValues)", name);
        }
    }

    StreamString foundOption;
    if (!hasErrors) {
        hasErrors = !sdi.Read(name, foundOption);
        if (hasErrors) {
            if (defaultValue.IsVoid()) {
                REPORT_ERROR_PROXY(ErrorManagement::ParametersError, owner, "%s does not exist and no default value specified", name);
            }
            else {
                hasErrors = !TypeConvert(foundOption, defaultValue);
                if (!hasErrors) {
                    REPORT_ERROR_PROXY(ErrorManagement::Information, owner, "%s does not exist. Using default value: %!", name, foundOption.Buffer());
                }
                else {
                    REPORT_ERROR_PROXY(ErrorManagement::ParametersError, owner, "%s does not exist. Default value specified: %!, but conversion failed.", name, defaultValue);
                }
            }
        }
    }
    if (!hasErrors) {
        Vector<StreamString> optionsV(numberOfOptions);
        Vector<StreamString> optionValuesV(numberOfOptions);
        hasErrors = !TypeConvert(optionsV, options);
        if (!hasErrors) {
            hasErrors = !TypeConvert(optionValuesV, optionValues);
            if (hasErrors) {
                REPORT_ERROR_PROXY(ErrorManagement::ParametersError, owner, "%s failed to convert option values: %!", name, optionValues);
            }
        }
        else {
            REPORT_ERROR_PROXY(ErrorManagement::ParametersError, owner, "%s failed to convert options: %!", name, options);
        }
        bool found = false;
        uint32 foundIdx = 0u;
        if (!hasErrors) {
            for (uint32 i=0u; (i<numberOfOptions) && (!found); i++) {
                found = (optionsV[i] == foundOption);
                if (found) {
                    foundIdx = i;
                }
            }
        }
        if (!hasErrors) {
            hasErrors = !found;
            if (hasErrors) {
                REPORT_ERROR_PROXY(ErrorManagement::ParametersError, owner, "%s option %s not found in list of available options: %!", name, foundOption.Buffer(), options);
            }
        } 
        if (!hasErrors) {
            hasErrors = !TypeConvert(value, optionValuesV[foundIdx]);
            if (hasErrors) {
                REPORT_ERROR_PROXY(ErrorManagement::ParametersError, owner, "%s failed to convert from %! to %!", value, foundIdx);
            }
        }
        if (!hasErrors) {
            REPORT_ERROR_PROXY(ErrorManagement::Information, owner, "%s successfully read: %! = %!", name, foundOption.Buffer(), value);
        }
    }
    return !hasErrors;

}

bool StructuredDataIHelper::ReadValidated(const char8 * const name, const AnyType &value, const char8 * const validationExpression) {
    StreamString validationExpressionStr = "RES = ";
    validationExpressionStr += validationExpression;
    validationExpressionStr += ";";
    if (!hasErrors) {
        hasErrors = !Read(name, value);
    }
    if (!hasErrors) {
        hasErrors = !validationExpressionStr.Seek(0LLU);
    }
    MathExpressionParser mathParser(validationExpressionStr);
    if (!hasErrors) {
       hasErrors = !mathParser.Parse();
       if (hasErrors) {
           REPORT_ERROR_PROXY(ErrorManagement::ParametersError, owner, "%s failed to parse validation expression: %s", name, validationExpressionStr.Buffer());
       }
    }
    StreamString stackMachineExpr;
    if (!hasErrors) {
        stackMachineExpr = mathParser.GetStackMachineExpression();
    }
    RuntimeEvaluator *evaluator = NULL_PTR(RuntimeEvaluator *);
    if (!hasErrors) {
        evaluator = new RuntimeEvaluator(stackMachineExpr);
        hasErrors = !evaluator->ExtractVariables();
    }
    if (!hasErrors) {
        hasErrors = !evaluator->SetOutputVariableType("RES", UnsignedInteger8Bit);
    }
    if (!hasErrors) {
        hasErrors = !evaluator->SetInputVariableType(name, value.GetTypeDescriptor());
        if (hasErrors) {
            REPORT_ERROR_PROXY(ErrorManagement::ParametersError, owner, "%s failed to add input variable to expression: %s", name, validationExpression);
        }
    }
    uint8 result = 0u;
    if (!hasErrors) {
        hasErrors = !evaluator->SetOutputVariableMemory("RES", &result);
    }
    if (!hasErrors) {
        hasErrors = !evaluator->SetInputVariableMemory(name, value.GetDataPointer());
    }
    if (!hasErrors) {
        hasErrors = !evaluator->Compile();
        if (hasErrors) {
            REPORT_ERROR_PROXY(ErrorManagement::ParametersError, owner, "%s failed to compile expression: %s", name, validationExpression);
        }

    }
    if (!hasErrors) {
        hasErrors = !evaluator->Execute();
        if (hasErrors) {
            REPORT_ERROR_PROXY(ErrorManagement::ParametersError, owner, "%s failed to execute expression: %s", name, validationExpression);
        }
    }
    if (!hasErrors) {
        hasErrors = (result != 1u);
        if (hasErrors) {
            REPORT_ERROR_PROXY(ErrorManagement::ParametersError, owner, "%s with value %! failed to meet validation critera: %s", name, value, validationExpression);
        }
        else {
            REPORT_ERROR_PROXY(ErrorManagement::Information, owner, "%s with value %! meets validation critera: %s", name, value, validationExpression);
        }
    }
    if (evaluator != NULL_PTR(RuntimeEvaluator *)) {
        delete evaluator;
    }
    return !hasErrors;
}

bool StructuredDataIHelper::MoveAbsolute(const char8 * const path) {
    if (!hasErrors) {
        hasErrors = !sdi.MoveAbsolute(path);
    }
    return !hasErrors;
}

bool StructuredDataIHelper::MoveRelative(const char8 * const path) {
    if (!hasErrors) {
        hasErrors = !sdi.MoveRelative(path);
    }
    return !hasErrors;
}

bool StructuredDataIHelper::MoveToChild(const uint32 childIdx) {
    if (!hasErrors) {
        hasErrors = !sdi.MoveToChild(childIdx);
    }
    return !hasErrors;
}

bool StructuredDataIHelper::MoveToAncestor(const uint32 generations) {
    if (!hasErrors) {
        hasErrors = !sdi.MoveToAncestor(generations);
    }
    return !hasErrors;
}

bool StructuredDataIHelper::CreateAbsolute(const char8 * const path) {
    if (!hasErrors) {
        hasErrors = !sdi.CreateAbsolute(path);
    }
    return !hasErrors;
}

bool StructuredDataIHelper::CreateRelative(const char8 * const path) {
    if (!hasErrors) {
        hasErrors = !sdi.CreateRelative(path);
    }
    return !hasErrors;
}

bool StructuredDataIHelper::AddToCurrentNode(Reference node) {
    if (!hasErrors) {
        hasErrors = !sdi.AddToCurrentNode(node);
    }
    return !hasErrors;
}

bool StructuredDataIHelper::Delete(const char8 * const name) {
    if (!hasErrors) {
        hasErrors = !sdi.Delete(name);
    }
    return !hasErrors;
}

const char8 *StructuredDataIHelper::GetName() {
    return sdi.GetName();
}

const char8 *StructuredDataIHelper::GetChildName(const uint32 index) {
    return sdi.GetChildName(index);
}

uint32 StructuredDataIHelper::GetNumberOfChildren() {
    return sdi.GetNumberOfChildren();
}

}
