#include "GeneralDefinitions.h"

namespace MARTe{ 

struct ParamNestedStructure { 
    float32 ininPar1;
}; 

/*fixed structure*/
struct Struct1 { 
    uint32 inPar1;
    int32 inPar2[2][3];
    ParamNestedStructure internalPar1;
}; 

/*array of structures*/
struct Struct2 { 
    int32 inPar1;
    int32 inPar2;
}; 

struct StructSignal1 { 
    uint32 parSignal1[2];
    float32 parSignal2;
}; 

struct StructSignal2 { 
    uint32 parSignal1;
    uint32 parSignal2;
}; 


}