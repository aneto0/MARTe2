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

static void GenerateOutputFileRecursive(ConfigurationDatabase &database,
                                        BasicFile &headerFile,
                                        BasicFile &sourceFile) {

    const char8* structName = database.GetName();

    for (uint32 j = 0u; j < database.GetNumberOfChildren(); j++) {

        // HEADER FILE MANAGEMENT
        const char8 *memberName = database.GetChildName(j);
        if (database.MoveRelative(memberName)) {

            for (uint32 k = 0u; k < database.GetNumberOfChildren(); k++) {
                const char8 * subStructName = database.GetChildName(k);

                if (database.MoveRelative(subStructName)) {
                    GenerateOutputFileRecursive(database, headerFile, sourceFile);
                    database.MoveToAncestor(1u);
                }
            }
            database.MoveToAncestor(1u);
        }
    }

    StreamString structType;
    if (database.Read("type", structType)) {

        PrintOnFile(headerFile, "struct ");
        PrintOnFile(headerFile, structType.Buffer());
        PrintOnFile(headerFile, " { \n");

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
                    PrintOnFile(headerFile, "    ");
                    PrintOnFile(headerFile, "/** ");
                    PrintOnFile(headerFile, attributes.Buffer());
                    PrintOnFile(headerFile, " */\n");
                }

                // print the comments before the member on the header file
                if (!database.Read("comments", comments)) {
                    printf("\n[%s.%s][comments] lacks: the comments is considered as an empty string", structName, memberName);
                    comments = "";
                }
                else {
                    PrintOnFile(headerFile, "    ");
                    PrintOnFile(headerFile, "/** ");
                    PrintOnFile(headerFile, comments.Buffer());
                    PrintOnFile(headerFile, " */\n");
                }

                // print the type
                PrintOnFile(headerFile, "    ");
                if (!database.Read("type", type)) {
                    printf("\n[%s.%s][type] lacks: the member type is considered void", structName, memberName);
                    type = "void";
                }

                PrintOnFile(headerFile, type.Buffer());
                PrintOnFile(headerFile, " ");

                if (!database.Read("modifiers", modifiers)) {
                    printf("\n[%s.%s][modifiers] lacks: the modifiers is considered as an empty string", structName, memberName);
                    modifiers = "";
                }

                uint32 nextIndex = GetModifierString(modifiers.BufferReference(), modifiersString);
                // print the modifiers
                PrintOnFile(headerFile, modifiersString.Buffer());

                // print the member name
                PrintOnFile(headerFile, memberName);
                while (modifiers[nextIndex] != '\0') {
                    PrintOnFile(headerFile, modifiers[nextIndex]);
                    nextIndex++;
                }
                PrintOnFile(headerFile, ";\n");

                // SOURCE FILE MANAGEMENT
                // declare the member introspection
                PrintOnFile(sourceFile, "DECLARE_CLASS_MEMBER(");
                PrintOnFile(sourceFile, structType.Buffer());
                PrintOnFile(sourceFile, ", ");
                PrintOnFile(sourceFile, memberName);
                PrintOnFile(sourceFile, ", ");
                PrintOnFile(sourceFile, type.Buffer());
                PrintOnFile(sourceFile, ", ");
                PrintOnFile(sourceFile, "\"");
                PrintOnFile(sourceFile, modifiers.Buffer());
                PrintOnFile(sourceFile, "\"");
                PrintOnFile(sourceFile, ", ");
                PrintOnFile(sourceFile, "\"");
                PrintOnFile(sourceFile, attributes.Buffer());
                PrintOnFile(sourceFile, "\"");

                PrintOnFile(sourceFile, ");\n");

                database.MoveToAncestor(1);
            }
        }
        PrintOnFile(sourceFile, "\n");
        PrintOnFile(headerFile, "}; \n\n");

        PrintOnFile(sourceFile, "static const IntrospectionEntry * ");
        PrintOnFile(sourceFile, structType.Buffer());
        PrintOnFile(sourceFile, "_array[] = {\n");

        for (uint32 j = 0u; j < database.GetNumberOfChildren(); j++) {
            const char8 *memberName = database.GetChildName(j);
            if (database.MoveRelative(memberName)) {
                PrintOnFile(sourceFile, "&");
                PrintOnFile(sourceFile, structType.Buffer());
                PrintOnFile(sourceFile, "_");
                PrintOnFile(sourceFile, memberName);
                PrintOnFile(sourceFile, "_introspectionEntry, \n");
                database.MoveToAncestor(1u);
            }
        }
        PrintOnFile(sourceFile, "0 };\n\n");

        // declare the class introspection
        PrintOnFile(sourceFile, "DECLARE_CLASS_INTROSPECTION(");
        PrintOnFile(sourceFile, structType.Buffer());
        PrintOnFile(sourceFile, ", ");
        PrintOnFile(sourceFile, structType.Buffer());
        PrintOnFile(sourceFile, "_array);\n");
        PrintOnFile(sourceFile, "INTROSPECTION_CLASS_REGISTER(");
        PrintOnFile(sourceFile, structType.Buffer());
        PrintOnFile(sourceFile, ", ");
        PrintOnFile(sourceFile, "\"1.0\", ");
        PrintOnFile(sourceFile, structType.Buffer());
        PrintOnFile(sourceFile, "_introspection)\n\n");
    }
    else {
        printf("\nError, undefined type for %s", structName);
    }
}

static void ReadTheType(const char8 *paramName,
                        const char8 *paramAddress,
                        const char8 *paramAlias,
                        BasicFile &objSource) {

    if (StringHelper::Length(paramAddress) > 0u) {
        PrintOnFile(objSource, "    localData.MoveAbsolute(");
        PrintOnFile(objSource, paramAddress);
        PrintOnFile(objSource, ")\n");
    }
    else {
        PrintOnFile(objSource, "    localData.MoveToRoot();\n");
    }

    PrintOnFile(objSource, "    if(ret) {\n");
    PrintOnFile(objSource, "        ret = localData.Read(\"");
    PrintOnFile(objSource, paramAlias);
    PrintOnFile(objSource, "\",");
    PrintOnFile(objSource, paramName);
    PrintOnFile(objSource, ");\n");
    PrintOnFile(objSource, "    }\n");

}

static void ReadTheTypeArray(const char8 *paramName,
                             const char8 *typeName,
                             const char8 *paramAddress,
                             BasicFile &objSource) {

    if (StringHelper::Length(paramAddress) > 0u) {
        PrintOnFile(objSource, "    localData.MoveAbsolute(");
        PrintOnFile(objSource, paramAddress);
        PrintOnFile(objSource, ")\n");
    }
    else {
        PrintOnFile(objSource, "    localData.MoveToRoot();\n");
    }
    PrintOnFile(objSource, "    if(ret) {\n");
    PrintOnFile(objSource, "        numberOf");
    PrintOnFile(objSource, paramName);
    PrintOnFile(objSource, " = 0u;\n");
    PrintOnFile(objSource, "        for(uint32 i=0u; i < localData.Size(); i++){\n"
                "            if(data.MoveRelative(localData.GetChildName(i)){\n"
                "                StreamString type;\n"
                "                if(data.Read(\"Class\", type){\n"
                "                    if(type == \"");
    PrintOnFile(objSource, typeName);
    PrintOnFile(objSource, "\"){\n"
                "                        numberOf");
    PrintOnFile(objSource, paramName);
    PrintOnFile(objSource, "++;\n");
    PrintOnFile(objSource, "                    }\n"
                "                 }\n"
                "                 data.MoveToAncestor(1u);\n"
                "             }\n"
                "         }\n        ");
    PrintOnFile(objSource, paramName);
    PrintOnFile(objSource, " = new ");
    PrintOnFile(objSource, typeName);
    PrintOnFile(objSource, "[numberOf");
    PrintOnFile(objSource, paramName);
    PrintOnFile(objSource, "];\n");
    PrintOnFile(objSource, "        for(uint32 i=0u; i < localData.Size() && ret; i++){\n"
                "            if(data.MoveRelative(localData.GetChildName(i)){\n"
                "                StreamString type;\n"
                "                if(localData.Read(\"Class\", type){\n"
                "                    if(type == \"");
    PrintOnFile(objSource, typeName);
    PrintOnFile(objSource, "\"){\n"
                "                        ret = localData.Read(localData.GetChildName(i), ");
    PrintOnFile(objSource, paramName);
    PrintOnFile(objSource, "[i]);\n"
                "                    }\n"
                "                }\n"
                "             }\n"
                "         }\n"
                "     }\n");
}

static void GenerateInitialiseFunction(ConfigurationDatabase &database,
                                       BasicFile &objHeader,
                                       BasicFile &objSource,
                                       const char8 * className) {

    PrintOnFile(objHeader, "    virtual bool Initialise(StructuredDataI &data);\n\n");
    PrintOnFile(objSource, "bool ");
    PrintOnFile(objSource, className);
    PrintOnFile(objSource, "::");
    PrintOnFile(objSource, "Initialise(StructuredDataI &data){\n");
    PrintOnFile(objSource, "    bool ret = ReferenceContainer::Initialise(data);\n"
                "    ConfigurationDatabase localData;\n"
                "    data.Copy(localData);\n");

    PrintOnFile(objHeader, "private:\n\n");
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
                PrintOnFile(objHeader, "    ");
                PrintOnFile(objHeader, typeName.Buffer());
                PrintOnFile(objHeader, " ");
                PrintOnFile(objHeader, paramName);
                PrintOnFile(objHeader, ";\n");
                if(paramName[0]=='*'){
                    PrintOnFile(objHeader, "    uint32 NumberOf");
                    PrintOnFile(objHeader, paramName+1);
                    PrintOnFile(objHeader, ";\n");
                }
                StreamString paramPath;
                StreamString paramAddress;
                StreamString paramAlias;
                if (database.Read("Source", paramPath)) {
                    paramPath.Seek(0ull);
                    char8 terminator;
                    StreamString token;
                    while (paramPath.GetToken(token, ".", terminator)) {
                        if (terminator == '.') {
                            if (paramAddress.Size() > 0u) {
                                paramAddress += ".";
                            }
                            paramAddress += token;
                        }
                        else {
                            paramAlias = token;
                        }
                        token = "";
                    }
                }
                else {
                    paramAlias = paramName;
                }
                if (paramName[0] == '*') {
                    ReadTheTypeArray(&paramName[1], typeName.Buffer(), paramAddress.Buffer(), objSource);
                }
                else {
                    ReadTheType(paramName, paramAddress.Buffer(), paramAlias.Buffer(), objSource);
                }
            }
            database.MoveToAncestor(1u);
        }
    }

    PrintOnFile(objSource, "    return ret;\n}\n\n");


}

static void GenerateObjFile(ConfigurationDatabase &database,
                            BasicFile &objHeader,
                            BasicFile &objSource,
                            const char8 * className) {

    PrintOnFile(objHeader, "class ");
    PrintOnFile(objHeader, className);

    PrintOnFile(objSource, className);
    PrintOnFile(objSource, "::");
    PrintOnFile(objSource, className);
    PrintOnFile(objSource, "(): ");

    AnyType at1 = database.GetType("Inherits");
    if (!at1.IsVoid()) {
        PrintOnFile(objHeader, " : ");

        if (at1.GetNumberOfDimensions() != 1) {
            printf("\n[Inherits] has to be a vector");
        }
        uint32 numberOfInherits = at1.GetNumberOfElements(0u);
        Vector<StreamString> inheritances(numberOfInherits);
        if (!database.Read("Inherits", inheritances)) {
            printf("\nCannot read [Inherits]\n");
        }
        for (uint32 i = 0u; i < numberOfInherits; i++) {
            PrintOnFile(objHeader, "public ");
            PrintOnFile(objHeader, inheritances[i].Buffer());
            PrintOnFile(objSource, inheritances[i].Buffer());
            PrintOnFile(objSource, "()");

            if (i != numberOfInherits - 1u) {
                PrintOnFile(objHeader, ", ");
                PrintOnFile(objSource, ", ");
            }
        }
    }

    //TODO the constructor and destructor must initialises the private members!!
    PrintOnFile(objHeader, "{\n\npublic:");
    PrintOnFile(objHeader, "\n\n    ");
    PrintOnFile(objHeader, className);
    PrintOnFile(objHeader, "();");
    PrintOnFile(objHeader, "\n\n    virtual ~");
    PrintOnFile(objHeader, className);
    PrintOnFile(objHeader, "();\n\n");

    PrintOnFile(objSource, " {\n}\n\n");

    PrintOnFile(objSource, className);
    PrintOnFile(objSource, "::");
    PrintOnFile(objSource, "~");
    PrintOnFile(objSource, className);
    PrintOnFile(objSource, "() {\n}\n\n");

    AnyType at2 = database.GetType("Functions");
    if (!at2.IsVoid()) {
        if (at2.GetNumberOfDimensions() != 1) {
            printf("\n[Functions] has to be a vector");
        }
        uint32 numberOfFunctions = at2.GetNumberOfElements(0u);
        Vector<StreamString> functions(numberOfFunctions);
        if (!database.Read("Functions", functions)) {
            printf("\nCannot read [Functions]\n");
        }
        for (uint32 i = 0u; i < numberOfFunctions; i++) {
            PrintOnFile(objHeader, "    ");
            PrintOnFile(objHeader, functions[i].Buffer());
            PrintOnFile(objHeader, ";\n\n");
        }
    }

    GenerateInitialiseFunction(database, objHeader, objSource, className);

    PrintOnFile(objHeader, "};");

}

static void GenerateOutputFiles(ConfigurationDatabase &database,
                                BasicFile &headerFile,
                                BasicFile &sourceFile) {

    StreamString className;
    database.Read("Class", className);

    BasicFile objHeader;
    StreamString objheaderName = className;
    objheaderName += ".h";
    // open the .h gam file
    if (!objHeader.Open(objheaderName.Buffer(),
                        BasicFile::FLAG_APPEND | BasicFile::FLAG_TRUNC | BasicFile::FLAG_CREAT | BasicFile::ACCESS_MODE_R | BasicFile::ACCESS_MODE_W)) {
        printf("\nUnable to open the gam header file");
        return;
    }

    BasicFile objSource;
    StreamString objSourceName = className;
    objSourceName += ".cpp";
    // open the .cpp gam file
    if (!objSource.Open(objSourceName.Buffer(),
                        BasicFile::FLAG_APPEND | BasicFile::FLAG_TRUNC | BasicFile::FLAG_CREAT | BasicFile::ACCESS_MODE_R | BasicFile::ACCESS_MODE_W)) {
        printf("\nUnable to open the gam source file");
        return;
    }

    GenerateObjFile(database, objHeader, objSource, className.Buffer());

    for (uint32 i = 0u; i < database.GetNumberOfChildren(); i++) {
        const char8* structName = database.GetChildName(i);

        //TODO if structName begins with + call again this function
        if (database.MoveRelative(structName)) {
            GenerateOutputFileRecursive(database, headerFile, sourceFile);
            database.MoveToAncestor(1u);
        }
    }

    //TODO Beginning from here check the * before the struct name and build the GAM code
    //TODO Here check the "Source" and "Name" fields if exists because
}

int main(int argc,
         char** argv) {

    // the parser
    ParserI *myParser = NULL;

    // the database created by the parser
    ConfigurationDatabase database;

    // the parser errors
    StreamString errors;

    // output files
    BasicFile headerOutputFile;
    BasicFile sourceOutputFile;

    // configuration input file
    BasicFile configFile;

    bool canReturn = false;

    if (argc < 3) {
        printf("\nPlease specify cfg name and output file name");
        canReturn = true;
    }

    char8 outputFileName[128];
    if (!canReturn) {
        StringHelper::Concatenate(argv[2], ".h", outputFileName);

        // open the .h output file
        if (!headerOutputFile.Open(
                outputFileName, BasicFile::FLAG_APPEND | BasicFile::FLAG_TRUNC | BasicFile::FLAG_CREAT | BasicFile::ACCESS_MODE_R | BasicFile::ACCESS_MODE_W)) {
            printf("\nUnable to open the output header file");
            canReturn = true;
        }
    }

    if (!canReturn) {
        StringHelper::Concatenate(argv[2], ".cpp", outputFileName);

        // open the .cpp output file
        if (!sourceOutputFile.Open(
                outputFileName, BasicFile::FLAG_APPEND | BasicFile::FLAG_TRUNC | BasicFile::FLAG_CREAT | BasicFile::ACCESS_MODE_R | BasicFile::ACCESS_MODE_W)) {
            printf("\nUnable to open the output source file");
            canReturn = true;
        }
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
            if ((argv[3])[0] == '1') {
                myParser = new StandardParser(configFile, database, &errors);
            }
            else if ((argv[3])[0] == '2') {
                myParser = new XMLParser(configFile, database, &errors);
            }
            else if ((argv[3])[0] == '3') {
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
        // print the header in the header file
        PrintOnFile(headerOutputFile, "#include \"GeneralDefinitions.h\"\n\n");
        PrintOnFile(headerOutputFile, "namespace MARTe{ \n\n");

        // print the header in the source file
        PrintOnFile(sourceOutputFile, "#include \"Object.h\"\n#include \"ClassRegistryDatabase.h\"\n");
        PrintOnFile(sourceOutputFile, "#include \"Introspection.h\"\n#include \"");
        PrintOnFile(sourceOutputFile, argv[2]);
        PrintOnFile(sourceOutputFile, ".h\"\n\n");
        PrintOnFile(sourceOutputFile, "namespace MARTe{ \n\n");

        GenerateOutputFiles(database, headerOutputFile, sourceOutputFile);

        PrintOnFile(headerOutputFile, "\n}");
        PrintOnFile(sourceOutputFile, "\n}");

    }
    if (myParser != NULL) {
        delete myParser;
    }

    printf("\n");
    return 0;

}
