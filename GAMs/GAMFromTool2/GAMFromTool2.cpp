/**
 * @file GAMFromTool2.cpp
 * @brief Source file for class GAMFromTool2
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
 * the class GAMFromTool2 (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "GAMFromTool2.h"
#include "stdio.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/



GAMFromTool2::GAMFromTool2() {
    ToolMembersConstructor();
}

GAMFromTool2::~GAMFromTool2() {
    ToolMembersDestructor();
}

bool GAMFromTool2::Initialise(StructuredDataI &data){
    bool ret = GAM::Initialise(data);
    if(ret){
        ret=ConfigureToolMembers(data);
    }
    return ret;
}

bool GAMFromTool2::Setup(){
    bool ret=ConfigureToolSignals();


    return ret;
}



bool GAMFromTool2::Execute(){
    return true;
}



CLASS_REGISTER(GAMFromTool2, "1.0");
