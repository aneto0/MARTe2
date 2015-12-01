/**
 * @file SocketSelectCore.h
 * @brief Header file for class SocketSelectCore
 * @date Nov 24, 2015
 * @author Frank Perez Paz
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

 * @details This header file contains the declaration of the class SocketSelectCore
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

		#ifndef SOCKETSELECTCORE_H_
		#define 		SOCKETSELECTCORE_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/
#include <winsock2.h>
#include <winsock.h>
#include <windows.h>
/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

struct SocketSelectCore {
    /** */
    fd_set readFDS;

    /** */
    fd_set writeFDS;

    /** */
    fd_set exceptFDS;

    /** */
    fd_set readFDS_done;

    /** */
    fd_set writeFDS_done;

    /** */
    fd_set exceptFDS_done;

};


/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

		#endif /* SOCKETSELECTCORE_H_ */
	
