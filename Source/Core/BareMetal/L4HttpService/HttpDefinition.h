/**
 * @file HttpDefinition.h
 * @brief Header file for class HttpDefinition
 * @date 17/08/2018
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

 * @details This header file contains the declaration of the class HttpDefinition
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef HTTPDEFINITION_H_
#define HTTPDEFINITION_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "BufferedStreamI.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief Functions and constants utilities for the HTTP protocol implementation
 */
namespace HttpDefinition {

/**
 * No content length specified
 */
static const int32 HTTPNoContentLengthSpecified = -2;

/**
 * None command
 */
static const int32 HSHCNone = 0;

/**
 * HTTP GET command
 */
static const int32 HSHCGet = 1;

/**
 * HTTP PUT command
 */
static const int32 HSHCPut = 2;

/**
 * HTTP POST command
 */
static const int32 HSHCPost = 3;

/**
 * HTTP HEAD command
 */
static const int32 HSHCHead = 4;

/**
 * HTTP REPLY command
 */
static const int32 HSHCReply = 0x10000000;

/**
 * HTTP REPLY OK command
 */
static const int32 HSHCReplyOK = (HSHCReply + 200);

/**
 * HTTP REPLY BAD REQUEST
 */
static const int32 HSHCReplyBadRequest = (HSHCReply + 400);

/**
 * HTTP REPLY AUTH REQUIRED command
 */
static const int32 HSHCReplyAUTH = (HSHCReply + 401);

/**
 * HTTP REPLY NOT FOUND
 */
static const int32 HSHCReplyNotFound = (HSHCReply + 404);

/**
 * HTTP TOO MANY REQUESTS
 */
static const int32 HSHCReplyTooManyRequests = (HSHCReply + 429);

/**
 * @brief Creates a HSHttpCommand relative to reply with a specific ErrorCode
 * @param[in] httpErrorCode the received HTTP error code.
 * @return the reply value associated to \a httpErrorCode.
 */
static inline int32 GenerateReplyCode(const int32 httpErrorCode);

/**
 * @brief Asserts if the command in input is a reply or not.
 * @param[in] command the command to be checked.
 * param[out] httpErrorCode contains the reply code in output if \a command is a reply.
 * @return true if command is a reply, false otherwise.
 */
static inline bool IsReplyCode(const int32 command, int32 &httpErrorCode);

/**
 * @brief return the HTTP string corresponding to the error code.
 * @param[in] httpErrorCode the error code in input.
 * @return the HTTP string corresponding to \a httpErrorCode.
 */
static inline const char8 *GetErrorCodeString(const int32 httpErrorCode);

/**
 * @brief Encodes the url in input.
 * @param[out] converted is the encoded url.
 * @param[in] original is the url in input to be encoded.
 */
bool HttpEncode(BufferedStreamI &converted, const char8 * const original);

/**
 * @brief Converts the hex value in the input character to a decimal value.
 * @param[in] c is the character in input containing '0' to '9' and 'A' to 'F'.
 * @return -1 if c does not represent a valid hex number, the converted decimal
 * value otherwise.
 */
int32 HexDecode(const char8 c);

/**
 * @brief Deodes the url in input.
 * @param[out] destination is the decoded url.
 * @param[in] source is the encoded url in input to be decoded.
 */
bool HttpDecode(BufferedStreamI &destination, BufferedStreamI &source);

}
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief Functions and constants utilities for the HTTP protocol implementation
 */
namespace HttpDefinition {

int32 GenerateReplyCode(const int32 httpErrorCode) {
    return (httpErrorCode + HSHCReply);
}

bool IsReplyCode(const int32 command, int32 &httpErrorCode) {

    bool ret = (command >= HSHCReply);
    if (ret) {
        httpErrorCode = command - HSHCReply;
    }
    return ret;
}

const char8 *GetErrorCodeString(const int32 httpErrorCode) {
    const char8 *ret = "Unknown code";

    switch (httpErrorCode) {
    case 200: {
        ret = "OK";
    }
        break;
    case 201: {
        ret = "CREATED";
    }
        break;
    case 202: {
        ret = "Accepted";
    }
        break;
    case 203: {
        ret = "Partial Information";
    }
        break;
    case 204: {
        ret = "No Response";
    }
        break;
    case 400: {
        ret = "Bad request";
    }
        break;
    case 401: {
        ret = "Unauthorized";
    }
        break;
    case 402: {
        ret = "PaymentRequired";
    }
        break;
    case 403: {
        ret = "Forbidden";
    }
        break;
    case 404: {
        ret = "Not found";
    }
        break;
    case 500: {
        ret = "Internal Error";
    }
        break;
    case 501: {
        ret = "Not implemented";
    }
        break;
    case 301: {
        ret = "Moved";
    }
        break;
    case 302: {
        ret = "Found";
    }
        break;
    case 303: {
        ret = "Method";
    }
        break;
    default: {

    }
    }

    return ret;
}
}
}

#endif /* HTTPDEFINITION_H_ */

