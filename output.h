#include "GeneralDefinitions.h"

namespace MARTe{ 

struct Nested { 
    uint32 member;
}; 

struct PIDInputStructure { 
    /** min=2 \n values={\"hello\"} */
    /** This is the proportional gain */
    uint8 const * const Kp[2][3];
    uint32 * const * const Ki[2][3];
    /** max=3 */
    Object * const Kd;
    Nested InternalStruct;
}; 

struct AnotherStruct { 
    uint8 * const attr_1[2][3];
    void * * const attr_2;
}; 


}