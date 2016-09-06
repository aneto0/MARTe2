/**
 * @file HttpDigestRealm.cpp
 * @brief Source file for class HttpDigestRealm
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

 * @details This source file contains the definition of all the methods for
 * the class HttpDigestRealm (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "HttpDigestRealm.h"

namespace MARTe {



/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/



/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

#if 0

OBJECTLOADREGISTER(HttpDigestRealm,"$Id: HttpDigestRealm.cpp 3 2012-01-15 16:26:07Z aneto $")

static bool SearchKey(const char *key,const char *name,FString &value){
    value.SetSize(0);
    if (key == NULL) return False;
    if (name == NULL) return False;

    const char *p = strstr(key,name);
    if (p == NULL) return False;
    p+= strlen(name);
    if (p[0] != '=') return False;
    if (p[1] != '"') return False;
    p+=2;
    while(*p != '"') value += *p++;
    return True;

}

static bool SearchKeyNoQuote(const char *key,const char *name,FString &value){
    value.SetSize(0);
    if (key == NULL) return False;
    if (name == NULL) return False;

    FString namep;
    namep.Printf("%s=",name);
    const char *p = strstr(key,namep.Buffer());
    if (p == NULL) return False;
    p+= namep.Size();
    char buffer[64];
    buffer[0] = 0;
    if (!Streamable::GetCStringToken(p,buffer,",",64)) return False;
    value = buffer;
    return True;
}

/** This tool only supports basic security */
bool HttpDigestRealm::DigestSecurityNeeded(){
    return True;
}

static inline const char *matchUser(const char *passwdLine,const char *userName){
    if (userName == NULL) return NULL;
    if (passwdLine == NULL) return NULL;
    while (*passwdLine == *userName){
        // it should have a : followed by the digest
        if (*passwdLine == 0) return NULL;
        passwdLine++;
        userName++;
    }
    // user not fully matched
    if (*userName != 0) return NULL;
    if (*passwdLine == ':') return (passwdLine + 1);
    return NULL;
}

static inline uint32 x2i(const char *s){
    uint32 result = 0;
    while (s[0] != 0){
        result = result << 4;
        if ((s[0]>='0') && (s[0]<='9')){
            result = result + (s[0] - '0');
        } else
        if ((s[0]>='A') && (s[0]<='F')){
            result = result + (s[0] - 'A' + 10);
        } else
        if ((s[0]>='a') && (s[0]<='f')){
            result = result + (s[0] - 'a' + 10);
        } else return result;
        s++;
    }
    return result;
}

bool HttpDigestRealm::FindUser(const char *userName,FString &HA1,HSHttpCommand command,uint32 ipNumber){
    const char **p = users;
    if (p == NULL)  return False;
    while (p[0] != NULL){
        const char *info = matchUser(p[0],userName);
        if (info){
            FString copy;
            copy = info;
            char *securityInfo;
            securityInfo = copy.BufferReference();

            /* no more data then no command match necessary */
            if (strlen(securityInfo) == 0){
                return True;
            }

            HA1 = "";
            // Generate HA1
            char *password;
            password = Streamable::DestructiveGetCStringToken(securityInfo,":",NULL,"");
            if (strlen(password) != 0){

                if (strncmp(password,"B64",3)==0){
                    FString b64CodedKey;
                    b64CodedKey = password;
                    FString deCodedKey;
                    B64Decode(b64CodedKey,deCodedKey);
                    GeneratePasswordDigest(userName,deCodedKey.Buffer(),RealmName(),HA1);
                } else
                if (strncmp(password,"CLEAR",3)==0){
                    GeneratePasswordDigest(userName,password,RealmName(),HA1);
                } else
                if (strncmp(password,"DIGEST",3)==0){
                    HA1= password;
                } else
                {
                    HA1= password;
                }
            }

            /* no more data then no command match necessary */
            if (strlen(securityInfo) == 0){
                return True;
            }
            // check command
            char *commandString;
            commandString = Streamable::DestructiveGetCStringToken(securityInfo,":",NULL,"");
            // empty string then step to next check
            if (strlen(commandString) != 0){
                switch (command){
                    case HSHCGet:{
                        if (strchr(commandString,'G')==NULL) return False;
                    } break;
                    case HSHCPut:{
                        if (strchr(commandString,'P')==NULL) return False;
                    } break;
                    case HSHCPost:{
                        if (strchr(commandString,'O')==NULL) return False;
                    } break;
                    case HSHCHead:{
                        if (strchr(commandString,'H')==NULL) return False;
                    } break;
                    default:{
                        return False;
                    }
                }
            }
            /* no more data then no ip match necessary */
            if (strlen(securityInfo) == 0){
                return True;
            }

            while(1){

                // check ip
                char *ipNumberString;
                uint32 ipNumberAllowed = 0;
                ipNumberString = Streamable::DestructiveGetCStringToken(securityInfo,":",NULL,"");
                if (strlen(ipNumberString) != 0){
                    ipNumberAllowed = x2i(ipNumberString);
                    Endianity::ToMotorola(ipNumberAllowed);
                    if (ipNumber == ipNumberAllowed){
                        return True;
                    }
                }

                // did not match ip and no more so Failed
                if (strlen(securityInfo) == 0){
                    return False;
                }

                // check ip+ mask
                char *ipMaskString;
                ipMaskString = Streamable::DestructiveGetCStringToken(securityInfo,":",NULL,"");
                if (strlen(ipMaskString) != 0){
                    uint32 ipMask = x2i(ipMaskString);
                    Endianity::ToMotorola(ipMask);
                    if ((ipNumber & ipMask) == (ipNumberAllowed & ipMask)){
                        return True;
                    }
                }

                // did not match ip and no more so Failed
                if (strlen(securityInfo) == 0){
                    return False;
                }

            }

            return False;
        }
        p++;
    }
    return False;
}


void HttpDigestRealm::CalculateNonce(FString &nonce){
    nonce.SetSize(0);
    FString tid;
    tid.Printf("%08x%08x",Threads::ThreadId,this);
    unsigned char buffer[16];
    md5( (unsigned char *)tid.BufferReference(), tid.Size(),buffer);
    for (int  i=0;i<16;i++){
        nonce.Printf("%02x",buffer[i]);
    }
}

/** authentication request */
bool HttpDigestRealm::GetAuthenticationRequest(FString &message){

    FString nonce;
    CalculateNonce(nonce);

    return message.Printf(
                "Digest realm=\"%s\","
                "qop=\"auth\","            /*,auth-int*/
                "nonce=\"%s\","
                "opaque=\"%s\"",
                RealmName(),
                nonce.Buffer(),
                nonce.Buffer());
}

/** The check function */
bool HttpDigestRealm::Validate(const char *key,HSHttpCommand command,uint32 ipNumber){

    if (strncmp(key,"Digest",6)==0){
        key += 6;
//printf("%s\n",key);

        unsigned char buffer[16];
        FString toEncode;
        FString HA1;
        FString user;
        FString password;
        if (!SearchKey(key,"username",user)) return False;
        if (!FindUser(user.Buffer(),HA1,command,ipNumber)) {
            AssertErrorCondition(FatalError,"Validate: user %s unknown",user.Buffer());
            return False;
        }
        // no password required: any password is fine...
        if (HA1.Size() == 0) return True;

        FString uri;
        if (!SearchKey(key,"uri",uri)){
            AssertErrorCondition(FatalError,"Validate: uri information missing");
            return False;
        }
        toEncode.SetSize(0);
        switch(command){
            case HSHCGet:{
                toEncode.Printf("GET:%s",uri.Buffer());
            } break;
            case HSHCPut:{
                toEncode.Printf("PUT:%s",uri.Buffer());
            } break;        toEncode.Printf("GET:%s",uri.Buffer());

            case HSHCPost:{
                toEncode.Printf("POST:%s",uri.Buffer());
            } break;
            case HSHCHead:{
                toEncode.Printf("HEAD:%s",uri.Buffer());
            } break;
            default:{
                toEncode.Printf("NONE:%s",uri.Buffer());
            }
        }

        FString HA2;
        md5( (unsigned char *)toEncode.BufferReference(), toEncode.Size(),buffer);
int i = 0;
        for (i=0;i<16;i++){
            HA2.Printf("%02x",buffer[i]);
        }

        FString response;
        FString nonce;
        FString cnonce;
        FString nc;
        FString qop;
        if (!SearchKeyNoQuote(key,"nc",nc)){
            AssertErrorCondition(FatalError,"Validate: nc information missing");
            return False;
        }
        if (!SearchKey(key,"cnonce",cnonce)) {
            AssertErrorCondition(FatalError,"Validate: cnonce information missing");
            return False;
        }
        if (!SearchKeyNoQuote(key,"qop",qop)) {
            AssertErrorCondition(FatalError,"Validate: qop information missing");
            return False;
        }
        CalculateNonce(nonce);
        toEncode.SetSize(0);
        toEncode.Printf("%s:%s:%s:%s:%s:%s",
            HA1.Buffer(),nonce.Buffer(),nc.Buffer(),
            cnonce.Buffer(),qop.Buffer(),HA2.Buffer()
            );
        md5( (unsigned char *)toEncode.BufferReference(), toEncode.Size(),buffer);
        for ( i=0;i<16;i++){
            response.Printf("%02x",buffer[i]);
        }


        FString value;
        SearchKey(key,"response",value);
//printf("response = %s\n",value.Buffer());
//printf("response = %s\n",response.Buffer());

        return (strcmp(value.Buffer(),response.Buffer())==0);
    }
    return False;
}

void GeneratePasswordDigest(
                const char *user,
                const char *password,
                const char *realm,
                FString &   HA1){
    HA1.SetSize(0);
    unsigned char buffer[16];
    FString toEncode;
    toEncode.Printf("%s:%s:%s",user,realm,password);
    md5( (unsigned char *)toEncode.BufferReference(), toEncode.Size(),buffer);
    for (int i=0;i<16;i++){
        HA1.Printf("%02x",buffer[i]);
    }
}

bool HttpDigestRealm::ObjectLoadSetup(
        ConfigurationDataBase & info,
        StreamInterface *       err){

    CleanUp();

    CDBExtended cdbx(info);
    if (cdbx->Move("Users")){
        int32 nOC = cdbx->NumberOfChildren();
        if (nOC <= 0){
            AssertErrorCondition(ParametersError,"ObjectLoadSetup: node Users is either a leaf or an empty node");
            return False;
        }
        // allocate for pointers and terminator
        users = (const char **)malloc((nOC+1)*sizeof(const char *));
        users[nOC] = NULL;
        for (int32 i = 0; i < nOC;i++){
            if (cdbx->MoveToChildren(i)){
                BString digest;
                BString name;
                cdbx->NodeName(name);
                cdbx.ReadBString(digest,"");
                FString compound;
                compound.Printf("%s:%s",name.Buffer(),digest.Buffer());
                users[i] = strdup(compound.Buffer());
                cdbx->MoveToFather();
            }
        }

    }

    return GCNamedObject::ObjectLoadSetup(info,err);
}


#endif

}
