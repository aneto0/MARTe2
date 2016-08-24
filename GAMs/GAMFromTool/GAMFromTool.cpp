/**
 * @file GAMFromTool.cpp
 * @brief Source file for class GAMFromTool
 * @date 22/ago/2016
 * @author pc
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
 * the class GAMFromTool (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "GAMFromTool.h"
#include "stdio.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/



GAMFromTool::GAMFromTool() {
    ToolMembersConstructor();
}

GAMFromTool::~GAMFromTool() {
    ToolMembersDestructor();
}

bool GAMFromTool::Initialise(StructuredDataI &data){
    bool ret = GAM::Initialise(data);
    if(ret){
        ret=ConfigureToolMembers(data);
    }
    return ret;
}

bool GAMFromTool::Setup(){
    bool ret=ConfigureToolSignals();


    printf("\npar1=%d", par1);
    printf("\npar2[0]=%f", par2[0]);
    printf("\npar2[1]=%f", par2[1]);
    printf("\npar4.inPar1=%d", par4.inPar1);
    for(uint32 i=0u;i<2; i++){
        for(uint32 j=0u;j<3; j++){
            printf("\npar4.inPar2[%d][%d]=%d",i, j, par4.inPar2[i][j]);
        }
    }
    printf("\npar4.internalPar1.ininPar1=%f", par4.internalPar1.ininPar1);
    for (uint32 i = 0u; i < numberOfpar5; i++) {
        printf("\npar5[%d]=%d",i, par5[i]);
    }
    printf("\nnumberOfpar6 ? %d",numberOfpar6);

    for (uint32 i = 0u; i < numberOfpar6; i++) {
        printf("\npar6[%d].inPar1=%d",i, par6[i].inPar1);
        printf("\npar6[%d].inPar2=%d",i, par6[i].inPar2);
    }


    return ret;
}



bool GAMFromTool::Execute(){
    return true;
}



CLASS_REGISTER(GAMFromTool, "1.0");
