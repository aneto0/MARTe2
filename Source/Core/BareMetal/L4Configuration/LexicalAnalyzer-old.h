/*
 * Copyright 2011 EFDA | European Fusion Development Agreement
 *
 * Licensed under the EUPL, Version 1.1 or - as soon they 
   will be approved by the European Commission - subsequent  
   versions of the EUPL (the "Licence"); 
 * You may not use this work except in compliance with the 
   Licence. 
 * You may obtain a copy of the Licence at: 
 *  
 * http://ec.europa.eu/idabc/eupl
 *
 * Unless required by applicable law or agreed to in 
   writing, software distributed under the Licence is 
   distributed on an "AS IS" basis, 
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either 
   express or implied. 
 * See the Licence for the specific language governing 
   permissions and limitations under the Licence. 
 *
 * $Id: LexicalAnalyzer.h 3 2012-01-15 16:26:07Z aneto $
 *
**/

/** 
 * @file
 * Lexical analyser. Is is used internally to parse the configuration files
 */
#include "System.h"
#include "FString.h"
#include "StaticQueueHolder.h"

#if !defined (LEXICAL_ANALYZER)
#define LEXICAL_ANALYZER


/** information abouta a lexicala element */
class LA_TokenInfo{
public:
    /** the code identifying the lexical meaning of this part of the text */
    int32           token;

    /** the meaning of the token */
    const char *    description;

    /** constructor */
    LA_TokenInfo(){
        token = 0;
        description = NULL;
    }

    /** setup */
    void Set(int32 token,const char *description){
        this->token = token;
        this->description = strdup(description);
    }

    /** constructor */
    ~LA_TokenInfo(){
        if (description) free((void *&)description);
    }
} ;

/** an element produced by the LexicalAnalyzer. */
class LA_TokenData{
    /** the code identifying the lexical meaning of this part of the text */
    int32           token;

    /** the parsed part of the text */
    FString         data;

    /** copied from a LA_TokenInfo, the meaning of the token */
    FString         description;

    /** what line the token was found at*/
    int32           lineNumber;

public:
    /** builds a token from a token description constant and the data content*/
    LA_TokenData(const LA_TokenInfo &info, const char *data,int32 lineNumber){
        token               = info.token;
        description         = info.description;
        this->data          = data;
        this->lineNumber    = lineNumber;
    }

    /** builds a token from a simple character */
    LA_TokenData(char c,int32 lineNumber){
        token               = c;
        description         = c;
        data                = c;
        this->lineNumber    = lineNumber;
    }

    /** destructor */
    virtual         ~LA_TokenData(){
    }

    /** returns the TokenDataType */
    int32           Token(){
        return token;
    };

    /** returns the line number */
    int32           LineNumber(){ return lineNumber; }

    /** copy content */
    void            operator=(LA_TokenData &td){
        token       = td.token;
        data        = td.data;
        description = td.description;
    }

    /** descrizione del contenuto */
    const char *    Description(){
        return description.Buffer();
    }

    /** il contenuto */
    const char *    Data(){
        return data.Buffer();
    }

};

/** a set of codes to list the complex terminals recognised by the analyser */
enum LA_TokenType{
    /** indicates EOF */
    LATV_EOF          = 0x100,

    /** indicates an identifier or a string */
    LATV_Ident        = 0x101,

    /** indicates an integer */
    LATV_Number       = 0x102,

    /** indicates a float */
    LATV_Float        = 0x103,

    /** indicates a wrongly constructed element */
    LATV_Error        = 0x104,

    /** indicates an element that separates parts of the phrase */
    LATV_Separator    = 0x105,

    /** indicates an element that is a token on its own */
    LATV_Terminal     = 0x106
};

class LexicalAnalyzer;

extern "C" {

    bool LATokenizeInput(LexicalAnalyzer &la,StreamInterface &stream);

}

/** A slightly programmable lexical analyzer.
    It recognizes Identifiers, Numbers and Floats.
    It allows to browse ahead without consuming the token,
    Tokenisation is performed on demand  */
class LexicalAnalyzer{

    /** a queue of pre-cooked tokens*/
    StaticQueueHolder       tokenQueue;

    /** a string made of separators, plus the  */
    FString                 separators;

    /** a string composed of the terminal characters in the order */
    FString                 terminals;

    /** list of token infos */
    LA_TokenInfo            tokenInfo[LATV_Error+2-0x100];

    /** */
    int                     lineCounter;
    
protected:

    friend bool LATokenizeInput(LexicalAnalyzer &la,StreamInterface &stream);

    /** add a token into the Q*/
    bool TokenizeInput(StreamInterface &stream){
        return LATokenizeInput(*this,stream);
    }

    void AddToken(LA_TokenData *token){
        tokenQueue.QueueAdd((intptr *)&token);
    }

public: // initialisation functions
    /**
     * Switches the parsing of numbers. If off the Lexical Analyser will not try 
     * to check if the string is a number
     */
    static bool parseNumbers;
   

    /** constructor */
    LexicalAnalyzer(): tokenQueue(sizeof(LA_TokenData *)/sizeof(int)){
        tokenInfo[LATV_EOF   - 0x100].Set(LATV_EOF   ,"EOF");
        tokenInfo[LATV_Ident - 0x100].Set(LATV_Ident ,"IDENT");
        tokenInfo[LATV_Number- 0x100].Set(LATV_Number,"NUMBER");
        tokenInfo[LATV_Float - 0x100].Set(LATV_Float ,"FLOAT");
        tokenInfo[LATV_Error - 0x100].Set(LATV_Error ,"ERROR");

        Reset();
    }

    /** constructor */
    ~LexicalAnalyzer(){
        Reset();
    }

    /** reset status */
    void Reset(){
        LA_TokenData *tokenData;
        while (tokenQueue.QueueSize()>0){
            if (tokenQueue.QueueExtract((intptr *)&tokenData)){
                if (tokenData) delete tokenData;
            }
        }
        lineCounter = 0;
    }

    /** set these characters as separators */
    void AddSeparators(const char *s){
        separators += s;
    }

    /** set these characters/LA_TokenValue as separators */
    void AddTerminals(const char *s){
        terminals += s;
    }

    /** change the token code associated with a given complex terminal.
        valid tokenNames are "EOF","IDENT","NUMBER","FLOAT","ERROR" */
    bool ChangeTokenCode(const char *tokenName,int token){
        if (tokenName == NULL) return False;
        if (tokenName[0] == 0) return False;

        // support SLK!
        if (strcmp(tokenName,"END_OF_SLK_INPUT")==0) tokenName = "EOF";
        int index = 0;
        while ((tokenInfo[index].description != NULL) &&
               (strcmp(tokenInfo[index].description,tokenName)!=0)) index++;

        if (tokenInfo[index].description == NULL) return False;

        tokenInfo[index].token = token;

        return True;
    }

    /** retrieve the toaken value associated with a certain tokenName */
    int GetTokenValue(const char *tokenName){
        if (tokenName == NULL) return -1;
        if (tokenName[0] == 0) return -1;

        // support SLK!
        if (strcmp(tokenName,"END_OF_SLK_INPUT")==0) tokenName = "EOF";
        int index = 0;
        while ((tokenInfo[index].description != NULL) &&
               (strcmp(tokenInfo[index].description,tokenName)!=0)) index++;

        if (tokenInfo[index].description == NULL) return 01;

        return tokenInfo[index].token;
    }

public:
    /** takes one token from the stack or processes the input for a new one
        moves the token into lasToken. The class allocates the data but does
        not provide to the deallocation once the structure has been extracted */
    LA_TokenData *GetToken(StreamInterface &stream){
        if (tokenQueue.QueueSize() == 0){
            if (!TokenizeInput(stream)) {
                LA_TokenData *tok = new LA_TokenData(tokenInfo[LATV_EOF-0x100], "",lineCounter);
                tokenQueue.QueueAdd((intptr *)&tok);
            }
        }

        LA_TokenData *tokenData;
        tokenQueue.QueueExtract((intptr *)&tokenData);
        return tokenData;
    }

    /** reads in the stack at position lookAhead or increases the stack to allow for it
        it returns the token but it still keeps hold of it */
    LA_TokenData *PeekToken(uint32 lookAhead,StreamInterface &stream){
        while (tokenQueue.QueueSize() < lookAhead){
            if (!TokenizeInput(stream)) {
                LA_TokenData *tok = new LA_TokenData(tokenInfo[LATV_EOF-0x100], "",lineCounter);
                tokenQueue.QueueAdd((intptr *)&tok);
            }
        }
        LA_TokenData *tokenData;
        tokenQueue.QueuePeek((intptr *)&tokenData,tokenQueue.QueueSize()-lookAhead);
        return tokenData;
    }
};




#endif
