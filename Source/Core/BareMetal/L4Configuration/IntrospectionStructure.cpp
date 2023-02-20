/**
 * @file IntrospectionStructure.cpp
 * @brief Source file for class IntrospectionStructure
 * @date 09/11/2018
 * @author Andre Neto
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
 * the class IntrospectionStructure (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "ClassRegistryItem.h"
#include "Introspection.h"
#include "IntrospectionStructure.h"
#include "ObjectRegistryDatabase.h"
#include "StreamString.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

IntrospectionStructure::ClassRegistryItemConfigurationStructureLoader::ClassRegistryItemConfigurationStructureLoader(StreamString typeNameIn,
                                                                                                                     const uint32 totalSizeIn) :
        ClassRegistryItem(cp) {
    typeName = typeNameIn.Buffer();
    totalSize = totalSizeIn;
    cp.Reset(typeName.Buffer(), typeName.Buffer(), "", totalSize);
}

IntrospectionStructure::ClassRegistryItemConfigurationStructureLoader::~ClassRegistryItemConfigurationStructureLoader() {

}

void IntrospectionStructure::ClassRegistryItemConfigurationStructureLoader::Update(StreamString typeNameIn,
                                                                                   const uint32 totalSizeIn) {
    typeName = typeNameIn.Buffer();
    totalSize = totalSizeIn;
    cp.Reset(typeName.Buffer(), typeName.Buffer(), "", totalSize);
}
}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

//lint -esym(843, *IntrospectionStructure::allowDestructor*) variable 'MARTe::IntrospectionStructure::allowDestructor' could be declared as const. Cannot be constant since it shall be modifiable.
bool IntrospectionStructure::allowDestructor = false;

IntrospectionStructure::IntrospectionStructure() :
        Object() {
    entries = NULL_PTR(IntrospectionEntry **);
    introMembers = NULL_PTR(Introspection *);
    numberOfMembers = 0u;
    memberInfo = NULL_PTR(MemberInfo **);
}

/*lint -e{1540} The entries cannot be destroyed, as otherwise the IntrospectionEntry** would be removed for the database*/
/*lint -e{1551} should not throw exception as the memory is checked. By the design memory if freed in the destructor.*/
IntrospectionStructure::~IntrospectionStructure() {
    const ClassRegistryItem *item = ClassRegistryDatabase::Instance()->Find(GetName());
    ClassRegistryItemConfigurationStructureLoader * criLoader =
            dynamic_cast<ClassRegistryItemConfigurationStructureLoader *>(const_cast<ClassRegistryItem *>(item));
    //The entries cannot be destroyed, as otherwise the IntrospectionEntry** would be removed for the database
    if (allowDestructor) {
        if (entries != NULL_PTR(IntrospectionEntry **)) {
            uint32 n;
            for (n = 0u; n < (numberOfMembers + 1u); n++) {
                if (entries[n] != NULL_PTR(IntrospectionEntry *)) {
                    delete entries[n];
                    entries[n] = NULL_PTR(IntrospectionEntry *);
                }
            }
            delete[] entries;
            entries = NULL_PTR(IntrospectionEntry **);
        }
        if (memberInfo != NULL_PTR(MemberInfo **)) {
            uint32 n;
            for (n = 0u; n < (numberOfMembers); n++) {
                if (memberInfo[n] != NULL_PTR(MemberInfo *)) {
                    delete memberInfo[n];
                    memberInfo[n] = NULL_PTR(MemberInfo *);
                }
            }
            delete[] memberInfo;
            memberInfo = NULL_PTR(MemberInfo **);
        }
        if (introMembers != NULL_PTR(Introspection *)) {
            delete introMembers;
            introMembers = NULL_PTR(Introspection *);
        }
        if (criLoader != NULL_PTR(ClassRegistryItemConfigurationStructureLoader*)) {
            criLoader->SetIntrospection(NULL_PTR(Introspection*));
        }
    }
    entries = NULL_PTR(IntrospectionEntry **);
    introMembers = NULL_PTR(Introspection *);
    memberInfo = NULL_PTR(MemberInfo **);
}

bool IntrospectionStructure::Initialise(StructuredDataI &data) {
    bool ok = Object::Initialise(data);
    uint32 totalSize = 0u;
    if (ok) {
        uint32 n;
        uint32 nOfChildren = data.GetNumberOfChildren();
        for (n = 0u; (n < nOfChildren) && (ok); n++) {
            StreamString memberName = data.GetChildName(n);
            /*lint -e {613} numberOfElements is always allocated if ok = true*/
            if (data.MoveRelative(memberName.Buffer())) {
                StreamString typeNameStr;
                ok = data.Read("Type", typeNameStr);
                if (!ok) {
                    REPORT_ERROR(ErrorManagement::ParametersError, "Member %s has no Type defined", memberName.Buffer());
                }
                uint32 numberOfDimensions = 0u;
                uint32 *numberOfElements = NULL_PTR(uint32 *);
                uint32 nd = 0u;

                if (ok) {
                    AnyType arrayDescription = data.GetType("NumberOfElements");
                    if (arrayDescription.GetDataPointer() == NULL_PTR(void *)) {
                        numberOfElements = new uint32[1u];
                        numberOfElements[0u] = 1u;
                    }
                    else {
                        numberOfDimensions = arrayDescription.GetNumberOfElements(0u);
                        if (numberOfDimensions < 2u) {
                            numberOfElements = new uint32[1u];
                            Vector<uint32> readVector(numberOfElements, numberOfDimensions);
                            if (!data.Read("NumberOfElements", readVector)) {
                                uint32 nElementsRead = 1u;
                                ok = data.Read("NumberOfElements", nElementsRead);
                                if (ok) {
                                    if (nElementsRead > 1u) {
                                        numberOfDimensions = 1u;
                                    }
                                    numberOfElements[0u] = nElementsRead;
                                }
                            }
                            if (numberOfDimensions == 1u) {
                                if (numberOfElements[0u] == 1u) {
                                    numberOfDimensions = 0u;
                                }
                            }
                        }
                        else {
                            numberOfElements = new uint32[numberOfDimensions];
                            Vector<uint32> readVector(numberOfElements, numberOfDimensions);
                            ok = data.Read("NumberOfElements", readVector);
                        }
                    }
                }
                /*lint -e{661} entries is a zero terminated array and as such the last index must be set to zero*/
                if (ok) {
                    StreamString modifiers;
                    for (nd = 0u; nd < numberOfDimensions; nd++) {
                        (void) modifiers.Printf("[%d]", numberOfElements[nd]);
                    }
                    uint32 z;
                    //entries is a zero terminated array
                    uint32 numberOfEntries = numberOfMembers + 2u;
                    IntrospectionEntry **newEntries = new IntrospectionEntry *[numberOfEntries];
                    numberOfEntries = numberOfMembers + 1u;
                    MemberInfo** newMemberInfo = new MemberInfo *[numberOfEntries];
                    for (z = 0u; z < numberOfMembers; z++) {
                        newEntries[z] = entries[z];
                        newMemberInfo[z] = memberInfo[z];
                    }
                    if (entries != NULL_PTR(IntrospectionEntry **)) {
                        delete[] entries;
                    }
                    if (memberInfo != NULL_PTR(MemberInfo **)) {
                        delete[] memberInfo;
                    }
                    entries = newEntries;
                    memberInfo = newMemberInfo;

                    uint32 memberSize = 0u;
                    TypeDescriptor td = TypeDescriptor::GetTypeDescriptorFromTypeName(typeNameStr.Buffer());
                    if (td != InvalidType) {
                        memberSize = static_cast<uint32>(td.numberOfBits) / 8u;
                    }
                    else {
                        const ClassRegistryItem *registeredMember = ClassRegistryDatabase::Instance()->Find(typeNameStr.Buffer());
                        if (registeredMember != NULL_PTR(const ClassRegistryItem *)) {
                            const Introspection *intro = registeredMember->GetIntrospection();
                            if (intro != NULL_PTR(const Introspection *)) {
                                memberSize = registeredMember->GetIntrospection()->GetClassSize();
                            }
                        }
                        else {
                            REPORT_ERROR(ErrorManagement::ParametersError, "Type %s is not known", typeNameStr.Buffer());
                        }
                    }
                    //These really need to exist as they own the memory for the strings of the IntrospectionEntry
                    newMemberInfo[z] = new MemberInfo();
                    newMemberInfo[z]->memberName = memberName.Buffer();
                    newMemberInfo[z]->memberType = typeNameStr.Buffer();
                    newMemberInfo[z]->memberModifier = modifiers.Buffer();

                    uint32 totalElements = 1u;
                    for (nd = 0u; nd < numberOfDimensions; nd++) {
                        if (numberOfElements[nd] > 0u) {
                            totalElements *= numberOfElements[nd];
                        }
                    }
                    if (totalElements == 0u) {
                        totalElements = 1u;
                    }
                    IntrospectionEntry *entry = new IntrospectionEntry(newMemberInfo[z]->memberName.Buffer(), newMemberInfo[z]->memberType.Buffer(),
                                                                       newMemberInfo[z]->memberModifier.Buffer(), "", memberSize, totalSize);
                    entries[z] = entry;
                    /*lint -e{679} entries is a zero terminated array*/
                    entries[z + 1u] = NULL_PTR(IntrospectionEntry *);
                    totalSize += (memberSize * totalElements);
                    numberOfMembers++;
                }
                if (ok) {
                    ok = data.MoveToAncestor(1u);
                }
                if (numberOfElements != NULL_PTR(uint32 *)) {
                    delete[] numberOfElements;
                }
            }
        }
    }
    if (ok) {
        ok = (totalSize > 0u);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Type %s has no parameters", GetName());
        }
    }
    if (ok) {
        StreamString typeName;
        (void) typeName.Printf("%s", GetName());
        introMembers = new Introspection(const_cast<const IntrospectionEntry **>(entries), totalSize);
        if (memberInfo != NULL_PTR(MemberInfo **)) {
            introMembers->SetIntrospectionEntryMemory(&memberInfo[0u]);
        }
        const ClassRegistryItem *item = ClassRegistryDatabase::Instance()->Find(GetName());
        bool exists = (item != NULL_PTR(const ClassRegistryItem *));
        ClassRegistryItemConfigurationStructureLoader * criLoader = NULL_PTR(ClassRegistryItemConfigurationStructureLoader *);
        if (!exists) {
            criLoader = new ClassRegistryItemConfigurationStructureLoader(typeName, totalSize);
            criLoader->SetIntrospection(introMembers);
            ClassRegistryDatabase::Instance()->Add(criLoader);
            REPORT_ERROR(ErrorManagement::ParametersError, "Registering type %s ", typeName.Buffer());
        }
        else {
            criLoader = dynamic_cast<ClassRegistryItemConfigurationStructureLoader *>(const_cast<ClassRegistryItem *>(item));
            ok = (criLoader != NULL_PTR(ClassRegistryItemConfigurationStructureLoader *));
            /*lint -e{613} crioLoader is != NULL*/
            if (ok) {
                //Delete the old Introspection * and old IntrospectionStructure * members to avoid memory leaks
                Introspection *oldIntrospection = const_cast<Introspection *>(criLoader->GetIntrospection());
                if (oldIntrospection != NULL_PTR(const Introspection *)) {
                    const IntrospectionEntry **oldFields = oldIntrospection->GetFields();
                    if (oldFields != NULL_PTR(const IntrospectionEntry **)) {
                        uint32 oldNumberOfEntries = oldIntrospection->GetNumberOfMembers();
                        for (uint32 n = 0u; n < (oldNumberOfEntries + 1u); n++) {
                            if (oldFields[n] != NULL_PTR(const IntrospectionEntry *)) {
                                delete oldFields[n];
                                oldFields[n] = NULL_PTR(const IntrospectionEntry *);
                            }
                        }
                        delete[] oldFields;
                        MemberInfo **oldMemberInfo = static_cast<MemberInfo **>(oldIntrospection->GetIntrospectionEntryMemory());
                        if (oldMemberInfo != NULL_PTR(MemberInfo **)) {
                            for (uint32 n = 0u; n < (oldNumberOfEntries); n++) {
                                if (oldMemberInfo[n] != NULL_PTR(MemberInfo*)) {
                                    delete oldMemberInfo[n];
                                    oldMemberInfo[n] = NULL_PTR(MemberInfo*);
                                }
                            }
                            delete[] oldMemberInfo;
                        }
                    }
                    delete oldIntrospection;
                }
                criLoader->Update(typeName, totalSize);
                criLoader->SetIntrospection(introMembers);
                REPORT_ERROR(ErrorManagement::ParametersError, "Updating type %s ", typeName.Buffer());
            }
            else {
                REPORT_ERROR(ErrorManagement::FatalError, "Tried to update a type %s which is not a ClassRegistryItemConfigurationStructureLoader",
                             typeName.Buffer());
            }
        }
        /*lint -e{429} criLoader pointer deleted by the IntrospectionStructure*/
    }

    return ok;
}

bool IntrospectionStructure::RegisterStructuredDataI(StructuredDataI &in,
                                                     const char8 * const typeIdentifier) {
    bool ok = true;
    static ReferenceT<ReferenceContainer> registeredStructuredTypes;
    if (!registeredStructuredTypes.IsValid()) {
        registeredStructuredTypes = ReferenceT<ReferenceContainer>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
        ok = ObjectRegistryDatabase::Instance()->Insert(registeredStructuredTypes);
    }
    ConfigurationDatabase cdbTypes;
    if (ok) {
        ok = RegisterStructuredDataIPriv(in, cdbTypes, typeIdentifier);
    }
    if (ok) {
        ok = cdbTypes.MoveToRoot();
    }
    uint32 nOfDiscoveredTypes = cdbTypes.GetNumberOfChildren();
    uint32 n;
    for (n = nOfDiscoveredTypes; (n > 0u) && (ok); n--) {
        ReferenceT<IntrospectionStructure> is(GlobalObjectsDatabase::Instance()->GetStandardHeap());
        ok = cdbTypes.MoveToChild(n - 1u);
        const char8 * const typeName = cdbTypes.GetName();
        if (ok) {
            is->SetName(typeName);
        }
        if (ok) {
            ok = is->Initialise(cdbTypes);
        }
        if (ok) {
            ok = cdbTypes.MoveToAncestor(1u);
        }
        if (ok) {
            (void) registeredStructuredTypes->Delete(typeName);
            ok = registeredStructuredTypes->Insert(is);
        }
    }

    return ok;
}

bool IntrospectionStructure::RegisterStructuredDataIPriv(StructuredDataI &in,
                                                         ConfigurationDatabase &typesCDB,
                                                         const char8 * const typeIdentifier) {
    StreamString typeName;
    if (typeIdentifier != NULL_PTR(const char8 * const)) {
        (void) in.Read(typeIdentifier, typeName);
    }
    if (typeName.Size() == 0LLU) {
        if (in.GetName() == NULL_PTR(const char8 * const)) {
            (void) typeName.Printf("AutoGeneratedRoot", voidAnyType);
        }
        else {
            //Remove any [] from the type name
            StreamString varArrayName = in.GetName();
            (void) varArrayName.Seek(0LLU);
            StreamString token;
            char8 ignore;
            if (varArrayName.GetToken(token, "[", ignore)) {
                varArrayName = token;
            }
            (void) typeName.Printf("AutoGenerated%s", varArrayName.Buffer());
        }
    }
    bool ok = typesCDB.CreateAbsolute(typeName.Buffer());
    uint32 n;
    uint32 nChilds = in.GetNumberOfChildren();
    //lint -e{850} n is modified inside the loop
    for (n = 0u; (n < nChilds) && (ok); n++) {
        const char8* const variableName = in.GetChildName(n);
        uint32 numberOfElements;

        if (in.MoveRelative(variableName)) {
            //Check if it is an array
            StreamString token;
            StreamString varArrayName = variableName;
            (void) varArrayName.Seek(0LLU);
            char8 ignore;
            bool isArray = varArrayName.GetToken(token, "[", ignore);
            if (isArray) {
                numberOfElements = 1u;
                varArrayName = token;
                (void) varArrayName.Seek(0LLU);
                ok = in.MoveToAncestor(1u);
                n++;
                while (isArray && ok && (n < nChilds)) {
                    const uint32 nn = n;
                    StreamString nextVarArrayName = in.GetChildName(nn);
                    (void) nextVarArrayName.Seek(0LLU);
                    token = "";
                    isArray = nextVarArrayName.GetToken(token, "[", ignore);
                    if (isArray) {
                        //Still the same array?
                        isArray = (token == varArrayName);
                    }
                    if (isArray) {
                        token = "";
                        ok = nextVarArrayName.GetToken(token, "]", ignore);
                        if (ok) {
                            ok = TypeConvert(numberOfElements, token.Buffer());
                            numberOfElements++;
                        }
                    }
                    n++;
                }
                //To perform the work below of registering the type hold by this array
                n--;
                if (ok) {
                    ok = in.MoveRelative(variableName);
                }
            }
            else {
                numberOfElements = 1u;
            }

            ConfigurationDatabase typesCDBCurrent = typesCDB;
            StreamString memberTypeName;
            if (typeIdentifier != NULL_PTR(const char8 * const)) {
                (void) in.Read(typeIdentifier, memberTypeName);
            }
            //The varArrayName already cleans any potential [] from the name
            if (memberTypeName.Size() == 0LLU) {
                (void) memberTypeName.Printf("AutoGenerated%s", varArrayName.Buffer());
            }
            if (ok) {
                ok = RegisterStructuredDataIPriv(in, typesCDB, typeIdentifier);
            }
            typesCDB = typesCDBCurrent;
            if (ok) {
                ok = in.MoveToAncestor(1u);
            }
            if (ok) {
                ok = typesCDB.CreateRelative(varArrayName.Buffer());
            }
            if (ok) {
                ok = typesCDB.Write("Type", memberTypeName.Buffer());
            }
            if (ok) {
                ok = typesCDB.Write("NumberOfElements", numberOfElements);
            }
            if (ok) {
                ok = typesCDB.MoveToAncestor(1u);
            }
        }
        else {
            bool isTypeIdentifier = (typeIdentifier != NULL_PTR(const char8 * const));
            if (isTypeIdentifier) {
                isTypeIdentifier = (StringHelper::Compare(variableName, typeIdentifier) == 0);
            }
            if (!isTypeIdentifier) {
                AnyType variableType = in.GetType(variableName);
                const char8* const variableTypeName = TypeDescriptor::GetTypeNameFromTypeDescriptor(variableType.GetTypeDescriptor());
                uint8 nOfDimensions = variableType.GetNumberOfDimensions();
                ok = typesCDB.CreateRelative(variableName);
                if (nOfDimensions == 0u) {
                    ok = typesCDB.Write("NumberOfElements", 1);
                }
                else if (nOfDimensions == 1u) {
                    ok = typesCDB.Write("NumberOfElements", variableType.GetNumberOfElements(0u));
                }
                else {
                    uint32 *numberOfElementsArr = new uint32[nOfDimensions];
                    Vector<uint32> numberOfElementsVec(numberOfElementsArr, nOfDimensions);
                    uint8 d;
                    for (d = 0u; d < nOfDimensions; d++) {
                        numberOfElementsArr[d] = variableType.GetNumberOfElements(d);
                    }
                    if (ok) {
                        ok = typesCDB.Write("NumberOfElements", numberOfElementsVec);
                    }
                    delete[] numberOfElementsArr;
                }
                if (ok) {
                    ok = typesCDB.Write("Type", variableTypeName);
                }
                if (ok) {
                    ok = typesCDB.MoveToAncestor(1u);
                }
            }
        }
    }
    return ok;
}
/**Ignore doxygen false positive*/
CLASS_REGISTER(IntrospectionStructure, "1.0");
}

