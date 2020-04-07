/**
 * @file Select.h
 * @brief Header file for class Select
 * @date 12/11/2015
 * @author Llorenç Capellà
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

 * @details This header file contains the declaration of the class Select
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef SELECT_H_
#define SELECT_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "GeneralDefinitions.h"
#include "MilliSeconds.h"
#include "BasicSocket.h"

#include INCLUDE_FILE_ENVIRONMENT(ENVIRONMENT, SelectProperties.h)

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

    /**
     * @brief Class for monitoring simultaneously I/O events from devices like
     * files, sockets, console, and so on.
     * @details The Select class provides a set of functions to monitor
     * several I/O (files, sockets, console...) events at the same time.
     * The class allows to add, remove or clear handles in read, write or
     * exception mode. Once the handles are selected, the WaitUntil()
     * function starts to monitor them returning when an even occurs.
     * The IsSet() function can be used to know which events were triggered.
     */
    class DLL_API Select {

    public:

        /**
         * @brief Default constructor
         * @post
         *   For all handleI of type HandleI, then !IsSet(handleI)
         */
        Select();

        /**
         * @brief Destructor
         */
        virtual ~Select();

        /**
         * @brief Adds a handle to be monitored in read mode.
         * @param[in] handle indicates the handle to be added.
         * @pre
         *     The handle must be valid &&
         *     The handle must not have been added previously.
         * @post
         *     IsSet(handle)
         * @return True if the handle is correctly added to the watch list.
         */
        template<class T>
        inline bool AddReadHandle(const T &handle);

        /**
         * @brief Adds a handle to be monitored in write mode.
         * @param[in] handle indicates the handle to be added.
         * @pre
         *     The handle must be valid &&
         *     The handle must not have been added previously.
         * @post
         *     IsSet(handle)
         * @return True if the handle is correctly added to the watch list.
         */
        template<class T>
        inline bool AddWriteHandle(const T &handle);

        /**
         * @brief Adds a handle to be monitored in exception mode.
         * @details If handle has a double handle (i.e. a different
         * handle for reading and writing), then both are added.
         * @param[in] handle indicates the handle to be added.
         * @pre
         *     The handle must be valid &&
         *     The handle must not have been added previously.
         * @post
         *     IsSet(handle)
         * @return True if the handle is correctly added to the watch list.
         */
        template<class T>
        inline bool AddExceptionHandle(const T &handle);

        /**
         * @brief Removes a handle from being monitored in the read mode.
         * @param[in] handle indicates the handle to be removed.
         * @pre
         *     The handle must be valid &&
         *     The handle must have been added previously.
         * @post
         *     not IsSet(handle)
         * @return True if the handle is correctly removed from the watch list.
         */
        template<class T>
        inline bool RemoveReadHandle(const T &handle);

        /**
         * @brief Removes a handle from being monitored in write mode.
         * @param[in] handle indicates the handle to be removed.
         * @pre
         *     The handle must be valid &&
         *     The handle must have been added previously.
         * @post
         *     not IsSet(handle)
         * @return True if the handle is correctly removed from the watch list.
         */
        template<class T>
        inline bool RemoveWriteHandle(const T &handle);

        /**
         * @brief Removes a handle from being monitored in exception mode.
         * @param[in] handle indicates the handle to be removed.
         * @pre
         *     The handle must be valid &&
         *     The handle must have been added previously.
         * @post
         *     not IsSet(handle)
         * @return True if the handle is correctly removed from the watch list.
         */
        template<class T>
        inline bool RemoveExceptionHandle(const T &handle);

        /**
         * @brief Removes all handles from being monitored in all modes.
         * @pre
         *     true
         * @post
         *    For all handleI of type HandleI, then !IsSet(handleI)
         */
        void ClearAllHandles();

        /**
         * @brief Queries if the handle is set in one of the three modes.
         * @param[in] handle indicates the handle to be search for.
         * @return True if the handle is set.
         */
        bool IsSet(const Handle &handle,bool readEvent=true,bool writeEvent=true,bool exceptEvent=true) const;

        /**
         * @brief Queries if the handle is set in one of the three modes.
         * @param[in] handle indicates the handle to be search for.
         * @return True if the handle is set.
         */
        bool IsSet(const BasicSocket &socket,bool readEvent=true,bool writeEvent=true,bool exceptEvent=true) const;

        /**
         * @brief Blocks until an I/O event occurs in one of the added handles, or the function timeouts.
         * @param[in] timeout is the timeout of the function, in ms. Default is no timeout.
         * @return -1 in case of errors, 0 if timeout expires, otherwise the number of handles which received an I/O event.
         */
        int32 WaitUntil(const MilliSeconds &timeout = MilliSeconds::Infinite);

    private:
        bool Add(const Handle &handle,bool readEvent=true,bool writeEvent=true,bool exceptEvent=true);
        bool Add(const BasicSocket &socket,bool readEvent=true,bool writeEvent=true,bool exceptEvent=true);
        bool Add(const SocketCore &socket,bool readEvent=true,bool writeEvent=true,bool exceptEvent=true);
        bool Remove(const Handle &handle,bool readEvent=true,bool writeEvent=true,bool exceptEvent=true);
        bool Remove(const BasicSocket &socket,bool readEvent=true,bool writeEvent=true,bool exceptEvent=true);

        /**
         * the platform specific internal storage required to support select operations
         */
        SelectProperties selectProperties;

        void operator=(const Select &){}

    };


/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

    template<class T>
    bool Select::AddReadHandle(const T &handle){
    	return Add(handle,true,false,false);
    }

    template<class T>
    bool Select::AddWriteHandle(const T &handle){
    	return Add(handle,false,true,false);
    }

    template<class T>
    bool Select::AddExceptionHandle(const T &handle){
    	return Add(handle,false,false,true);
    }

    template<class T>
    bool Select::RemoveReadHandle(const T &handle){
    	return Remove(handle,true,false,false);
    }

    template<class T>
    bool Select::RemoveWriteHandle(const T &handle){
    	return Remove(handle,false,true,false);
    }

    template<class T>
    bool Select::RemoveExceptionHandle(const T &handle){
    	return Remove(handle,false,false,true);
    }




}
#endif /* _SELECT_H_ */

