/**
 * @file ConfigurationParserI.cpp
 * @brief Source file for class ConfigurationParserI
 * @date 06/05/2020
 * @author Nicolo Ferron
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
 * the class ConfigurationParserI (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "AdvancedErrorManagement.h"
#include "ConfigurationParserI.h"
#include "MathExpressionParser.h"
#include "RuntimeEvaluator.h"
#include "TypeConversion.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

static void PrintErrorOnStream(const char8 * const format,
                               const uint32 lineNumber,
                               BufferedStreamI * const err) {
    if (err != NULL) {
        if (!err->Printf(format, lineNumber)) {
            REPORT_ERROR_STATIC(ErrorManagement::FatalError, "PrintErrorOnStream: Failed Printf() on parseError stream");
        }
    }
    
    REPORT_ERROR_STATIC(ErrorManagement::FatalError,  format, lineNumber);
}

static void PrintErrorOnStreamNoLineNumber(const char8 * const format,
                               BufferedStreamI * const err) {
    if (err != NULL) {
        if (!err->Printf(format, voidAnyType)) {
            REPORT_ERROR_STATIC(ErrorManagement::FatalError, "PrintErrorOnStream: Failed Printf() on parseError stream");
        }
    }
    
    REPORT_ERROR_STATIC(ErrorManagement::FatalError, format);
}

static const char8* GetCurrentTokenData(Token * const token) {

    return (token != NULL)?(token->GetData()):(static_cast<const char8*>(NULL));
}

static uint32 GetCurrentTokenId(const Token * const token) {
    return (token != NULL)?(token->GetId()):(ERROR_TOKEN);
}

static uint32 GetCurrentTokenLineNumber(const Token * const token) {
    return (token != NULL)?(token->GetLineNumber()):0u;
}

}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

ConfigurationParserI::ConfigurationParserI(StreamI &stream,
                               StructuredDataI &databaseIn,
                               BufferedStreamI * const err,
                               const GrammarInfo &grammarIn)
        : ParserI(stream, err, grammarIn)
        , memory(1u) { numberOfColumns = 0u;
    firstNumberOfColumns = 0u;
    numberOfRows = 0u;
    database = &databaseIn;
    tokenType = 0u;
    numberOfDimensions = 0u;
    handleMathExpr = false; 
    //For the time-being math expressions can only be handled if the output database is a ConfigurationDatabase 
    //as it allows to navigate the database while it is being constructed.
    ConfigurationDatabase *cdbDatabaseIn = dynamic_cast<ConfigurationDatabase *>(&databaseIn);
    outputSupportsMathExpr = (cdbDatabaseIn != NULL_PTR(ConfigurationDatabase *));
}

ConfigurationParserI::~ConfigurationParserI() {
    database=static_cast<StructuredDataI*>(NULL);
}

void ConfigurationParserI::GetTypeCast() {
    typeName = GetCurrentTokenData(currentToken);
}

/*lint -e{613} database cannot be NULL as it is set by reference in the constructor.*/
void ConfigurationParserI::GetExprCast() {
    if (outputSupportsMathExpr) {
        //Get the math expression output type
        const char8 * const outputType = GetCurrentTokenData(currentToken);
        //Get the path to the root
        StreamString path = "";
        //Do not disturb the database (which is an output being modified by the parser)
        ConfigurationDatabase cdbDatabase = *dynamic_cast<ConfigurationDatabase *>(database);
        bool hasAncestor = true;
        while (hasAncestor) {
            StreamString oldPath = path;
            const char8 * pathC = cdbDatabase.GetName();
            if (pathC != NULL_PTR(const char8 *)) {
                path = pathC;
                if (oldPath.Size() > 0u) {
                    path += ".";
                    path += oldPath;
                }
                hasAncestor = cdbDatabase.MoveToAncestor(1u);
            }
            else {
                hasAncestor = false;
            }
        }
        isError = !mathExpressionsCDB.MoveToRoot();
        uint32 numberOfExpressions = 0u;
        StreamString nn;
        if (!isError) {
            numberOfExpressions = mathExpressionsCDB.GetNumberOfChildren();
            isError = !nn.Printf("%d", numberOfExpressions);
        }
        if (!isError) {
            isError = !mathExpressionsCDB.CreateAbsolute(nn.Buffer());
        }
        if (!isError) {
            isError = !mathExpressionsCDB.MoveAbsolute(nn.Buffer());
        }
        if (!isError) {
            isError = !mathExpressionsCDB.Write("OutputType", outputType);
        }
        if (!isError) {
            isError = !mathExpressionsCDB.Write("Path", path.Buffer());
        }
        handleMathExpr = !isError;
    }
    else {
        PrintErrorOnStream("Failed StructuredDataI::GetExprCast()! Output database does not support math expressions [%d]", GetCurrentTokenLineNumber(currentToken), errorStream);
        isError = true;
    }
}

void ConfigurationParserI::BlockEnd() {
    if (!database->MoveToAncestor(1u)) {
        PrintErrorOnStream("Failed StructuredDataI::MoveToAncestor(1)! [%d]", GetCurrentTokenLineNumber(currentToken), errorStream);
        isError = true;
    }
}

void ConfigurationParserI::CreateNode() {
    if (!database->CreateRelative(GetCurrentTokenData(currentToken))) {
        PrintErrorOnStream("Failed StructuredDataI::CreateRelative(*)! [%d]", GetCurrentTokenLineNumber(currentToken), errorStream);
        isError = true;
    }
}

void ConfigurationParserI::AddLeaf() {
    // use numberOfRows and numberOfColumns as dimensions # elements
    if (numberOfDimensions == 1u) {
        numberOfRows = 1u;
    }
    // a scalar
    if (numberOfDimensions == 0u) {
        numberOfRows = 1u;
        numberOfColumns = firstNumberOfColumns;
    }

    ;
    uint32 dimSizes[3] = { numberOfColumns, numberOfRows, 1u };
    /*lint -e{613} . Justification: if (memory==NULL) ---> (ret==false) */
    AnyType element = memory.Create(numberOfDimensions, &dimSizes[0]);
    bool ret = (element.GetDataPointer() != NULL);
    if (ret) {
        ret = database->Write(nodeName.Buffer(), element);
        if (ret) {
            if (handleMathExpr) {
                isError = !mathExpressionsCDB.Write("Leaf", nodeName.Buffer());
            }
        }
        else {
            PrintErrorOnStream("Failed adding a leaf to the configuration database! [%d]", GetCurrentTokenLineNumber(currentToken), errorStream);
            isError = true;
        }
    }
    else {
        PrintErrorOnStream("Possible empty vector or matrix! [%d]", GetCurrentTokenLineNumber(currentToken), errorStream);
        isError = true;
    }

    typeName = defaultTypeName;
    numberOfColumns = 0u;
    firstNumberOfColumns = 0u;
    numberOfRows = 0u;
    tokenType = 0u;
    numberOfDimensions = 0u;
    handleMathExpr = false;
    memory.CleanUp(1u);
}

void ConfigurationParserI::GetNodeName() {
    nodeName = GetCurrentTokenData(currentToken);
}

void ConfigurationParserI::AddScalar() {
    if (tokenType == 0u) {
        tokenType = GetCurrentTokenId(currentToken);
    }

    if (tokenType == GetCurrentTokenId(currentToken)) {
        bool ret = memory.Add(typeName.Buffer(), GetCurrentTokenData(currentToken));

        if (ret) {
            firstNumberOfColumns++;
        }
        else {
            PrintErrorOnStream("Failed read or conversion! [%d]", GetCurrentTokenLineNumber(currentToken), errorStream);
            isError = true;
        }
    }
    else {
        PrintErrorOnStream("Cannot mix different types in a vector or matrix! [%d]", GetCurrentTokenLineNumber(currentToken), errorStream);
        isError = true;
    }
}

void ConfigurationParserI::EndVector() {
    if (numberOfColumns == 0u) {
        numberOfColumns = firstNumberOfColumns;
    }
    else {
        if (numberOfColumns != firstNumberOfColumns) {
            PrintErrorOnStream("Incorrect matrix format! [%d]", GetCurrentTokenLineNumber(currentToken), errorStream);
            isError = true;
        }
    }
    firstNumberOfColumns = 0u;
    numberOfRows++;
    if (numberOfDimensions == 0u) {
        numberOfDimensions = 1u;
    }
}

void ConfigurationParserI::EndVectorB() {
    const char8 * const thisNodeName = vectorStructureIdx.GetName();
    if (vectorStructureIdx.MoveToAncestor(1u)) {
        (void) vectorStructureIdx.Delete(thisNodeName);
    }
}

void ConfigurationParserI::EndMatrix() {
    numberOfDimensions = 2u;
}

void ConfigurationParserI::End() {
    //Check if there any math expressions to parse
    if (outputSupportsMathExpr) {
        if (!isError) {
            isError = !mathExpressionsCDB.MoveToRoot();
        }
        uint32 numberOfExpressions = 0u;
        if (!isError) {
            numberOfExpressions = mathExpressionsCDB.GetNumberOfChildren();
        }
        for (uint32 i=0u; (i < numberOfExpressions) && (!isError); i++) {
            StreamString path;
            StreamString leaf;
            StreamString outputType;
            isError = !mathExpressionsCDB.MoveToChild(i);
            if (!isError) {
                isError = !mathExpressionsCDB.Read("Path", path); 
            }
            if (!isError) {
                isError = !mathExpressionsCDB.Read("Leaf", leaf); 
            }
            if (!isError) {
                isError = !mathExpressionsCDB.Read("OutputType", outputType); 
            }
            if (!isError) {
                isError = !ExpandExpression(path.Buffer(), leaf.Buffer(), outputType.Buffer());
            }
            if (!isError) {
                isError = !mathExpressionsCDB.MoveToAncestor(1u);
            }
        }
    }
    if (!isError) {
        if (!database->MoveToRoot()) {
            PrintErrorOnStream("Failed StructuredDataI::MoveToRoot() at the end! [%d]", GetCurrentTokenLineNumber(currentToken), errorStream);
            isError = true;
        }
    }
}

/*lint -e{613} evaluator == NULL => !ok.*/
bool ConfigurationParserI::ExpandExpression(const char8 * const nodePath, const char8 * const nodeNameIn, const char8 * const outputTypeName) {
    bool ok = true;
    if (StringHelper::Length(nodePath) > 0u) {
        ok = database->MoveAbsolute(nodePath);
    }
    else {
        ok = database->MoveToRoot();
    }
    StreamString nodeExpr;
    if (ok) {
        //Read the math expression (which is going to be expanded and patched with the final value).
        ok = database->Read(nodeNameIn, nodeExpr);
    }
    StreamString validationExpressionStr = "RES = ";
    if (ok) {
        validationExpressionStr += nodeExpr.Buffer();
        validationExpressionStr += ";";
        ok = validationExpressionStr.Seek(0LLU);
    }
    MathExpressionParser mathParser(validationExpressionStr);
    if (ok) {
        ok = mathParser.Parse();
        if (!ok) {
            StreamString err;
            (void) err.Printf("Failed ConfigurationParserI::ExpandExpression() at parsing the expression %s", validationExpressionStr.Buffer());
            PrintErrorOnStreamNoLineNumber(err.Buffer(), errorStream);
        }
    }
    StreamString stackMachineExpr;
    if (ok) {
        stackMachineExpr = mathParser.GetStackMachineExpression();
    }
    RuntimeEvaluator *evaluator = NULL_PTR(RuntimeEvaluator *);
    if (ok) {
        evaluator = new RuntimeEvaluator(stackMachineExpr);
        ok = evaluator->ExtractVariables();
    }
    TypeDescriptor outVarType = TypeDescriptor::GetTypeDescriptorFromTypeName(outputTypeName);
    if (ok) {
        ok = outVarType.IsNumericType();
        if (ok) {
            //It will not fail if the type does not exist
            ok = evaluator->SetOutputVariableType("RES", outVarType);
        }
        if (!ok) {
            StreamString err;
            (void) err.Printf("Failed ConfigurationParserI::ExpandExpression() at setting the output variable type %s", outputTypeName);
            PrintErrorOnStreamNoLineNumber(err.Buffer(), errorStream);
        }
    }
    //Loop through all the input variables
    if (ok) {
        ok = database->MoveToRoot();
    }
    if (ok) {
        ok = BrowseExpressionVariables(evaluator);
    }
    uint8 *memResult = NULL_PTR(uint8 *);
    if (ok) {
        uint32 byteSize = outVarType.numberOfBits;
        byteSize /= 8u;
        memResult = new uint8[byteSize];
        /*lint -e{429} -e{593} memResult is freed by the caller.*/
        ok = evaluator->SetOutputVariableMemory("RES", memResult);
    }
    if (ok) {
        ok = evaluator->Compile();
        if (!ok) {
            StreamString err;
            (void) err.Printf("Failed ConfigurationParserI::ExpandExpression() failed to compile expression: %s", nodeExpr.Buffer());
            PrintErrorOnStreamNoLineNumber(err.Buffer(), errorStream);
        }
    }
    if (ok) {
        ok = evaluator->Execute();
        if (!ok) {
            StreamString err;
            (void) err.Printf("Failed ConfigurationParserI::ExpandExpression() failed to execute expression: %s", nodeExpr.Buffer());
            PrintErrorOnStreamNoLineNumber(err.Buffer(), errorStream);
        }
    }
    if (ok) {
        if (StringHelper::Length(nodePath) > 0u) {
            ok = database->MoveAbsolute(nodePath);
        }
        else {
            ok = database->MoveToRoot();
        }
    }
    if (ok) {
        //Write back the result
        AnyType outAnyType(outVarType, 0u, static_cast<void *>(&memResult[0u]));
        ok = database->Write(nodeNameIn, outAnyType);
    }
    if (memResult != NULL_PTR(uint8 *)) {
        delete []memResult;
    }
    if (evaluator != NULL_PTR(RuntimeEvaluator *)) {
        delete evaluator;
    }
    return ok;
/*lint -e{429} -e{593} memResult is freed by the caller.*/
}

/*lint -e{613} -e{429} BrowseExpressionVariables only called if evaluator != NULL. Evaluator is freed by the caller.*/
bool ConfigurationParserI::BrowseExpressionVariables(RuntimeEvaluator * const evaluator) {
    bool ok = true;
    uint32 index = 0u;
    VariableInformation *variableInformation;
    while (evaluator->BrowseInputVariable(index, variableInformation) == ErrorManagement::NoError) {
        const char8 * varNameC = variableInformation->name.Buffer();
        if (varNameC != NULL_PTR(const char8 *)) {
            if (StringHelper::CompareN(varNameC, "Constant@", 9u) != 0) {    // exclude constants
                //Check if it is a path
                StreamString token;
                StreamString lastToken;
                StreamString varPath;
                StreamString varName = varNameC;
                (void) varName.Seek(0LLU);
                char8 ignored;
                while (varName.GetToken(token, ".", ignored)) {
                    if (varPath.Size() > 0u) {
                        varPath += ".";
                    }
                    if (lastToken.Size() > 0u) {
                        varPath += lastToken;
                    }
                    lastToken = token;
                    token = "";
                }
                varName = lastToken;
                AnyType varDB;
                if (ok) {
                    //Not a path
                    if (varPath.Size() == 0u) {
                        varDB = database->GetType(varNameC);
                    }
                    else {
                        //Navigate to the variable path
                        ok = database->MoveAbsolute(varPath.Buffer());
                        if (ok) {
                            varDB = database->GetType(varName.Buffer());
                        }
                        else {
                            StreamString err;
                            (void) err.Printf("Failed ConfigurationParserI::ExpandExpression() variable with path %s not found", varPath.Buffer());
                            PrintErrorOnStreamNoLineNumber(err.Buffer(), errorStream);
                        }
                    }
                    if (ok) {
                        ok = !varDB.IsVoid();
                    }
                    if (!ok) {
                        StreamString err;
                        (void) err.Printf("Failed ConfigurationParserI::ExpandExpression() variable %s not found", varNameC);
                        PrintErrorOnStreamNoLineNumber(err.Buffer(), errorStream);
                    }
                }
                if (ok) {
                    ok = evaluator->SetInputVariableType(varNameC, varDB.GetTypeDescriptor());
                }
                if (ok) {
                    ok = evaluator->SetInputVariableMemory(varNameC, varDB.GetDataPointer());
                }
                if (!ok) {
                    break;
                }
            }
        }
        index++;
    }
    return ok;
/*lint -e{429} Evaluator is freed by the caller.*/
}

void ConfigurationParserI::CreateNodeV() {
    StreamString vecNodeName = GetCurrentTokenData(currentToken);
    if (!vectorStructureIdx.CreateRelative(vecNodeName.Buffer())) {
        PrintErrorOnStream("Failed StructuredDataI::CreateNodeV() CreateRelative [%d]", GetCurrentTokenLineNumber(currentToken), errorStream);
        isError = true;
    }
    if (!vectorStructureIdx.Write("Idx", 0)) {
        PrintErrorOnStream("Failed StructuredDataI::CreateNodeV() Write Idx 0 [%d]", GetCurrentTokenLineNumber(currentToken), errorStream);
        isError = true;
    }
}

void ConfigurationParserI::CreateNodeVE() {
    StreamString vecNodeName;
    uint32 idx = 0u;
    if (!vectorStructureIdx.Read("Idx", idx)) {
        PrintErrorOnStream("Failed StructuredDataI::CreateNodeVE() Read Idx [%d]", GetCurrentTokenLineNumber(currentToken), errorStream);
        isError = true;
    }
    if(!vecNodeName.Printf("%s[%d]", vectorStructureIdx.GetName(), idx)) {
        PrintErrorOnStream("Failed StructuredDataI::CreateNodeVE() Printf nodeName [%d]", GetCurrentTokenLineNumber(currentToken), errorStream);
        isError = true;
    }
    if(!database->CreateRelative(vecNodeName.Buffer())) {
        PrintErrorOnStream("Failed StructuredDataI::CreateNodeVE() CreateRelative [%d]", GetCurrentTokenLineNumber(currentToken), errorStream);
        isError = true;
    }
    idx++;
    if (!vectorStructureIdx.Write("Idx", idx)) {
        PrintErrorOnStream("Failed StructuredDataI::CreateNodeVE() Write Idx [%d]", GetCurrentTokenLineNumber(currentToken), errorStream);
        isError = true;
    }
}
}

