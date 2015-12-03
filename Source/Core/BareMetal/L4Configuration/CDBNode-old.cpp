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
 * $Id: CDBNode.cpp 3 2012-01-15 16:26:07Z aneto $
 *
**/

#include "CDBNode.h"
#define CDB_INTERNAL
#include "CDB.h"
#include "LexicalAnalyzer.h"


OBJECTREGISTER(CDBNode,"$Id: CDBNode.cpp 3 2012-01-15 16:26:07Z aneto $")

int32 CDBNode::counter = 0;

void CDBNode::DeAllocate(){
    if (name != NULL) free((void *&)name);
    name = NULL;
}

void CDBNode::Init(const char *name){

    referenceCounter = 0;

    DeAllocate();
    if (name == NULL) {
        this->name = NULL;
    } else {
        this->name = strdup(name);
    }
    father = this;
}

//static int nNodes = 0;

CDBNode::CDBNode(){
    name = NULL;
    father = this;
    referenceCounter = 0;
    counter++;

//nNodes++;
}


CDBNode::~CDBNode(){
    counter--;
    DeAllocate();
    if (!IsRootNode()) father->RemoveChild(this);

//nNodes--;
//printf("nNodes = %i\n",nNodes);

}

int CDBNode::IncreaseReferenceCount(){
    Atomic::Increment(&referenceCounter);
    return referenceCounter;
}

int CDBNode::DecreaseReferenceCount(){
    if (referenceCounter > 0)
    Atomic::Decrement(&referenceCounter);
    return referenceCounter;
}

CDBNode *CDBNode::Children(int childNumber) {
    if (childNumber < 0){
        CDBNode *cdbn = this;
        while(childNumber++ < 0){
            cdbn = cdbn->Father();
        }
        return cdbn;
    }
    return NULL;
}

int CDBNode::TreePosition(bool leafOnly){
    // I am not in a data node!
    if ((leafOnly) && (!IsDataNode())) return -2;

    int position = SubTreeSize(leafOnly);

    CDBNode *node = this;
    // find first brother
    CDBNode *cdbn = node->Father()->Children(0);

    // not a root node!
    while (node->Father() != node){
        // add brothers size
        while (cdbn != node){
            position += cdbn->SubTreeSize(leafOnly);
            cdbn = cdbn->Next();
        }
        // move one level up
        node = node->Father();
        cdbn = node->Father()->Children(0);
    }

    return position-1;
}

CDBNode * CDBNode::FindSegment(const char *segmentName,CDBNMode functionMode,
                        SortFilterFn *  sortFn,const char * containerClassName, bool caseSensitive){
    // do not move
    if (segmentName == NULL) return this;
    if (strlen(segmentName) == 0) return this;

    CDBNode *node = this;

    if (strcmp("UpNode",segmentName)==0) {
        if (node->IsRootNode()) return this;
        return node->Father();
    }

    if (strcmp("RootNode",segmentName)==0) {
        while (!node->IsRootNode()) node = node->Father();
        return node;
    }

    return node->Children(segmentName,functionMode,sortFn,containerClassName, caseSensitive);
}

static inline bool IsSeparator(char c){
    if (c == 0) return True;
    const char *s = CDBSeparators;
    while(s[0] != 0){
        if (s[0] == c) return True;
        s++;
    }
    return False;
}

static inline const char *ExtractSegment(const char *name,FString &segment){
    if (name == NULL) return NULL;
    segment = "";
    const char *p = name;
    while(!IsSeparator(p[0])){
        segment += p[0];
        p++;
    }
    while((p[0]!=0) && IsSeparator(p[0]))p++;
    return p;
}

CDBNode * CDBNode::Find2(const char *&  childName,  
                         CDBNMode       functionMode,
                         SortFilterFn*  sortFn,     
                         const char *   containerClassName,
                         bool           caseSensitive) {

    CDBNode *node = this;
    bool matchAllButLast = (functionMode & CDBN_MatchAllButLast);
    bool partial = (functionMode & CDBN_PartialMatch);

    FString segment;
    const char *remainder    = ExtractSegment(childName,segment);

    int steps = 0;
    while (segment.Size() > 0) {
        // check for partial match

        if ((strlen(remainder)==0) && matchAllButLast) return node;

        CDBNode *newNode = node->FindSegment(segment.Buffer(),functionMode,sortFn,containerClassName,caseSensitive);
        if (newNode == NULL) {
            if (partial && (steps > 0)) return node;
            return NULL;
        }
        node      = newNode;
        childName = remainder;
        remainder = ExtractSegment(childName,segment);
        steps++;
    }
    return node;
}

CDBNode *CDBNode::FindSubTree(const char *subtree,CDBAddressMode cdbam){
    // need a starting point

    CDBNode *node = this;

    if (!(cdbam & CDBAM_SkipCurrent)){
        CDBNode *newNode = Find(subtree);
        if (newNode) return node;
    }

    // search
    while (True){
        // move to the next in the search pattern
        CDBNode *newNode = node->Children(0);
        if (!newNode)  newNode = node->Next();
        while ((newNode == NULL) && (!node->IsRootNode())) {
            node = node->Father();
            newNode = node->Next();
        }
        if (newNode == NULL) return NULL;
        node = newNode;

        newNode = node->Find(subtree);
        if (newNode) return node;

    }
    // search failed
    return NULL;
}


CDBNode *CDBNode::FindBrother(int steps){

    if (IsRootNode()) return NULL;
    if (steps == 0) return this;

    CDBNode *node = this;

    if ( steps > 0 ){
        while ((steps > 0) && (node->Next() != NULL)){
            node = node->Next();
            steps--;
        }

        if ( steps != 0 ) return NULL;
    }

    if (steps < 0){
        CDBNode *first = node->Father()->Children(0);
        int ix = 0;
        // Count the number of children
        while((first != NULL) && (first != node)){
            ix++;
            first = (CDBNode *)first->Next();
        }
        if (first != NULL){
            ix = ix+steps;
            if (ix < 0){
                //steps = ix;
                return NULL;
                ix = 0;
            }
            node = (CDBNode *)node->Father()->Children(ix);
        }
    }
    return node;
}

bool CDBNode::ReadContent(void *value,const CDBTYPE &valueType,int size,...){
    va_list argList;
    va_start(argList,size);
    bool ret = ReadContent(value,valueType,size,argList);
    va_end(argList);
    return ret;
}

void CDBNode::SubTreeName(Streamable &location,const char *sep){
    location.SetSize(0);
    FString temp;
    CDBNode *cdbn = this;
    while (!cdbn->IsRootNode()){
        if (temp.Size()==0){
            temp.Printf("%s",cdbn->Name());
        } else {
            FString tt;
        tt = temp.Buffer();
        temp.SetSize(0);
            temp.Printf("%s%s%s",cdbn->Name(),sep,tt.Buffer());
        }
        cdbn = cdbn->Father();
    }
    uint32 size = temp.Size()+1;
    location.Write(temp.Buffer(),size);
}

CDBNode *CDBNode::Find(int index,CDBAddressMode cdbam){
    bool leafOnly = cdbam & CDBAM_LeafsOnly  ;
    bool local    = cdbam & CDBAM_SubTreeOnly;
    bool relative = cdbam & CDBAM_Relative   ;

    if (relative == False) {
        CDBNode *cdbn = this;
        if (local == False) {
            // move to root
            while (!cdbn->IsRootNode()){
                cdbn = cdbn->Father();
            }
        }

        int subTreeSize = cdbn->SubTreeSize(leafOnly);

        // transform into 1-N
        index++;

        // cannot index outside this tree
        if (index > subTreeSize) return NULL;

        // found it and it is the current node!
        if ((!leafOnly) && (index == subTreeSize)) return this;

        // span children and recurse down
        cdbn = cdbn->Children(0);
        while (
               (cdbn != NULL)        &&    /* search has failed */
               (index != 0)                /* we are in the data node corresponding to the index */
              ){

            int subTreeSize = 1;
            if (cdbn->IsGroupNode()){
                subTreeSize = cdbn->SubTreeSize(leafOnly);
            }

            if (index > subTreeSize ){
                index -= subTreeSize;
                cdbn = cdbn->Next();
            } else
            if ((index == subTreeSize) &&
                ((!leafOnly) || (!cdbn->IsGroupNode()))){
                index = 0;
            } else {
                // we have found the node in this case
                cdbn = cdbn->Children(0);
            }
        }
        if ((cdbn == NULL) || (index != 0)) return NULL;

        return cdbn;
    }

    // relative = True
    int absoluteIndex = index + TreePosition(leafOnly);

    cdbam = CDBAM_None;
    if (leafOnly) cdbam = CDBAM_LeafsOnly;
    return Find(absoluteIndex,cdbam);

}

static int isIndexName(const char *name){
    if (name==NULL) return -1;
    int N = atoi(name);
    char s[32];
    sprintf(s,"%i",N);
    if (strcmp(s,name)==0) return N;
    return -1;
}

bool CDBNode::GetArrayDims(int *size,int &maxDim,const char *configName,CDBArrayIndexingMode cdbaim, bool caseSensitive){
    if (size == NULL) return False;

    bool strict    = ((cdbaim & CDBAIM_Strict   ) !=0 );
    bool firstTime = ((cdbaim & CDBAIM_Recursion) == 0);

    CDBNode *node = this;
    if (configName != NULL){
        node = Find(configName,CDBN_FollowLink,NULL,NULL,caseSensitive);
        if (node == NULL) return False;
    }

    if (firstTime) for(int i = 0;i < maxDim;i++)   size[i] = 0;

    if (maxDim <= 0) return False;
    if (node->IsDataNode()) {
        int vectorSize = node->NumberOfElements();
        maxDim = 1;

        if (strict && (vectorSize != size[0]) && (size[0] != 0)) return False;
        if (vectorSize > size[0]) size[0] = vectorSize;
    } else
    if (node->IsGroupNode()) {
        if (strict) node->SortChildren(CDBC_SortNumFilterFn);

        int maxIndex = -1;
        int maxDepth = -1;
        CDBNode *child = node->Children(0);
        if (child){
            while (child){
                int n = isIndexName(child->Name());
                // check perfect sequence in case of strictness
                if ((strict)&&((n<0)||(n!= (maxIndex+1)))) return False;
                // upgrade max
                if (n > maxIndex) maxIndex = n;
                // only for index nodes
                if (n >= 0){
                    int subTreeMaxDim = maxDim - 1;
                    child->GetArrayDims(size+1,subTreeMaxDim,NULL,cdbaim | CDBAIM_Recursion, caseSensitive);
                    if (strict && (maxDepth != -1) && (maxDepth != subTreeMaxDim)) return False;
                    if (maxDepth < subTreeMaxDim) {
                        maxDepth = subTreeMaxDim;
                    }
                }
                child = child->Next();
            }
            size[0] = maxIndex+1;
            maxDim = maxDepth+1;
        } else {
            AssertErrorCondition(FatalError,"GetArrayDims:node %s is a groupnode with no children",node->Name());
            maxDim = 0;
            return False;
        }
    }

    if (firstTime && (maxDim == 1) && ( size[0] <= 1)) maxDim = 0;

    return True;
}


enum CDBParserTokenValue{
    CDBTV_EOF          = 0x0100,
    CDBTV_Ident        = 0x0101,
    CDBTV_OpenCurly    =    '{',  // {
    CDBTV_CloseCurly   =    '}',  // }
    CDBTV_Equal        =    '=',  // =
    CDBTV_OpenRound    =    '(',  // (
    CDBTV_CloseRound   =    ')',  // )
    CDBTV_Number       = 0x0102,
    CDBTV_Float        = 0x0103,
    CDBTV_Error        = 0x0104
};


enum CDBParserStateValue {
    CDBPSV_lvalue       = 0x0001,
    CDBPSV_equal        = 0x0002,
    CDBPSV_rvalue       = 0x0003,
    CDBPSV_block        = 0x0004,
    CDBPSV_block2       = 0x0005,
    CDBPSV_typeCast     = 0x0006
};

struct CDBParserState {
    CDBParserStateValue value;
    const char *        name;
};

static const CDBParserState  CDBPS_lvalue   = { CDBPSV_lvalue,  "Left  Value  " };
static const CDBParserState  CDBPS_equal    = { CDBPSV_equal ,  "Equal Value  " };
static const CDBParserState  CDBPS_rvalue   = { CDBPSV_rvalue,  "Right Value  " };
static const CDBParserState  CDBPS_block    = { CDBPSV_block ,  "Status-Block " };
static const CDBParserState  CDBPS_block2   = { CDBPSV_block2,  "Status-Block2" };
static const CDBParserState  CDBPS_typeCast = { CDBPSV_typeCast,"Type Cast    " };


bool CDBNode::ReadFromStream(   StreamInterface &   stream,
                                StreamInterface *   err,
                                bool                parserReportEnabled,
                                SortFilterFn *      sorter) {
						    	
    // the parser status in the processing of the stream
    const CDBParserState *  status          = &CDBPS_lvalue;
    // the name of the variable
    FString                 lValue;
    // the value of the variable
    FString                 rValue;
    // contains the array ?
    FString                 array;
    // count the opened parenthesys within the assignment
    int                     parLevel        = 0;
    // counts the overall parenthesys (opened and closed)
    int                     parCount        = 0;
    // line Counter
    int                     lineCount       = 1;
    // the name of the variable
    FString                 typeValue;
    //
    CDBTYPE                 cdbType         = CDBTYPE_Interpret;

    typeValue = "";
    static int linkCounter = 0;

    if (err && parserReportEnabled)  err->Printf(">>START<<\n");

    bool globalRet = True;

    CDBNode *node = this;

    LexicalAnalyzer la;
    la.AddSeparators("\n\r\t ");
    la.AddTerminals("{}(),=");
    la.ChangeTokenCode("EOF"   ,CDBTV_EOF   );
    la.ChangeTokenCode("IDENT" ,CDBTV_Ident );
    la.ChangeTokenCode("NUMBER",CDBTV_Number);
    la.ChangeTokenCode("FLOAT" ,CDBTV_Float );
    la.ChangeTokenCode("ERROR" ,CDBTV_Error );


    LA_TokenData *latd;


    // stores the terminator
    while((latd = la.GetToken(stream)) != NULL){
        if (latd->Token() == CDBTV_EOF) break;

        if (latd->Token() == CDBTV_Error){
            if (err) err->Printf("Line[%i] lexical error %s %s\n",latd->LineNumber(),latd->Description(),latd->Data());
            AssertErrorCondition(SyntaxError,"Line[%i] lexical error %s %s\n",latd->LineNumber(),latd->Description(),latd->Data());
            return False;
        }

        if (err && parserReportEnabled) {
            FString location;
            node->SubTreeName (location,".");
            err->Printf("Line[%i] {C=%i,L=%i} base=%s type=%s status=%s value=%s\n",latd->LineNumber(),parCount,parLevel,location.Buffer(),latd->Description(),status->name,latd->Data());
        }

        switch (status->value){
            case CDBPSV_lvalue:{
                switch(latd->Token()){
                    case CDBTV_Number:
                    case CDBTV_Ident:{
                        if (latd->Data()[0]=='*'){
                            FString name;
                            name.Printf("link%i",linkCounter++);
                            const char *linkTo = latd->Data() + 1;
                            bool ret = node->WriteArray(
                                        name.Buffer(),&linkTo,CDBTYPE_String,
                                        NULL,1,CDBN_CreateLinkNode,sorter);
                            globalRet = globalRet && ret;
                        } else {
                            status = &CDBPS_equal;
                            lValue = latd->Data();
                        }
                    } break;

                    case CDBTV_CloseCurly:{
                        if (parCount > 0){
                            parCount--;
                            node = node->Father();
                            status = &CDBPS_lvalue;
                            lValue = "";
                        } else {
                            if (err) err->Printf("Line[%i] unmatched %s at state %s\n",latd->LineNumber(),latd->Description(),status->name);
                            AssertErrorCondition(SyntaxError,"Line[%i] unmatched %s at state %s\n",latd->LineNumber(),latd->Description(),status->name);
                            return False;
                        }

                    } break;
                    default:{
                        if (err) err->Printf("Line[%i] unexpected type %s at status %s\n",latd->LineNumber(),latd->Description(),status->name);
                        AssertErrorCondition(SyntaxError,"Line[%i] unexpected type %s at status %s\n",latd->LineNumber(),latd->Description(),status->name);
                        return False;
                    }
                }
            } break;

            case CDBPSV_equal:{
                if (latd->Token() == CDBTV_Equal){
                    status = &CDBPS_rvalue;
                    // not assigned type at this stage
                    typeValue = "";
                    cdbType.containerClassName = NULL;
                } else {
                    if (err) err->Printf("Line[%i] unexpected type %s at status %s\n",latd->LineNumber(),latd->Description(),status->name);
                    AssertErrorCondition(SyntaxError,"Line[%i] unexpected type %s at status %s\n",latd->LineNumber(),latd->Description(),status->name);
                    status = &CDBPS_lvalue;
                    return False;
                }
            } break;

            case CDBPSV_rvalue:{
                switch (latd->Token()){
                    case CDBTV_Number:
                    case CDBTV_Float:
                    case CDBTV_Ident:{

                        FString buffer;
                        // encapsulate with curlies to allow parsing of strings with spaces
                        buffer.Printf("\"%s\"", latd->Data());
                        const char *buf = buffer.Buffer();

                        bool ret = node->WriteArray(lValue.Buffer(),&buf,cdbType,NULL,1,CDBN_CreateStringNode,sorter);
                        if (!ret) {
                            if (err) err->Printf("Line[%i] cannot write %s at node %s\n",latd->LineNumber(),latd->Data(),lValue.Buffer());
                            globalRet = False;
                            AssertErrorCondition(FatalError,"Line[%i] cannot write %s at node %s\n",latd->LineNumber(),latd->Data(),lValue.Buffer());
                        }

                        // NEXT STATUS !
                        status = &CDBPS_lvalue;
                    } break;

                    case CDBTV_OpenCurly:{
                        // NEXT STATUS !
                        status = &CDBPS_block;
                        parCount++;
                        rValue = "";
                        array  = "";
                        array  = "{\n";
                        parLevel = 1;
                    } break;

                    case CDBTV_OpenRound:{
                        // NEXT STATUS !
                        status    = &CDBPS_typeCast;
                    } break;

                    default:{
                        // NEXT STATUS !
                        status = &CDBPS_lvalue;

                        if (err) err->Printf("Line[%i] unexpected type %s at status %s\n",latd->LineNumber(),latd->Description(),status->name);
                        AssertErrorCondition(SyntaxError,"Line[%i] unexpected type %s at status %s\n",latd->LineNumber(),latd->Description(),status->name);
                        return False;
                    }
                }
            } break;

            case CDBPSV_typeCast:{
                switch (latd->Token()){
                    case CDBTV_Ident:{
                        if (typeValue.Size() != 0){
                            if (err) err->Printf("Line[%i] double definition of type was %s now set to %s\n",latd->LineNumber(),typeValue.Buffer(),latd->Data());
                            AssertErrorCondition(SyntaxError,"Line[%i] double definition of type was %s now set to %s\n",latd->LineNumber(),typeValue.Buffer(),latd->Data());
                            status = &CDBPS_lvalue;
                            return False;
                        }

                        typeValue = latd->Data();
                        cdbType.containerClassName = typeValue.Buffer();
                    } break;

                    case CDBTV_CloseRound:{
                        status = &CDBPS_rvalue;
                    } break;

                    default:{
                        if (err) err->Printf("Line[%i] unexpected type %s at status %s\n",latd->LineNumber(),latd->Description(),status->name);
                        AssertErrorCondition(SyntaxError,"Line[%i] unexpected type %s at status %s\n",latd->LineNumber(),latd->Description(),status->name);
                        status = &CDBPS_lvalue;
                        return False;
                    }
                }
            } break;

            case CDBPSV_block:{
                switch (latd->Token()){
                    case CDBTV_Number:
                    case CDBTV_Float:
                    case CDBTV_Ident:{
                        // we find a link, this means we are in a GroupNode
                        if (latd->Data()[0]=='*'){
                            node = node->Children(lValue.Buffer(),CDBN_SearchAndCreate |
                                        CDBN_CreateGroupNode, sorter, typeValue.Buffer());

                            typeValue = "";
                            cdbType.containerClassName = NULL;

                            if (node == NULL) {
                                if (err) err->Printf("Line[%i] cannot move/create node %s\n",lineCount,lValue.Buffer());
                                AssertErrorCondition(FatalError,"Line[%i] cannot move/create node %s\n",lineCount,lValue.Buffer());
                                return False;
                            }

                            FString name;
                            name.Printf("link%i",linkCounter++);
                            const char *linkTo = latd->Data()+1;
                            bool ret = node->WriteArray(
                                        name.Buffer(),&linkTo,CDBTYPE_String,
                                        NULL,1,CDBN_CreateLinkNode,sorter);
                            globalRet = globalRet && ret;

                            status = &CDBPS_lvalue;
                            array  = "";
                            lValue = "";
                            rValue = "";
                            parLevel = 0;
                        } else {
                            rValue = latd->Data();
                            status = &CDBPS_block2;
                            array += " \"";
                            array += latd->Data();
                            array += "\" ";
                        }
                    } break;

                    case CDBTV_OpenCurly:{
                        parCount++;
                        parLevel++;
                        array +="{\n";
                        status = &CDBPS_block;
                    } break;

                    case CDBTV_CloseCurly:{
                        if (parCount > 0){
                            parCount--;
                        } else {
                            if (err) err->Printf("Line[%i] unmatched %s at status %s\n",latd->LineNumber(),latd->Description(),status->name);
                            AssertErrorCondition(SyntaxError,"Line[%i] unmatched %s at status %s\n",latd->LineNumber(),latd->Description(),status->name);
                            return False;
                        }
                        parLevel--;
                        array +="}\n";
                        if (parLevel == 0){


                            const char *buffer = array.Buffer();
                            bool ret = node->WriteArray(lValue.Buffer(),&buffer,cdbType,NULL,1,CDBN_CreateStringNode,sorter);
                            if (!ret){
                                if (err) err->Printf("Line[%i] cannot write %s at node %s\n",latd->LineNumber(),array.Buffer(),lValue.Buffer());
                                globalRet = False;
                                AssertErrorCondition(FatalError,"Line[%i] cannot write %s at node %s\n",latd->LineNumber(),array.Buffer(),lValue.Buffer());
                            }

                            array = "";
                            status = &CDBPS_lvalue;
                        } else
                            status = &CDBPS_block;
                    } break;

                    default:{
                        if (err) err->Printf("Line[%i] unexpected type %s at status %s\n",latd->LineNumber(),latd->Description(),status->name);
                        AssertErrorCondition(SyntaxError,"Line[%i] unexpected type %s at status %s\n",latd->LineNumber(),latd->Description(),status->name);
                        status = &CDBPS_lvalue;
                        return False;
                    }
                }
            } break;

            case CDBPSV_block2:{
                switch (latd->Token()){
                    case CDBTV_Number:
                    case CDBTV_Float:
                    case CDBTV_Ident:{
                        rValue = "";
                        //array += latd->Data();                        
                        array += " \"";
                        array += latd->Data();
                        array += "\" ";
                        array += ' ';
                    } break;

                    case CDBTV_OpenCurly:{
                        parCount++;
                        parLevel++;
                        array +="{\n";
                        status = &CDBPS_block;
                    } break;

                    case CDBTV_CloseCurly:{
                        if (parCount > 0){
                            parCount--;
                        } else {
                            if (err) err->Printf("Line[%i] unmatched %s at status %s\n",latd->LineNumber(),latd->Description(),status->name);
                            AssertErrorCondition(SyntaxError,"Line[%i] unmatched %s at status %s\n",latd->LineNumber(),latd->Description(),status->name);
                            return False;
                        }
                        parLevel--;
                        if (parLevel<0){
                            if (err) err->Printf("Line[%i] unexpected Negative parLevel %i at status %s\n",latd->LineNumber(),parLevel,status->name);
                            AssertErrorCondition(SyntaxError,"Line[%i] unexpected Negative parLevel %i at status %s\n",latd->LineNumber(),parLevel,status->name);
                            return False;
                        }
                        array +="}\n";
                        if (parLevel==0){

                            const char *buffer = array.Buffer();
                            bool ret = node->WriteArray(lValue.Buffer(),&buffer,cdbType,NULL,1,CDBN_CreateStringNode,sorter);
                            if (!ret){
                                if (err) err->Printf("Line[%i] cannot write %s at node %s\n",latd->LineNumber(),array.Buffer(),lValue.Buffer());
                                AssertErrorCondition(FatalError,"Line[%i] cannot write %s at node %s\n",latd->LineNumber(),array.Buffer(),lValue.Buffer());
                                globalRet = False;
                            }


                            array="";
                            status = &CDBPS_lvalue;
                        } else
                            status = &CDBPS_block;
                    } break;

                    case CDBTV_Equal:{
                        if (parLevel == 1){

                            node = node->Find(lValue.Buffer(),CDBN_SearchAndCreate |
                                        CDBN_CreateGroupNode, sorter, typeValue.Buffer());

                            typeValue = "";
                            cdbType.containerClassName = NULL;

                            if (node == NULL) {
                                if (err) err->Printf("Line[%i] cannot move/create node %s\n",latd->LineNumber(),lValue.Buffer());
                                AssertErrorCondition(FatalError,"Line[%i] cannot move/create node %s\n",latd->LineNumber(),lValue.Buffer());
                                return False;
                            }

                            array = "";
                            lValue = rValue;
                            rValue = "";
                            status = &CDBPS_rvalue;
                            parLevel = 0;

                        } else
                            status = &CDBPS_lvalue;
                    } break;

                    default:{
                        if (err) err->Printf("Line[%i] unexpected type %s at status %s\n",latd->LineNumber(),latd->Description(), status->name);
                        status = &CDBPS_lvalue;
                        return False;
                    }
                }
            } break;

            default:{
                if (err) err->Printf("Line[%i] unexpected type %s at status %s\n",latd->LineNumber(),latd->Description(),status->name);
                AssertErrorCondition(SyntaxError,"Line[%i] unexpected type %s at status %s\n",latd->LineNumber(),latd->Description(),status->name);
                return False;
            }

        }

        delete latd;
        latd = NULL;
    }

    if (parCount != 0){
        if (err) err->Printf("unexpected parCount = %i. Should be 0!\n",parCount);
        AssertErrorCondition(SyntaxError,"unexpected parCount = %i. Should be 0!\n",parCount);
        return False;
    }

    if (err && parserReportEnabled)  err->Printf(">>END<<\n");
    return globalRet;
}








