/**
 * @file Errors.cpp
 * @brief Source file for class Errors
 * @date 06/07/2015
 * @author Andre' Neto
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
 * the class Errors (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "FlagsType.h"
#include "Errors.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
const FlagsType Errors::Information(0);
const FlagsType Errors::Debug(1);
const FlagsType Errors::Warning (2);
const FlagsType Errors::FatalError(3);
const FlagsType Errors::RecoverableError(4);
const FlagsType Errors::InitialisationError(5);
const FlagsType Errors::OSError(6);
const FlagsType Errors::ParametersError(7);
const FlagsType Errors::IllegalOperation(8);
const FlagsType Errors::ErrorSharing(9);
const FlagsType Errors::ErrorAccessDenied(10);
const FlagsType Errors::Exception(11);
const FlagsType Errors::Timeout(12);
const FlagsType Errors::CommunicationError(13);
const FlagsType Errors::SyntaxError(14);
const FlagsType Errors::UnsupportedError(15);

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

