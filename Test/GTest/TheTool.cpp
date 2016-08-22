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

static void IntrospectionOnHeader(const char8 *memberName,
                                  StreamString &modifiers,
                                  StreamString &type,
                                  StreamString &attributes,
                                  StreamString &comments,
                                  BasicFile &structHeader,
                                  bool isMacro = false) {
    // print the attributes as comment before the member on the header file
    if (attributes.Size() > 0u) {
        PrintOnFile(structHeader, "    ");
        PrintOnFile(structHeader, "/** ");
        PrintOnFile(structHeader, attributes.Buffer());
        PrintOnFile(structHeader, " */");
        if (isMacro) {
            PrintOnFile(structHeader, "\\");
        }
        PrintOnFile(structHeader, "\n");
    }

    // print the comments before the member on the header file
    if (comments.Size() > 0u) {
        PrintOnFile(structHeader, "    ");
        PrintOnFile(structHeader, "/** ");
        PrintOnFile(structHeader, comments.Buffer());
        PrintOnFile(structHeader, " */");
        if (isMacro) {
            PrintOnFile(structHeader, "\\");
        }
        PrintOnFile(structHeader, "\n");
    }

    // print the type
    PrintOnFile(structHeader, "    ");

    PrintOnFile(structHeader, type.Buffer());
    PrintOnFile(structHeader, " ");
    StreamString modifiersString;

    uint32 nextIndex = GetModifierString(modifiers.BufferReference(), modifiersString);
    // print the modifiers
    PrintOnFile(structHeader, modifiersString.Buffer());

    // print the member name
    PrintOnFile(structHeader, memberName);
    while (modifiers[nextIndex] != '\0') {
        PrintOnFile(structHeader, modifiers[nextIndex]);
        nextIndex++;
    }
    PrintOnFile(structHeader, ";");

}

static void IntrospectionOnSourceBefore(const char8 *memberName,
                                        StreamString &modifiers,
                                        StreamString &structType,
                                        StreamString &type,
                                        StreamString &attributes,
                                        BasicFile &structSource,
                                        bool isParent = false) {
    if (isParent) {
        PrintOnFile(structSource, "DECLARE_CLASS_PARENT(");
    }
    else {
        PrintOnFile(structSource, "DECLARE_CLASS_MEMBER(");
    }
    PrintOnFile(structSource, structType.Buffer());
    PrintOnFile(structSource, ", ");
    PrintOnFile(structSource, memberName);
    PrintOnFile(structSource, ", ");
    if (!isParent) {
        PrintOnFile(structSource, type.Buffer());
        PrintOnFile(structSource, ", ");
    }
    PrintOnFile(structSource, "\"");
    PrintOnFile(structSource, modifiers.Buffer());
    PrintOnFile(structSource, "\"");
    PrintOnFile(structSource, ", ");
    PrintOnFile(structSource, "\"");
    PrintOnFile(structSource, attributes.Buffer());
    PrintOnFile(structSource, "\"");

    PrintOnFile(structSource, ");\n");

}

static void IntrospectionOnSourceAfter(ConfigurationDatabase &database,
                                       StreamString &structType,
                                       BasicFile &structSource,
                                       bool isClass = false) {

    PrintOnFile(structSource, "\nstatic const IntrospectionEntry * ");
    PrintOnFile(structSource, structType.Buffer());
    PrintOnFile(structSource, "_array[] = {\n");

    for (uint32 j = 0u; j < database.GetNumberOfChildren(); j++) {
        const char8 *memberName = database.GetChildName(j);
        if (database.MoveRelative(memberName)) {
            if (memberName[0] != '*') {
                PrintOnFile(structSource, "&");
                PrintOnFile(structSource, structType.Buffer());
                PrintOnFile(structSource, "_");
                PrintOnFile(structSource, &memberName[memberName[0] == '$']);
                PrintOnFile(structSource, "_introspectionEntry, \n");
            }
            database.MoveToAncestor(1u);
        }
    }
    PrintOnFile(structSource, "0 };\n\n");

    // declare the class introspection
    if (isClass) {
        PrintOnFile(structSource, "DECLARE_CLASS_INTROSPECTION(");
    }
    else {
        PrintOnFile(structSource, "DECLARE_STRUCT_INTROSPECTION(");
    }
    PrintOnFile(structSource, structType.Buffer());
    PrintOnFile(structSource, ", ");
    PrintOnFile(structSource, structType.Buffer());
    PrintOnFile(structSource, "_array);\n");

}

static void ReadIntrospectionAttributes(ConfigurationDatabase &database,
                                        StreamString &modifiers,
                                        StreamString &type,
                                        StreamString &attributes,
                                        StreamString &comments,
                                        const char8* structName,
                                        const char8* memberName) {

    // print the attributes as comment before the member on the header file
    if (!database.Read("attributes", attributes)) {
        printf("\n[%s.%s][attributes] lacks: the attributes is considered as an empty string", structName, memberName);
        attributes = "";
    }

    // print the comments before the member on the header file
    if (!database.Read("comments", comments)) {
        printf("\n[%s.%s][comments] lacks: the comments is considered as an empty string", structName, memberName);
        comments = "";
    }

    if (!database.Read("type", type)) {
        printf("\n[%s.%s][type] lacks: the member type is considered void", structName, memberName);
        type = "void";
    }

    if (!database.Read("modifiers", modifiers)) {
        printf("\n[%s.%s][modifiers] lacks: the modifiers is considered as an empty string", structName, memberName);
        modifiers = "";
    }

}

static void PrintIntrospection(ConfigurationDatabase &database,
                               const char8 * structName,
                               BasicFile &structHeader,
                               BasicFile &structSource) {
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
                StreamString type;
                StreamString attributes;
                StreamString comments;

                ReadIntrospectionAttributes(database, modifiers, type, attributes, comments, structName, memberName);
                IntrospectionOnHeader(memberName, modifiers, type, attributes, comments, structHeader);
                PrintOnFile(structHeader, "\n");

                // SOURCE FILE MANAGEMENT
                // declare the member introspection
                IntrospectionOnSourceBefore(memberName, modifiers, structType, type, attributes, structSource);

                database.MoveToAncestor(1);
            }
        }

        IntrospectionOnSourceAfter(database, structType, structSource);

        PrintOnFile(structHeader, "}; \n\n");
    }
    else {
        printf("\nError, undefined type for %s", structName);
    }
}

#if 0
static void GenerateParentsIntrospectionNode(ConfigurationDatabase &database,
        BasicFile &structSource,
        const char8 *className) {
    uint32 numberOfParents = database.GetNumberOfChildren();
    StreamString classNameStr = className;

    //flat view on the parameters
    for (uint32 i = 0u; i < numberOfParents; i++) {
        const char8 * parentName = database.GetChildName(i);
        if (database.MoveRelative(parentName)) {

            StreamString modifiers;
            StreamString type;
            StreamString attributes;
            StreamString comments;

            ReadIntrospectionAttributes(database, modifiers, type, attributes, comments, className, parentName);

            if (type.Size() == 0u) {
                type = parentName;
            }

            //Manage the parent introspection

            // SOURCE FILE MANAGEMENT
            // declare the member introspection
            IntrospectionOnSourceBefore(parentName, modifiers, classNameStr, type, attributes, structSource, true);
            database.MoveToAncestor(1u);
        }
    }
    IntrospectionOnSourceAfter(database, classNameStr, structSource);

}

static void GenerateParentsIntrospectionVector(ConfigurationDatabase &database,
        BasicFile &structSource,
        const char8 *className) {
    uint32 numberOfParents = database.GetNumberOfElements("Parents", 0u);
    Vector<StreamString> parentNames(numberOfParents);
    StreamString classNameStr = className;

    if (database.Read("Parents", parentNames)) {

        //flat view on the parameters
        for (uint32 i = 0u; i < numberOfParents; i++) {

            StreamString modifiers;
            StreamString type;
            StreamString attributes;
            StreamString comments;

            type = parentNames[i];

            //Manage the parent introspection

            // SOURCE FILE MANAGEMENT
            // declare the member introspection
            IntrospectionOnSourceBefore(parentNames[i].Buffer(), modifiers, classNameStr, type, attributes, structSource, true);

        }
    }
    else {
        printf("No Parents Introspection declared");
    }
    IntrospectionOnSourceAfter(database, classNameStr, structSource);

}

static void GenerateParentsIntrospection(ConfigurationDatabase &database,
        BasicFile &structSource,
        const char8 * className) {

    if (database.MoveRelative("Parents")) {
        GenerateParentsIntrospectionNode(database, structSource, className);
        database.MoveToAncestor(1u);
    }
    else {
        GenerateParentsIntrospectionVector(database, structSource, className);
    }

}
#endif

static void GenerateStructFile(ConfigurationDatabase &database,
                               BasicFile &structHeader,
                               BasicFile &structSource) {

    const char8* structName = database.GetName();
    bool isStruct = false;
    for (uint32 j = 0u; j < database.GetNumberOfChildren(); j++) {

        // HEADER FILE MANAGEMENT
        const char8 *memberName = database.GetChildName(j);
        if (database.MoveRelative(memberName)) {
            isStruct = true;
            GenerateStructFile(database, structHeader, structSource);
            database.MoveToAncestor(1u);
        }
    }
    if (isStruct) {
        PrintIntrospection(database, structName, structHeader, structSource);
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
        PrintOnFile(objSource, "\",\"");
        PrintOnFile(objSource, attributes);
        PrintOnFile(objSource, "\", ");
        PrintOnFile(objSource, typeName);
        PrintOnFile(objSource, "_at);\n");
    }
    else {
        PrintOnFile(objSource, "        ret = data.AdvancedRead(\"");
        PrintOnFile(objSource, paramAddress);
        PrintOnFile(objSource, "\", \"");
        PrintOnFile(objSource, attributes);
        PrintOnFile(objSource, "\", ");
        PrintOnFile(objSource, paramName);
        PrintOnFile(objSource, ");\n");

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

    //TODO calculate the alias here
    StreamString paramAddressStr = paramAddress;
    paramAddressStr.Seek(0ull);
    char8 terminator;
    StreamString paramAlias;
    StreamString paramPath;

    while (paramAddressStr.GetToken(paramAlias, ".", terminator)) {
        if (terminator == '.') {
            if (paramPath.Size() > 0u) {
                paramPath += ".";
            }
            paramPath += paramAlias;
            paramAlias = "";
        }
    }

    PrintOnFile(objSource, "    if(ret) {\n");
    PrintOnFile(objSource, "        StreamString currentPath;\n"
                "        data.GetFullPath(currentPath);\n");

    if (paramPath.Size() > 0u) {
        PrintOnFile(objSource, "        data.AdvancedMove(\"");
        PrintOnFile(objSource, paramPath.Buffer());
        PrintOnFile(objSource, "\");\n");
    }

    PrintOnFile(objSource, "        numberOf");
    PrintOnFile(objSource, paramName);
    PrintOnFile(objSource, " = 0u;\n");
    PrintOnFile(objSource, "        for(uint32 i=0u; i < data.GetNumberOfChildren(); i++){\n"
                "            StreamString childName = \"");
    PrintOnFile(objSource, paramAlias.Buffer());
    PrintOnFile(objSource, "\";\n");
    PrintOnFile(objSource, "            childName.Printf(\"%d\", i);\n");
    bool isStructType = (TypeDescriptor::GetTypeDescriptorFromTypeName(typeName) == InvalidType);
    if (isStructType) {
        PrintOnFile(objSource, "            if(data.MoveRelative(childName.Buffer())){\n"
                    "                StreamString type;\n"
                    "                if(data.Read(\"Class\", type)){\n"
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
        PrintOnFile(objSource, "            AnyType at=data.GetType(childName.Buffer());\n"
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
    PrintOnFile(objSource, "        for(uint32 i=0u; i < data.GetNumberOfChildren() && ret; i++){\n"
                "            StreamString childName = \"");
    PrintOnFile(objSource, paramAlias.Buffer());
    PrintOnFile(objSource, "\";\n");
    PrintOnFile(objSource, "            childName.Printf(\"%d\", i);\n");
    if (isStructType) {
        PrintOnFile(objSource, "            if(data.MoveRelative(childName.Buffer())){\n"
                    "                StreamString type;\n"
                    "                if(data.Read(\"Class\", type)){\n"
                    "                    if(type == \"");
        PrintOnFile(objSource, typeName);
        PrintOnFile(objSource, "\"){\n"
                    "                        ANY_TYPE_STRUCT_BUILDER(");
        PrintOnFile(objSource, typeName);
        PrintOnFile(objSource, ", ");
        PrintOnFile(objSource, paramName);
        PrintOnFile(objSource, "[i]);\n"
                    "                        data.MoveToAncestor(1u);\n"
                    "                        ret = data.Read(childName.Buffer(), ");
        PrintOnFile(objSource, typeName);
        PrintOnFile(objSource, "_at);\n"
                    "                        data.MoveRelative(childName.Buffer());\n"
                    "                    }\n"
                    "                }\n"
                    "                data.MoveToAncestor(1u);\n"
                    "            }\n");
    }
    else {
        PrintOnFile(objSource, "            AnyType at=data.GetType(childName.Buffer());\n"
                    "            if(at.GetTypeDescriptor() == TypeDescriptor::GetTypeDescriptorFromTypeName(\"");
        PrintOnFile(objSource, typeName);
        PrintOnFile(objSource, "\")) {\n"
                    "                ret = data.AdvancedRead(childName.Buffer(), \"");
        PrintOnFile(objSource, attributes);
        PrintOnFile(objSource, "\", ");
        PrintOnFile(objSource, paramName);
        PrintOnFile(objSource, ");\n");
        PrintOnFile(objSource, "            }\n");

    }
    PrintOnFile(objSource, "        }\n"
                "        data.MoveAbsolute(currentPath.Buffer());\n");
    PrintOnFile(objSource, "        if(!ret) {\n"
                "            REPORT_ERROR(ErrorManagement::FatalError, \"Failed loading the parameters *");
    if (paramPath.Size() > 0u) {
        PrintOnFile(objSource, paramPath.Buffer());
        PrintOnFile(objSource, ".");
    }
    PrintOnFile(objSource, paramAlias.Buffer());
    PrintOnFile(objSource, "\");\n");
    PrintOnFile(objSource, "        }\n");
    PrintOnFile(objSource, "    }\n");
}

static void GenerateInitialiseFunction(ConfigurationDatabase &database,
                                       BasicFile &objHeader,
                                       BasicFile &objSource,
                                       BasicFile &structSource,
                                       const char8 * className) {
    StreamString classNameStr = className;

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

                StreamString modifiers;
                StreamString type;
                StreamString attributes;
                StreamString comments;

                ReadIntrospectionAttributes(database, modifiers, type, attributes, comments, className, paramName);

                if (paramName[0] != '$') {
                    if (type.Size() == 0u) {
                        printf("\nError, undefined type for %s", paramName);
                    }
                    else {
                        PrintOnFile(objHeader, "\\\n");
                        IntrospectionOnHeader(paramName, modifiers, type, attributes, comments, objHeader, true);

                        if (paramName[0] == '*') {
                            PrintOnFile(objHeader, "\\\n    uint32 numberOf");
                            PrintOnFile(objHeader, paramName + 1);
                            PrintOnFile(objHeader, ";");
                        }
                        StreamString paramPath;
                        StreamString paramAddress;
                        if (paramName[0] == '*') {
                            if (!database.Read("source", paramAddress)) {
                                paramAddress = "";
                            }
                            ReadTheTypeArray(&paramName[1], type.Buffer(), paramAddress.Buffer(), attributes.Buffer(), objSource);
                        }
                        else {
                            if (!database.Read("source", paramAddress)) {
                                paramAddress = paramName;
                            }
                            ReadTheType(paramName, type.Buffer(), paramAddress.Buffer(), attributes.Buffer(), objSource);
                        }
                    }
                }
                //Manage the parameter introspection

                if (paramName[0] != '*') {
                    uint32 begin = paramName[0] == '$';
                    // SOURCE FILE MANAGEMENT
                    // declare the member introspection
                    IntrospectionOnSourceBefore(&paramName[begin], modifiers, classNameStr, type, attributes, structSource, begin);
                }
                database.MoveToAncestor(1u);
            }
        }
        IntrospectionOnSourceAfter(database, classNameStr, structSource, true);

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
    PrintOnFile(objSource, ", signalId, signalName.Buffer()) > 0;\n");
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
                "            int32 level = GetSignalIndex(");
    PrintOnFile(objSource, direction);
    PrintOnFile(objSource, ", signalId, signalName.Buffer());\n");
    PrintOnFile(objSource, "            ret = (level > 0);\n");

    PrintOnFile(objSource, "            if(ret) {\n"
                "                StreamString typeName;\n"
                "                ret = GetSignalType(");
    PrintOnFile(objSource, direction);
    PrintOnFile(objSource, ", signalId, typeName, level);\n"
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
        PrintOnFile(objSource, " = (");
        PrintOnFile(objSource, type);
        PrintOnFile(objSource, " *) ");
        PrintOnFile(objSource, "GetInputSignalMemory(signalId);\n");
    }
    else {
        PrintOnFile(objSource, " = (");
        PrintOnFile(objSource, type);
        PrintOnFile(objSource, " *) ");
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
                "        int32 level = GetSignalIndex(");
    PrintOnFile(objSource, direction);
    PrintOnFile(objSource, ", signalId, \"");
    PrintOnFile(objSource, signalAlias);
    PrintOnFile(objSource, "\");\n");
    PrintOnFile(objSource, "        ret = (level > 0);\n");

    PrintOnFile(objSource, "        if(ret) {\n"
                "            StreamString typeName;\n"
                "            ret = GetSignalType(");
    PrintOnFile(objSource, direction);
    PrintOnFile(objSource, ", signalId, typeName, level);\n"
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
                            PrintOnFile(objHeader, "\\\n    uint32 numberOf");
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
                            BasicFile &structSource,
                            const char8 * className) {

    //generate the .cpp and .h files with all the object configuration
    //implementing the Object::Initialise() function

    PrintOnFile(objHeader, "#define TOOL_METHODS_DECLARATION() \\\n");
    bool isSignal = true;
    if (!database.MoveRelative("InputSignals")) {
        if (!database.MoveRelative("InputSignals")) {
            isSignal = false;
        }
    }
    if (isSignal) {
        PrintOnFile(objHeader, "    virtual bool ConfigureToolSignals();\\\n");
        database.MoveToAncestor(1u);
    }

    PrintOnFile(objHeader, "    virtual void ToolMembersConstructor();\\\n");
    PrintOnFile(objHeader, "    virtual void ToolMembersDestructor(); \\\n");

    PrintOnFile(objSource, "#include \"");
    PrintOnFile(objSource, className);
    PrintOnFile(objSource, ".h\"\n\n");

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
                    bool initAsNull = StringHelper::Compare(nodeNames[n], "Parameters") != 0;
                    if (!initAsNull) {
                        initAsNull = (paramName[0] == '*');
                    }
                    if (initAsNull) {
                        PrintOnFile(objSource, "    ");
                        PrintOnFile(objSource, &paramName[paramName[0] == '*']);
                        PrintOnFile(objSource, " = NULL;\n");
                    }
                    if (paramName[0] == '*') {
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
    GenerateInitialiseFunction(database, objHeader, objSource, structSource, className);
    //Populates the Setup function in the cpp file
    GenerateConfigureFunction(database, objHeader, objSource, className);

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
    classStructName += "_params";

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
    PrintOnFile(structSource, "#include \"IntrospectionT.h\"\n#include \"");
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

    GenerateObjFile(database, objHeader, objSource, structSource, className.Buffer());

    uint32 n = 0u;
    while (nodeNames[n] != NULL) {
        if (database.MoveRelative(nodeNames[n])) {
            for (uint32 i = 0u; i < database.GetNumberOfChildren(); i++) {
                const char8* structName = database.GetChildName(i);

                if (database.MoveRelative(structName)) {
                    GenerateStructFile(database, structHeader, structSource);
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
