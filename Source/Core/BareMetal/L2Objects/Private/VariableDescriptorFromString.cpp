/**
 * @file String2Variable.cpp
 * @brief Header file for class AnyType
 * @date Jun 4, 2020
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



/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#define DLL_API

#include "CompositeErrorManagement.h"
#include "RegularExpression.h"
#include "VariableDescriptor.h"
#include "Private/VariableDescriptorLib.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe{
namespace VariableDescriptorLib{

enum ScanTokens{
	MatrixToken='M',VectorToken='V',ZTAToken='Z',PointerToken='P',ConstToken='C',IdentToken='I',NumberToken='N'
};

enum ScanStatus{
/* processing first part i.e. the type or a templated type like Matrix<type> */
	TypeSide  = 1,
	Template  = 2,
	TypeSide2 = 3,
/* processing the pointer modifiers */
	LeftSide  = 11,
//	LeftSideConst,
/* processing the [] modifiers */
	RightSide = 21,
	Array     = 22,
	ArrayEnd  = 23,

	Finished = 99
};

static const RegularExpression::PatternInformation scanRules[]={
            {"Matrix"                                                                            ,"Matrix"           , MatrixToken ,false},
            {"Vector"                                                                            ,"Vector"           , VectorToken ,false},
            {"ZeroTerminatedArray"                                                               ,"ZTA"              , ZTAToken    ,false},
            {"\\*"                                                                               ,"Pointer"          , PointerToken,false},
            {"const"                                                                             ,"Const"            , ConstToken  ,false},
            {"+[ \r\n\t,;]"                                                                      ,"separator"        , 0           ,true},
            {"<"                                                                                 ,"open<"            , '<'         ,false},
            {">"                                                                                 ,"close>"           , '>'         ,false},
            {"\\["                                                                               ,"open ["           , '['         ,false},
            {"\\]"                                                                               ,"close ]"          , ']'         ,false},
            {"\\("                                                                               ,"open("            , '('         ,false},
            {"\\)"                                                                               ,"close)"           , ')'         ,false},
            {"[\\w_]*[\\d\\w_]"                                                                  ,"identifier"       , IdentToken  ,false},
            {"($BODY(+\\d)$FRACTION(?(.*\\d))|$FRACTION(.*\\d))?([eE]!$EXP(?[+\\-]{1,5}\\d))"    ,"number"           , NumberToken ,false},
            {"\\"                                                                                ,"EOF"              , 0           ,false},
            RegularExpression::emptyPattern
};

void toLower(char8 &c){
	if ((c >= 'A') && (c <= 'Z' )){
		c = c + 32;
	}
}

DLL_API ErrorManagement::ErrorType FromString(
		DynamicCString &				modifiers,
		TypeDescriptor &				typeDescriptor,
		CCString &						typeName,
//		VariableDescriptor &			vd,
		bool 							modifiersOnly){

	ErrorManagement::ErrorType ret;
	ScanStatus status = TypeSide;
//printf("scanning  {%s} \n",typeName.GetList());

	if (modifiersOnly){
		status = LeftSide;
	}

	// the calculated TypeDescriptor
	typeDescriptor = InvalidType(0);

	// the modifier of the type from the scan of the first part
	DynamicCString typeSide;
	// the area with * const *
	DynamicCString leftSide;
	// the area between ()
	DynamicCString coreSide;
	// the area of the [][]
	DynamicCString rightSide;
	bool constActive = false;

	while (ret && (status != Finished)){
		const RegularExpression::PatternInformation *selectedRule;
		DynamicCString matched;
		ret = MatchRules(typeName,scanRules,selectedRule,matched);
//printf("%c %s %s  --> {%s}\n",selectedRule->ruleId,selectedRule->ruleName.GetList(),matched.GetList(),typeName.GetList());

		if (ret && !selectedRule->skip){
			switch (selectedRule->ruleId){
			case IdentToken: {
				ret.syntaxError = (status != TypeSide);
				REPORT_ERROR(ret,"found Identifier in the wrong status");

				if (ret){
					typeDescriptor = TypeDescriptor(matched);
					ret.syntaxError = typeDescriptor.IsInvalid();
					REPORT_ERROR(ret,"Invalid type Identifier");
				}

				if (ret){
					status = TypeSide2;
				}
			}break;
			case MatrixToken:{
				ret.syntaxError = (status != TypeSide);
				REPORT_ERROR(ret,"found Matrix in the wrong status");

				if (ret){
					if (constActive){
						typeSide().Append('m');
						constActive = false;
					} else {
						typeSide().Append('M');
					}

					status = Template;
				}
			} break;
			case VectorToken:{
				ret.syntaxError = (status != TypeSide);
				REPORT_ERROR(ret,"found Vector in the wrong status");

				if (ret){
					if (constActive){
						typeSide().Append('v');
						constActive = false;
					} else {
						typeSide().Append('V');
					}

					status = Template;
				}
			} break;
			case ZTAToken:{
				ret.syntaxError = (status != TypeSide);
				REPORT_ERROR(ret,"found ZeroTerminatedArray in the wrong status");

				if (ret){
					if (constActive){
						typeSide().Append('z');
						constActive = false;
					} else {
						typeSide().Append('Z');
					}

					status = Template;
				}
			} break;
			case ConstToken:{
				ret.syntaxError = (status > LeftSide);
				REPORT_ERROR(ret,"found const in the right side");

				if (status == TypeSide2){
					if (typeSide.GetSize() > 0){
						toLower(typeSide[0]);
					} else {
						// modify the type
						typeDescriptor.SetDataConstant(true);
					}
				} else
				if (status == TypeSide){
					constActive = true;
				} else
				if (status == LeftSide){
					toLower(leftSide[leftSide.GetSize()-1]);
				}
			}break;
			case '<':{
				ret.syntaxError = (status != Template);
				REPORT_ERROR(ret,"found < not after a template type");
				DynamicCString  typeSide2;
				if (ret){
					ret = FromString(typeSide2,typeDescriptor,typeName,false);
					REPORT_ERROR(ret,"error in scan of the template <argument>");
				}
				if (ret){
//					td = vd2.GetFinalTypeDescriptor();
					typeSide().Append(typeSide2);
					status = TypeSide2;
				}
			}break;
			case PointerToken:{
				ret.syntaxError = ((status != TypeSide2) && (status != LeftSide));
				REPORT_ERROR(ret,"found * in the wrong status");

				if (ret){
					status = LeftSide;
					leftSide().Append('P');
				}
			}break;
			case '(':{
				ret.syntaxError = ((status != TypeSide2) && (status != LeftSide));
				REPORT_ERROR(ret,"found * in the wrong status");
//				VariableDescriptor vd2;
				TypeDescriptor dummy;
				if (ret){
					ret = FromString(coreSide,dummy,typeName,true);
					REPORT_ERROR(ret,"error in scan of the core type modifiers (argument)");
				}
				if (ret){
//					coreSide().Append(vd2.GetModifiers());
					status = RightSide;
				}
			}break;
			case '[':{
				ret.syntaxError = ((status != TypeSide2) && (status != LeftSide) && (status != RightSide));
				REPORT_ERROR(ret,"found [ in the wrong status");

				if (ret){
					status = Array;
				}
			}break;
			case NumberToken:{
				ret.syntaxError = (status != Array) ;
				REPORT_ERROR(ret,"found number not in the Array status");

				if (ret){
					rightSide().Append('A').Append(matched);
					status = ArrayEnd;
				}
			}break;
			case ']':{
				ret.syntaxError = (status != ArrayEnd) ;
				REPORT_ERROR(ret,"found ] not in the ArrayEnd status");

				if (ret){
					status = RightSide;
				}
			}break;

			case 0 :
			case ')':
			case '>':{
				ret.syntaxError = ((status != TypeSide2) && (status != RightSide) && (status != LeftSide));
				REPORT_ERROR(ret,"found >) or EOF not in a valid status");
				status = Finished;

			}break;

			default:{
				ret.fatalError = true;
			}break;

			};
		}
	}

	// construct the vd modifier string
	if (ret){

//printf(" {%s} {%s} {%s} {%s}\n",typeSide.GetList(),leftSide.GetList(),coreSide.GetList(),rightSide.GetList());

		modifiers = coreSide;
		// append RightSide;
		int i = 0;
		char8 lastChar = 0;
		if (modifiers.GetSize()> 0){
			lastChar = modifiers[modifiers.GetSize()-1];
		}
		for (i=0;i<static_cast<int>(rightSide.GetSize());i++){
			char8 c= rightSide[i];
			if ((c=='A') && (lastChar == 'P')){
				modifiers[modifiers.GetSize()-1] = 'F';
			} else
			if ((c=='A') && (lastChar == 'p')){
				modifiers[modifiers.GetSize()-1] = 'f';
			} else {
				modifiers().Append(c);
			}
			lastChar = c;
		}

		// append LeftSide in reverse;
		for (i=static_cast<int>(leftSide.GetSize())-1;i>=0;i--){
			char8 c= leftSide[i];
			modifiers().Append(c);
		}
		// append typeSide ;
		modifiers().Append(typeSide);

	}


	return ret;
}

}
} //MARTe
