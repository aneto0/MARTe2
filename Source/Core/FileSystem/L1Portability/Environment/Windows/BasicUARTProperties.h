#ifndef BASICUARTPROPERTIES_H_
#define BASICUARTPROPERTIES_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/
#include <Windows.h>

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * Structure containing the variables/parameters for the Basic UART under
 * Windows, including those for the select() method.
 */
struct DLL_API BasicUARTProperties {

    /**
     * The file descriptor associated with the Open operation.
     */
    HANDLE fileDescriptor;

    /**
     * The speed set with SetSpeed.
     */
    DWORD speedCode;

    /**
     * The file descriptor array for read in the select.
     * Note: The use of fd_set and select is not needed on Windows.
     * Read and write operations are performed using the ReadFile and WriteFile functions.
     * You can use the fileDescriptor directly for checking readiness or waiting.
     */
    // fd_set readFDS;

    /**
     * The file descriptor array ready to be read as returned by the select.
     */
    // fd_set readFDS_done;

    /**
     * The file descriptor array for write in the select.
     * Note: The use of fd_set and select is not needed on Windows.
     * Read and write operations are performed using the ReadFile and WriteFile functions.
     * You can use the fileDescriptor directly for checking readiness or waiting.
     */
    // fd_set writeFDS;

    /**
     * The file descriptor array ready to be written as returned by the select.
     */
    // fd_set writeFDS_done;

};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* BASICUARTPROPERTIES_H_ */
