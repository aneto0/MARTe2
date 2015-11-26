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
 * $Id: CDBNode.h 3 2012-01-15 16:26:07Z aneto $
 *
**/

/** 
 * @file
 * The CDB node definition
 */
#if !defined(CONFIGURATION_DATABASE_NODE)
#define CONFIGURATION_DATABASE_NODE

#include "FString.h"
#include "LinkedListable.h"
#include "LinkedListHolder.h"
#include "Atomic.h"
#include "System.h"
#include "CDBTypes.h"
#include "CDBVirtual.h"

class CDBNodeRef;
class CDBNode;
class CDBLinkNode;


OBJECT_DLL(CDBNode)

class CDBNode: public LinkedListable, public Object{
OBJECT_DLL_STUFF(CDBNode)
private:

static int32 counter;

private:

    /** the name of the field. */
    char                            *name;

    /** the container */
    CDBNode                         *father;

    /** a NodeRef points to this node is referenceCounter >0 */
    int32                           referenceCounter;

    /** deallocate memory used in node */
            void        DeAllocate();

public:
    /** init node and add a name */
            void        Init(const char *name);

public:
    /** constructor */
                        CDBNode();

    /** destructor. removes the node from its container also ! */
    virtual             ~CDBNode();

    /** get the name */
            const char *Name()                      const   {  return name;   }

    /** returns the name of the subtree starting from this node.
        this call is safe, does not require locking of tree */
            void        SubTreeName (   Streamable&     location,
                                        const char *    sep);

    /** whether it can be navigated down */
    inline  bool        IsGroupNode()               const   { return (NumberOfChildren() >= 0); }

    /** whether it is to be considered a leaf node */
    inline  bool        IsDataNode()                const   { return (NumberOfElements() >= 0); }

    /** whether it is a link to a different subtree */
    virtual bool        IsLinkNode()                const   { return False; }

    /** change father */
            void        SetFather(CDBNode *father)          {  this->father = father;  }

    /** increase number of users. an atomic function */
            int         IncreaseReferenceCount();

    /** decrease number of users. an atomic function  */
            int         DecreaseReferenceCount();

    /** get number of users */
            int         NumberOfReferences()        const   { return referenceCounter;  }

    /** get the next node */
            CDBNode *   Next()                      const   { return (CDBNode *)LinkedListable::Next();    }

    /** if father = this then it is the root of the tree*/
            bool        IsRootNode()                const   { return (father == this); }

    /** get the father */
            CDBNode *   Father()                    const   { return father;  }

    /** allows accessing the subtrees and uptrees (-1 -2...).
        the links are not expanded by this function */
    virtual CDBNode *   Children(       int                 childNumber     = 0) ;

    /** allows accessing the subtrees by name,
        @param childName is the name of the node. It cannot be a full subtree
        @param functionMode see CDBNMode for explanation
        allowed flags are  CDBN_None CDBN_SearchOnly CDBN_SearchAndCreate
        CDBN_CreateOnly CDBN_FollowLink CDBN_CreateGroupNode
        CDBN_CreateStringNode CDBN_CreateLinkNode
        @param sortFn used only if adding a node
        @param caseSensitive the childName is case sensitive
    */
    virtual CDBNode *   Children(       const char *        childName,
                                        CDBNMode            functionMode       = CDBN_SearchOnly,
                                        SortFilterFn *      sortFn             = NULL,
                                        const char *        containerClassName = NULL,
                                        bool                caseSensitive      = True)
                                                            { return NULL;  }

    /** moves recursively in the tree from current position
        UpNode is one level up
        RootNode is all the way up
        Any movement onto unexisting subtree will fail
        Any movement out of unexisting subtree will erase temporary holders
        Movement is relative to current location
        @param functionMode. if it contains CDBN_FollowLink then link are followed
        This is the one of the few functions that allows following a link
        Note that once a link is stepped into, one cannot return from the same path
        calls FindSegment and the functionMode is past unchanged 
        @param caseSensitive the childName is case sensitive*/
    inline  CDBNode *   Find(           const char *        childName,
                                        CDBNMode            functionMode        = CDBN_None,
                                        SortFilterFn *      sortFn              = NULL,
                                        const char *        containerClassName  = NULL,
                                        bool                caseSensitive       = True)
                                                            {
                                                                const char *s = childName;
                                                                return Find2(s,functionMode,sortFn,containerClassName, caseSensitive);
                                                            }

    /** moves recursively in the tree from current position
        UpNode is one level up
        RootNode is all the way up
        Any movement onto unexisting subtree will fail
        Any movement out of unexisting subtree will erase temporary holders
        Movement is relative to current location
        @param functionMode. if it contains CDBN_FollowLink then link are followed
        This is the one of the few functions that allows following a link
        Note that once a link is stepped into, one cannot return from the same path
        calls FindSegment and the functionMode is past unchanged
        @param childName is returned modified to reflect the depth of matching 
        @param caseSensitive the childName is case sensitive
    */
            CDBNode *   Find2(          const char *&       childName,
                                        CDBNMode            functionMode       = CDBN_None,
                                        SortFilterFn *      sortFn             = NULL,
                                        const char *        containerClassName = NULL,
                                        bool                caseSensitive      = True);

    /** find a location within the whole (sub)tree.
        The nodes are numbered from left to right
        and from subnode to supernode
        @param cdbam if it contains CDBAM_LeafsOnly   it will not account the group nodes
                     if it contains CDBAM_SubTreeOnly addresses only the subtree
                     if it contains CDBAM_Relative    it is a step from current location
        If the node does not exist returns False and remains in the start position   */
            CDBNode *   Find(           int                 index,
                                        CDBAddressMode      cdbam           = CDBAM_LeafsOnly );

    /** moves one level in the tree
        UpNode is one level up
        RootNode is all the way up
        Any movement onto unexisting subtree will fail
        Any movement out of unexisting subtree will erase temporary holders
        Movement is relative to current location
        @param functionMode. if it contains CDBN_FollowLink then link are followed
        This is the one of the few functions that allows following a link
        Note that once a link is stepped into, one cannot return from the same path
        calls FindOrCreateChild and the functionMode is past unchanged 
        @param caseSensitive childName is case sensitive
        */
            CDBNode *   FindSegment(    const char *        childName,
                                        CDBNMode            functionMode       = CDBN_None,
                                        SortFilterFn *      sortFn             = NULL,
                                        const char *        containerClassName = NULL,
                                        bool                caseSensitive      = True);


    /** moves to a node pointing to the specified subtree */
            CDBNode *   FindSubTree(    const char *        subtree,
                                        CDBAddressMode      cdbam);

    /** allows accessing the brothers. negative and positive numbers allow to
        move relative to start position  */
            CDBNode *   FindBrother(int steps);

    /** how many elements in the subtree */
    virtual int         SubTreeSize(bool leafOnly)          { return 1; }

    /** Returns th eposition of the node within the tree.
        @param leafOnly if true, only counts leaf nodes
        @return A negative value (-2) means this is not a
        leaf node while leafOnly was set to True */
    virtual int         TreePosition(bool leafOnly = True);

    /** How many childrens of this node. -1 means it is not a node that has children */
    virtual int32       NumberOfChildren()         const    { return -1; }

    /** How much data elelments contained by this node. -1 means it is not a node that has data */
    virtual int32       NumberOfElements()         const    { return -1; };

    /** removes all unreferenced subtrees */
    virtual void        DeleteSubTree()                     {}

    /** removes specified child, if  */
    virtual bool        RemoveChild(    CDBNode *           node)
                                                            { return False; }

    /** writes data on a node.
        @param valueType specifies data type
        @param size specifies how many elements */
    virtual bool        WriteContent(   const void *        value,
                                        const CDBTYPE &     valueType,
                                        int                 size)
                                                            { return False; };

    /** Writes content into a data node. Creates a data node or modifies an existing
        @param configName is the address of the parameter relative to the current node
        @param array is the data in wahtever form specified by valueType
        @param valueType specifies tha data type in array
        @param size is a vector of matrix dimensions if size == NULL it treats the input as a monodimensional array of size nDim
        @param nDim how many dimensions the array possesses or the vector size if size = NULL
        @param functionMode allows specifying the creation of a node different frpm the CDBStringDataNode
        @param sortFn allows inserting newly created nodes in a specific order */
    virtual bool        WriteArray(     const char *        configName,
                                        const void *        array,
                                        const CDBTYPE &     valueType,
                                        const int *         size,
                                        int                 nDim,
                                        CDBNMode            functionMode    = CDBN_CreateStringNode,
                                        SortFilterFn *      sortFn          = NULL)
                                                            { return False; }

    /** reads data from a node.
        @param valueType specifies data type
        @param size specifies how many elements
        @param value contains a pointer to memory
        where to write the data */
    virtual bool        ReadContent(    void *              value,
                                        const CDBTYPE &     valueType,
                                        int                 size,
                                        va_list             argList)
                                                            { return False; }

    /** writes data on a node.
        @param valueType specifies data type
        @param size specifies how many elements */
            bool        ReadContent(    void *              value,
                                        const CDBTYPE &     valueType,
                                        int                 size,
                                        ...);

    /** reads content from a data node.
        @param configName is the address of the parameter relative to the current node
        @param array is the data in wahtever form specified by valueType
        @param valueType specifies tha data type in array
        @param size is a vector of matrix dimensions if size == NULL it treats the input as a monodimensional array of size nDim
        @param nDim how many dimensions the array possesses or the vector size if size = NULL 
        @param caseSensitive the configName is case sensitive
    */
    virtual bool        ReadArray(      const char *        configName,
                                        void *              array,
                                        const CDBTYPE &     valueType,
                                        const int *         size,
                                        int                 nDim,
                                        bool                caseSensitive = True)
                                                            { return False; }

    /** apply sorting */
    virtual bool        SortChildren(   SortFilterFn *  sortFn)
                                                            { return False; }

    /** Reads the dimensions of a matrix.
        if cdbaim is CDBAIM_Rigid, it expects all the indexes to be found form 0 to
        n-1 and the same number in all subtrees. */
            bool        GetArrayDims(   int *                size,
                                        int &                maxDim,
                                        const char *         configName,
                                        CDBArrayIndexingMode cdbaim,
                                        bool                 caseSensitive = True);


    /** parses the stream and build a configuration database according to the simple cdb syntax */
            bool        ReadFromStream( StreamInterface &   stream,
                                        StreamInterface *   err,
                                        bool                parserReportEnabled,
                                        SortFilterFn *      sorter);

};



#endif
