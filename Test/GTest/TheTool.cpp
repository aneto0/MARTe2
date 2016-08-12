/**
 * @file IntrospectionParser.cpp
 * @brief Source file for class IntrospectionParser
 * @date 19/01/2016
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

 * @details This source file contains the definition of all the methods for
 * the class IntrospectionParser (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "StandardParser.h"
#include "XMLParser.h"
#include "JsonParser.h"
#include "BasicFile.h"
#include "ConfigurationDatabase.h"
#include "AnyTypeCreator.h"
#include "StreamString.h"
#include <stdio.h>

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

static const char8 * nodeNames[] = { "Parameters", "InputSignals", "OutputSignals", 0 };

static void PrintOnFile(BasicFile &fileIn,
                        const char8 *data) {
    uint32 stringSize = StringHelper::Length(data);
    fileIn.Write(data, stringSize);
}

static void PrintOnFile(BasicFile &fileIn,
                        const char8 data) {
    uint32 charSize = 1;
    fileIn.Write(&data, charSize);
}

static uint32 GetModifierString(char8* data,
                                StreamString &modifiers) {
    uint32 dataSize = StringHelper::Length(data);
    uint32 i;
    for (i = 0u; i < dataSize; i++) {
        if (data[i] == '*') {
            modifiers += "* ";
        }
        else if (data[i] == 'C') {
            modifiers += "const ";
        }
        else {
            break;
        }
    }

    return i;
}

static void GenerateStructFileRecursive(ConfigurationDatabase &database,
                                        BasicFile &structHeader,
                                        BasicFile &structSource) {

    const char8* structName = database.GetName();

    for (uint32 j = 0u; j < database.GetNumberOfChildren(); j++) {

        // HEADER FILE MANAGEMENT
        const char8 *memberName = database.GetChildName(j);
        if (database.MoveRelative(memberName)) {

            for (uint32 k = 0u; k < database.GetNumberOfChildren(); k++) {
                const char8 * subStructName = database.GetChildName(k);

                if (database.MoveRelative(subStructName)) {
                    GenerateStructFileRecursive(database, structHeader, structSource);
                    database.MoveToAncestor(1u);
                }
            }
            database.MoveToAncestor(1u);
        }
    }

    StreamString structType;
    if (database.Read("type", structType)) {

        PrintOnFile(structHeader, "struct ");
        PrintOnFile(structHeader, structType.Buffer());
        PrintOnFile(structHeader, " { \n");

        for (uint32 j = 0u; j < database.GetNumberOfChildren(); j++) {
            // HEADER FILE MANAGEMENT
            const char8 *memberName = database.GetChildName(j);
            if (database.MoveRelative(memberName)) {

                StreamString modifiers;
                StreamString modifiersString;
                StreamString type;
                StreamString attributes;
                StreamString comments;

                // print the attributes as comment before the member on the header file
                if (!database.Read("attributes", attributes)) {
                    printf("\n[%s.%s][attributes] lacks: the attributes is considered as an empty string", structName, memberName);
                    attributes = "";
                }
                else {
                    PrintOnFile(structHeader, "    ");
                    PrintOnFile(structHeader, "/** ");
                    PrintOnFile(structHeader, attributes.Buffer());
                    PrintOnFile(structHeader, " */\n");
                }

                // print the comments before the member on the header file
                if (!database.Read("comments", comments)) {
                    printf("\n[%s.%s][comments] lacks: the comments is considered as an empty string", structName, memberName);
                    comments = "";
                }
                else {
                    PrintOnFile(structHeader, "    ");
                    PrintOnFile(structHeader, "/** ");
                    PrintOnFile(structHeader, comments.Buffer());
                    PrintOnFile(structHeader, " */\n");
                }

                // print the type
                PrintOnFile(structHeader, "    ");
                if (!database.Read("type", type)) {
                    printf("\n[%s.%s][type] lacks: the member type is considered void", structName, memberName);
                    type = "void";
                }

                PrintOnFile(structHeader, type.Buffer());
                PrintOnFile(structHeader, " ");

                if (!database.Read("modifiers", modifiers)) {
                    printf("\n[%s.%s][modifiers] lacks: the modifiers is considered as an empty string", structName, memberName);
                    modifiers = "";
                }

                uint32 nextIndex = GetModifierString(modifiers.BufferReference(), modifiersString);
                // print the modifiers
                PrintOnFile(structHeader, modifiersString.Buffer());

                // print the member name
                PrintOnFile(structHeader, memberName);
                while (modifiers[nextIndex] != '\0') {
                    PrintOnFile(structHeader, modifiers[nextIndex]);
                    nextIndex++;
                }
                PrintOnFile(structHeader, ";\n");

                // SOURCE FILE MANAGEMENT
                // declare the member introspection
                PrintOnFile(structSource, "DECLARE_CLASS_MEMBER(");
                PrintOnFile(structSource, structType.Buffer());
                PrintOnFile(structSource, ", ");
                PrintOnFile(structSource, memberName);
                PrintOnFile(structSource, ", ");
                PrintOnFile(structSource, type.Buffer());
                PrintOnFile(structSource, ", ");
                PrintOnFile(structSource, "\"");
                PrintOnFile(structSource, modifiers.Buffer());
                PrintOnFile(structSource, "\"");
                PrintOnFile(structSource, ", ");
                PrintOnFile(structSource, "\"");
                PrintOnFile(structSource, attributes.Buffer());
                PrintOnFile(structSource, "\"");

                PrintOnFile(structSource, ");\n");

                database.MoveToAncestor(1);
            }
        }
        PrintOnFile(structSource, "\n");
        PrintOnFile(structHeader, "}; \n\n");

        PrintOnFile(structSource, "static const IntrospectionEntry * ");
        PrintOnFile(structSource, structType.Buffer());
        PrintOnFile(structSource, "_array[] = {\n");

        for (uint32 j = 0u; j < database.GetNumberOfChildren(); j++) {
            const char8 *memberName = database.GetChildName(j);
            if (database.MoveRelative(memberName)) {
                PrintOnFile(structSource, "&");
                PrintOnFile(structSource, structType.Buffer());
                PrintOnFile(structSource, "_");
                PrintOnFile(structSource, memberName);
                PrintOnFile(structSource, "_introspectionEntry, \n");
                database.MoveToAncestor(1u);
            }
        }
        PrintOnFile(structSource, "0 };\n\n");

        // declare the class introspection
        PrintOnFile(structSource, "DECLARE_CLASS_INTROSPECTION(");
        PrintOnFile(structSource, structType.Buffer());
        PrintOnFile(structSource, ", ");
        PrintOnFile(structSource, structType.Buffer());
        PrintOnFile(structSource, "_array);\n");
        PrintOnFile(structSource, "INTROSPECTION_CLASS_REGISTER(");
        PrintOnFile(structSource, structType.Buffer());
        PrintOnFile(structSource, ", ");
        PrintOnFile(structSource, "\"1.0\", ");
        PrintOnFile(structSource, structType.Buffer());
        PrintOnFile(structSource, "_introspection)\n\n");
    }
    else {
        printf("\nError, undefined type for %s", structName);
    }
}

static void ReadTheType(const char8 *paramName,
                        const char8 *typeName,
                        const char8 *paramAddress,
                        const char8 *attributes,
                        BasicFile &objSource) {
    bool isStructType = (TypeDescriptor::GetTypeDescriptorFromTypeName(typeName) == InvalidType);

    PrintOnFile(objSource, "    if(ret) {\n");
    if (isStructType) {
        PrintOnFile(objSource, "        ANY_TYPE_STRUCT_BUILDER(");
        PrintOnFile(objSource, typeName);
        PrintOnFile(objSource, ", ");
        PrintOnFile(objSource, paramName);
        PrintOnFile(objSource, ");\n");
        PrintOnFile(objSource, "        ret = data.AdvancedRead(\"");
        PrintOnFile(objSource, paramAddress);
        PrintOnFile(objSource, "\",");
        PrintOnFile(objSource, paramName);
        PrintOnFile(objSource, "_at);\n");
    }
    else {
        PrintOnFile(objSource, "        ret = data.Read(\"");
        PrintOnFile(objSource, paramAddress);
        PrintOnFile(objSource, "\",");
        PrintOnFile(objSource, paramName);
        PrintOnFile(objSource, ");\n");
        PrintOnFile(objSource, "        if(ret) {\n");
        PrintOnFile(objSource, "            ret = Validate(");
        PrintOnFile(objSource, paramName);
        PrintOnFile(objSource, ", \"");
        PrintOnFile(objSource, attributes);
        PrintOnFile(objSource, "\");\n");
        PrintOnFile(objSource, "        }\n");

    }
    PrintOnFile(objSource, "        if(!ret) {\n"
                "            REPORT_ERROR(ErrorManagement::FatalError, \"Failed loading the parameter ");
    PrintOnFile(objSource, paramAddress);
    PrintOnFile(objSource, "\");\n");
    PrintOnFile(objSource, "        }\n");
    PrintOnFile(objSource, "    }\n");

}

static void ReadTheTypeArray(const char8 *paramName,
                             const char8 *typeName,
                             const char8 *paramAddress,
                             const char8 *attributes,
                             BasicFile &objSource) {

    PrintOnFile(objSource, "    if(ret) {\n");
    PrintOnFile(objSource, "        StreamString currentPath;\n"
                "        data.GetFullPath(currentPath);\n");

    if (StringHelper::Length(paramAddress) > 0u) {
        PrintOnFile(objSource, "        data.AdvancedMove(\"");
        PrintOnFile(objSource, paramAddress);
        PrintOnFile(objSource, "\");\n");
    }

    PrintOnFile(objSource, "        numberOf");
    PrintOnFile(objSource, paramName);
    PrintOnFile(objSource, " = 0u;\n");
    PrintOnFile(objSource, "        for(uint32 i=0u; i < data.Size(); i++){\n"
                "            const char8 * childName = data.GetChildName(i);\n");
    bool isStructType = (TypeDescriptor::GetTypeDescriptorFromTypeName(typeName) == InvalidType);
    if (isStructType) {
        PrintOnFile(objSource, "            if(data.MoveRelative(childName){\n"
                    "                StreamString type;\n"
                    "                if(data.Read(\"Class\", type){\n"
                    "                    if(type == \"");
        PrintOnFile(objSource, typeName);
        PrintOnFile(objSource, "\"){\n");
        PrintOnFile(objSource, "                        numberOf");
        PrintOnFile(objSource, paramName);
        PrintOnFile(objSource, "++;\n");
        PrintOnFile(objSource, "                    }\n"
                    "                }\n"
                    "                data.MoveToAncestor(1u);\n"
                    "            }\n");
    }
    else {
        PrintOnFile(objSource, "            AnyType at=data.GetType(childName);\n"
                    "            if(at.GetTypeDescriptor() == TypeDescriptor::GetTypeDescriptorFromTypeName(\"");
        PrintOnFile(objSource, typeName);
        PrintOnFile(objSource, "\")) {\n");
        PrintOnFile(objSource, "                numberOf");
        PrintOnFile(objSource, paramName);
        PrintOnFile(objSource, "++;\n"
                    "            }\n");
    }

    PrintOnFile(objSource, "        }\n        ");
    PrintOnFile(objSource, paramName);
    PrintOnFile(objSource, " = new ");
    PrintOnFile(objSource, typeName);
    PrintOnFile(objSource, "[numberOf");
    PrintOnFile(objSource, paramName);
    PrintOnFile(objSource, "];\n");
    PrintOnFile(objSource, "        for(uint32 i=0u; i < data.Size() && ret; i++){\n"
                "            const char8 *childName = data.GetChildName(i)\n");
    if (isStructType) {
        PrintOnFile(objSource, "            if(data.MoveRelative(childName){\n"
                    "                StreamString type;\n"
                    "                if(data.Read(\"Class\", type){\n"
                    "                    if(type == \"");
        PrintOnFile(objSource, typeName);
        PrintOnFile(objSource, "\"){\n"
                    "                        ANY_TYPE_STRUCT_BUILDER(");
        PrintOnFile(objSource, typeName);
        PrintOnFile(objSource, ", ");
        PrintOnFile(objSource, paramName);
        PrintOnFile(objSource, "[i]);\n"
                    "                        data.MoveToAncestor(1u);\n"
                    "                        ret = data.Read(childName, ");
        PrintOnFile(objSource, paramName);
        PrintOnFile(objSource, "_at);\n"
                    "                        data.MoveRelative(childName);\n"
                    "                    }\n"
                    "                }\n"
                    "                data.MoveToAncestor(1u);\n"
                    "            }\n");
    }
    else {
        PrintOnFile(objSource, "            AnyType at=data.GetType(childName);\n"
                    "            if(at.GetTypeDescriptor() == TypeDescriptor::GetTypeDescriptorFromTypeName(\"");
        PrintOnFile(objSource, typeName);
        PrintOnFile(objSource, "\")) {\n"
                    "                ret = data.Read(childName, ");
        PrintOnFile(objSource, paramName);
        PrintOnFile(objSource, ");\n");
        if (StringHelper::Length(attributes) > 0u) {
            PrintOnFile(objSource, "                if(ret) {\n");
            PrintOnFile(objSource, "                    ret = Validate(");
            PrintOnFile(objSource, paramName);
            PrintOnFile(objSource, ", \"");
            PrintOnFile(objSource, attributes);
            PrintOnFile(objSource, "\");\n");
            PrintOnFile(objSource, "                }\n");
        }
        PrintOnFile(objSource, "            }\n");

    }
    PrintOnFile(objSource, "        }\n"
                "        data.MoveAbsolute(currentPath.Buffer());\n");
    PrintOnFile(objSource, "        if(!ret) {\n"
                "            REPORT_ERROR(ErrorManagement::FatalError, \"Failed loading the parameters *");
    if (StringHelper::Length(paramAddress) > 0u) {
        PrintOnFile(objSource, paramAddress);
        PrintOnFile(objSource, ".");
    }
    PrintOnFile(objSource, paramName);
    PrintOnFile(objSource, "\");\n");
    PrintOnFile(objSource, "        }\n");
    PrintOnFile(objSource, "    }\n");
}

static void GenerateInitialiseFunction(ConfigurationDatabase &database,
                                       BasicFile &objHeader,
                                       BasicFile &objSource,
                                       const char8 * className) {

    PrintOnFile(objHeader, "    virtual bool ConfigureToolMembers(StructuredDataI &data);\n");

    PrintOnFile(objSource, "bool ");
    PrintOnFile(objSource, className);
    PrintOnFile(objSource, "::");
    PrintOnFile(objSource, "ConfigureToolMembers(StructuredDataI &data){\n");
    PrintOnFile(objSource, "    bool ret = true;\n");

    PrintOnFile(objHeader, "#define TOOL_MEMBERS_DECLARATION()");

    if (database.MoveRelative("Parameters")) {
        uint32 numberOfPars = database.GetNumberOfChildren();

        //flat view on the parameters
        for (uint32 i = 0u; i < numberOfPars; i++) {
            const char8 * paramName = database.GetChildName(i);
            if (database.MoveRelative(paramName)) {
                StreamString typeName;
                if (!database.Read("type", typeName)) {
                    printf("\nError, undefined type for %s", paramName);
                }
                else {
                    PrintOnFile(objHeader, "\\\n    ");
                    PrintOnFile(objHeader, typeName.Buffer());
                    PrintOnFile(objHeader, " ");
                    PrintOnFile(objHeader, paramName);
                    PrintOnFile(objHeader, ";");
                    if (paramName[0] == '*') {
                        PrintOnFile(objHeader, "\\\n    uint32 NumberOf");
                        PrintOnFile(objHeader, paramName + 1);
                        PrintOnFile(objHeader, ";");
                    }
                    StreamString paramPath;
                    StreamString paramAddress;
                    StreamString attributes;
                    if (!database.Read("attributes", attributes)) {
                        attributes = "";
                    }
                    if (paramName[0] == '*') {
                        if (!database.Read("source", paramAddress)) {
                            paramAddress = "";
                        }
                        ReadTheTypeArray(&paramName[1], typeName.Buffer(), paramAddress.Buffer(), attributes.Buffer(), objSource);
                    }
                    else {
                        if (!database.Read("source", paramAddress)) {
                            paramAddress = paramName;
                        }
                        ReadTheType(paramName, typeName.Buffer(), paramAddress.Buffer(), attributes.Buffer(), objSource);
                    }
                }
                database.MoveToAncestor(1u);
            }
        }
        database.MoveToAncestor(1u);

    }
    PrintOnFile(objSource, "    return ret;\n}\n\n");

}

static void AssignSignalArray(const char8 *signalName,
                              const char8 *signalAlias,
                              const char8 *direction,
                              const char8 *type,
                              BasicFile &objSource) {
    StreamString dir = direction;
    PrintOnFile(objSource, "    if(ret) {\n");
    PrintOnFile(objSource, "        uint32 numberOfSignals = GetNumberOf");
    PrintOnFile(objSource, direction);
    PrintOnFile(objSource, "();\n");
    PrintOnFile(objSource, "        bool ok = true;\n"
                "        numberOf");
    PrintOnFile(objSource, signalName);
    PrintOnFile(objSource, "= 0u;\n");
    PrintOnFile(objSource, "        for(uint32 i=0u; (i<numberOfSignals) && ret && ok; i++){\n"
                "            StreamString signalName = \"");
    PrintOnFile(objSource, signalAlias);
    PrintOnFile(objSource, "\";\n");
    PrintOnFile(objSource, "            signalName.Printf(\"%d\", i);\n");
    PrintOnFile(objSource, "            uint32 signalId;\n"
                "            ok = GetSignalIndex(");
    PrintOnFile(objSource, direction);
    PrintOnFile(objSource, ", signalId, signalName.Buffer());\n");
    PrintOnFile(objSource, "            if(ok) {\n"
                "                numberOf");
    PrintOnFile(objSource, signalName);
    PrintOnFile(objSource, "++;\n");
    PrintOnFile(objSource, "            }\n"
                "        }\n");
    PrintOnFile(objSource, "        ");
    PrintOnFile(objSource, signalName);
    PrintOnFile(objSource, " = new ");
    PrintOnFile(objSource, type);
    PrintOnFile(objSource, "* [numberOf");
    PrintOnFile(objSource, signalName);
    PrintOnFile(objSource, "];\n");
    PrintOnFile(objSource, "        ok = true;\n");
    PrintOnFile(objSource, "        for(uint32 i=0u; (i<numberOfSignals) && ret && ok; i++){\n"
                "            StreamString signalName = \"");
    PrintOnFile(objSource, signalAlias);
    PrintOnFile(objSource, "\";\n");
    PrintOnFile(objSource, "            signalName.Printf(\"%d\", i);\n");
    PrintOnFile(objSource, "            uint32 signalId;\n"
                "            ok = GetSignalIndex(");
    PrintOnFile(objSource, direction);
    PrintOnFile(objSource, ", signalId, signalName.Buffer());\n");
    PrintOnFile(objSource, "            if(ok) {\n"
                "                StreamString typeName;\n"
                "                ret = GetSignalType(");
    PrintOnFile(objSource, direction);
    PrintOnFile(objSource, ", signalId, typeName)\n"
                "                if(ret) {\n"
                "                    ret = typeName == \"");
    PrintOnFile(objSource, type);
    PrintOnFile(objSource, "\";\n"
                "                }\n");
    PrintOnFile(objSource, "                if(ret) {\n"
                "                    ");
    PrintOnFile(objSource, signalName);
    PrintOnFile(objSource, "[i]");
    if (dir == "InputSignals") {
        PrintOnFile(objSource, " = GetInputSignalMemory(signalId);\n");
    }
    else {
        PrintOnFile(objSource, " = GetOutputSignalMemory(signalId);\n");
    }
    PrintOnFile(objSource, "                    ret = (");
    PrintOnFile(objSource, signalName);
    PrintOnFile(objSource, "[i]");
    PrintOnFile(objSource, " != NULL);\n");
    PrintOnFile(objSource, "                }\n");
    PrintOnFile(objSource, "            }\n"
                "        }\n");
    PrintOnFile(objSource, "        if(!ret) {\n"
                "            REPORT_ERROR(ErrorManagement::FatalError, \"Failed loading the signals *");
    PrintOnFile(objSource, signalAlias);
    PrintOnFile(objSource, "\");\n");
    PrintOnFile(objSource, "        }\n");
    PrintOnFile(objSource, "    }\n");
}

static void AssignSignal(const char8 *signalName,
                         const char8 *signalAlias,
                         const char8 *direction,
                         const char8 *type,
                         BasicFile &objSource) {
    StreamString dir = direction;
    PrintOnFile(objSource, "    if(ret) {\n"
                "        uint32 signalId;\n"
                "        ret = GetSignalIndex(");
    PrintOnFile(objSource, direction);
    PrintOnFile(objSource, ", signalId, \"");
    PrintOnFile(objSource, signalAlias);
    PrintOnFile(objSource, "\");\n");

    PrintOnFile(objSource, "        if(ret) {\n"
                "            StreamString typeName;\n"
                "            ret = GetSignalType(");
    PrintOnFile(objSource, direction);
    PrintOnFile(objSource, ", signalId, typeName)\n"
                "            if(ret) {\n"
                "                ret = typeName == \"");
    PrintOnFile(objSource, type);
    PrintOnFile(objSource, "\";\n"
                "            }\n"
                "        }\n");
    PrintOnFile(objSource, "        if(ret) {\n");
    PrintOnFile(objSource, "            ");
    PrintOnFile(objSource, signalName);
    if (dir == "InputSignals") {
        PrintOnFile(objSource, " = GetInputSignalMemory(signalId);\n");
    }
    else {
        PrintOnFile(objSource, " = GetOutputSignalMemory(signalId);\n");
    }
    PrintOnFile(objSource, "            ret = (");
    PrintOnFile(objSource, signalName);
    PrintOnFile(objSource, " != NULL);\n");
    PrintOnFile(objSource, "        }\n");
    PrintOnFile(objSource, "        if(!ret) {\n"
                "            REPORT_ERROR(ErrorManagement::FatalError, \"Failed loading the signal ");
    PrintOnFile(objSource, signalAlias);
    PrintOnFile(objSource, "\");\n");
    PrintOnFile(objSource, "        }\n");
    PrintOnFile(objSource, "    }\n");
}

static void GenerateConfigureFunction(ConfigurationDatabase &database,
                                      BasicFile &objHeader,
                                      BasicFile &objSource,
                                      const char8 * className) {
    PrintOnFile(objSource, "bool ");
    PrintOnFile(objSource, className);
    PrintOnFile(objSource, "::ConfigureToolSignals() {\n");
    PrintOnFile(objSource, "    bool ret = true;\n");

    const char8 * signalsNode[] = { "InputSignals", "OutputSignals", 0 };
    uint32 n = 0u;
    while (signalsNode[n] != NULL) {
        if (database.MoveRelative(signalsNode[n])) {
            uint32 numberOfPars = database.GetNumberOfChildren();

            for (uint32 i = 0u; i < numberOfPars; i++) {
                const char8 * signalName = database.GetChildName(i);
                if (database.MoveRelative(signalName)) {
                    StreamString typeName;
                    if (!database.Read("type", typeName)) {
                        printf("\nError, undefined type for %s", signalName);
                    }
                    else {
                        StreamString signalAlias;
                        if (!database.Read("Source", signalAlias)) {
                            uint32 begin = signalName[0] == '*';
                            signalAlias = signalName + begin;
                        }
                        PrintOnFile(objHeader, "\\\n    ");
                        PrintOnFile(objHeader, typeName.Buffer());
                        PrintOnFile(objHeader, " *");
                        PrintOnFile(objHeader, signalName);
                        PrintOnFile(objHeader, ";");

                        if (signalName[0] == '*') {
                            PrintOnFile(objHeader, "\\\n    uint32 NumberOf");
                            PrintOnFile(objHeader, signalName + 1);
                            PrintOnFile(objHeader, ";");
                            AssignSignalArray(signalName + 1, signalAlias.Buffer(), signalsNode[n], typeName.Buffer(), objSource);
                        }
                        else {
                            AssignSignal(signalName, signalAlias.Buffer(), signalsNode[n], typeName.Buffer(), objSource);
                        }
                    }
                    database.MoveToAncestor(1u);
                }
            }
            database.MoveToAncestor(1u);
        }
        n++;
    }
    PrintOnFile(objSource, "    return ret;\n");
    PrintOnFile(objSource, "}\n\n");

}

static void GenerateObjFile(ConfigurationDatabase &database,
                            BasicFile &objHeader,
                            BasicFile &objSource,
                            const char8 * className) {

    //generate the .cpp and .h files with all the object configuration
    //implementing the Object::Initialise() function




    PrintOnFile(objHeader, "#define TOOL_METHODS_DECLARATION() \\ \n");
    bool isSignal = true;
    if (!database.MoveRelative("InputSignals")) {
        if (!database.MoveRelative("InputSignals")) {
            isSignal = false;
        }
    }
    if (isSignal) {
        PrintOnFile(objHeader, "    virtual bool ConfigureToolSignals();\\ \n");
        database.MoveToAncestor(1u);
    }


    PrintOnFile(objHeader, "    virtual bool ToolMembersConstructor();\\ \n");
    PrintOnFile(objHeader, "    virtual bool ToolMembersDestructor(); \\ \n");

    PrintOnFile(objSource, "#include \"");
    PrintOnFile(objSource, className);
    PrintOnFile(objSource, ".h\"\n");
    PrintOnFile(objSource, "namespace MARTe {\n\n");

    // Constructor and destructor
    PrintOnFile(objSource, "void ");
    PrintOnFile(objSource, className);
    PrintOnFile(objSource, "::");
    PrintOnFile(objSource, "ToolMembersConstructor");
    PrintOnFile(objSource, "() {\n");

    uint32 n = 0u;
    while (nodeNames[n] != NULL) {
        if (database.MoveRelative(nodeNames[n])) {
            uint32 numberOfPars = database.GetNumberOfChildren();

            for (uint32 i = 0u; i < numberOfPars; i++) {
                const char8 * paramName = database.GetChildName(i);
                if (database.MoveRelative(paramName)) {
                    if (paramName[0] == '*') {
                        PrintOnFile(objSource, "    ");
                        PrintOnFile(objSource, &paramName[1]);
                        PrintOnFile(objSource, " = NULL;\n");
                        PrintOnFile(objSource, "    numberOf");
                        PrintOnFile(objSource, &paramName[1]);
                        PrintOnFile(objSource, " = 0u;\n");
                    }
                    database.MoveToAncestor(1u);
                }
            }
            database.MoveToAncestor(1u);
        }
        n++;
    }

    PrintOnFile(objSource, "}\n\n");

    PrintOnFile(objSource, "void ");
    PrintOnFile(objSource, className);
    PrintOnFile(objSource, "::");
    PrintOnFile(objSource, "ToolMembersDestructor");
    PrintOnFile(objSource, "() {\n");
    n = 0u;
    while (nodeNames[n] != NULL) {
        if (database.MoveRelative(nodeNames[n])) {
            uint32 numberOfPars = database.GetNumberOfChildren();

            for (uint32 i = 0u; i < numberOfPars; i++) {
                const char8 * paramName = database.GetChildName(i);
                if (database.MoveRelative(paramName)) {
                    if (paramName[0] == '*') {
                        PrintOnFile(objSource, "    if(");
                        PrintOnFile(objSource, &paramName[1]);
                        PrintOnFile(objSource, " != NULL) {\n"
                                    "        delete [] ");
                        PrintOnFile(objSource, &paramName[1]);
                        PrintOnFile(objSource, ";\n"
                                    "    }\n");
                    }
                    database.MoveToAncestor(1u);
                }
            }
            database.MoveToAncestor(1u);
        }
        n++;
    }
    PrintOnFile(objSource, "}\n\n");


    //Populates the Initialise function in the cpp file
    GenerateInitialiseFunction(database, objHeader, objSource, className);
    //Populates the Setup function in the cpp file
    GenerateConfigureFunction(database, objHeader, objSource, className);
    PrintOnFile(objSource, "}\n\n");


}

static void GenerateOutputFiles(ConfigurationDatabase &database) {
// output files
    BasicFile structHeader;
    BasicFile structSource;

    StreamString className;
    if (!database.Read("Class", className)) {
        className = "Object";
    }

    StreamString classStructName = className;
    classStructName += "_Params";

    StreamString structHeaderName = classStructName;
    structHeaderName += ".h";

// open the .h output file
    if (!structHeader.Open(structHeaderName.Buffer(),
                           BasicFile::FLAG_APPEND | BasicFile::FLAG_TRUNC | BasicFile::FLAG_CREAT | BasicFile::ACCESS_MODE_R | BasicFile::ACCESS_MODE_W)) {
        printf("\nUnable to open the output header file");
        return;
    }

    StreamString structSourceName = classStructName;
    structSourceName += ".cpp";

// open the .cpp output file
    if (!structSource.Open(structSourceName.Buffer(),
                           BasicFile::FLAG_APPEND | BasicFile::FLAG_TRUNC | BasicFile::FLAG_CREAT | BasicFile::ACCESS_MODE_R | BasicFile::ACCESS_MODE_W)) {
        printf("\nUnable to open the output source file");
        return;
    }

// print the header in the header file
    PrintOnFile(structHeader, "#include \"GeneralDefinitions.h\"\n\n");
    PrintOnFile(structHeader, "namespace MARTe{ \n\n");

// print the header in the source file
    PrintOnFile(structSource, "#include \"Object.h\"\n#include \"ClassRegistryDatabase.h\"\n");
    PrintOnFile(structSource, "#include \"Introspection.h\"\n#include \"");
    PrintOnFile(structSource, className.Buffer());
    PrintOnFile(structSource, ".h\"\n\n");
    PrintOnFile(structSource, "namespace MARTe{ \n\n");

    BasicFile objHeader;
    StreamString objheaderName = className;
    objheaderName += "_macros.h";
// open the .h gam file
    if (!objHeader.Open(objheaderName.Buffer(),
                        BasicFile::FLAG_APPEND | BasicFile::FLAG_TRUNC | BasicFile::FLAG_CREAT | BasicFile::ACCESS_MODE_R | BasicFile::ACCESS_MODE_W)) {
        printf("\nUnable to open the gam header file");
        return;
    }

    BasicFile objSource;
    StreamString objSourceName = className;
    objSourceName += "_aux.cpp";
// open the .cpp gam file
    if (!objSource.Open(objSourceName.Buffer(),
                        BasicFile::FLAG_APPEND | BasicFile::FLAG_TRUNC | BasicFile::FLAG_CREAT | BasicFile::ACCESS_MODE_R | BasicFile::ACCESS_MODE_W)) {
        printf("\nUnable to open the gam source file");
        return;
    }

    GenerateObjFile(database, objHeader, objSource, className.Buffer());

    uint32 n = 0u;
    while (nodeNames[n] != NULL) {
        if (database.MoveRelative(nodeNames[n])) {
            for (uint32 i = 0u; i < database.GetNumberOfChildren(); i++) {
                const char8* structName = database.GetChildName(i);

                if (database.MoveRelative(structName)) {
                    GenerateStructFileRecursive(database, structHeader, structSource);
                    database.MoveToAncestor(1u);
                }
            }
            database.MoveToAncestor(1u);
        }
        n++;
    }
    PrintOnFile(structHeader, "\n}");
    PrintOnFile(structSource, "\n}");
}

int main(int argc,
         char** argv) {

// the parser
    ParserI *myParser = NULL;

// the database created by the parser
    ConfigurationDatabase database;

// the parser errors
    StreamString errors;

// configuration input file
    BasicFile configFile;

    bool canReturn = false;

    if (argc < 2) {
        printf("\nPlease specify cfg name and output file name");
        canReturn = true;
    }

    if (!canReturn) {

        // open the input file
        if (!configFile.Open((const char*) argv[1], BasicFile::ACCESS_MODE_R)) {
            printf("\nUnable to open the input configuration file");
            canReturn = true;
        }
    }

    if (!canReturn) {
        configFile.Seek(0);
        if (argc > 3) {
            if ((argv[2])[0] == '1') {
                myParser = new StandardParser(configFile, database, &errors);
            }
            else if ((argv[2])[0] == '2') {
                myParser = new XMLParser(configFile, database, &errors);
            }
            else if ((argv[2])[0] == '3') {
                myParser = new JsonParser(configFile, database, &errors);
            }
            else {
                myParser = new StandardParser(configFile, database, &errors);
            }
        }
        // standard parser by default
        else {
            myParser = new StandardParser(configFile, database, &errors);
        }

        // parse the cfg
        if (!myParser->Parse()) {
            printf("\nError Parse(*) %s", errors.Buffer());
            canReturn = true;
        }
    }

    if (!canReturn) {
        GenerateOutputFiles(database);
    }
    if (myParser != NULL) {
        delete myParser;
    }

    printf("\n");
    return 0;

}
