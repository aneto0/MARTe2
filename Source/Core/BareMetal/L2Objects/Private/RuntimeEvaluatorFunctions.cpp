/**
 * @file RuntimeEvaluatorFunctions.cpp
 * @brief Header file for class AnyType
 * @date 8 Apr 2020
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

#include <math.h>
#include <Private/RuntimeEvaluatorFunctions.h>
#include <Private/RuntimeEvaluatorVariableInformation.h>
#include <RuntimeEvaluator.h>
#include "SafeMath.h"
#include "ErrorManagement.h"
#include "RuntimeEvaluator.h"
#include "AnyType.h"


namespace MARTe{

uint32 availableFunctions = 0;

RuntimeEvaluatorFunction functionRecords[maxFunctions];



ErrorManagement::ErrorType  RuntimeEvaluatorFunction::Check(
		CCString 									nameIn,
		Stack<AnyType> &					        typeStack,
		bool 										matchOutput
){
	ErrorManagement::ErrorType ret;

	// match function name
    ret.comparisonFailure = !(name == nameIn);

    AnyType at;

    // match first output if matchOutput is set
    uint32 index = 0U;
    if (ret && matchOutput){
        ret = typeStack.Peek(at,index++);
        COMPOSITE_REPORT_ERROR(ret,"typeStack.Peek(at,",index-1,") failed");
    }
    VariableDescriptor vd;
    if (ret && matchOutput){
        vd = at.GetFullVariableDescriptor();
        ret.comparisonFailure = !types[numberOfInputs].SameAs(vd);
#if 0
if (ret){
    DynamicCString cs;
    CStringTool cst = cs();
    cst.Append("(vd:)");
    vd.ToString(cst);
    cst.Append("==(types[i]:)");
    types[numberOfInputs].ToString(cst);
    printf("%i outputMatch=%s\n",(types[numberOfInputs].GetModifiers() == vd.GetModifiers()),cs.GetList());
}
#endif
    }

    // match inputs types
    for (uint32 i = 0U; ret && (i < numberOfInputs); i++){
        ret = typeStack.Peek(at,index++);
        COMPOSITE_REPORT_ERROR(ret,"typeStack.Peek(vd,",index-1,") failed");
        if (ret){
            vd = at.GetFullVariableDescriptor();
            ret.comparisonFailure = !vd.SameAs(types[i]);
        }
#if 0
if (ret){
    DynamicCString cs;
    CStringTool cst = cs();
    cst.Append("(vd:)");
    vd.ToString(cst);
    cst.Append("==(types[i]:)");
    types[i].ToString(cst);
    printf("inputMatch=%s\n",cs.GetList());
}
#endif
    }

    return ret;
}

/**
 * to register a function
 */
void RegisterFunction(const RuntimeEvaluatorFunction &record){
    if (availableFunctions < maxFunctions){
        functionRecords[availableFunctions++] = record;
    }
}

ErrorManagement::ErrorType  FindPCode(
		RuntimeEvaluator::CodeMemoryElement &   code,
		CCString 					            nameIn,
		Stack<AnyType> &				        typeStack,
		bool 								    matchOutput
)
{
	ErrorManagement::ErrorType ret;

    RuntimeEvaluator::CodeMemoryElement i = 0;
    code = RuntimeEvaluator::InvalidCodeMemoryElement;
    for (i=0; ret && (i < availableFunctions);i++ ){
//        ret = functionRecords[i].TryConsume(nameIn,typeStack,matchOutput,dataStackSize);
    	ret = functionRecords[i].Check(nameIn,typeStack,matchOutput);

    	if (ret){
            code = i;
            // force exit
            i = availableFunctions;
    	} else {
            ret.comparisonFailure = false;
    	}
    }
    ret.comparisonFailure = (code == RuntimeEvaluator::InvalidCodeMemoryElement);

    return ret;
}
#if 0
static inline int toDigit(char8 c){
	int ret = -1;
	if ((c <= '9') && (c >= '0')){
		ret = static_cast<int>(c) - static_cast<int>('0');
	}
	return ret;
}
#endif
#if 0
static inline uint32 toUint32(CCString &s){
	uint32 ret = 0;

	int digit = 0;
	while ((digit = toDigit(s[0])) > 0) {
		ret = ret *10 + static_cast<uint32>(digit);
		s++;
	}
	return ret;
}
#endif
#if 0
static ErrorManagement::ErrorType GetMatrixInfo(const VariableDescriptor &vd,uint32 &nRows, uint32 &nColumns){

	ErrorManagement::ErrorType ret;
    TypeDescriptor td 		= vd.GetFinalTypeDescriptor();
    CCString modifiers 		= vd.GetModifiers();

	ret.internalSetupError = (!td.SameAs(Float32Bit) ) && (!td.SameAs(Float64Bit));

	if (ret){

	}

    if (ret){
    	ret.internalSetupError = (modifiers[0] != 'A');
    	modifiers++;
    }
    if (ret){
        nRows = toUint32(modifiers);

        ret.internalSetupError = (modifiers[0] != 'A');
    	modifiers++;
    }

    if (ret){
        nColumns = toUint32(modifiers);

        ret.internalSetupError = (modifiers[0] != '\0');
    }

    return ret;
}
#endif

static inline ErrorManagement::ErrorType IsValidMatrix(bool& isValidMatrix,const AnyType &at,uint32 &nRows, uint32 &nColumns){
    ErrorManagement::ErrorType ret;
    VariableDescriptor vd = at.GetFullVariableDescriptor();
    CCString modifiers = CCString(vd.GetModifiers());

    // to avoid accessing NULL ptrs
    isValidMatrix = (modifiers.GetSize() != 0);

    if (isValidMatrix){
        isValidMatrix = (( modifiers == "M") || (modifiers == "m"));
    }

    if (isValidMatrix){
        isValidMatrix = ((vd.GetFinalTypeDescriptor().SameAs(Float32Bit)) || (vd.GetFinalTypeDescriptor().SameAs(Float64Bit)));
    }

    if (isValidMatrix){
        ret.internalSetupError = (at.GetVariablePointer() == NULL);
    }

    if (isValidMatrix && ret){
        const Matrix<float> *mf = reinterpret_cast<const Matrix<float> *> (at.GetVariablePointer());
        nRows = mf->GetNumberOfRows();
        nRows = mf->GetNumberOfColumns();
    }

    return ret;
}



ErrorManagement::ErrorType RuntimeEvaluatorFunction::UpdateStack(
		Stack<AnyType> &							        typeStack,
		RuntimeEvaluator::DataMemoryAddress & 			    dataStackSize,
		List<RuntimeEvaluator::VariableInformation> &	    db,
		bool 												matchOutput,
		uint32 &                                            dummyID){

	ErrorManagement::ErrorType ret;

	if (updateStackFun != NULL){
		return updateStackFun(*this,typeStack,dataStackSize,db,dummyID);
	}

	// remove first element which is the matched output type
    if (ret && matchOutput){
        AnyType at;
        ret = typeStack.Pop(at);
        REPORT_ERROR(ret,"typeStack.Pop(vd) failed");
    }

    // remove inputs types
    for (uint32 i = 0U; ret && (i < numberOfInputs); i++){
        AnyType at;
        ret = typeStack.Pop(at);
        REPORT_ERROR(ret,"typeStack.Pop(at) failed");

        bool isValidMatrix = false;
        uint32 nc=0,nr=0;
        if (ret){
            ret = IsValidMatrix(isValidMatrix,at,nc,nr);
            REPORT_ERROR(ret,"error in IsValidMatrix");
        }

        if (ret){
        	if (isValidMatrix){
                dataStackSize -= RuntimeEvaluator::ByteSizeToDataMemorySize(sizeof(RuntimeEvaluator::DataMemoryAddress));
        	} else {
                dataStackSize -= RuntimeEvaluator::ByteSizeToDataMemorySize(at.GetFullVariableDescriptor().GetSummaryTypeDescriptor().StorageSize());
        	}
        }
    }
    // insert output types
    for (uint32 i = 0U; ret && (i < numberOfOutputs); i++){
    	VariableDescriptor vd = types[i+numberOfInputs];
        ret = typeStack.Push(AnyType(vd,NULL));
        REPORT_ERROR(ret,"typeStack.Push(..) failed");
        if (ret){
        	uint32 nc=0,nr=0;
        	// TODO FIX
        	// in this method we do not support matrices as output
        	bool isValidMatrix = true;
        	ret = IsValidMatrix(isValidMatrix,AnyType(vd,NULL),nr,nc);
            REPORT_ERROR(ret,"cannot handle matrices as output");
        }
        if (ret){
            dataStackSize += RuntimeEvaluator::ByteSizeToDataMemorySize(vd.GetSummaryTypeDescriptor().StorageSize());
        }
    }

    return ret;
}

static const VariableDescriptor Float32BitMatrix = VariableDescriptor(Float32Bit,"M");
static const VariableDescriptor Float64BitMatrix = VariableDescriptor(Float64Bit,"M");

ErrorManagement::ErrorType Matrix_Write_UpdateStack(
        const RuntimeEvaluatorFunction &                    ref,
        Stack<AnyType> &                                    typeStack,
        RuntimeEvaluator::DataMemoryAddress &               dataStackSize,
        List<RuntimeEvaluator::VariableInformation> &       db,
        uint32 &                                            dummyID){

    ErrorManagement::ErrorType ret;

    AnyType at1,at2;
    ret = typeStack.Pop(at1);
    REPORT_ERROR(ret,"typeStack.Pop(at1) failed");
    dataStackSize -= RuntimeEvaluator::ByteSizeToDataMemorySize(sizeof(RuntimeEvaluator::DataMemoryAddress));

    if (ret){
        ret = typeStack.Pop(at2);
        REPORT_ERROR(ret,"typeStack.Pop(at2) failed");
        dataStackSize -= RuntimeEvaluator::ByteSizeToDataMemorySize(sizeof(RuntimeEvaluator::DataMemoryAddress));
    }

    if (ret){
        ret.unsupportedFeature = !(at1.GetFullVariableDescriptor().SameAs(at2.GetFullVariableDescriptor()));
        REPORT_ERROR(ret,"error expecting two matrices");
    }

    uint32 nRows        = 1;
    uint32 nColumns     = 1;
    bool isValidMatrix  = false;

    if (ret){
        ret = IsValidMatrix(isValidMatrix,at1,nColumns,nRows);
        REPORT_ERROR(ret,"error first param not valid Matrix");
    }

    if (ret){
        ret.internalSetupError = !isValidMatrix;
        REPORT_ERROR(ret,"error expecting valid matrix as param 1");
    }

    uint32 nRows2        = 1;
    uint32 nColumns2     = 1;
    if (ret){
        ret = IsValidMatrix(isValidMatrix,at2,nColumns2,nRows2);
        REPORT_ERROR(ret,"error second param not valid Matrix");
    }

    if (ret){
        ret.internalSetupError = !isValidMatrix;
        REPORT_ERROR(ret,"error expecting valid matrix as param 2");
    }

    if (ret){
        ret.unsupportedFeature = (nRows != nRows2) || (nColumns != nColumns2);
        REPORT_ERROR(ret,"error expecting matrices of same size");
    }

    return ret;
}


template <typename T> void MatrixWrite(RuntimeEvaluator &context){
    RuntimeEvaluator::CodeMemoryElement index;
    index = context.GetPseudoCode();
    context.Push(index);

    RuntimeEvaluator::DataMemoryAddress  y1;
    context.Pop(y1);
    RuntimeEvaluator::CodeMemoryElement yOut;
    yOut = context.GetPseudoCode();

    Matrix<T> &z1   = context.Variable< Matrix<T> >(y1);
    Matrix<T> &zOut = context.Variable< Matrix<T> >(yOut);

    if (!zOut.Copy(z1)){
        context.runtimeError.internalSetupError = true;
    }
    context.Push(yOut);
}


REGISTER_PCODE_MATRIX_FUNCTION(WRITE,float32_M,1,0,MatrixWrite<float32>,Matrix_Write_UpdateStack,Float32BitMatrix,Float32BitMatrix)
REGISTER_PCODE_MATRIX_FUNCTION(WRITE,float64_M,1,0,MatrixWrite<float64>,Matrix_Write_UpdateStack,Float64BitMatrix,Float64BitMatrix)

ErrorManagement::ErrorType Matrix_Read_UpdateStack(
        const RuntimeEvaluatorFunction &                    ref,
        Stack<AnyType> &                                    typeStack,
        RuntimeEvaluator::DataMemoryAddress &               dataStackSize,
        List<RuntimeEvaluator::VariableInformation> &       db,
        uint32 &                                            dummyID){

    ErrorManagement::ErrorType ret;

    return ret;
}

void MatrixRead(RuntimeEvaluator &context){
    RuntimeEvaluator::CodeMemoryElement index;
    index = context.GetPseudoCode();
    context.Push(index);
}


REGISTER_PCODE_MATRIX_FUNCTION(READ,float32_M,0,1,MatrixRead,Matrix_Read_UpdateStack,Float32BitMatrix)
REGISTER_PCODE_MATRIX_FUNCTION(READ,float64_M,0,1,MatrixRead,Matrix_Read_UpdateStack,Float64BitMatrix)


ErrorManagement::ErrorType Matrix_Addition_UpdateStack(
        const RuntimeEvaluatorFunction &                    ref,
		Stack<AnyType> &							        typeStack,
		RuntimeEvaluator::DataMemoryAddress & 			    dataStackSize,
		List<RuntimeEvaluator::VariableInformation> &	    db,
		uint32 &                                            dummyID){

	ErrorManagement::ErrorType ret;

	AnyType at1,at2;
    ret = typeStack.Pop(at1);
    REPORT_ERROR(ret,"typeStack.Pop(at1) failed");
    dataStackSize -= RuntimeEvaluator::ByteSizeToDataMemorySize(sizeof(RuntimeEvaluator::DataMemoryAddress));

    if (ret){
        ret = typeStack.Pop(at2);
        REPORT_ERROR(ret,"typeStack.Pop(at2) failed");
        dataStackSize -= RuntimeEvaluator::ByteSizeToDataMemorySize(sizeof(RuntimeEvaluator::DataMemoryAddress));
    }

    if (ret){
    	ret.unsupportedFeature = !(at1.GetFullVariableDescriptor().SameAs(at2.GetFullVariableDescriptor()));
        REPORT_ERROR(ret,"error expecting two matrices");
    }

    uint32 nRows        = 1;
    uint32 nColumns     = 1;
    bool isValidMatrix  = false;

    if (ret){
        ret = IsValidMatrix(isValidMatrix,at1,nColumns,nRows);
        REPORT_ERROR(ret,"error first param not valid Matrix");
    }

    if (ret){
        ret.internalSetupError = !isValidMatrix;
        REPORT_ERROR(ret,"error expecting valid matrix as param 1");
    }

    uint32 nRows2        = 1;
    uint32 nColumns2     = 1;
    if (ret){
        ret = IsValidMatrix(isValidMatrix,at2,nColumns2,nRows2);
        REPORT_ERROR(ret,"error second param not valid Matrix");
    }

    if (ret){
        ret.internalSetupError = !isValidMatrix;
        REPORT_ERROR(ret,"error expecting valid matrix as param 2");
    }

    if (ret){
        ret.unsupportedFeature = (nRows != nRows2) || (nColumns != nColumns2);
        REPORT_ERROR(ret,"error expecting matrices of same size");
    }

    // prepare the space for the intermediate results
    RuntimeEvaluator::VariableInformation variableInformation;

    if (ret){
        dataStackSize += sizeof(RuntimeEvaluator::DataMemoryAddress);

        // need to allocate variable and memory
        DynamicCString name;
        name().Append("Temp").Append('@').Append(dummyID++);
        variableInformation.SetName(name);
        // extracts matrix size from at1
        ret = variableInformation.SetType(at1);
        REPORT_ERROR(ret,"error setting type of variableInformation");
    }

    if (ret){
        ret = variableInformation.AllocateMatrixMemory();
        REPORT_ERROR(ret,"error variableInformation external memory");
    }
#if 0
    if (ret){
        AnyType at(at1.GetFullVariableDescriptor(),);
        ret = typeStack.Push(at);
        REPORT_ERROR(ret,"error adding result type to stack");
    }
#endif
    if (ret){
        // should move memory management responsibility to new member of database
    	ret = db.Insert(variableInformation );
    }

    return ret;
}

template <typename T> void Matrix_Addition(RuntimeEvaluator &context){
	RuntimeEvaluator::DataMemoryAddress  y1,y2;
    context.Pop(y1);
    context.Pop(y2);
    RuntimeEvaluator::CodeMemoryElement yOut;
    yOut = context.GetPseudoCode();

    Matrix<T> &z1   = context.Variable< Matrix<T> >(y1);
    Matrix<T> &z2   = context.Variable< Matrix<T> >(y2);
    Matrix<T> &zOut = context.Variable< Matrix<T> >(yOut);

    if (!z1.Sum(z2,zOut)){
    	context.runtimeError.internalSetupError = true;
    }
    context.Push(yOut);
}

REGISTER_PCODE_MATRIX_FUNCTION(ADD,float32_M,2,1,Matrix_Addition<float32>,Matrix_Addition_UpdateStack,Float32BitMatrix,Float32BitMatrix,Float32BitMatrix)
REGISTER_PCODE_MATRIX_FUNCTION(ADD,float64_M,2,1,Matrix_Addition<float64>,Matrix_Addition_UpdateStack,Float64BitMatrix,Float64BitMatrix,Float64BitMatrix)

/*********************************************************************************************************
 *********************************************************************************************************
 *
 *                      Type casting operators
 *
 *********************************************************************************************************
 **********************************************************************************************************/


#define REGISTER_CAST_FUNCTION(name,type1,type2,function)\
    static const VariableDescriptor name ## type1 ## type2 ## _FunctionTypes[] = {Type2TypeDescriptor<type1>(), Type2TypeDescriptor<type2>()}; \
    static const RuntimeEvaluatorFunction name ## type1 ## type2 ## _FunctionRecord={#name,1,1,name ## type1 ## type2 ## _FunctionTypes,&function<type1,type2>,NULLUpdateStackFun}; \
    static class name ## type1 ## type2 ## RegisterClass { \
    public: name ## type1 ## type2 ## RegisterClass(){\
            RegisterFunction(name ## type1 ## type2 ## _FunctionRecord);\
        }\
    } name ## type1 ## type2 ## RegisterClassInstance;

template <typename T1,typename T2> void Casting(RuntimeEvaluator &context){
    T1 x1;
    T2 x2;
    bool ret;
    context.Pop(x1);
    ret = SafeNumber2Number(x1,x2);
    context.Push(x2);
    if (!ret){
        context.runtimeError.outOfRange = true;
    }
}

#define REGISTER_CAST_FUNCTION_BLOCK(type1,function)   \
  REGISTER_CAST_FUNCTION(CAST,type1,float64,function)  \
  REGISTER_CAST_FUNCTION(CAST,type1,float32,function)  \
  REGISTER_CAST_FUNCTION(CAST,type1,uint64 ,function)  \
  REGISTER_CAST_FUNCTION(CAST,type1,int64  ,function)  \
  REGISTER_CAST_FUNCTION(CAST,type1,uint32 ,function)  \
  REGISTER_CAST_FUNCTION(CAST,type1,int32  ,function)  \
  REGISTER_CAST_FUNCTION(CAST,type1,uint16 ,function)  \
  REGISTER_CAST_FUNCTION(CAST,type1,int16  ,function)  \
  REGISTER_CAST_FUNCTION(CAST,type1,uint8  ,function)  \
  REGISTER_CAST_FUNCTION(CAST,type1,int8   ,function)

REGISTER_CAST_FUNCTION_BLOCK(float64,Casting)
REGISTER_CAST_FUNCTION_BLOCK(float32,Casting)
REGISTER_CAST_FUNCTION_BLOCK(uint64 ,Casting)
REGISTER_CAST_FUNCTION_BLOCK(int64  ,Casting)
REGISTER_CAST_FUNCTION_BLOCK(uint32 ,Casting)
REGISTER_CAST_FUNCTION_BLOCK(int32  ,Casting)
REGISTER_CAST_FUNCTION_BLOCK(uint16 ,Casting)
REGISTER_CAST_FUNCTION_BLOCK(int16  ,Casting)
REGISTER_CAST_FUNCTION_BLOCK(uint8  ,Casting)
REGISTER_CAST_FUNCTION_BLOCK(int8   ,Casting)

/*********************************************************************************************************
 *********************************************************************************************************
 *
 *                      float math functions
 *
 *********************************************************************************************************
 **********************************************************************************************************/

#define REGISTER_1_FUNCTION(name,fname)                                 \
        template <typename T> void function ## fname ## ication (RuntimeEvaluator &context){ \
            T x,res;                                                                \
            context.Pop(x);                                                         \
            res = fname (x);                                                        \
            context.Push(res);                                                      \
        }                                                                           \
        REGISTER_PCODE_FUNCTION(name,float32,1,1,function ## fname ## ication <float32>,Float32Bit,Float32Bit)  \
        REGISTER_PCODE_FUNCTION(name,float64,1,1,function ## fname ## ication <float64>,Float64Bit,Float64Bit)

REGISTER_1_FUNCTION(SIN,sin)
REGISTER_1_FUNCTION(COS,cos)
REGISTER_1_FUNCTION(TAN,tan)
REGISTER_1_FUNCTION(EXP,exp)
REGISTER_1_FUNCTION(LOG,log)
REGISTER_1_FUNCTION(LOG10,log10)

#define REGISTER_2_FUNCTION(name,fname)                                 \
        template <typename T> void function ## fname ## ication (RuntimeEvaluator &context){ \
            T x1,x2,res;                                                            \
            context.Pop(x1);                                                        \
            context.Pop(x2);                                                        \
            res = fname (x2,x1);                                                    \
            context.Push(res);                                                      \
        }                                                                           \
        REGISTER_PCODE_FUNCTION(name,float32,2,1,function ## fname ## ication <float32>,Float32Bit,Float32Bit,Float32Bit)  \
        REGISTER_PCODE_FUNCTION(name,float64,2,1,function ## fname ## ication <float64>,Float64Bit,Float64Bit,Float64Bit)

REGISTER_2_FUNCTION(POW,pow)


/*********************************************************************************************************
 *********************************************************************************************************
 *
 *                      Same Type Comparison operators
 *
 *********************************************************************************************************
 **********************************************************************************************************/

#define REGISTER_COMPARE_OPERATOR(name,oper,fname)                                  \
        template <typename T> void function ## fname ## ication (RuntimeEvaluator &context){ \
            T x1,x2;                                                                \
            bool ret;                                                               \
            context.Pop(x1);                                                        \
            context.Pop(x2);                                                        \
            ret = x2 oper x1;                                                       \
            context.Push(ret);                                                      \
        }                                                                           \
        REGISTER_PCODE_FUNCTION(name,float64,2,1,function ## fname ## ication <float64>,Float64Bit          ,Float64Bit             ,UnsignedInteger8Bit) \
        REGISTER_PCODE_FUNCTION(name,float32,2,1,function ## fname ## ication <float32>,Float32Bit          ,Float32Bit             ,UnsignedInteger8Bit) \
        REGISTER_PCODE_FUNCTION(name,uint64 ,2,1,function ## fname ## ication <uint64> ,UnsignedInteger64Bit,UnsignedInteger64Bit   ,UnsignedInteger8Bit) \
        REGISTER_PCODE_FUNCTION(name,int64  ,2,1,function ## fname ## ication <int64>  ,SignedInteger64Bit  ,SignedInteger64Bit     ,UnsignedInteger8Bit) \
        REGISTER_PCODE_FUNCTION(name,uint32 ,2,1,function ## fname ## ication <uint32> ,UnsignedInteger32Bit,UnsignedInteger32Bit   ,UnsignedInteger8Bit) \
        REGISTER_PCODE_FUNCTION(name,int32  ,2,1,function ## fname ## ication <int32>  ,SignedInteger32Bit  ,SignedInteger32Bit     ,UnsignedInteger8Bit) \
        REGISTER_PCODE_FUNCTION(name,uint16 ,2,1,function ## fname ## ication <uint16> ,UnsignedInteger16Bit,UnsignedInteger16Bit   ,UnsignedInteger8Bit) \
        REGISTER_PCODE_FUNCTION(name,int16  ,2,1,function ## fname ## ication <int16>  ,SignedInteger16Bit  ,SignedInteger16Bit     ,UnsignedInteger8Bit) \
        REGISTER_PCODE_FUNCTION(name,uint8  ,2,1,function ## fname ## ication <uint8>  ,UnsignedInteger8Bit,UnsignedInteger8Bit     ,UnsignedInteger8Bit) \
        REGISTER_PCODE_FUNCTION(name,int8   ,2,1,function ## fname ## ication <int8>   ,SignedInteger8Bit   ,SignedInteger8Bit      ,UnsignedInteger8Bit)

REGISTER_COMPARE_OPERATOR(EQ, == ,Equal)
REGISTER_COMPARE_OPERATOR(NEQ, != ,Different)
REGISTER_COMPARE_OPERATOR(GT, > ,Greater)
REGISTER_COMPARE_OPERATOR(LT, < ,Smaller)
REGISTER_COMPARE_OPERATOR(GTE, >= ,Great)
REGISTER_COMPARE_OPERATOR(LTE, <= ,Small)

/*********************************************************************************************************
 *********************************************************************************************************
 *
 *                      Same Type logical operators
 *
 *********************************************************************************************************
 **********************************************************************************************************/


#define REGISTER_LOGICAL_OPERATOR(name,oper,fname)                                  \
        void function ## fname ## ication (RuntimeEvaluator &context){                       \
            bool x1,x2,ret;                                                         \
            context.Pop(x1);                                                        \
            context.Pop(x2);                                                        \
            ret = x2 oper x1;                                                       \
            context.Push(ret);                                                      \
        }                                                                           \
        REGISTER_PCODE_FUNCTION(name,boolean,2,1,function ## fname ## ication,UnsignedInteger8Bit,UnsignedInteger8Bit,UnsignedInteger8Bit)

REGISTER_LOGICAL_OPERATOR(AND, && ,And)
REGISTER_LOGICAL_OPERATOR(OR, || ,Or)
REGISTER_LOGICAL_OPERATOR(XOR, ^ ,xor)


/*********************************************************************************************************
 *********************************************************************************************************
 *
 *                      Same Type converting float math operators
 *
 *********************************************************************************************************
 **********************************************************************************************************/

#define REGISTER_OPERATOR(name,oper,fname)                                          \
        template <typename T> void function ## fname ## ication (RuntimeEvaluator &context){ \
            T x1,x2,x3;                                                             \
            context.Pop(x1);                                                        \
            context.Pop(x2);                                                        \
            x3 = static_cast<T>(x2 oper x1);                                        \
            context.Push(x3);                                                       \
        }                                                                           \
        REGISTER_PCODE_FUNCTION(name,float64,2,1,function ## fname ## ication <float64>,Float64Bit,Float64Bit,Float64Bit)  \
        REGISTER_PCODE_FUNCTION(name,float32,2,1,function ## fname ## ication <float32>,Float32Bit,Float32Bit,Float32Bit)  \

REGISTER_OPERATOR(ADD, + ,Addition)
REGISTER_OPERATOR(SUB, - ,Subtract)
REGISTER_OPERATOR(MUL, * ,Multipl)
REGISTER_OPERATOR(DIV, / ,Division)

/*********************************************************************************************************
 *********************************************************************************************************
 *
 *                      Type converting Integer math operators
 *
 *********************************************************************************************************
 **********************************************************************************************************/

template <typename T1,typename T2,typename Tout> void Addition_3T(RuntimeEvaluator &context){
    Tout x1,x2,x3;
    T1 y1;
    T2 y2;
    context.Pop(y1);
    context.Pop(y2);
    x1 = static_cast<Tout>(y1);
    x2 = static_cast<Tout>(y2);
    x3 = static_cast<Tout>(x2 + x1);
    context.Push(x3);
}

template <typename T1,typename T2,typename Tout> void Subtraction_3T(RuntimeEvaluator &context){
    Tout x1,x2,x3;
    T1 y1;
    T2 y2;
    context.Pop(y1);
    context.Pop(y2);
    x1 = static_cast<Tout>(y1);
    x2 = static_cast<Tout>(y2);
    x3 = static_cast<Tout>(x2 - x1);
    context.Push(x3);
}

template <typename T1,typename T2,typename Tout> void Multiplication_3T(RuntimeEvaluator &context){
    Tout x1,x2,x3;
    T1 y1;
    T2 y2;
    context.Pop(y1);
    context.Pop(y2);
    x1 = static_cast<Tout>(y1);
    x2 = static_cast<Tout>(y2);
    x3 = static_cast<Tout>(x2 * x1);
    context.Push(x3);
}

template <typename T1,typename T2,typename Tout> void Division_3T(RuntimeEvaluator &context){
    Tout x1,x2,x3=0;
    T1 y1;
    T2 y2;
    context.Pop(y1);
    context.Pop(y2);
    if (y2 != 0) {
        x1 = static_cast<Tout>(y1);
        x2 = static_cast<Tout>(y2);
        x3 = static_cast<Tout>(x2 / x1);
    } else {
        context.runtimeError.overflow = true;
    }
    context.Push(x3);
}

template <typename T1,typename T2,typename Tout> void SAddition_3T(RuntimeEvaluator &context){
    if (context.runtimeError){
        Tout x1,x2,x3;
        T1 y1;
        T2 y2;
        context.Pop(y1);
        context.Pop(y2);
        x1 = static_cast<Tout>(y1);
        x2 = static_cast<Tout>(y2);
        context.runtimeError = SafeMath::Addition(x2,x1,x3);
        context.Push(x3);
    }
}

template <typename T1,typename T2,typename Tout> void SSubtraction_3T(RuntimeEvaluator &context){
    if (context.runtimeError){
        Tout x1,x2,x3;
        T1 y1;
        T2 y2;
        context.Pop(y1);
        context.Pop(y2);
        x1 = static_cast<Tout>(y1);
        x2 = static_cast<Tout>(y2);
        context.runtimeError = SafeMath::Subtraction(x2,x1,x3);
        context.Push(x3);
    }
}

template <typename T1,typename T2,typename Tout> void SMultiplication_3T(RuntimeEvaluator &context){
    if (context.runtimeError){
        Tout x1,x2,x3;
        T1 y1;
        T2 y2;
        context.Pop(y1);
        context.Pop(y2);
        x1 = static_cast<Tout>(y1);
        x2 = static_cast<Tout>(y2);
        context.runtimeError = SafeMath::Multiplication(x2,x1,x3);
        context.Push(x3);
    }
}

template <typename T1,typename T2,typename Tout> void SDivision_3T(RuntimeEvaluator &context){
    Tout x1,x2,x3=0;
    T1 y1;
    T2 y2;
    context.Pop(y1);
    context.Pop(y2);
    bool sat1 = SafeNumber2Number(y1,x1);
    bool sat2 = SafeNumber2Number(y2,x2);
    if ((y2 != 0) && sat1 && sat2) {
        x3 = static_cast<Tout>(x2 / x1);
    } else {
        context.runtimeError.overflow = true;
    }
    context.Push(x3);
}

template <typename T1,typename T2,typename Tout> void SSAddition_3T(RuntimeEvaluator &context){
    if (context.runtimeError){
        Tout x1,x2,x3;
        T1 y1;
        T2 y2;
        context.Pop(y1);
        context.Pop(y2);
        bool sat1 = SafeNumber2Number(y1,x1);
        bool sat2 = SafeNumber2Number(y2,x2);
        if (sat1 && sat2) {
            context.runtimeError = SafeMath::Addition(x2,x1,x3);
        } else {
            context.runtimeError.overflow = true;
        }
        context.Push(x3);
    }
}

template <typename T1,typename T2,typename Tout> void SSSubtraction_3T(RuntimeEvaluator &context){
    if (context.runtimeError){
        Tout x1,x2,x3 = 0;
        T1 y1;
        T2 y2;
        context.Pop(y1);
        context.Pop(y2);
        bool sat1 = SafeNumber2Number(y1,x1);
        bool sat2 = SafeNumber2Number(y2,x2);
        if (sat1 && sat2) {
            context.runtimeError = SafeMath::Subtraction(x2,x1,x3);
        } else {
            context.runtimeError.overflow = true;
        }
        context.Push(x3);
    }
}

template <typename T1,typename T2,typename Tout> void SSMultiplication_3T(RuntimeEvaluator &context){
    if (context.runtimeError){
        Tout x1,x2,x3=0;
        T1 y1;
        T2 y2;
        context.Pop(y1);
        context.Pop(y2);
        bool sat1 = SafeNumber2Number(y1,x1);
        bool sat2 = SafeNumber2Number(y2,x2);
        if (sat1 && sat2) {
            context.runtimeError = SafeMath::Multiplication(x2,x1,x3);
        } else {
            context.runtimeError.overflow = true;
        }
        context.Push(x3);
    }
}

// register operators with no differences in input types (type1 op type1)==>typeOut
#define REGISTER_2T_OPERATOR(name,fname,typeIn,typeOut)                                         \
    static Function functionP ## fname ## typeIn ## typeIn ## typeOut = & fname ## _3T<typeIn,typeIn,typeOut>;    \
    REGISTER_PCODE_FUNCTION(name,typeIn ## typeIn ## typeOut,2,1,*functionP ## fname ## typeIn ## typeIn ## typeOut,Type2TypeDescriptor<typeIn>(),Type2TypeDescriptor<typeIn>(),Type2TypeDescriptor<typeOut>())

// register operators with differences in input types (type1 op type2)==>typeOut
#define REGISTER_3T_OPERATOR(name,fname,typeIn1,typeIn2,typeOut)                                            \
    static Function functionP ## fname ## typeIn1 ## typeIn2 ## typeOut = & fname ## _3T<typeIn1,typeIn2,typeOut>;    \
    static Function functionP ## fname ## typeIn2 ## typeIn1 ## typeOut = & fname ## _3T<typeIn2,typeIn1,typeOut>;    \
    REGISTER_PCODE_FUNCTION(name,typeIn1 ## typeIn2 ## typeOut,2,1,*functionP ## fname ## typeIn1 ## typeIn2 ## typeOut,Type2TypeDescriptor<typeIn1>(),Type2TypeDescriptor<typeIn2>(),Type2TypeDescriptor<typeOut>())   \
    REGISTER_PCODE_FUNCTION(name,typeIn2 ## typeIn1 ## typeOut,2,1,*functionP ## fname ## typeIn2 ## typeIn1 ## typeOut,Type2TypeDescriptor<typeIn2>(),Type2TypeDescriptor<typeIn1>(),Type2TypeDescriptor<typeOut>())   \


REGISTER_2T_OPERATOR(ADD, Addition,int8  ,int32)
REGISTER_2T_OPERATOR(ADD, Addition,int16 ,int32)
REGISTER_2T_OPERATOR(ADD, SAddition,int32 ,int64)
REGISTER_2T_OPERATOR(ADD, SAddition,int64 ,int64)
REGISTER_2T_OPERATOR(ADD, Addition,uint8 ,uint32)
REGISTER_2T_OPERATOR(ADD, Addition,uint16,uint32)
REGISTER_2T_OPERATOR(ADD, SAddition,uint32,uint32)
REGISTER_2T_OPERATOR(ADD, Addition,uint32,uint64)
REGISTER_2T_OPERATOR(ADD, SAddition,uint64,uint64)

REGISTER_3T_OPERATOR(ADD, SAddition,int8  ,int32 ,int32)
REGISTER_3T_OPERATOR(ADD, SAddition,int16 ,int32 ,int32)
REGISTER_3T_OPERATOR(ADD, SAddition,uint8 ,int32 ,int32)
REGISTER_3T_OPERATOR(ADD, SAddition,uint16,int32 ,int32)
REGISTER_3T_OPERATOR(ADD, SSAddition,uint32,int32 ,int32)

REGISTER_3T_OPERATOR(ADD, SAddition,int8  ,int64 ,int64)
REGISTER_3T_OPERATOR(ADD, SAddition,int16 ,int64 ,int64)
REGISTER_3T_OPERATOR(ADD, SAddition,int32 ,int64 ,int64)
REGISTER_3T_OPERATOR(ADD, SAddition,uint8 ,int64 ,int64)
REGISTER_3T_OPERATOR(ADD, SAddition,uint16,int64 ,int64)
REGISTER_3T_OPERATOR(ADD, SAddition,uint32,int64 ,int64)
REGISTER_3T_OPERATOR(ADD, SSAddition,uint64,int64 ,int64)

REGISTER_3T_OPERATOR(ADD, SAddition,uint8 ,uint32,uint32)
REGISTER_3T_OPERATOR(ADD, SAddition,uint16,uint32,uint32)

REGISTER_3T_OPERATOR(ADD, SAddition,uint8 ,uint64,uint64)
REGISTER_3T_OPERATOR(ADD, SAddition,uint16,uint64,uint64)
REGISTER_3T_OPERATOR(ADD, SAddition,uint32,uint64,uint64)


REGISTER_2T_OPERATOR(SUB, Subtraction  ,int8         ,int32)
REGISTER_2T_OPERATOR(SUB, Subtraction  ,int16        ,int32)
REGISTER_2T_OPERATOR(SUB, SSubtraction ,int32        ,int32)
REGISTER_2T_OPERATOR(SUB, SSubtraction ,int64        ,int64)
REGISTER_2T_OPERATOR(SUB, Subtraction  ,uint8        ,int32)
REGISTER_2T_OPERATOR(SUB, Subtraction  ,uint16       ,int32)
REGISTER_2T_OPERATOR(SUB, SSSubtraction,uint32       ,int32)
REGISTER_2T_OPERATOR(SUB, SSSubtraction,uint64       ,int64)
REGISTER_3T_OPERATOR(SUB, SSubtraction ,int8  ,int32 ,int32)
REGISTER_3T_OPERATOR(SUB, SSubtraction ,int16 ,int32 ,int32)
REGISTER_3T_OPERATOR(SUB, SSubtraction ,uint8 ,int32 ,int32)
REGISTER_3T_OPERATOR(SUB, SSubtraction ,uint16,int32 ,int32)
REGISTER_3T_OPERATOR(SUB, SSSubtraction,uint32,int32 ,int32)
REGISTER_3T_OPERATOR(SUB, SSubtraction, int8  ,int64 ,int64)
REGISTER_3T_OPERATOR(SUB, SSubtraction, int16 ,int64 ,int64)
REGISTER_3T_OPERATOR(SUB, SSubtraction, int32 ,int64 ,int64)
REGISTER_3T_OPERATOR(SUB, SSubtraction, uint8 ,int64 ,int64)
REGISTER_3T_OPERATOR(SUB, SSubtraction, uint16,int64 ,int64)
REGISTER_3T_OPERATOR(SUB, SSubtraction, uint32,int64 ,int64)
REGISTER_3T_OPERATOR(SUB, SSSubtraction,uint64,int64 ,int64)
REGISTER_3T_OPERATOR(SUB, SSSubtraction,uint8 ,uint32,int32)
REGISTER_3T_OPERATOR(SUB, SSSubtraction,uint16,uint32,int32)
REGISTER_3T_OPERATOR(SUB, SSSubtraction,uint8 ,uint64,int64)
REGISTER_3T_OPERATOR(SUB, SSSubtraction,uint16,uint64,int64)
REGISTER_3T_OPERATOR(SUB, SSSubtraction,uint32,uint64,int64)


REGISTER_2T_OPERATOR(MUL, Multiplication,int8  ,int32)
REGISTER_2T_OPERATOR(MUL, Multiplication,int16 ,int32)
REGISTER_2T_OPERATOR(MUL, Multiplication,int32 ,int64)
REGISTER_2T_OPERATOR(MUL, Multiplication,int64 ,int64)
REGISTER_2T_OPERATOR(MUL, Multiplication,uint8 ,uint32)
REGISTER_2T_OPERATOR(MUL, Multiplication,uint16,uint32)
REGISTER_2T_OPERATOR(MUL, Multiplication,uint32,uint32)
REGISTER_2T_OPERATOR(MUL, Multiplication,uint32,uint64)
REGISTER_2T_OPERATOR(MUL, Multiplication,uint64,uint64)

REGISTER_3T_OPERATOR(MUL, SMultiplication,int8  ,int32 ,int32)
REGISTER_3T_OPERATOR(MUL, SMultiplication,int16 ,int32 ,int32)
REGISTER_3T_OPERATOR(MUL, SMultiplication,uint8 ,int32 ,int32)
REGISTER_3T_OPERATOR(MUL, SMultiplication,uint16,int32 ,int32)
REGISTER_3T_OPERATOR(MUL, SSMultiplication,uint32,int32 ,int32)

REGISTER_3T_OPERATOR(MUL, SMultiplication,int8  ,int64 ,int64)
REGISTER_3T_OPERATOR(MUL, SMultiplication,int16 ,int64 ,int64)
REGISTER_3T_OPERATOR(MUL, SMultiplication,int32 ,int64 ,int64)
REGISTER_3T_OPERATOR(MUL, SMultiplication,uint8 ,int64 ,int64)
REGISTER_3T_OPERATOR(MUL, SMultiplication,uint16,int64 ,int64)
REGISTER_3T_OPERATOR(MUL, SMultiplication,uint32,int64 ,int64)
REGISTER_3T_OPERATOR(MUL, SSMultiplication,uint64,int64 ,int64)

REGISTER_3T_OPERATOR(MUL, SMultiplication,uint8 ,uint32,uint32)
REGISTER_3T_OPERATOR(MUL, SMultiplication,uint16,uint32,uint32)

REGISTER_3T_OPERATOR(MUL, SMultiplication,uint8 ,uint64,uint64)
REGISTER_3T_OPERATOR(MUL, SMultiplication,uint16,uint64,uint64)
REGISTER_3T_OPERATOR(MUL, SMultiplication,uint32,uint64,uint64)

REGISTER_2T_OPERATOR(DIV, Division,int8  ,int32)
REGISTER_2T_OPERATOR(DIV, Division,int16 ,int32)
REGISTER_2T_OPERATOR(DIV, Division,int32 ,int32)
REGISTER_2T_OPERATOR(DIV, Division,int64 ,int64)
REGISTER_2T_OPERATOR(DIV, Division,uint8 ,uint32)
REGISTER_2T_OPERATOR(DIV, Division,uint16,uint32)
REGISTER_2T_OPERATOR(DIV, Division,uint32,uint32)
REGISTER_2T_OPERATOR(DIV, Division,uint64,uint64)

REGISTER_3T_OPERATOR(DIV, Division,int8  ,int32 ,int32)
REGISTER_3T_OPERATOR(DIV, Division,int16 ,int32 ,int32)
REGISTER_3T_OPERATOR(DIV, Division,uint8 ,int32 ,int32)
REGISTER_3T_OPERATOR(DIV, Division,uint16,int32 ,int32)
REGISTER_3T_OPERATOR(DIV, SDivision,uint32,int32 ,int32)

REGISTER_3T_OPERATOR(DIV, Division,int8  ,int64 ,int64)
REGISTER_3T_OPERATOR(DIV, Division,int16 ,int64 ,int64)
REGISTER_3T_OPERATOR(DIV, Division,int32 ,int64 ,int64)
REGISTER_3T_OPERATOR(DIV, Division,uint8 ,int64 ,int64)
REGISTER_3T_OPERATOR(DIV, Division,uint16,int64 ,int64)
REGISTER_3T_OPERATOR(DIV, Division,uint32,int64 ,int64)
REGISTER_3T_OPERATOR(DIV, SDivision,uint64,int64 ,int64)

REGISTER_3T_OPERATOR(DIV, Division,uint8 ,uint32,uint32)
REGISTER_3T_OPERATOR(DIV, Division,uint16,uint32,uint32)

REGISTER_3T_OPERATOR(DIV, Division,uint8 ,uint64,uint64)
REGISTER_3T_OPERATOR(DIV, Division,uint16,uint64,uint64)
REGISTER_3T_OPERATOR(DIV, Division,uint32,uint64,uint64)



/*********************************************************************************************************
 *********************************************************************************************************
 *
 *                      Asymmetric compare operators
 *
 *********************************************************************************************************
 **********************************************************************************************************/

template <typename T1,typename T2,typename Ttest> void Greater_3T(RuntimeEvaluator &context){
    T1 x1;
    T2 x2;
    Ttest y1,y2;
    context.Pop(x1);
    context.Pop(x2);
    bool result=false;
    bool ret1,ret2;
    ret1 = SafeNumber2Number(x1,y1);
    ret2 = SafeNumber2Number(x2,y2);
    if (ret1 && ret2){
        result = y2 > y1;
    } else {
        context.runtimeError.outOfRange = true;
//      COMPOSITE_REPORT_ERROR(context.runtimeError,x1,"!=",x2);
    }
    context.Push(result);
}

template <typename T1,typename T2,typename Ttest> void Lower_3T(RuntimeEvaluator &context){
    T1 x1;
    T2 x2;
    Ttest y1,y2;
    context.Pop(x1);
    context.Pop(x2);
    bool result=false;
    bool ret1,ret2;
    ret1 = SafeNumber2Number(x1,y1);
    ret2 = SafeNumber2Number(x2,y2);
    if (ret1 && ret2){
        result = y2 < y1;
    } else {
        context.runtimeError.outOfRange = true;
//      COMPOSITE_REPORT_ERROR(context.runtimeError,x1,"!=",x2);
    }
    context.Push(result);
}

template <typename T1,typename T2,typename Ttest> void GreaterOrSame_3T(RuntimeEvaluator &context){
    T1 x1;
    T2 x2;
    Ttest y1,y2;
    context.Pop(x1);
    context.Pop(x2);
    bool result=false;
    bool ret1,ret2;
    ret1 = SafeNumber2Number(x1,y1);
    ret2 = SafeNumber2Number(x2,y2);
    if (ret1 && ret2){
        result = y2 >= y1;
    } else {
        context.runtimeError.outOfRange = true;
    }
    context.Push(result);
}

template <typename T1,typename T2,typename Ttest> void LowerOrSame_3T(RuntimeEvaluator &context){
    T1 x1;
    T2 x2;
    Ttest y1,y2;
    context.Pop(x1);
    context.Pop(x2);
    bool result=false;
    bool ret1,ret2;
    ret1 = SafeNumber2Number(x1,y1);
    ret2 = SafeNumber2Number(x2,y2);
    if (ret1 && ret2){
        result = y2 <= y1;
    } else {
        context.runtimeError.outOfRange = true;
    }
    context.Push(result);
}

template <typename T1,typename T2,typename Ttest> void Same_3T(RuntimeEvaluator &context){
    T1 x1;
    T2 x2;
    Ttest y1,y2;
    context.Pop(x1);
    context.Pop(x2);
    bool result=false;
    bool ret1,ret2;
    ret1 = SafeNumber2Number(x1,y1);
    ret2 = SafeNumber2Number(x2,y2);
    if (ret1 && ret2){
        result = y2 == y1;
    } else {
        context.runtimeError.outOfRange = true;
    }
    context.Push(result);
}

template <typename T1,typename T2,typename Ttest> void Different_3T(RuntimeEvaluator &context){
    T1 x1;
    T2 x2;
    Ttest y1,y2;
    context.Pop(x1);
    context.Pop(x2);
    bool result=false;
    bool ret1,ret2;
    ret1 = SafeNumber2Number(x1,y1);
    ret2 = SafeNumber2Number(x2,y2);
    if (ret1 && ret2){
        result = y2 == y1;
    } else {
        context.runtimeError.outOfRange = true;
    }
    context.Push(result);
}


// register compare operators with two input types but (type1 op type1)==>typeOut
#define REGISTER_2T_COMP_OPERATOR(name,fname,type,typeTest)                                                          \
    static Function functionP ## fname ## type ## typeTest ## typeTestR = & fname ## _3T<type,typeTest,typeTest>;    \
    static Function functionP ## fname ## typeTest ## type ## typeTestL = & fname ## _3T<typeTest,type,typeTest>;    \
    REGISTER_PCODE_FUNCTION(name,type ## typeTest ## typeTest,2,1,*functionP ## fname ## type ## typeTest ## typeTestR,Type2TypeDescriptor<type>(),Type2TypeDescriptor<typeTest>(),Type2TypeDescriptor<uint8>())\
    REGISTER_PCODE_FUNCTION(name,typeTest ## type ## typeTest,2,1,*functionP ## fname ## typeTest ## type ## typeTestL,Type2TypeDescriptor<typeTest>(),Type2TypeDescriptor<type>(),Type2TypeDescriptor<uint8>())

#define REGISTER_2T_COMP_OPERATOR_BLOCK(name,fname)                \
  REGISTER_2T_COMP_OPERATOR(name,fname,int8  ,int32)               \
  REGISTER_2T_COMP_OPERATOR(name,fname,int16 ,int32)               \
  REGISTER_2T_COMP_OPERATOR(name,fname,uint8 ,int32)               \
  REGISTER_2T_COMP_OPERATOR(name,fname,uint16,int32)               \
  REGISTER_2T_COMP_OPERATOR(name,fname,uint32,int32)               \
  REGISTER_2T_COMP_OPERATOR(name,fname,int8  ,int64)               \
  REGISTER_2T_COMP_OPERATOR(name,fname,int16 ,int64)               \
  REGISTER_2T_COMP_OPERATOR(name,fname,int32 ,int64)               \
  REGISTER_2T_COMP_OPERATOR(name,fname,uint8 ,int64)               \
  REGISTER_2T_COMP_OPERATOR(name,fname,uint16,int64)               \
  REGISTER_2T_COMP_OPERATOR(name,fname,uint32,int64)               \
  REGISTER_2T_COMP_OPERATOR(name,fname,uint64,int64)               \
  REGISTER_2T_COMP_OPERATOR(name,fname,uint8 ,uint32)              \
  REGISTER_2T_COMP_OPERATOR(name,fname,uint16,uint32)              \
  REGISTER_2T_COMP_OPERATOR(name,fname,int8  ,uint64)              \
  REGISTER_2T_COMP_OPERATOR(name,fname,int16 ,uint64)              \
  REGISTER_2T_COMP_OPERATOR(name,fname,int32 ,uint64)              \
  REGISTER_2T_COMP_OPERATOR(name,fname,uint8 ,uint64)              \
  REGISTER_2T_COMP_OPERATOR(name,fname,uint16,uint64)              \
  REGISTER_2T_COMP_OPERATOR(name,fname,uint32,uint64)

REGISTER_2T_COMP_OPERATOR_BLOCK(GT,Greater)
REGISTER_2T_COMP_OPERATOR_BLOCK(LT,Lower)
REGISTER_2T_COMP_OPERATOR_BLOCK(GTE,GreaterOrSame)
REGISTER_2T_COMP_OPERATOR_BLOCK(LTE,LowerOrSame)
REGISTER_2T_COMP_OPERATOR_BLOCK(EQ,Same)
REGISTER_2T_COMP_OPERATOR_BLOCK(NEQ,Different)


/*********************************************************************************************************
 *********************************************************************************************************
 *
 *                      READ WRITE  operators
 *
 *********************************************************************************************************
 **********************************************************************************************************/

template <typename T> void Read(RuntimeEvaluator &context){
    RuntimeEvaluator::CodeMemoryElement index;
    index = context.GetPseudoCode();
    context.Push(context.Variable<T>(index));
}

template <typename T> void Write(RuntimeEvaluator &context){
    RuntimeEvaluator::CodeMemoryElement index;
    index = context.GetPseudoCode();
    context.Pop(context.Variable<T>(index));
}

REGISTER_PCODE_FUNCTION(READ,double,0,1,Read<float64>,Float64Bit)
REGISTER_PCODE_FUNCTION(READ,float,0,1,Read<float32>,Float32Bit)
REGISTER_PCODE_FUNCTION(READ,uint64,0,1,Read<uint64>,UnsignedInteger64Bit)
REGISTER_PCODE_FUNCTION(READ,int64,0,1,Read<int64>,SignedInteger64Bit)
REGISTER_PCODE_FUNCTION(READ,uint32,0,1,Read<uint32>,UnsignedInteger32Bit)
REGISTER_PCODE_FUNCTION(READ,int32,0,1,Read<int32>,SignedInteger32Bit)
REGISTER_PCODE_FUNCTION(READ,uint16,0,1,Read<uint16>,UnsignedInteger16Bit)
REGISTER_PCODE_FUNCTION(READ,int16,0,1,Read<int16>,SignedInteger16Bit)
REGISTER_PCODE_FUNCTION(READ,uint8,0,1,Read<uint8>,UnsignedInteger8Bit)
REGISTER_PCODE_FUNCTION(READ,int8,0,1,Read<int8>,SignedInteger8Bit)

REGISTER_PCODE_FUNCTION(WRITE,double,1,0,Write<float64>,Float64Bit,Float64Bit)
REGISTER_PCODE_FUNCTION(WRITE,float,1,0,Write<float32> ,Float32Bit,Float32Bit)
REGISTER_PCODE_FUNCTION(WRITE,uint64,1,0,Write<uint64> ,UnsignedInteger64Bit,UnsignedInteger64Bit)
REGISTER_PCODE_FUNCTION(WRITE,int64,1,0,Write<int64>   ,SignedInteger64Bit  ,SignedInteger64Bit  )
REGISTER_PCODE_FUNCTION(WRITE,uint32,1,0,Write<uint32> ,UnsignedInteger32Bit,UnsignedInteger32Bit)
REGISTER_PCODE_FUNCTION(WRITE,int32,1,0,Write<int32>   ,SignedInteger32Bit  ,SignedInteger32Bit  )
REGISTER_PCODE_FUNCTION(WRITE,uint16,1,0,Write<uint16> ,UnsignedInteger16Bit,UnsignedInteger16Bit)
REGISTER_PCODE_FUNCTION(WRITE,int16,1,0,Write<int16>   ,SignedInteger16Bit  ,SignedInteger16Bit  )
REGISTER_PCODE_FUNCTION(WRITE,uint8,1,0,Write<uint8>   ,UnsignedInteger8Bit ,UnsignedInteger8Bit )
REGISTER_PCODE_FUNCTION(WRITE,int8,1,0,Write<int8>     ,SignedInteger8Bit   ,SignedInteger8Bit   )
/*********************************************************************************************************
 *********************************************************************************************************
 *
 *                      Type converting Write operators
 *
 *********************************************************************************************************
 **********************************************************************************************************/


template <typename Tin,typename Tout> void Write_2T(RuntimeEvaluator &context){
    RuntimeEvaluator::CodeMemoryElement index;
    index = context.GetPseudoCode();
    Tin x1;
    Tout x2;
    context.Pop(x1);
    bool ret;
    ret = SafeNumber2Number(x1,x2);
    if (!ret){
        context.runtimeError.outOfRange = true;
    }
    context.Variable<Tout>(index) = x2;
}


// register function with difference between input and outout type   fun(type1)==>typeOut
#define REGISTER_WRITECONV(name,fname,typeIn,typeOut)                                           \
    static Function functionP ## fname ## typeIn ## typeOut = & fname ## _2T<typeIn,typeOut>;    \
    REGISTER_PCODE_FUNCTION(name,typeIn ## typeOut,1,0,*functionP ## fname ## typeIn ## typeOut,Type2TypeDescriptor<typeIn>(),Type2TypeDescriptor<typeOut>())

REGISTER_WRITECONV(WRITE,Write,uint64,uint8)
REGISTER_WRITECONV(WRITE,Write,uint64,uint16)
REGISTER_WRITECONV(WRITE,Write,uint64,uint32)
REGISTER_WRITECONV(WRITE,Write,int64 ,uint8)
REGISTER_WRITECONV(WRITE,Write,int64 ,uint16)
REGISTER_WRITECONV(WRITE,Write,int64 ,uint32)
REGISTER_WRITECONV(WRITE,Write,int64 ,uint64)
REGISTER_WRITECONV(WRITE,Write,int64 ,int8)
REGISTER_WRITECONV(WRITE,Write,int64 ,int16)
REGISTER_WRITECONV(WRITE,Write,int64 ,int32)
REGISTER_WRITECONV(WRITE,Write,uint32,uint8)
REGISTER_WRITECONV(WRITE,Write,uint32,uint16)
REGISTER_WRITECONV(WRITE,Write,int32 ,uint8)
REGISTER_WRITECONV(WRITE,Write,int32 ,uint16)
REGISTER_WRITECONV(WRITE,Write,int32 ,uint32)
REGISTER_WRITECONV(WRITE,Write,int32 ,int8)
REGISTER_WRITECONV(WRITE,Write,int32 ,int16)
REGISTER_WRITECONV(WRITE,Write,uint8 ,int8)

/*********************************************************************************************************
 *********************************************************************************************************
 *
 *                      Remote Type converting Write operators
 *
 *********************************************************************************************************
 **********************************************************************************************************/

template <typename T> void RRead(RuntimeEvaluator &context){
    RuntimeEvaluator::CodeMemoryElement index;
    index = context.GetPseudoCode();
    T *x = context.Variable<T *>(index);
    context.Push(*x);
}
template <typename T> void RWrite(RuntimeEvaluator &context){
    RuntimeEvaluator::CodeMemoryElement index;
    index = context.GetPseudoCode();
    T *x = context.Variable<T *>(index);
    context.Pop(*x);
}

REGISTER_PCODE_FUNCTION(RREAD,double,0,1,RRead<float64>,Float64Bit)
REGISTER_PCODE_FUNCTION(RREAD,float ,0,1,RRead<float32>,Float32Bit)
REGISTER_PCODE_FUNCTION(RREAD,uint64,0,1,RRead<uint64>,UnsignedInteger64Bit)
REGISTER_PCODE_FUNCTION(RREAD,int64 ,0,1,RRead<int64>,SignedInteger64Bit)
REGISTER_PCODE_FUNCTION(RREAD,uint32,0,1,RRead<uint32>,UnsignedInteger32Bit)
REGISTER_PCODE_FUNCTION(RREAD,int32 ,0,1,RRead<int32>,SignedInteger32Bit)
REGISTER_PCODE_FUNCTION(RREAD,uint16,0,1,RRead<uint16>,UnsignedInteger16Bit)
REGISTER_PCODE_FUNCTION(RREAD,int16 ,0,1,RRead<int16>,SignedInteger16Bit)
REGISTER_PCODE_FUNCTION(RREAD,uint8 ,0,1,RRead<uint8>,UnsignedInteger8Bit)
REGISTER_PCODE_FUNCTION(RREAD,int8  ,0,1,RRead<int8>,SignedInteger8Bit)

REGISTER_PCODE_FUNCTION(RWRITE,double,1,0,RWrite<float64>,Float64Bit,Float64Bit)
REGISTER_PCODE_FUNCTION(RWRITE,float ,1,0,RWrite<float32> ,Float32Bit,Float32Bit)
REGISTER_PCODE_FUNCTION(RWRITE,uint64,1,0,RWrite<uint64> ,UnsignedInteger64Bit,UnsignedInteger64Bit)
REGISTER_PCODE_FUNCTION(RWRITE,int64 ,1,0,RWrite<int64>   ,SignedInteger64Bit  ,SignedInteger64Bit  )
REGISTER_PCODE_FUNCTION(RWRITE,uint32,1,0,RWrite<uint32> ,UnsignedInteger32Bit,UnsignedInteger32Bit)
REGISTER_PCODE_FUNCTION(RWRITE,int32 ,1,0,RWrite<int32>   ,SignedInteger32Bit  ,SignedInteger32Bit  )
REGISTER_PCODE_FUNCTION(RWRITE,uint16,1,0,RWrite<uint16> ,UnsignedInteger16Bit,UnsignedInteger16Bit)
REGISTER_PCODE_FUNCTION(RWRITE,int16 ,1,0,RWrite<int16>   ,SignedInteger16Bit  ,SignedInteger16Bit  )
REGISTER_PCODE_FUNCTION(RWRITE,uint8 ,1,0,RWrite<uint8>   ,UnsignedInteger8Bit ,UnsignedInteger8Bit )
REGISTER_PCODE_FUNCTION(RWRITE,int8  ,1,0,RWrite<int8>     ,SignedInteger8Bit   ,SignedInteger8Bit   )

template <typename Tin,typename Tout> void RWrite_2T(RuntimeEvaluator &context){
    RuntimeEvaluator::CodeMemoryElement index;
    index = context.GetPseudoCode();
    Tin x1;
    context.Pop(x1);
    Tout *x2 = context.Variable<Tout *>(index);
    bool ret;
    ret = SafeNumber2Number(x1,*x2);
    if (!ret){
        context.runtimeError.outOfRange = true;
    }
}

// register function with difference between input and outout type   fun(type1)==>typeOut
#define REGISTER_WRITECONV(name,fname,typeIn,typeOut)                                           \
    static Function functionP ## fname ## typeIn ## typeOut = & fname ## _2T<typeIn,typeOut>;    \
    REGISTER_PCODE_FUNCTION(name,typeIn ## typeOut,1,0,*functionP ## fname ## typeIn ## typeOut,Type2TypeDescriptor<typeIn>(),Type2TypeDescriptor<typeOut>())

REGISTER_WRITECONV(RWRITE,RWrite,uint64,uint8)
REGISTER_WRITECONV(RWRITE,RWrite,uint64,uint16)
REGISTER_WRITECONV(RWRITE,RWrite,uint64,uint32)
REGISTER_WRITECONV(RWRITE,RWrite,int64 ,uint8)
REGISTER_WRITECONV(RWRITE,RWrite,int64 ,uint16)
REGISTER_WRITECONV(RWRITE,RWrite,int64 ,uint32)
REGISTER_WRITECONV(RWRITE,RWrite,int64 ,uint64)
REGISTER_WRITECONV(RWRITE,RWrite,int64 ,int8)
REGISTER_WRITECONV(RWRITE,RWrite,int64 ,int16)
REGISTER_WRITECONV(RWRITE,RWrite,int64 ,int32)
REGISTER_WRITECONV(RWRITE,RWrite,uint32,uint8)
REGISTER_WRITECONV(RWRITE,RWrite,uint32,uint16)
REGISTER_WRITECONV(RWRITE,RWrite,int32 ,uint8)
REGISTER_WRITECONV(RWRITE,RWrite,int32 ,uint16)
REGISTER_WRITECONV(RWRITE,RWrite,int32 ,uint32)
REGISTER_WRITECONV(RWRITE,RWrite,int32 ,int8)
REGISTER_WRITECONV(RWRITE,RWrite,int32 ,int16)



} //MARTe
