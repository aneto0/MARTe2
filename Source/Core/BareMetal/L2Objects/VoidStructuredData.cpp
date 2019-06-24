/**
 * @file VoidStructuredDataI.cpp
 * @brief Header file for class AnyType
 * @date 24 Jun 2019
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

#include "VoidStructuredData.h"
#include "ErrorManagement.h"
#include "Reference.h"

namespace MARTe{

ErrorManagement::ErrorType VoidStructuredData::Read(CCString path,const AnyType &value){
	ErrorManagement::ErrorType ret;
	ret.unsupportedFeature = true;
	REPORT_ERROR(ret,"VoidStructuredData::Read");
	return ret;
}

ErrorManagement::ErrorType VoidStructuredData::Read(CCString path,Reference &object,bool borrow){
	ErrorManagement::ErrorType ret;
	ret.unsupportedFeature = true;
	REPORT_ERROR(ret,"VoidStructuredData::Read");
	return ret;
}

ErrorManagement::ErrorType VoidStructuredData::GetVariableInformation(
		CCString 			path,
		TypeDescriptor &	td,
		uint32 &			nOfDimensions,
		uint32 *			dimensionSizes) const{
	ErrorManagement::ErrorType ret;
	ret.unsupportedFeature = true;
	REPORT_ERROR(ret,"VoidStructuredData::GetVariableInformation");
	return ret;
}

ErrorManagement::ErrorType VoidStructuredData::Write(CCString name, const AnyType &value){
	ErrorManagement::ErrorType ret;
	ret.unsupportedFeature = true;
	REPORT_ERROR(ret,"VoidStructuredData::Write");
	return ret;
}

ErrorManagement::ErrorType VoidStructuredData::Write(Reference object){
	ErrorManagement::ErrorType ret;
	ret.unsupportedFeature = true;
	REPORT_ERROR(ret,"VoidStructuredData::Write");
	return ret;
}

ErrorManagement::ErrorType VoidStructuredData::Copy(StructuredDataI &destination){
	ErrorManagement::ErrorType ret;
	ret.unsupportedFeature = true;
	REPORT_ERROR(ret,"VoidStructuredData::Copy");
	return ret;
}

ErrorManagement::ErrorType VoidStructuredData::MoveToRoot(){
	ErrorManagement::ErrorType ret;
	ret.unsupportedFeature = true;
	REPORT_ERROR(ret,"VoidStructuredData::MoveToRoot");
	return ret;
}

ErrorManagement::ErrorType VoidStructuredData::MoveToAncestor(uint32 generations){
	ErrorManagement::ErrorType ret;
	ret.unsupportedFeature = true;
	REPORT_ERROR(ret,"VoidStructuredData::MoveToAncestor");
	return ret;
}

ErrorManagement::ErrorType VoidStructuredData::MoveAbsolute(CCString path){
	ErrorManagement::ErrorType ret;
	ret.unsupportedFeature = true;
	REPORT_ERROR(ret,"VoidStructuredData::MoveAbsolute");
	return ret;
}

ErrorManagement::ErrorType VoidStructuredData::MoveRelative(CCString path){
	ErrorManagement::ErrorType ret;
	ret.unsupportedFeature = true;
	REPORT_ERROR(ret,"VoidStructuredData::MoveRelative");
	return ret;
}

ErrorManagement::ErrorType VoidStructuredData::MoveToChild(const uint32 childIdx){
	ErrorManagement::ErrorType ret;
	ret.unsupportedFeature = true;
	REPORT_ERROR(ret,"VoidStructuredData::MoveToChild");
	return ret;
}

CCString VoidStructuredData::GetName(){
	ErrorManagement::ErrorType ret;
	ret.unsupportedFeature = true;
	REPORT_ERROR(ret,"VoidStructuredData::GetName");
	return emptyString;
}

CCString VoidStructuredData::GetChildName(const uint32 index){
	ErrorManagement::ErrorType ret;
	ret.unsupportedFeature = true;
	REPORT_ERROR(ret,"VoidStructuredData::GetChildName");
	return emptyString;
}

uint32 VoidStructuredData::GetNumberOfChildren(){
	ErrorManagement::ErrorType ret;
	ret.unsupportedFeature = true;
	REPORT_ERROR(ret,"VoidStructuredData::GetNumberOfChildren");
	return 0;
}

ErrorManagement::ErrorType VoidStructuredData::AddToCurrentNode(Reference node){
	ErrorManagement::ErrorType ret;
	ret.unsupportedFeature = true;
	REPORT_ERROR(ret,"VoidStructuredData::AddToCurrentNode");
	return ret;
}

ErrorManagement::ErrorType VoidStructuredData::CreateAbsolute(CCString path){
	ErrorManagement::ErrorType ret;
	ret.unsupportedFeature = true;
	REPORT_ERROR(ret,"VoidStructuredData::CreateAbsolute");
	return ret;
}

ErrorManagement::ErrorType VoidStructuredData::CreateRelative(CCString path){
	ErrorManagement::ErrorType ret;
	ret.unsupportedFeature = true;
	REPORT_ERROR(ret,"VoidStructuredData::CreateRelative");
	return ret;
}

ErrorManagement::ErrorType VoidStructuredData::Delete(CCString path){
	ErrorManagement::ErrorType ret;
	ret.unsupportedFeature = true;
	REPORT_ERROR(ret,"VoidStructuredData::Delete");
	return ret;
}



} //MARTe
