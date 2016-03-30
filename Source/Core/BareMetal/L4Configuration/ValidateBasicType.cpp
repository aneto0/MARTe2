/**
 * @file ValidateBasicType.cpp
 * @brief Source file for class ValidateBasicType
 * @date 25/gen/2016
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
 * the class ValidateBasicType (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#define DLL_API
/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "ValidateBasicType.h"
#include "TypeDescriptor.h"
#include "TypeConversion.h"
#include "StringHelper.h"
#include "StandardParser.h"
#include "ConfigurationDatabase.h"
#include "ErrorManagement.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

static const uint32 defaultBufferSize = 64u;

/**
 * @brief Checks the validity of a string.
 * @param[in] stringValue is the value to be checked.
 * @param[in] cdb contains the attributes.
 */
static bool PrivateValidate(const char8 * const stringValue,
                            ConfigurationDatabase &cdb) {

    bool ret = true;

    char8 max[defaultBufferSize];
    if (cdb.Read("max", max)) {
        if (StringHelper::Compare(&max[0], stringValue) == 1) {
            ret = false;
        }
    }
    if (ret) {
        char8 min[defaultBufferSize];
        if (cdb.Read("min", min)) {
            if (StringHelper::Compare(&min[0], stringValue) == 2) {
                ret = false;
            }
        }
    }
    if (ret) {
        uint32 size;
        if (cdb.Read("min_size", size)) {
            if (StringHelper::Length(stringValue) < size) {
                ret = false;
            }
        }
    }

    if (ret) {
        uint32 size;
        if (cdb.Read("max_size", size)) {
            if (StringHelper::Length(stringValue) > size) {
                ret = false;
            }
        }
    }
    if (ret) {

        AnyType values = cdb.GetType("values");

        if (values.GetDataPointer() != NULL) {
            uint32 numberOfValues = values.GetNumberOfElements(0u);
            Matrix<char8> elements(numberOfValues, defaultBufferSize);
            if(cdb.Read("values", elements)) {
                ret=false;
                for (uint32 i = 0u; (i < numberOfValues) && (!ret); i++) {
                    Vector<char8> temp=elements[i];
                    ret = (StringHelper::Compare(&(temp[0u]), stringValue) == 0);
                }
            }
        }
    }

    return ret;

}

/**
 * @brief Checks the validity of a 8 bit signed integer.
 * @param[in] intValue is the value to be checked.
 * @param[in] cdb contains the attributes.
 */
static bool PrivateValidate(const int8 intValue,
                            ConfigurationDatabase &cdb) {
    bool ret = true;

    int8 max;
    if (cdb.Read("max", max)) {
        if (intValue > max) {
            ret = false;
        }
    }

    if (ret) {
        int8 min;
        if (cdb.Read("min", min)) {
            if (intValue < min) {
                ret = false;
            }
        }
    }
    if (ret) {
        uint32 size;
        if (cdb.Read("min_size", size)) {
            char8 buffer[defaultBufferSize];
            if (TypeConvert(buffer, intValue)) {
                if (StringHelper::Length(&buffer[0]) < size) {
                    ret = false;
                }
            }
        }
    }

    if (ret) {
        uint32 size;
        if (cdb.Read("max_size", size)) {
            char8 buffer[defaultBufferSize];
            if (TypeConvert(buffer, intValue)) {
                if (StringHelper::Length(&buffer[0]) > size) {
                    ret = false;
                }
            }
        }
    }

    if (ret) {
        AnyType values = cdb.GetType("values");

        if (values.GetDataPointer() != NULL) {
            uint32 numberOfValues = values.GetNumberOfElements(0u);
            Vector<int8> elements(numberOfValues);
            if(cdb.Read("values", elements)) {

                ret = false;

                for (uint32 i = 0u; (i < numberOfValues) && (!ret); i++) {
                    ret = (intValue == elements[i]);
                }
            }
        }
    }

    return ret;

}

/**
 * @brief Checks the validity of a 16 bit signed integer.
 * @param[in] intValue is the value to be checked.
 * @param[in] cdb contains the attributes.
 */
static bool PrivateValidate(const int16 intValue,
                            ConfigurationDatabase &cdb) {

    bool ret = true;

    int16 max;
    if (cdb.Read("max", max)) {
        if (intValue > max) {
            ret = false;
        }
    }

    if (ret) {
        int16 min;
        if (cdb.Read("min", min)) {

            if (intValue < min) {
                ret = false;
            }
        }
    }
    if (ret) {
        uint32 size;
        if (cdb.Read("min_size", size)) {
            char8 buffer[defaultBufferSize];
            if (TypeConvert(buffer, intValue)) {
                if (StringHelper::Length(&buffer[0]) < size) {
                    ret = false;
                }
            }
        }
    }

    if (ret) {
        uint32 size;
        if (cdb.Read("max_size", size)) {
            char8 buffer[defaultBufferSize];
            if (TypeConvert(buffer, intValue)) {
                if (StringHelper::Length(&buffer[0]) > size) {
                    ret = false;
                }
            }
        }
    }

    if (ret) {
        AnyType values = cdb.GetType("values");

        if (values.GetDataPointer() != NULL) {
            uint32 numberOfValues = values.GetNumberOfElements(0u);
            Vector<int16> elements(numberOfValues);
            if(cdb.Read("values", elements)) {

                ret = false;

                for (uint32 i = 0u; (i < numberOfValues) && (!ret); i++) {
                    ret = (intValue == elements[i]);
                }
            }
        }
    }
    return ret;

}

/**
 * @brief Checks the validity of a 32 bit signed integer.
 * @param[in] intValue is the value to be checked.
 * @param[in] cdb contains the attributes.
 */
static bool PrivateValidate(const int32 intValue,
                            ConfigurationDatabase &cdb) {

    bool ret = true;

    int32 max;
    if (cdb.Read("max", max)) {
        if (intValue > max) {
            ret = false;
        }
    }

    if (ret) {
        int32 min;
        if (cdb.Read("min", min)) {

            if (intValue < min) {
                ret = false;
            }
        }
    }

    if (ret) {
        uint32 size;
        if (cdb.Read("min_size", size)) {
            char8 buffer[defaultBufferSize];
            if (TypeConvert(buffer, intValue)) {
                if (StringHelper::Length(&buffer[0]) < size) {
                    ret = false;
                }
            }
        }
    }

    if (ret) {
        uint32 size;
        if (cdb.Read("max_size", size)) {
            char8 buffer[defaultBufferSize];
            if (TypeConvert(buffer, intValue)) {
                if (StringHelper::Length(&buffer[0]) > size) {
                    ret = false;
                }
            }
        }
    }
    if (ret) {
        AnyType values = cdb.GetType("values");

        if (values.GetDataPointer() != NULL) {
            uint32 numberOfValues = values.GetNumberOfElements(0u);
            Vector<int32> elements(numberOfValues);
            if(cdb.Read("values", elements)) {

                ret = false;

                for (uint32 i = 0u; (i < numberOfValues) && (!ret); i++) {
                    ret = (intValue == elements[i]);
                }
            }
        }
    }
    return ret;

}

/**
 * @brief Checks the validity of a 64 bit signed integer.
 * @param[in] intValue is the value to be checked.
 * @param[in] cdb contains the attributes.
 */
static bool PrivateValidate(const int64 intValue,
                            ConfigurationDatabase &cdb) {
    bool ret = true;

    int64 max;
    if (cdb.Read("max", max)) {
        if (intValue > max) {
            ret = false;
        }
    }

    if (ret) {
        int64 min;
        if (cdb.Read("min", min)) {

            if (intValue < min) {
                ret = false;
            }
        }
    }
    if (ret) {
        uint32 size;
        if (cdb.Read("min_size", size)) {
            char8 buffer[defaultBufferSize];
            if (TypeConvert(buffer, intValue)) {
                if (StringHelper::Length(&buffer[0]) < size) {
                    ret = false;
                }
            }
        }
    }

    if (ret) {
        uint32 size;
        if (cdb.Read("max_size", size)) {
            char8 buffer[defaultBufferSize];
            if (TypeConvert(buffer, intValue)) {
                if (StringHelper::Length(&buffer[0]) > size) {
                    ret = false;
                }
            }
        }
    }

    if (ret) {
        AnyType values = cdb.GetType("values");

        if (values.GetDataPointer() != NULL) {
            uint32 numberOfValues = values.GetNumberOfElements(0u);
            Vector<int64> elements(numberOfValues);
            if(cdb.Read("values", elements)) {

                ret = false;

                for (uint32 i = 0u; (i < numberOfValues) && (!ret); i++) {
                    ret = (intValue == elements[i]);
                }
            }
        }
    }
    return ret;
}

/**
 * @brief Checks the validity of a 8 bit unsigned integer.
 * @param[in] uintValue is the value to be checked.
 * @param[in] cdb contains the attributes.
 */
static bool PrivateValidate(const uint8 uintValue,
                            ConfigurationDatabase &cdb) {

    bool ret = true;

    uint8 max;
    if (cdb.Read("max", max)) {
        if (uintValue > max) {
            ret = false;
        }
    }

    if (ret) {
        uint8 min;
        if (cdb.Read("min", min)) {

            if (uintValue < min) {
                ret = false;
            }
        }
    }

    if (ret) {
        uint32 size;
        if (cdb.Read("min_size", size)) {
            char8 buffer[defaultBufferSize];
            if (TypeConvert(buffer, uintValue)) {
                if (StringHelper::Length(&buffer[0]) < size) {
                    ret = false;
                }
            }
        }
    }

    if (ret) {
        uint32 size;
        if (cdb.Read("max_size", size)) {
            char8 buffer[defaultBufferSize];
            if (TypeConvert(buffer, uintValue)) {
                if (StringHelper::Length(&buffer[0]) > size) {
                    ret = false;
                }
            }
        }
    }
    if (ret) {
        AnyType values = cdb.GetType("values");

        if (values.GetDataPointer() != NULL) {
            uint32 numberOfValues = values.GetNumberOfElements(0u);
            Vector<uint8> elements(numberOfValues);
            if(cdb.Read("values", elements)) {

                ret = false;

                for (uint32 i = 0u; (i < numberOfValues) && (!ret); i++) {
                    ret = (uintValue == elements[i]);
                }
            }
        }
    }
    return ret;
}

/**
 * @brief Checks the validity of a 16 bit unsigned integer.
 * @param[in] uintValue is the value to be checked.
 * @param[in] cdb contains the attributes.
 */
static bool PrivateValidate(const uint16 uintValue,
                            ConfigurationDatabase &cdb) {

    bool ret = true;

    uint16 max;
    if (cdb.Read("max", max)) {
        if (uintValue > max) {
            ret = false;
        }
    }

    if (ret) {
        uint16 min;
        if (cdb.Read("min", min)) {

            if (uintValue < min) {
                ret = false;
            }
        }
    }
    if (ret) {
        uint32 size;
        if (cdb.Read("min_size", size)) {
            char8 buffer[defaultBufferSize];
            if (TypeConvert(buffer, uintValue)) {
                if (StringHelper::Length(&buffer[0]) < size) {
                    ret = false;
                }
            }
        }
    }

    if (ret) {
        uint32 size;
        if (cdb.Read("max_size", size)) {
            char8 buffer[defaultBufferSize];
            if (TypeConvert(buffer, uintValue)) {
                if (StringHelper::Length(&buffer[0]) > size) {
                    ret = false;
                }
            }
        }
    }
    if (ret) {
        AnyType values = cdb.GetType("values");

        if (values.GetDataPointer() != NULL) {
            uint32 numberOfValues = values.GetNumberOfElements(0u);
            Vector<uint16> elements(numberOfValues);
            if(cdb.Read("values", elements)) {

                ret = false;

                for (uint32 i = 0u; (i < numberOfValues) && (!ret); i++) {
                    ret = (uintValue == elements[i]);
                }
            }
        }
    }
    return ret;
}

/**
 * @brief Checks the validity of a 32 bit unsigned integer.
 * @param[in] uintValue is the value to be checked.
 * @param[in] cdb contains the attributes.
 */
static bool PrivateValidate(const uint32 uintValue,
                            ConfigurationDatabase &cdb) {

    bool ret = true;

    uint32 max;
    if (cdb.Read("max", max)) {
        if (uintValue > max) {
            ret = false;
        }
    }

    if (ret) {
        uint32 min;
        if (cdb.Read("min", min)) {

            if (uintValue < min) {
                ret = false;
            }
        }
    }
    if (ret) {
        uint32 size;
        if (cdb.Read("min_size", size)) {
            char8 buffer[defaultBufferSize];
            if (TypeConvert(buffer, uintValue)) {
                if (StringHelper::Length(&buffer[0]) < size) {
                    ret = false;
                }
            }
        }
    }

    if (ret) {
        uint32 size;
        if (cdb.Read("max_size", size)) {
            char8 buffer[defaultBufferSize];
            if (TypeConvert(buffer, uintValue)) {
                if (StringHelper::Length(&buffer[0]) > size) {
                    ret = false;
                }
            }
        }
    }
    if (ret) {
        AnyType values = cdb.GetType("values");

        if (values.GetDataPointer() != NULL) {
            uint32 numberOfValues = values.GetNumberOfElements(0u);
            Vector<uint32> elements(numberOfValues);
            if(cdb.Read("values", elements)) {

                ret = false;

                for (uint32 i = 0u; (i < numberOfValues) && (!ret); i++) {
                    ret = (uintValue == elements[i]);
                }
            }
        }
    }
    return ret;

}

/**
 * @brief Checks the validity of a 64 bit unsigned integer.
 * @param[in] uintValue is the value to be checked.
 * @param[in] cdb contains the attributes.
 */
static bool PrivateValidate(const uint64 uintValue,
                            ConfigurationDatabase &cdb) {

    bool ret = true;

    uint64 max;
    if (cdb.Read("max", max)) {
        if (uintValue > max) {
            ret = false;
        }
    }

    if (ret) {
        uint64 min;
        if (cdb.Read("min", min)) {

            if (uintValue < min) {
                ret = false;
            }
        }
    }
    if (ret) {
        uint32 size;
        if (cdb.Read("min_size", size)) {
            char8 buffer[defaultBufferSize];
            if (TypeConvert(buffer, uintValue)) {
                if (StringHelper::Length(&buffer[0]) < size) {
                    ret = false;
                }
            }
        }
    }

    if (ret) {
        uint32 size;
        if (cdb.Read("max_size", size)) {
            char8 buffer[defaultBufferSize];
            if (TypeConvert(buffer, uintValue)) {
                if (StringHelper::Length(&buffer[0]) > size) {
                    ret = false;
                }
            }
        }
    }
    if (ret) {
        AnyType values = cdb.GetType("values");

        if (values.GetDataPointer() != NULL) {
            uint32 numberOfValues = values.GetNumberOfElements(0u);
            Vector<uint64> elements(numberOfValues);
            if(cdb.Read("values", elements)) {

                ret = false;

                for (uint32 i = 0u; (i < numberOfValues) && (!ret); i++) {
                    ret = (uintValue == elements[i]);
                }
            }
        }
    }
    return ret;

}

/**
 * @brief Checks the validity of a 32 bit float.
 * @param[in] floatValue is the value to be checked.
 * @param[in] cdb contains the attributes.
 */
static bool PrivateValidate(const float32 floatValue,
                            ConfigurationDatabase &cdb) {
    bool ret = true;

    float32 max;
    if (cdb.Read("max", max)) {
        if (floatValue > max) {
            ret = false;
        }
    }

    if (ret) {
        float32 min;
        if (cdb.Read("min", min)) {

            if (floatValue < min) {
                ret = false;
            }
        }
    }
    if (ret) {
        uint32 size;
        if (cdb.Read("min_size", size)) {
            char8 buffer[defaultBufferSize];
            if (TypeConvert(buffer, floatValue)) {
                if (StringHelper::Length(&buffer[0]) < size) {
                    ret = false;
                }
            }
        }
    }

    if (ret) {
        uint32 size;
        if (cdb.Read("max_size", size)) {
            char8 buffer[defaultBufferSize];
            if (TypeConvert(buffer, floatValue)) {
                if (StringHelper::Length(&buffer[0]) > size) {
                    ret = false;
                }
            }
        }
    }
    if (ret) {
        AnyType values = cdb.GetType("values");

        if (values.GetDataPointer() != NULL) {
            uint32 numberOfValues = values.GetNumberOfElements(0u);
            Vector<float32> elements(numberOfValues);
            if(cdb.Read("values", elements)) {

                ret = false;

                for (uint32 i = 0u; (i < numberOfValues) && (!ret); i++) {
                    ret = IsEqual(floatValue, elements[i]);
                }
            }
        }
    }
    return ret;

}

/**
 * @brief Checks the validity of a 64 bit float.
 * @param[in] floatValue is the value to be checked.
 * @param[in] cdb contains the attributes.
 */
static bool PrivateValidate(const float64 floatValue,
                            ConfigurationDatabase &cdb) {

    bool ret = true;

    float64 max;
    if (cdb.Read("max", max)) {
        if (floatValue > max) {
            ret = false;
        }
    }

    if (ret) {
        float64 min;
        if (cdb.Read("min", min)) {

            if (floatValue < min) {
                ret = false;
            }
        }
    }
    if (ret) {
        uint32 size;
        if (cdb.Read("min_size", size)) {
            char8 buffer[defaultBufferSize];
            if (TypeConvert(buffer, floatValue)) {
                if (StringHelper::Length(&buffer[0]) < size) {
                    ret = false;
                }
            }
        }
    }

    if (ret) {
        uint32 size;
        if (cdb.Read("max_size", size)) {
            char8 buffer[defaultBufferSize];
            if (TypeConvert(buffer, floatValue)) {
                if (StringHelper::Length(&buffer[0]) > size) {
                    ret = false;
                }
            }
        }
    }
    if (ret) {
        AnyType values = cdb.GetType("values");

        if (values.GetDataPointer() != NULL) {
            uint32 numberOfValues = values.GetNumberOfElements(0u);
            Vector<float64> elements(numberOfValues);
            if(cdb.Read("values", elements)) {

                ret = false;

                for (uint32 i = 0u; (i < numberOfValues) && (!ret); i++) {
                    ret = IsEqual(floatValue, elements[i]);
                }
            }
        }
    }
    return ret;
}

/**
 * @brief Checks the validity of a char.
 * @param[in] charValue is the value to be checked.
 * @param[in] cdb contains the attributes.
 */
static bool PrivateValidate(const char8 charValue,
                            ConfigurationDatabase &cdb) {

    bool ret = true;

    char8 max;
    if (cdb.Read("max", max)) {
        if (static_cast<int8>(charValue) > static_cast<int8>(max)) {
            ret = false;
        }
    }

    if (ret) {
        char8 min;
        if (cdb.Read("min", min)) {

            if (static_cast<int8>(charValue) < static_cast<int8>(min)) {
                ret = false;
            }
        }
    }
    if (ret) {
        uint32 size;
        if (cdb.Read("min_size", size)) {
            if (size > 1u) {
                ret = false;
            }
        }
    }

    if (ret) {
        uint32 size;
        if (cdb.Read("max_size", size)) {
            if (size == 0u) {
                ret = false;
            }
        }
    }
    if (ret) {

        char8 elements[defaultBufferSize];
        if (cdb.Read("values", elements)) {
            uint32 lastIndex = defaultBufferSize - 1u;
            elements[lastIndex] = '\0';
            uint32 size = StringHelper::Length(&elements[0]);

            ret = false;

            for (uint32 i = 0u; (i < size) && (!ret); i++) {
                ret = (charValue == elements[i]);
            }
        }
    }
    return ret;
}

bool ValidateBasicType(const AnyType &value,
                       const char8* const attributes) {

    bool ret = true;
    bool recognized = true;
    ConfigurationDatabase cdb;

    StreamString configString = attributes;
    if (configString.Seek(0ULL)) {

        StandardParser parser(configString, cdb);
        if (parser.Parse()) {

            if (cdb.MoveToRoot()) {

                uint32 indexCols = (value.GetNumberOfDimensions() > 0u) ? (static_cast<uint32>(value.GetNumberOfDimensions()) - 1u) : (0u);
                uint32 indexRows = (indexCols > 0u) ? (indexCols - 1u) : (0u);

                uint32 nCols = value.GetNumberOfElements(indexCols);
                uint32 nRows = value.GetNumberOfElements(indexRows);

                for (uint32 i = 0u; (i < nRows) && (ret) && (recognized); i++) {
                    for (uint32 j = 0u; (j < nCols) && (ret) && (recognized); j++) {

                        AnyType at;
                        if (value.GetNumberOfDimensions() == 0u) {
                            at = value;
                        }
                        else if (value.GetNumberOfDimensions() == 1u) {
                            at = value[j];
                        }
                        else if (value.GetNumberOfDimensions() == 2u) {
                            at = value[i][j];
                        }
                        else {

                        }

                        if (at.GetDataPointer() != NULL) {
                            TypeDescriptor descriptor = at.GetTypeDescriptor();
                            if (descriptor == CharString) {
                                const char8 * stringValue = static_cast<const char8*>(at.GetDataPointer());
                                ret = PrivateValidate(stringValue, cdb);
                            }
                            else if (descriptor == SignedInteger8Bit) {
                                int8 int8Value = *static_cast<int8*>(at.GetDataPointer());
                                ret = PrivateValidate(int8Value, cdb);
                            }
                            else if (descriptor == SignedInteger16Bit) {
                                int16 int16Value = *static_cast<int16*>(at.GetDataPointer());
                                ret = PrivateValidate(int16Value, cdb);
                            }
                            else if (descriptor == SignedInteger32Bit) {
                                int32 int32Value = *static_cast<int32*>(at.GetDataPointer());
                                ret = PrivateValidate(int32Value, cdb);
                            }
                            else if (descriptor == SignedInteger64Bit) {
                                int64 int64Value = *static_cast<int64*>(at.GetDataPointer());
                                ret = PrivateValidate(int64Value, cdb);
                            }
                            else if (descriptor == UnsignedInteger8Bit) {
                                uint8 uint8Value = *static_cast<uint8*>(at.GetDataPointer());
                                ret = PrivateValidate(uint8Value, cdb);
                            }
                            else if (descriptor == UnsignedInteger16Bit) {
                                uint16 uint16Value = *static_cast<uint16*>(at.GetDataPointer());
                                ret = PrivateValidate(uint16Value, cdb);
                            }
                            else if (descriptor == UnsignedInteger32Bit) {
                                uint32 uint32Value = *static_cast<uint32*>(value.GetDataPointer());
                                ret = PrivateValidate(uint32Value, cdb);
                            }
                            else if (descriptor == UnsignedInteger64Bit) {
                                uint64 uint64Value = *static_cast<uint64*>(at.GetDataPointer());
                                ret = PrivateValidate(uint64Value, cdb);
                            }
                            else if (descriptor == Float32Bit) {
                                float32 float32Value = *static_cast<float32*>(at.GetDataPointer());
                                ret = PrivateValidate(float32Value, cdb);
                            }
                            else if (descriptor == Float64Bit) {
                                float64 float64Value = *static_cast<float64*>(at.GetDataPointer());
                                ret = PrivateValidate(float64Value, cdb);
                            }
                            else if (descriptor == Character8Bit) {
                                char8 char8Value = *static_cast<char8*>(at.GetDataPointer());
                                ret = PrivateValidate(char8Value, cdb);
                            }
                            else {
                                recognized=false;
                                REPORT_ERROR(ErrorManagement::Warning, "ValidateBasicType: Basic Type not matched");
                            }
                        }
                    }
                }
            }
        }
    }

    return ret;
}

}
