/**
 * @file StructuredDataIHelper.h
 * @brief Header file for class StructuredDataIHelper
 * @date 09/06/2022
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

 * @details This header file contains the declaration of the class StructuredDataIHelper
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef STRUCTUREDDATAIHELPER_H_
#define STRUCTUREDDATAIHELPER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "AdvancedErrorManagement.h"
#include "AnyObject.h"
#include "Reference.h"
#include "StructuredDataI.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief Proxy StructuredDataI implementation that facilitates the reading and validation of parameters.
 * @details The StructuredDataIHelper allows to read arrays and matrices without having to query 
 * for their size (see ReadArray and ReadMatrix); to read enum-like parameters (see ReadEnum); to 
 * assign default parameters and to perform data validation against simple mathematical expressions (see ReadValidated).
 *
 * The StructuredDataIHelper is also stateful (see HasErrors), meaning that as soon as any operation fails, all the subsequent 
 * operations will be ignored and an errors returned. The error state can be reset by calling ResetErrors.
 *
 */
class DLL_API StructuredDataIHelper : public StructuredDataI {

public:
    /**
     * @brief Constructor.
     * @param[in] sdiIn the StructuredDataI to be proxied.
     * @param[in] ownerIn the MARTe2 Object that is the owner of the proxied StructuredDataI.
     */
    StructuredDataIHelper(StructuredDataI &sdiIn, Reference ownerIn);

    /**
     * @brief Destructor.
     */
    virtual ~StructuredDataIHelper();

    /**
     * @brief Clears all the errors found so far.
     */
    void ResetErrors();

    /**
     * @brief Returns true if any error was triggered while calling any of the helper methods below.
     */
    bool HasErrors() const;

    /**
     * @see StructuredDataI::Read
     */
    virtual bool Read(const char8 * const name, const AnyType &value);

    /**
     * @see StructuredDataI::Read
     * @details Calls the proxied Read method and uses the defaultValue if the parameter does not exist.
     */
    /*lint -e{1411} same method name as in the StructuredDataI for the sake of code readability.*/
    bool Read(const char8 * const name, const AnyType &value, const AnyType &defaultValue);

    /**
     * @brief See ReadEnum - noting that it will return false if a parameter with this name is not found.
     */
    bool ReadEnum(const char8 * const name, const AnyType &value, const AnyType &options, const AnyType &optionValues);

    /**
     * @brief Reads as enum. 
     * @details The StructuredDataI::Read method reads as a string. This string is then tested against all the provided options. If a match occurs, the optionValue at the found index will be set as the output value.
     * @param[in] name see StructuredDataI::Read
     * @param[out] value the option value at the index where the option was matched.
     * @param[in] list of option strings (the enum keys) (=> options.GetNumberOfElements() > 0).
     * @param[in] optionValues the value to associate to the string if found (=> optionValues.GetNumberOfElements() > 0).
     * @param[in] defaultValue the value to set if no matching string is found.
     * @return true if no parameter with name is found or if it is found and the value successfully read. It will return false if the parameter is successfully read but the option is not found in the input list.
     */
    bool ReadEnum(const char8 * const name, const AnyType &value, const AnyType &options, const AnyType &optionValues, const AnyType &defaultValue);

    /**
     * @brief Reads and validates againt the validationExpression (e.g. "((MyParam > (uint32)2) && (MyParam < (uint32)5))", where MyParam is the name of the variable to be read).
     * @param[in] name see StructuredDataI::Read
     * @param[out] value see StructuredDataI::Read
     * @param[in] validationExpression a mathematical expression that can be parsed by the MathExpressionParser and whose variable is the name of the variable to be read. Note that the final ; shall not be added to the validationExpression.
     * @return true if the variable is successfully read and if the condition expressed in the validationExpression is met.
     */
    bool ReadValidated(const char8 * const name, const AnyType &value, const char8 * const validationExpression);

    /**
     * @brief Helper method for the reading of arrays.
     * @param[in] name see StructuredDataI::Read
     * @param[out] arr the array to be read (the caller is responsible for freeing (with delete []) the memory!).
     * @param[out] numberOfElements the number of elements of the array.
     * @return true if the array is successfully read. The caller will be responsible for freeing (with delete []) the allocated memory.
     */
    template<typename T>
    inline bool ReadArray(const char8 * const name, T* &arr, uint32 &numberOfElements);

    /**
     * @brief Helper method to read matrices.
     * @param[in] name see StructuredDataI::Read
     * @param[out] mat the matrix to be read (the caller is responsible for freeing (with delete[] for i=0:numberOfRows; delete[] mat, the allocated memory).
     * @param[out] numberOfRows the number of matrix rows.
     * @param[out] numberOfCols the number of matrix columns.
     * @return true if the matrix is successfully read. The caller is responsible for freeing both the matrix rows (delete []) and the matrix itself (delete []).
     */
    template<typename T>
    inline bool ReadMatrix(const char8 * const name, T** &mat, uint32 &numberOfRows, uint32 &numberOfCols);

    /**
     * @see StructuredDataI::Write
     */
    virtual bool Write(const char8 * const name, const AnyType &value);

    /**
     * @see StructuredDataI::Copy
     */
    virtual bool Copy(StructuredDataI &destination);

    /**
     * @see StructuredDataI::GetType
     */
    virtual AnyType GetType(const char8 * const name);

    /**
     * @see StructuredDataI::MoveToRoot
     */
    virtual bool MoveToRoot();

    /**
     * @see StructuredDataI::MoveToAncestor
     */
    virtual bool MoveToAncestor(const uint32 generations);

    /**
     * @see StructuredDataI::MoveAbsolute
     */
    virtual bool MoveAbsolute(const char8 * const path);

    /**
     * @see StructuredDataI::MoveRelative
     */
    virtual bool MoveRelative(const char8 * const path);

    /**
     * @see StructuredDataI::MoveToChild
     */
    virtual bool MoveToChild(const uint32 childIdx);

    /**
     * @see StructuredDataI::CreateAbsolute
     */
    virtual bool CreateAbsolute(const char8 * const path);

    /**
     * @see StructuredDataI::CreateRelative
     */
    virtual bool CreateRelative(const char8 * const path);

    /**
     * @see StructuredDataI::AddToCurrentNode
     */
    virtual bool AddToCurrentNode(Reference node);

    /**
     * @see StructuredDataI::Delete
     */
    virtual bool Delete(const char8 * const name);

    /**
     * @brief StructuredDataI::GetName
     */
    virtual const char8 *GetName();

    /**
     * @brief StructuredDataI::GetChildName
     */
    virtual const char8 *GetChildName(const uint32 index);

    /**
     * @brief StructuredDataI::GetNumberOfChildren
     */
    virtual uint32 GetNumberOfChildren();

private:

    /**
     * Any errors detected?
     */
    bool hasErrors;

    /**
     * The backend StructuredDataI.
     */
    /*lint -e{1725} initialised in the constructor.*/
    StructuredDataI &sdi;

    /**
     * The owner Object.
     */
    Reference owner;
};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/
namespace MARTe {
template<typename T>
bool StructuredDataIHelper::ReadArray(const char8 * const name, T* &arr, uint32 &numberOfElements) {
    AnyType arrDef = sdi.GetType(name);
    if (!hasErrors) {
        hasErrors = (arrDef.GetDataPointer() == NULL_PTR(void *));
        if (hasErrors) {
            REPORT_ERROR_PROXY(ErrorManagement::ParametersError, owner, "%s does not exist", name);
        }
    }
    if (!hasErrors) {
        numberOfElements = arrDef.GetNumberOfElements(0u);
        hasErrors = (numberOfElements == 0u);
        if (hasErrors) {
            REPORT_ERROR_PROXY(ErrorManagement::ParametersError, owner, "%s has zero elements", name);
        }
    }
    if (!hasErrors) {
        arr = new T[numberOfElements];
        Vector<T> vec(arr, numberOfElements);
        hasErrors = !sdi.Read(name, vec);
        if (hasErrors) { 
            REPORT_ERROR_PROXY(ErrorManagement::ParametersError, owner, "%s failed to read array", name);
            delete []arr;
            arr = NULL_PTR(T*);
        }
        else {
            REPORT_ERROR_PROXY(ErrorManagement::Information, owner, "%s successfully read : %!", name, vec);
        }
    }
    return !hasErrors;
}

template<typename T>
bool StructuredDataIHelper::ReadMatrix(const char8 * const name, T** &mat, uint32 &numberOfRows, uint32 &numberOfCols) {
    AnyType matDef = sdi.GetType(name);
    if (!hasErrors) {
        hasErrors = (matDef.GetDataPointer() == NULL_PTR(void *));
        if (hasErrors) {
            REPORT_ERROR_PROXY(ErrorManagement::ParametersError, owner, "%s does not exist", name);
        }
    }
    if (!hasErrors) {
        uint32 numberOfDimensions = matDef.GetNumberOfDimensions();
        hasErrors = (numberOfDimensions != 2u);
        if (hasErrors) {
            REPORT_ERROR_PROXY(ErrorManagement::ParametersError, owner, "%s does not have exactly two dimensions", name);
        }
    }
    if (!hasErrors) {
        numberOfRows = matDef.GetNumberOfElements(1u);
        numberOfCols = matDef.GetNumberOfElements(0u);
        mat = new T*[numberOfRows];
        for (uint32 r=0u; r<numberOfRows; r++) {
            mat[r] = new T[numberOfCols];
        }
        Matrix<T> matReader(mat, numberOfRows, numberOfCols);
        hasErrors = !sdi.Read(name, matReader);
        if (hasErrors) { 
            REPORT_ERROR_PROXY(ErrorManagement::ParametersError, owner, "%s failed to read matrix", name);
            for (uint32 r=0u; r<numberOfRows; r++) {
                delete [] mat[r];
            }
            delete []mat;
            mat = NULL_PTR(T **);
        }
        else {
            REPORT_ERROR_PROXY(ErrorManagement::Information, owner, "%s successfully read matrix %!", name, matReader);
        }
    }
    return !hasErrors;

}

}

#endif /* STRUCTUREDDATAIHELPER_H_ */

