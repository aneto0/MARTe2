/**
 * @file AnyTypeStructuredData.cpp
 * @brief Header file for class AnyType
 * @date Jul 2, 2020
 * @author Filippo Sartori
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

 * @details This header file contains the declaration of the class AnyType
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
*/

#include "AnyTypeStructuredData.h"
#include "CompositeErrorManagement.h"
#include "AnyObject.h"
#include "DynamicCString.h"

namespace MARTe{


template <class T >
AnyTypeStructuredData::AnyTypeStructuredData(T &data){
    AnyType at(data);
    referencesHistory.Push(at);
    pathLengthHistory.Push(0);
}

AnyTypeStructuredData::AnyTypeStructuredData(const AnyTypeStructuredData *data){
    AnyType at(data);
    referencesHistory.Push(at);
    pathLengthHistory.Push(0);
}


AnyTypeStructuredData::~AnyTypeStructuredData(){

}

ErrorManagement::ErrorType AnyTypeStructuredData::Read(CCString path,const AnyType &value){
    ErrorManagement::ErrorType ret;

    AnyType toCopyFrom;
    ret = Access(path,toCopyFrom);
    COMPOSITE_REPORT_ERROR(ret,"failed accessing ",path);

    if (ret){
        ret = toCopyFrom.CopyTo(value);
        REPORT_ERROR(ret,"CopyTo failed");
    }

    return ret;
}

ErrorManagement::ErrorType AnyTypeStructuredData::Read(CCString path,Reference &object,bool borrow=true) {
    ErrorManagement::ErrorType ret;

    AnyType toClone;
    ret = Access(path,toClone);
    COMPOSITE_REPORT_ERROR(ret,"failed accessing ",path);

    if (ret && borrow){
        ret.unsupportedFeature = true;
        REPORT_ERROR(ret,"Reference borrowing currently not supported");
    }

    if (ret && !borrow){
        ret = toClone.Clone(object);
        REPORT_ERROR(ret,"AnyType Clone failed");
    }

    return ret;
}

ErrorManagement::ErrorType AnyTypeStructuredData::GetVariableInformation(
        CCString            path,
        TypeDescriptor &    td,
        uint32 &            nOfDimensions,
        uint32 *            dimensionSizes) const {

    ErrorManagement::ErrorType ret;

    AnyType at;
    ret = Access(path,at);
    COMPOSITE_REPORT_ERROR(ret,"failed accessing",path);

    if (ret){
        ret = at.GetVariableInformation(td,nOfDimensions,dimensionSizes);
        REPORT_ERROR(ret,"AnyType GetVariableInformation failed");
    }

    return ret;
}

ErrorManagement::ErrorType AnyTypeStructuredData::Write(Reference object){
    ErrorManagement::ErrorType ret;

    ret.parametersError = !object.IsValid();
    REPORT_ERROR(ret,"parameter is an invalid reference");

    AnyType at;

    if (ret){
        // obtain the object type and address
        object.ToAnyType(at);

        CCString name = object->GetName();
        ret = Write(name,at);
        COMPOSITE_REPORT_ERROR(ret,"Write(",name,") failed");
    }
    return ret;
}

ErrorManagement::ErrorType AnyTypeStructuredData::Write(CCString name, const AnyType &value){

    ErrorManagement::ErrorType ret;

    AnyType toCopyTo;
    ret = Access(name,toCopyTo);
    COMPOSITE_REPORT_ERROR(ret,"failed accessing",name);

    if (ret){
        ret = toCopyTo.CopyTo(value);
        REPORT_ERROR(ret,"CopyTo failed");
    }
    return ret;
}

ErrorManagement::ErrorType AnyTypeStructuredData::Copy(StructuredDataI &destination) {
    ErrorManagement::ErrorType ret;

    // NOTE that we never address leaves, only nodes
    uint32 nOfChildren = GetNumberOfChildren();
    for (uint32 i = 0u; (i < nOfChildren) && (ret); i++) {

        AnyType at;
        DynamicCString name;
        ret = Access(i,at,name);
        COMPOSITE_REPORT_ERROR(ret,"failed accessing ",i,"-th element");

        bool isLeaf = false;
        if (ret){
            ret = IsLeaf(at,isLeaf);
        }

        if (ret && isLeaf) {
            ret = destination.Write(name,at);
        } else
        if (ret && !isLeaf) {
            ret = destination.CreateRelative(name);
            COMPOSITE_REPORT_ERROR(ret,"Failed to Create node ",name);

            if (ret){
                ret = MoveRelative(name);
                COMPOSITE_REPORT_ERROR(ret,"Failed to Move to ",name);
            }

            if (ret) {
                // go recursively !
                ret = Copy(destination);
                REPORT_ERROR(ret,"Failed to Copy recursively");
            }

            if (ret){
                ret = MoveToAncestor(1u);
                REPORT_ERROR(ret,"Failed to move back one level");
            }

            if (ret){
                ret = destination.MoveToAncestor(1u);
                REPORT_ERROR(ret,"Failed to move back one level destination");
            }
        }
    }
    return ret;
}

ErrorManagement::ErrorType AnyTypeStructuredData::MoveToRoot(){
    ErrorManagement::ErrorType ret;

    AnyType at;
    while ((referencesHistory.Size() > 1)  && ret){
        ret = referencesHistory.Pop(at);
        REPORT_ERROR(ret,"referencesHistory.Pop failed");
    }

    while ((pathLengthHistory.Size() > 1)  && ret){
        uint32 n;
        ret = pathLengthHistory.Pop(n);
        REPORT_ERROR(ret,"pathLengthHistory.Pop failed");
    }

    if (ret){
        navigationPath().SetSize(0);
    }

    return ret;
}

ErrorManagement::ErrorType AnyTypeStructuredData::MoveToAncestor(uint32 generations){
    ErrorManagement::ErrorType ret;

    AnyType at;
    if ((referencesHistory.Size() > 1)  && ret){
        ret = referencesHistory.Pop(at);
    }
    uint32 length;
    if ((pathLengthHistory.Size() > 1)  && ret){
        ret = pathLengthHistory.Pop(length);
    }
    if (ret){
        navigationPath().SetSize(length);
    }

    return ret;
}

ErrorManagement::ErrorType AnyTypeStructuredData::MoveAbsolute(CCString path){
    ErrorManagement::ErrorType ret;

    ret = MoveToRoot();

    if (ret){
        ret = MoveRelative(path);
    }

    return ret;
}

ErrorManagement::ErrorType AnyTypeStructuredData::MoveRelative(CCString path){
    ErrorManagement::ErrorType ret;

    DynamicCString token;
    uint32 limit = 0;

    while ((path.GetSize() > 0) && ret){
        path = DynamicCString::Tokenize(path,token,limit,CCString("."),CCString(""));

        ret.syntaxError = (token.GetSize() == 0);
        REPORT_ERROR(ret,"token has 0 size");

        if (ret){
            ret = Move1Step(token);
            COMPOSITE_REPORT_ERROR(ret,"Move1Step(",token,") failed");
        }
    }

    return ret;
}

ErrorManagement::ErrorType AnyTypeStructuredData::MoveToChild(const uint32 childIdx){
    ErrorManagement::ErrorType ret;

    DynamicCString name;
    AnyType at;
    ret = Access(childIdx,at,name);
    COMPOSITE_REPORT_ERROR(ret,"failed Access(",childIdx,")");

    if (ret){
        ret = referencesHistory.Push(at);
        REPORT_ERROR(ret,"failed Push(at)");
    }

    if (ret){
        uint32 pathSize = navigationPath.GetSize();

        if (pathSize == 0){
            navigationPath().Append('.');
        }
        navigationPath().Append(name);

        ret = pathLengthHistory.Push(navigationPath.GetSize());
        REPORT_ERROR(ret,"failed Push(size)");
    }

    return ret;
}

CCString AnyTypeStructuredData::GetName(){
    return navigationPath;
}

CCString AnyTypeStructuredData::GetChildName(const uint32 childIdx){
    ErrorManagement::ErrorType ret;

    DynamicCString name;
    AnyType at;
    ret = Access(childIdx,at,name);
    COMPOSITE_REPORT_ERROR(ret,"failed Access(",childIdx,")");

    //TODO change API

    return ret;
}

uint32 AnyTypeStructuredData::GetNumberOfChildren(){
    ErrorManagement::ErrorType ret;

    //TODO

    return ret;
}

ErrorManagement::ErrorType AnyTypeStructuredData::AddToCurrentNode(Reference node) {
    return ErrorManagement::InvalidOperation;
}

ErrorManagement::ErrorType AnyTypeStructuredData::CreateAbsolute(CCString path){
    return ErrorManagement::InvalidOperation;
}

ErrorManagement::ErrorType AnyTypeStructuredData::CreateRelative(CCString path){
    return ErrorManagement::InvalidOperation;
}

ErrorManagement::ErrorType AnyTypeStructuredData::Delete(CCString path){
    return ErrorManagement::InvalidOperation;
}

ErrorManagement::ErrorType AnyTypeStructuredData::Move1Step(CCString name) {
    ErrorManagement::ErrorType ret;

    AnyType at;
    ret = referencesHistory.Peek(at,0);
    REPORT_ERROR(ret,"failed Peek(0)");

    if (ret){

        // using MultipleDereference as it handles indexes and fields
        // this is one level though
        ret = at.MultipleDereference(name);
        COMPOSITE_REPORT_ERROR(ret,"failed MultipleDereference(",name,")");
    }

    if (ret){
        ret = referencesHistory.Push(at);
        REPORT_ERROR(ret,"failed Push(at)");
    }

    if (ret){
        uint32 pathSize = navigationPath.GetSize();

        if (pathSize == 0){
            navigationPath().Append('.');
        }
        navigationPath().Append(name);

        ret = pathLengthHistory.Push(navigationPath.GetSize());
        REPORT_ERROR(ret,"failed Push(size)");
    }

    return ret;
}


ErrorManagement::ErrorType AnyTypeStructuredData::Access(CCString name,AnyType &atRef) const{
    ErrorManagement::ErrorType ret;

    AnyType at;
    ret = referencesHistory.Peek(at,0);
    REPORT_ERROR(ret,"referencesHistory.Peek(0) failed");

    if (ret){
        ret = at.MultipleDereference(name);
        COMPOSITE_REPORT_ERROR(ret,"MultipleDereference(",name,") failed");
    }

    if (ret){
        atRef = at;
    }

    return ret;
}


ErrorManagement::ErrorType AnyTypeStructuredData::Access(uint32 childIdx,AnyType &atRef,DynamicCString &name) const{
    ErrorManagement::ErrorType ret;

    AnyType at;
    ret = referencesHistory.Peek(at,0);
    REPORT_ERROR(ret,"failed Peek(0)");

    if (ret){
        uint32 index = childIdx;
        name().Truncate(0);
        // SmartDereference(uint32 &,DynamicCString &)
        ret = at.SmartDereference(index,name);
        COMPOSITE_REPORT_ERROR(ret,"failed SmartDereference(",index,")");
    }

    if (ret){
        atRef = at;
    }

    return ret;
}

ErrorManagement::ErrorType AnyTypeStructuredData::IsLeaf(const AnyType &at,bool &isLeaf) const{
    ErrorManagement::ErrorType ret;

    TypeDescriptor td;
    uint32 nDims = 2;
    uint32 dims[2];
    // peel off up to layers and check if the result is a leaf
    // we try to handle matrices as 1 item
    ret = at.GetVariableInformation(td,nDims,dims);
    REPORT_ERROR(ret,"at.GetVariableInformation failed");
    if (ret){
        isLeaf = !td.IsStructuredData() && !td.SameAs(GenericPointer) && !td.SameAs(GenericArray);
    }
    return ret;
}


} //MARTe
