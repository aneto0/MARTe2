/**
 * @file Md5Encrypt.cpp
 * @brief Source file for class Md5Encrypt
 * @date 23/08/2018
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

 * @details This source file contains the definition of all the methods for
 * the class Md5Encrypt (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "Md5Encrypt.h"
#include "MemoryOperationsHelper.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

/*lint -e{9158} -e{9159} -e{9021} -e{123} allow to use #define for this implementation, but this should be revisited*/
namespace Md5Encrypt {

#ifndef MDS5_GET_UINT32_LE
/**
 * @brief 32-bit integer manipulation macros (little endian getter)
 */
#define MDS5_GET_UINT32_LE(n,b,i)                              \
{                                                         \
    (n) = ( static_cast<osulong> ((b)[(i)    ])        )  \
        | ( static_cast<osulong> ((b)[(i) + 1]) <<  8u )  \
        | ( static_cast<osulong> ((b)[(i) + 2]) << 16u )  \
        | ( static_cast<osulong> ((b)[(i) + 3]) << 24u ); \
}
#endif

#ifndef MDS5_PUT_UINT32_LE
/**
 * @brief 32-bit integer manipulation macros (little endian putter)
 */
#define MDS5_PUT_UINT32_LE(n,b,i)                            \
{                                                       \
    (b)[(i)    ] = static_cast<uint8> ( (n)       );    \
    (b)[(i) + 1] = static_cast<uint8> ( (n) >>  8u );   \
    (b)[(i) + 2] = static_cast<uint8> ( (n) >> 16u );   \
    (b)[(i) + 3] = static_cast<uint8> ( (n) >> 24u );   \
}
#endif

/**
 * @brief 32-bit Md5 S function.
 */
#define S(x,n) ((x << n) | ((x & 0xFFFFFFFFu) >> (32u - n)))

/**
 * @brief 32-bit Md5 F1 function.
 */
#define MDS5_F1(x,y,z) (z ^ (x & (y ^ z)))

/**
 * @brief 32-bit Md5 F2 function.
 */
#define MDS5_F2(x,y,z) (y ^ (z & (x ^ y)))

/**
 * @brief 32-bit Md5 F3 function.
 */
#define MDS5_F3(x,y,z) (x ^ y ^ z)

/**
 * @brief 32-bit Md5 F4 function.
 */
#define MDS5_F4(x,y,z) (y ^ (x | ~z))

/**
 * @brief 32-bit Md5 P function.
 */
#define MDS5_P(F,a,b,c,d,k,s,t)                    \
{                                                  \
a += F(b,c,d) + X[k] + t; a = S(a,s) + b;          \
}

static const uint8 md5_padding[64] = { 0x80u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u,
        0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u };

/**
 * @brief Helper class to compute the Md5 hash.
 */
class Md5Context {
public:

    /**
     * @brief Constructor. NOOP.
     */
    Md5Context();
    
    /**
     * @brief Destructor. NOOP.
     */    
    ~Md5Context();

    /**
     * @brief MD5 context setup.
     */
    void Starts();

    /**
     * @brief Udpates the data.
     * @param[in] input the data.
     * @param[in] ilen input length.
     */
    void Update(uint8 *input,
                uint32 ilen);

    /**
     * @brief MD5 computation final step.
     * @param[out] output the computed data.
     */
    void Finish(uint8 *output);

    /**
     * @brief MD5 computation.
     * @param[in] data the data.
     */
    void Md5Process(const uint8 data[64]);

    /**
     * @brief MD5 HMAC context setup
     * @param[in] key the key.
     * @param[in] keylen key length.
     */
    void HmacStarts(const uint8 * const key,
                    const uint32 keylen);

    /**
     * @brief MD5 HMAC process buffer
     * @param[in] input the data.
     * @param[in] ilen input length.
     */
    void HmacUpdate(uint8 * const input,
                    const uint32 ilen);

    /**
     * @brief MD5 HMAC final digest
     * @param[out] output MD5 output.
     */
    void HmacFinish(uint8 * const output);

private:

    osulong total[2]; /*!< number of bytes processed  */
    osulong state[4]; /*!< intermediate digest state  */
    uint8 buffer[64]; /*!< data block being processed */
    uint8 ipad[64]; /*!< HMAC: inner padding        */
    uint8 opad[64]; /*!< HMAC: outer padding        */

};

Md5Context::Md5Context() {
    total[0] = 0u;
    total[1] = 0u;

    state[0] = 0x67452301u;
    state[1] = 0xEFCDAB89u;
    state[2] = 0x98BADCFEu;
    state[3] = 0x10325476u;

    (void) MemoryOperationsHelper::Set(&buffer[0], '\0', 64u);
    (void) MemoryOperationsHelper::Set(&ipad[0], '\0', 64u);
    (void) MemoryOperationsHelper::Set(&opad[0], '\0', 64u);

}

Md5Context::~Md5Context() {

}

/*
 * MD5 context setup
 */
void Md5Context::Starts() {
    total[0] = 0u;
    total[1] = 0u;

    state[0] = 0x67452301u;
    state[1] = 0xEFCDAB89u;
    state[2] = 0x98BADCFEu;
    state[3] = 0x10325476u;
}

void Md5Context::Update(uint8 *input,
                        uint32 ilen) {
    uint32 fill;
    osulong left;

    if (ilen > 0u) {

        left = total[0] & 0x3Fu;
        fill = 64u - static_cast<uint32>(left);

        total[0] += ilen;
        total[0] &= 0xFFFFFFFFu;

        if (total[0] < static_cast<osulong>(ilen)) {
            total[1]++;
        }

        if ((left != 0u) && (ilen >= fill)) {
            (void) MemoryOperationsHelper::Copy(reinterpret_cast<void *>(&buffer[left]), reinterpret_cast<void *>(input), fill);
            Md5Process(&buffer[0]);
            input = &input[fill];
            ilen -= fill;
            left = 0u;
        }

        while (ilen >= 64u) {
            Md5Process(input);
            input = &input[64];
            ilen -= 64u;
        }

        if (ilen > 0u) {
            (void) MemoryOperationsHelper::Copy(reinterpret_cast<void *>(&buffer[left]), reinterpret_cast<void *>(input), ilen);
        }
    }
}

void Md5Context::Finish(uint8 * const output) {
    osulong last;
    osulong padn;
    osulong high;
    osulong low;
    uint8 msglen[8];

    high = (total[0] >> 29u) | (total[1] << 3u);
    low = (total[0] << 3u);

    /*lint -e{835} zero sum allowed*/
    MDS5_PUT_UINT32_LE(low, msglen, 0)
    MDS5_PUT_UINT32_LE(high, msglen, 4)

    last = total[0] & 0x3Fu;
    padn = (last < 56u) ? (56u - last) : (120u - last);

    Update(const_cast<uint8 *>(md5_padding), static_cast<uint32>(padn));
    Update(&msglen[0], 8u);

    /*lint -e{835} zero sum allowed*/
    MDS5_PUT_UINT32_LE(state[0], output, 0)
    MDS5_PUT_UINT32_LE(state[1], output, 4)
    MDS5_PUT_UINT32_LE(state[2], output, 8)
    MDS5_PUT_UINT32_LE(state[3], output, 12)
}

void Md5Context::Md5Process(const uint8 data[64]) {
    osulong X[16];
    osulong A;
    osulong B;
    osulong C;
    osulong D;

    /*lint -e{835} zero sum allowed*/
    MDS5_GET_UINT32_LE(X[0], data, 0)
    MDS5_GET_UINT32_LE(X[1], data, 4)
    MDS5_GET_UINT32_LE(X[2], data, 8)
    MDS5_GET_UINT32_LE(X[3], data, 12)
    MDS5_GET_UINT32_LE(X[4], data, 16)
    MDS5_GET_UINT32_LE(X[5], data, 20)
    MDS5_GET_UINT32_LE(X[6], data, 24)
    MDS5_GET_UINT32_LE(X[7], data, 28)
    MDS5_GET_UINT32_LE(X[8], data, 32)
    MDS5_GET_UINT32_LE(X[9], data, 36)
    MDS5_GET_UINT32_LE(X[10], data, 40)
    MDS5_GET_UINT32_LE(X[11], data, 44)
    MDS5_GET_UINT32_LE(X[12], data, 48)
    MDS5_GET_UINT32_LE(X[13], data, 52)
    MDS5_GET_UINT32_LE(X[14], data, 56)
    MDS5_GET_UINT32_LE(X[15], data, 60)

    A = state[0];
    B = state[1];
    C = state[2];
    D = state[3];

    MDS5_P(MDS5_F1, A, B, C, D, 0, 7u, 0xD76AA478u)
    MDS5_P(MDS5_F1, D, A, B, C, 1, 12u, 0xE8C7B756u)
    MDS5_P(MDS5_F1, C, D, A, B, 2, 17u, 0x242070DBu)
    MDS5_P(MDS5_F1, B, C, D, A, 3, 22u, 0xC1BDCEEEu)
    MDS5_P(MDS5_F1, A, B, C, D, 4, 7u, 0xF57C0FAFu)
    MDS5_P(MDS5_F1, D, A, B, C, 5, 12u, 0x4787C62Au)
    MDS5_P(MDS5_F1, C, D, A, B, 6, 17u, 0xA8304613u)
    MDS5_P(MDS5_F1, B, C, D, A, 7, 22u, 0xFD469501u)
    MDS5_P(MDS5_F1, A, B, C, D, 8, 7u, 0x698098D8u)
    MDS5_P(MDS5_F1, D, A, B, C, 9, 12u, 0x8B44F7AFu)
    MDS5_P(MDS5_F1, C, D, A, B, 10, 17u, 0xFFFF5BB1u)
    MDS5_P(MDS5_F1, B, C, D, A, 11, 22u, 0x895CD7BEu)
    MDS5_P(MDS5_F1, A, B, C, D, 12, 7u, 0x6B901122u)
    MDS5_P(MDS5_F1, D, A, B, C, 13, 12u, 0xFD987193u)
    MDS5_P(MDS5_F1, C, D, A, B, 14, 17u, 0xA679438Eu)
    MDS5_P(MDS5_F1, B, C, D, A, 15, 22u, 0x49B40821u)

    MDS5_P(MDS5_F2, A, B, C, D, 1, 5u, 0xF61E2562u)
    MDS5_P(MDS5_F2, D, A, B, C, 6, 9u, 0xC040B340u)
    MDS5_P(MDS5_F2, C, D, A, B, 11, 14u, 0x265E5A51u)
    MDS5_P(MDS5_F2, B, C, D, A, 0, 20u, 0xE9B6C7AAu)
    MDS5_P(MDS5_F2, A, B, C, D, 5, 5u, 0xD62F105Du)
    MDS5_P(MDS5_F2, D, A, B, C, 10, 9u, 0x02441453u)
    MDS5_P(MDS5_F2, C, D, A, B, 15, 14u, 0xD8A1E681u)
    MDS5_P(MDS5_F2, B, C, D, A, 4, 20u, 0xE7D3FBC8u)
    MDS5_P(MDS5_F2, A, B, C, D, 9, 5u, 0x21E1CDE6u)
    MDS5_P(MDS5_F2, D, A, B, C, 14, 9u, 0xC33707D6u)
    MDS5_P(MDS5_F2, C, D, A, B, 3, 14u, 0xF4D50D87u)
    MDS5_P(MDS5_F2, B, C, D, A, 8, 20u, 0x455A14EDu)
    MDS5_P(MDS5_F2, A, B, C, D, 13, 5u, 0xA9E3E905u)
    MDS5_P(MDS5_F2, D, A, B, C, 2, 9u, 0xFCEFA3F8u)
    MDS5_P(MDS5_F2, C, D, A, B, 7, 14u, 0x676F02D9u)
    MDS5_P(MDS5_F2, B, C, D, A, 12, 20u, 0x8D2A4C8Au)

    MDS5_P(MDS5_F3, A, B, C, D, 5, 4u, 0xFFFA3942u)
    MDS5_P(MDS5_F3, D, A, B, C, 8, 11u, 0x8771F681u)
    MDS5_P(MDS5_F3, C, D, A, B, 11, 16u, 0x6D9D6122u)
    MDS5_P(MDS5_F3, B, C, D, A, 14, 23u, 0xFDE5380Cu)
    MDS5_P(MDS5_F3, A, B, C, D, 1, 4u, 0xA4BEEA44u)
    MDS5_P(MDS5_F3, D, A, B, C, 4, 11u, 0x4BDECFA9u)
    MDS5_P(MDS5_F3, C, D, A, B, 7, 16u, 0xF6BB4B60u)
    MDS5_P(MDS5_F3, B, C, D, A, 10, 23u, 0xBEBFBC70u)
    MDS5_P(MDS5_F3, A, B, C, D, 13, 4u, 0x289B7EC6u)
    MDS5_P(MDS5_F3, D, A, B, C, 0, 11u, 0xEAA127FAu)
    MDS5_P(MDS5_F3, C, D, A, B, 3, 16u, 0xD4EF3085u)
    MDS5_P(MDS5_F3, B, C, D, A, 6, 23u, 0x04881D05u)
    MDS5_P(MDS5_F3, A, B, C, D, 9, 4u, 0xD9D4D039u)
    MDS5_P(MDS5_F3, D, A, B, C, 12, 11u, 0xE6DB99E5u)
    MDS5_P(MDS5_F3, C, D, A, B, 15, 16u, 0x1FA27CF8u)
    MDS5_P(MDS5_F3, B, C, D, A, 2, 23u, 0xC4AC5665u)

    MDS5_P(MDS5_F4, A, B, C, D, 0, 6u, 0xF4292244u)
    MDS5_P(MDS5_F4, D, A, B, C, 7, 10u, 0x432AFF97u)
    MDS5_P(MDS5_F4, C, D, A, B, 14, 15u, 0xAB9423A7u)
    MDS5_P(MDS5_F4, B, C, D, A, 5, 21u, 0xFC93A039u)
    MDS5_P(MDS5_F4, A, B, C, D, 12, 6u, 0x655B59C3u)
    MDS5_P(MDS5_F4, D, A, B, C, 3, 10u, 0x8F0CCC92u)
    MDS5_P(MDS5_F4, C, D, A, B, 10, 15u, 0xFFEFF47Du)
    MDS5_P(MDS5_F4, B, C, D, A, 1, 21u, 0x85845DD1u)
    MDS5_P(MDS5_F4, A, B, C, D, 8, 6u, 0x6FA87E4Fu)
    MDS5_P(MDS5_F4, D, A, B, C, 15, 10u, 0xFE2CE6E0u)
    MDS5_P(MDS5_F4, C, D, A, B, 6, 15u, 0xA3014314u)
    MDS5_P(MDS5_F4, B, C, D, A, 13, 21u, 0x4E0811A1u)
    MDS5_P(MDS5_F4, A, B, C, D, 4, 6u, 0xF7537E82u)
    MDS5_P(MDS5_F4, D, A, B, C, 11, 10u, 0xBD3AF235u)
    MDS5_P(MDS5_F4, C, D, A, B, 2, 15u, 0x2AD7D2BBu)
    MDS5_P(MDS5_F4, B, C, D, A, 9, 21u, 0xEB86D391u)

    state[0] += A;
    state[1] += B;
    state[2] += C;
    state[3] += D;
}

/*
 * MD5 HMAC context setup
 */
void Md5Context::HmacStarts(const uint8 * const key,
                            const uint32 keylen) {

    (void) MemoryOperationsHelper::Set(&ipad[0], static_cast<char8>(0x36), 64u);
    (void) MemoryOperationsHelper::Set(&opad[0], static_cast<char8>(0x5C), 64u);

    for (uint32 i = 0u; i < keylen; i++) {
        if (i >= 64u) {
            break;
        }

        ipad[i] ^= key[i];
        opad[i] ^= key[i];
    }

    Starts();
    Update(&ipad[0], 64u);
}

/*
 * MD5 HMAC process buffer
 */
void Md5Context::HmacUpdate(uint8 * const input,
                            const uint32 ilen) {
    Update(input, ilen);
}

/*
 * MD5 HMAC final digest
 */
void Md5Context::HmacFinish(uint8 * const output) {
    uint8 tmpbuf[16];

    Finish(&tmpbuf[0]);
    Starts();
    Update(&opad[0], 64u);
    Update(&tmpbuf[0], 16u);
    Finish(&output[0]);

    (void) MemoryOperationsHelper::Set(&tmpbuf[0], '\0', 16u);
}

/*
 * Output = MD5( input buffer )
 */
void Md5(uint8 * const input,
         const uint32 ilen,
         uint8 * const output) {
    Md5Context ctx;

    ctx.Starts();
    ctx.Update(input, ilen);
    ctx.Finish(output);

}

/*
 * output = HMAC-MD5( hmac key (keylen), input buffer )
 */
void Md5Hmac(const uint8 * const key,
             const uint32 keylen,
             uint8 * const input,
             const uint32 ilen,
             uint8 * const output) {
    Md5Context ctx;

    ctx.HmacStarts(key, keylen);
    ctx.HmacUpdate(input, ilen);
    ctx.HmacFinish(output);

    (void) MemoryOperationsHelper::Set(&ctx, '\0', static_cast<uint32>(sizeof(Md5Context)));
}

}
}
