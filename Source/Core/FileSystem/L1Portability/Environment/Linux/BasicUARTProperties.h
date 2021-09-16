/**
 * @file BasicUARTProperties.h
 * @brief Header file for class BasicUARTProperties.
 * @date 18/08/2021
 * @author Pedro Lourenco
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
 *
 * @details This header file contains the declaration of the class
 * BasicUARTProperties with all of its public, protected and private members.
 * It may also include definitions for inline methods which need to be visible
 * to the compiler.
 */

#ifndef BASICUARTPROPERTIES_H_
#define BASICUARTPROPERTIES_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/
#include <errno.h>
#include <fcntl.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <termios.h>
#include <unistd.h>

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * Structure containing the variables/parameters for the Basic UART under
 * Linux, including those for the select() method.
 */
struct BasicUARTProperties {

    /**
     * The file descriptor associated to the Open operation.
     */
    int32 fileDescriptor;

    /**
     * The speed set with SetSpeed.
     */
    int32 speedCode;

    /**
     * The file descriptor array for the read in the select.
     */
    fd_set readFDS;

    /**
     * The file descriptor array ready to be read as returned by the select.
     */
    fd_set readFDS_done;

    /**
     * The file descriptor array for the write in the select.
     */
    fd_set writeFDS;

    /**
     * The file descriptor array ready to be written as returned by the select.
     */
    fd_set writeFDS_done;

};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* BASICUARTPROPERTIES_H_ */

