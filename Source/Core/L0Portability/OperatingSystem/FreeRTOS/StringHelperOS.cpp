/**
 * @file StringHelper.cpp
 * @brief Source file for class StringHelper
 * @date 30/06/2015
 * @author Giuseppe Ferr�
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
 * the class StringHelper (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "../../StringHelper.h"
#include "../../StringPortable.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

char8 *StringHelper::Concatenate(char8 *destination, const char8 *source) {
    StringPortable::Concatenate(destination, source);
    return destination;
}

char8 *StringHelper::ConcatenateN(char8 *destination, const char8 *source, const uint32 size) {
    uint32 sizeToSet = size;
    StringPortable::ConcatenateN(destination, source, sizeToSet);
    return destination;
}

const char8 *StringHelper::SearchChar(const char8 *string, const char8 c) {
    StringPortable::SearchChar(string, c);
    return string;
}

int32 StringHelper::Compare(const char8 *string1, const char8 *string2) {
    return StringPortable::Compare(string1, string2);
}

int32 StringHelper::CompareN(const char8 *string1, const char8 *string2, const uint32 size) {
    uint32 sizeToCompare = size;
    return StringPortable::CompareN(string1, string2, sizeToCompare);
}

bool StringHelper::Copy(char8 *destination, const char8 *source) {
    return StringPortable::Copy(destination, source);
}

bool StringHelper::CopyN(char8 *destination, const char8 *source, const uint32 size) {
    uint32 sizeToCopy = size;
    return StringPortable::CopyN(destination, source, sizeToCopy);
}

int32 StringHelper::SearchIndex(const char8 *string1, const char8 *string2) {
    return StringPortable::SearchIndex(string1, string2);
}

uint32 StringHelper::Length(const char8 *string) {
    return StringPortable::Length(string);
}

const char8 *StringHelper::SearchChars(const char8 *string1, const char8 *string2) {
    return StringPortable::SearchChars(string1, string2);
}

const char8 *StringHelper::SearchLastChar(const char8 *string, char8 c) {
    return StringPortable::SearchLastChar(string, c);
}

const char8 *StringHelper::SearchString(const char8 *string, const char8 *substring) {
    return StringPortable::SearchString(string, substring);
}

bool StringHelper::SetChar(char8 *string, const uint32 &size, const char8 &c) {
    return StringPortable::SetChar(string, size, c);
}
