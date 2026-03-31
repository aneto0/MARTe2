/**
 * @file ResultT.h
 * @brief Header file for class ResultT
 * @date 27/10/2015
 * @author Martino Ferrari
 * @author Giuseppe Ferro
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

 * @details This header file contains the declaration of the class ResultT
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef RESULTT_H_
#define RESULTT_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "GeneralDefinitions.h"
#include "ErrorManagement.h"
#include "HardwareI.h"
#include "Sleep.h"
#include "StringHelper.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief A templated class to facilitate the return of a result as a combination of error and value.
 *
 * @details The class is composed by a memory holding a copy of the value it is instaciated with, and by a ErrorInformation
 * holding the error associated to the result.
 */
template<typename T>
class DLL_API ResultT {

public:

    /**
     * @brief Constructor from templated value
     * @param[in] val is the result value
     */
    ResultT(const T &val);
    
    /**
     * @brief Copy constructor
     * @param[in] copy is the result to copy from
     */
    ResultT(const ResultT<T> &copy);
    
    /**
     * @brief Destructor
     */
    virtual ~ResultT();

    /**
     * @brief Sets the result error
     * @details To facilitate, user might call this call by defining only code and errorDescription and use ERR_ARGS to 
     * automatically get the rest of the informations.
     * @param[in] code the error code
     * @param[in] errorDescription the error description
     * @param[in] fileName the file name generating the error
     * @param[in] lineNumber the line number generating the error
     * @param[in] functionName the function generating the error
     */
    void SetError(const ErrorManagement::ErrorType &code,
              const char8 * const errorDescription,
              const char8 * const fileName,
              const int16 lineNumber,
              const char8 * const functionName); 

    /**
     * @brief Prints the error
     * @details Prints the error using the defined errorMessageProcessFunction
     */
    void PrintError();

    /**
     * @brief Returns the result value
     * @return the result value
     */
    T &GetValue();

    /**
     * @brief Bool operator
     * @return true if error, false otherwise
     */
    operator bool() const;

    /**
     * @brief Assignment operator
     * @param[in] copy is the result to copy from
     */
    ResultT<T> &operator =(const ResultT<T>& copy);
    
    /**
     * @brief Assignment operator from templated value
     * @details the error is set to NoError
     * @brief param[in] copy is the value to copy from
     */
    ResultT<T> &operator =(const T& copy);
    
    /**
     * @brief Equal operator with templated value
     * @details Checks just the value, not the error
     * @param[in] other is the other value to compare with
     * @return true if the values are equal, false otherwise
     */
    bool operator ==(const T& other) const;
    
    /**
     * @brief Different operator with templated value
     * @details Checks just the value, not the error
     * @param[in] other is the other value to compare with
     * @return true if the values are different, false otherwise
     */
    bool operator !=(const T& other) const;
 
    /**
     * @brief Equal operator with other result
     * @details Checks the value, the error and the error description
     * @param[in] other is the other result to compare with
     * @return true if the results are equal, false otherwise
     */
    bool operator ==(const ResultT<T>& other) const;
 
    /**
     * @brief Different operator with other result
     * @details Checks the value, the error and the error description
     * @param[in] other is the other result to compare with
     * @return true if the results are different, false otherwise
     */
    bool operator !=(const ResultT<T>& other) const;

private:

    /**
     * Holds the error
     */
    ErrorManagement::ErrorInformation error;    

    /**
     * Holds the error description
     */
    const char8 *errorDesc;

    /**
     * Holds the value
     */
    T *ptrHolder;
};


template<typename T>
ResultT<T>::ResultT(const T &val) {
    uint32 sizeToCopy = static_cast<uint32>(sizeof(val));
    ptrHolder = new T(val);
    error.header.errorType = ErrorManagement::NoError;
    errorDesc = NULL_PTR(const char8 *);
}


template<typename T>
ResultT<T>::ResultT(const ResultT<T> &copy){
    ptrHolder = new T(*copy.ptrHolder);
    error = copy.error;
    errorDesc = copy.errorDesc;
}


template<typename T>
ResultT<T>::~ResultT(){
    if(ptrHolder != NULL){
        delete ptrHolder;
    }
}

template<typename T>
void ResultT<T>::SetError(const ErrorManagement::ErrorType &code,
              const char8 * const errorDescription,
              const char8 * const fileName,
              const int16 lineNumber,
              const char8 * const functionName) {
    error.objectPointer = static_cast<void*>(NULL);
    error.className = static_cast<const char8 *>(NULL);
    error.header.errorType = code;
    error.header.lineNumber = lineNumber;
    error.fileName = fileName;
    error.functionName = functionName;
    error.hrtTime = HighResolutionTimer::Counter();
    error.timeSeconds = Sleep::GetDateSeconds();
    error.platformSpecificString = HardwareI::GetPlatformSpecificString();
    errorDesc = errorDescription;
}


template<typename T>
void ResultT<T>::PrintError() {
    ErrorManagement::errorMessageProcessFunction(error, errorDesc);
}

template<typename T>
T &ResultT<T>::GetValue(){
    return *ptrHolder;
}

template<typename T>
ResultT<T>::operator bool() const{
    return error.header.errorType.ErrorsCleared();
}

template<typename T>
ResultT<T> &ResultT<T>::operator =(const ResultT<T>& copy){
    delete ptrHolder;
    ptrHolder = new T(*copy.ptrHolder);
    error = copy.error;
    errorDesc = copy.errorDesc;
    return *this;
}

template<typename T>
ResultT<T> &ResultT<T>::operator =(const T& copy){
    delete ptrHolder;
    ptrHolder = new T(copy);
    error.header.errorType = ErrorManagement::NoError;
    errorDesc = "";
    return *this;
}

    
template<typename T>
bool ResultT<T>::operator ==(const T& other) const{
    return ((*ptrHolder) == other);
}

template<typename T>
bool ResultT<T>::operator !=(const T& other) const{
    return !(*this == other);
}


template<typename T>
bool ResultT<T>::operator ==(const ResultT<T>& other) const{
    bool check = ((*ptrHolder) == (*other.ptrHolder));
    if (check){
        check = (error.header.errorType.format_as_integer == other.error.header.errorType.format_as_integer);
    }
    if (check){
        check = (errorDesc == NULL && other.errorDesc == NULL);
        if(!check){
            check = (StringHelper::Compare(errorDesc, other.errorDesc) == 0);
        }
    }
    return check;
}


template<typename T>
bool ResultT<T>::operator !=(const ResultT<T>& other) const{
    return !(*this == other);
}

}

#define ERR_ARGS __FILE__,__LINE__,__ERROR_FUNCTION_NAME__

#endif
