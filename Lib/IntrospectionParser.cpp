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
                                char8* modifiers) {
    uint32 dataSize = StringHelper::Length(data);
    uint32 i;
    for (i = (data[0] == 'Z'); i < dataSize; i++) {
        if (data[i] == '*') {
            StringHelper::Concatenate(modifiers, "* ");
        }
        else if (data[i] == 'C') {
            StringHelper::Concatenate(modifiers, "const ");
        }
        else {
            break;
        }
    }

    return i;
}

static void GenerateOutputFiles(ConfigurationDatabase &database,
                                BasicFile &headerFile,
                                BasicFile &sourceFile) {

    for (uint32 i = 0u; i < database.GetNumberOfChildren(); i++) {
        const char8* structName = database.GetChildName(i);
        PrintOnFile(headerFile, "struct ");
        PrintOnFile(headerFile, structName);
        PrintOnFile(headerFile, " { \n");

        database.MoveRelative(structName);

        for (uint32 j = 0u; j < database.GetNumberOfChildren(); j++) {

            // HEADER FILE MANAGEMENT
            const char8 *memberName = database.GetChildName(j);
            database.MoveRelative(memberName);

            char8 data[32] = { 0 };
            char8 type[32] = { 0 };
            char8 modifiers[32] = { 0 };
            char8 attributes[32] = { 0 };
            bool isConstantMember = false;
            PrintOnFile(headerFile, "    ");
            if (!database.Read("isConstant", data)) {
                printf("\n[isConstant] attribute lacks: the member type is considered not constant");
            }
            else {
                isConstantMember = (StringHelper::Compare(data, "true") == 0);
                if (isConstantMember) {
                    PrintOnFile(headerFile, "const ");
                }
            }

            if (!database.Read("type", type)) {
                printf("\n[type] attribute lacks: the member type is considered void");
                StringHelper::Copy(type, "void");
            }
            else {
                PrintOnFile(headerFile, type);
                PrintOnFile(headerFile, " ");
            }
            if (!database.Read("modifiers", modifiers)) {
                printf("\n[modifiers] attribute lacks: the modifiers is considered as an empty string");
                StringHelper::Copy(modifiers, "");
            }

            uint32 nextIndex = GetModifierString(modifiers, attributes);
            PrintOnFile(headerFile, attributes);
            PrintOnFile(headerFile, memberName);
            while (modifiers[nextIndex] != '\0') {
                PrintOnFile(headerFile, modifiers[nextIndex]);
                nextIndex++;
            }
            PrintOnFile(headerFile, ";\n");

            // SOURCE FILE MANAGEMENT
            // a basic type !!!
            PrintOnFile(sourceFile, "DECLARE_CLASS_MEMBER(");
            PrintOnFile(sourceFile, structName);
            PrintOnFile(sourceFile, ",");
            PrintOnFile(sourceFile, memberName);
            PrintOnFile(sourceFile, ",");
            PrintOnFile(sourceFile, type);
            PrintOnFile(sourceFile, ",");
            (isConstantMember) ? (PrintOnFile(sourceFile, "true")) : (PrintOnFile(sourceFile, "false"));
            PrintOnFile(sourceFile, ",");
            PrintOnFile(sourceFile, "\"");
            PrintOnFile(sourceFile, modifiers);
            PrintOnFile(sourceFile, "\"");
            PrintOnFile(sourceFile, ");\n");

            database.MoveToAncestor(1);
        }
        PrintOnFile(sourceFile, "\n");
        PrintOnFile(headerFile, "}; \n\n");
        database.MoveToAncestor(1);
    }

    // SOURCE FILE MANAGEMENT
    database.MoveToRoot();
    for (uint32 i = 0u; i < database.GetNumberOfChildren(); i++) {

        const char8* structName = database.GetChildName(i);
        PrintOnFile(sourceFile, "static const IntrospectionEntry * ");
        PrintOnFile(sourceFile, structName);
        PrintOnFile(sourceFile, "_array[] = {\n");
        database.MoveRelative(structName);

        for (uint32 j = 0u; j < database.GetNumberOfChildren(); j++) {
            const char8 *memberName = database.GetChildName(j);
            database.MoveRelative(memberName);
            PrintOnFile(sourceFile, "&");
            PrintOnFile(sourceFile, structName);
            PrintOnFile(sourceFile, "_");
            PrintOnFile(sourceFile, memberName);
            PrintOnFile(sourceFile, "_introspectionEntry, \n");
            database.MoveToAncestor(1);
        }
        PrintOnFile(sourceFile, "0 };\n\n");
        PrintOnFile(sourceFile, "static Introspection ");
        PrintOnFile(sourceFile, structName);
        PrintOnFile(sourceFile, "_introspection(");
        PrintOnFile(sourceFile, structName);
        PrintOnFile(sourceFile, "_array);\n");
        PrintOnFile(sourceFile, "INTROSPECTION_CLASS_REGISTER(");
        PrintOnFile(sourceFile, structName);
        PrintOnFile(sourceFile, ",");
        PrintOnFile(sourceFile, "\"1.0\",");
        PrintOnFile(sourceFile, structName);
        PrintOnFile(sourceFile, "_introspection);\n\n");
        database.MoveToAncestor(1);
    }
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

    char8 outputFileName[32];
    StringHelper::Concatenate(argv[2], ".h", outputFileName);

    bool canReturn = false;

    // open the .h output file
    if (!headerOutputFile.Open(outputFileName,
                               BasicFile::FLAG_APPEND | BasicFile::FLAG_TRUNC | BasicFile::FLAG_CREAT | BasicFile::ACCESS_MODE_R | BasicFile::ACCESS_MODE_W)) {
        printf("\nUnable to open the output header file");
        canReturn = true;
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
        PrintOnFile(headerOutputFile, "using namespace MARTe; \n\n");

        // print the header in the source file
        PrintOnFile(sourceOutputFile, "#include \"Object.h\"\n#include \"ClassRegistryDatabase.h\"\n");
        PrintOnFile(sourceOutputFile, "#include \"Introspection.h\"\n#include \"");
        PrintOnFile(sourceOutputFile, argv[2]);
        PrintOnFile(sourceOutputFile, ".h\"\n\n");
        PrintOnFile(sourceOutputFile, "using namespace MARTe; \n\n");

        GenerateOutputFiles(database, headerOutputFile, sourceOutputFile);
    }
    if (myParser != NULL) {
        delete myParser;
    }
    return 0;

}
