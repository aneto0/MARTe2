/**
 * @file HttpDigestRealm.h
 * @brief Header file for class HttpDigestRealm
 * @date Sep 6, 2016
 * @author fsartori
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

 * @details This header file contains the declaration of the class HttpDigestRealm
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef L5HTTPSERVICE_HTTPDIGESTREALM_H_
#define L5HTTPSERVICE_HTTPDIGESTREALM_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "HttpRealmI.h"
#include "ReferenceContainer.h"

namespace MARTe{
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
/**
 * TODO
 */
class HttpDigestRealm: public Object, public HttpRealmI {

    CLASS_REGISTER_DECLARATION()

    /**
     * TODO
     */
    struct UserInfo{

    public:

        /**
         *
         */
        bool  Initialise(CCString userNameIn, CCString parameters){
            userName = userNameIn;
            StreamString parametersS = parameters;
            parametersS.Seek(0);


            /// used to detect end of stream
            bool finished = false;
            /// meaning is positional
            typedef int8 positionFlag;
            /// first the code
            const positionFlag  codePos = 0;
            /// second the commands
            const positionFlag  commandPos = 1;
            /// third/fifth/... the bare ip
            const positionFlag  ipPos      = 2;
            /// third/fifth/... the ip mask
            const positionFlag  ipMaksPos  = 3;
            /// third/fifth/... the bare ip
            const positionFlag  ipPos2     = 4;
            ///
            positionFlag position  = codePos;
            /**
            * I need to scan IP sections twice
            * First time to count the slots
            */
            uint32 ipStartPosition = 0;

            while (parameters[0] != 0 ){
                StreamString output;
                char8 saveTerminator;
                finished = parametersS.GetToken(output,":",saveTerminator,"[] \t\n\015");

                switch (position){
                case codePos:{

                    if (output.Size()==0){
                        model = noSecurity;
                        passwordDigest = "";
                    } else {

                    }

                    position++;
                }break;
                case commandPos:{

                    position++;
                }break;
                case ipPos:{

                    position++;
                }break;
                case ipMaksPos:{

                    position++;
                }break;
                case ipPos2:{

                    position = ipPos2;
                }break;
                default:{

                }
                }
            }

            parametersS.Seek(ipStartPosition);
            position  = ipPos;
            while (parameters[0] != 0 ){
                StreamString output;
                char8 saveTerminator;
                finished = parametersS.GetToken(output,":",saveTerminator,"[]");
            }

        }

        UserInfo(){
            numberOfValidIPs = 0;
        }

        ~UserInfo(){
            if (validIPs != NULL_PTR(ipRecord *)){
                delete[] validIPs;
            }
            numberOfValidIPs = 0;
            validIPs = NULL_PTR(ipRecord *);
            model = digest;
            mask = 0;

        }

    public:
        /**
         * TODO
         */
        StreamString  userName;

        /**
         * digest of user:realm:password (lower case exadecimal characters)
         * or BASE64 of Password
         */
        StreamString  passwordDigest;

        /**
         * TODO
         * one of 3 models
         */
        SecurityModel model;

        /**
         * TODO
         * 0xF means pass all
         */
        CommandMask   mask;

        /**
         * valid ip supports 64 bit internet
         * 0 means do not check ip
         */
        struct ipRecord {
            uint64 ip;
            uint64 mask;
        } *validIPs;

        /**
         *
         */
        uint32 numberOfValidIPs;
    };

public:

    /** This tool only supports digest security */
    virtual bool        DigestSecurityNeeded();

    /** True if the key is valid in the context
        @param command is the HTTP command used
        @param ipNumber is the ip of the client */
    virtual bool        Validate(CCString   key, HSHttpCommand  command, uint32 ipNumber);

    /** authentication request */
    virtual bool        GetAuthenticationRequest(StreamString &message);

    /** constructor */
                        HttpDigestRealm();

    /** destructor */
    virtual            ~HttpDigestRealm()

    /**
        Users = {
            UserName = [code]:[commands{Get Put Head pOst}]:[ip]:[mask(FFFFFFFF)]:[ip]:[mask(FFFFFFFF)]...
            code is either [DIGEST]<32 character md5(128) digest of user:realm:password (lower case exadecimal characters)>
            or             B64<BASE64 of Password>
            or             CLEAR<Password>
            ....
            empty code means do not check code
            empty commands means do not check commands
            etc..

        }

        + those of GCNamedObject */
    virtual bool  Initialise(StructuredDataI &data);
private:

    /** calculates a code that is unique to connection instance
        the call is multi-thread safe since it uses thread-specific information */
    void CalculateNonce(FString &nonce);

    /** a NULL terminated list of strings */
    const char **   users;

    /** memory deallocation */
    void                CleanUp()
    {
        if (users != NULL){
            int index = 0;
            while (users[index] != NULL){
                free((void *&)users[index]);
                index++;
            }
            free((void *&)users);
        }
    }

    /** retrieves the cooked up user and password and validate
        against @param command and @param ipNumber*/
    bool                FindUser(
                            const char *            userName,
                            FString &               HA1,
                            HSHttpCommand           command,
                            uint32                  ipNumber);
public:




};



/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

}

#endif /* L5HTTPSERVICE_HTTPDIGESTREALM_H_ */
	
