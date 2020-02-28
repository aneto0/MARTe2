/**
 * @file StreamStructuredData.h
 * @brief Header file for class StreamStructuredData
 * @date 14/09/2018
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

 * @details This header file contains the declaration of the class StreamStructuredData
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef STREAMSTRUCTUREDDATA_H_
#define STREAMSTRUCTUREDDATA_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "BufferedStreamI.h"
#include "DoubleBufferedStream.h"
#include "LinkedListable.h"
#include "ReferenceContainer.h"
#include "ReferenceContainer.h"
#include "ReferenceContainerFilterObjectName.h"
#include "ReferenceContainerFilterReferences.h"
#include "ReferenceT.h"
#include "SingleBufferedStream.h"
#include "StreamString.h"
#include "StreamStructuredDataI.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief A descriptor used to keep the status during the
 * StreamStructuredData operations.
 */
class StreamStructuredDataNodeDes: public ReferenceContainer {
public:

    /**
     * @brief Constructor
     */
    StreamStructuredDataNodeDes();

    /**
     * @brief destructor
     */
    virtual ~StreamStructuredDataNodeDes();

    /**
     * Denote a node as closed.
     */
    uint8 isClosed;

    /**
     * Stores the number of variables
     * within a node.
     */
    uint32 numberOfVariables;

    /**
     * True if a separator is required to be written before starting the next block.
     */
    bool needsSeparatorBeforeNextBlock;

    /**
     * True if a separator is required to be written before starting the next write.
     */
    bool needsSeparatorBeforeNextWrite;
};

/**
 * @brief Implementation of a StructuredDataI interface wrapped over a stream.
 * @see StreamStructuredDataI
 * @details This class allows to use the StructuredDataI interface without need to store data
 * in memory, but streaming out directly on a chosen stream. Only write operations are
 * allowed.
 * @details This templated version allows to choose a printer (that must be a descendant of PrinterI)
 * to print on the stream using the database language implemented by the printer object.
 * @details All the StructuredDataI read operations will return false because this object is intended to
 * be used only to write out data. Since the write operations are directly flushed on the internal stream,
 * it is not allowed to move or create new nodes backward in the database. For Instance in these sequence of operations:\n
 *   CreateAbsolute("A.B") //allowed\n
 *   CreateAbsolute("A.C") //allowed but now B is closed\n
 *   CreateAbsolute("A.B.D") //not allowed: it is not possible return back into B
 */
template<class Printer>
class StreamStructuredData: public StreamStructuredDataI {

public:

    /**
     * @brief Default Constructor
     */
    StreamStructuredData();

    /**
     * @brief Constructor.
     * @details Sets the internal stream.
     * @param[in] streamIn is the internal stream.
     */
    StreamStructuredData(BufferedStreamI &streamIn);

    /**
     * @brief Destructor
     */
    virtual ~StreamStructuredData();

    /**
     * @brief Sets the internal stream.
     * @param[in] streamIn is the internal stream.
     */
    virtual void SetStream(BufferedStreamI &streamIn);

    /**
     * @see StructuredDataI::Read.
     * @details Not implemented. It returns false.
     */
    virtual bool Read(const char8 * const name, const AnyType &value);

    /**
     * @see StructuredDataI::GetType.
     * @details Not implemented. It returns false.
     */
    virtual AnyType GetType(const char8 * const name);

    /**
     * @see StructuredDataI::Write.
     */
    virtual bool Write(const char8 * const name, const AnyType &value);

    /**
     * @see StructuredDataI::GetType.
     * @details Not implemented. It returns false.
     */
    virtual bool Copy(StructuredDataI &destination);

    /**
     * @see StructuredDataI::AddToCurrentNode.
     */
    virtual bool AddToCurrentNode(Reference node);

    /**
     * @see StructuredDataI::MoveToRoot.
     */
    virtual bool MoveToRoot();

    /**
     * @see StructuredDataI::MoveToAncestor.
     */
    virtual bool MoveToAncestor(uint32 generations);

    /**
     * @see StructuredDataI::MoveAbsolute.
     */
    virtual bool MoveAbsolute(const char8 * const path);

    /**
     * @see StructuredDataI::MoveRelative.
     */
    virtual bool MoveRelative(const char8 * const path);

    /**
     * @see StructuredDataI::MoveToChild.
     */
    virtual bool MoveToChild(const uint32 childIdx);

    /**
     * @see StructuredDataI::CreateAbsolute.
     */
    virtual bool CreateAbsolute(const char8 * const path);

    /**
     * @see StructuredDataI::CreateRelative.
     */
    virtual bool CreateRelative(const char8 * const path);

    /**
     * @see StructuredDataI::Delete.
     */
    virtual bool Delete(const char8 * const name);

    /**
     * @see StructuredDataI::GetName.
     */
    virtual const char8 *GetName();

    /**
     * @see StructuredDataI::GetChildName.
     */
    virtual const char8 *GetChildName(const uint32 index);

    /**
     * @see StructuredDataI::GetNumberOfChildren.
     */
    virtual uint32 GetNumberOfChildren();

    /**
     * @return the Printer
     */
    Printer *GetPrinter();

protected:

    /**
     * Stores the last path accessed.
     */
    StreamString currentPath;

    /**
     * The root descriptor
     */
    ReferenceT<StreamStructuredDataNodeDes> treeDescriptor;

    /**
     * The current node descriptor
     */
    ReferenceT<StreamStructuredDataNodeDes> currentNode;

    /**
     * The printer that defines the database language
     */
    Printer printer;

    /**
     * Used to check if a block is closed or not.
     */
    bool blockCloseState;
};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

namespace MARTe {

template<class Printer>
StreamStructuredData<Printer>::StreamStructuredData(BufferedStreamI &streamIn) :
        treeDescriptor(GlobalObjectsDatabase::Instance()->GetStandardHeap()), printer(streamIn) {
    stream = &streamIn;

    currentNode = treeDescriptor;

    blockCloseState = false;
}

template<class Printer>
StreamStructuredData<Printer>::StreamStructuredData() :
        treeDescriptor(GlobalObjectsDatabase::Instance()->GetStandardHeap()), printer() {

    stream = NULL;

    currentNode = treeDescriptor;

    blockCloseState = false;
}

template<class Printer>
void StreamStructuredData<Printer>::SetStream(BufferedStreamI &streamIn) {
    StreamStructuredDataI::SetStream(streamIn);
    printer.SetStream(streamIn);
}

template<class Printer>
StreamStructuredData<Printer>::~StreamStructuredData() {

}

template<class Printer>
bool StreamStructuredData<Printer>::Read(const char8 * const name, const AnyType &value) {
    return false;
}

template<class Printer>
AnyType StreamStructuredData<Printer>::GetType(const char8 * const name) {
    return voidAnyType;
}

template<class Printer>
bool StreamStructuredData<Printer>::Write(const char8 * const name, const AnyType &value) {

    bool ret = true;
    if (currentNode->numberOfVariables > 0u) {
        ret = printer.PrintVariableSeparator();
        currentNode->needsSeparatorBeforeNextWrite = false;
    }
    if (ret) {
        if (currentNode->needsSeparatorBeforeNextWrite) {
            ret = printer.PrintBlockSeparator();
            currentNode->needsSeparatorBeforeNextWrite = false;
        }
    }
    if (ret) {
        currentNode->needsSeparatorBeforeNextBlock = true;
        ret = stream->Printf("%s", "\n\r");
    }
    if (ret) {
        //use custom component to print
        ret = printer.PrintOpenAssignment(name);
    }
    if (ret) {
        ret = printer.PrintVariable(value);
    }
    if (ret) {
        ret = printer.PrintCloseAssignment(name);
    }

    if (ret) {
        ret = stream->Flush();
        currentNode->numberOfVariables++;
    }
    return ret;
}

template<class Printer>
bool StreamStructuredData<Printer>::Copy(StructuredDataI &destination) {
    return false;
}

template<class Printer>
bool StreamStructuredData<Printer>::AddToCurrentNode(Reference node) {
    ReferenceT<StreamStructuredDataNodeDes> toAdd = node;
    bool ret = toAdd.IsValid();
    if (ret) {
        ret = currentNode->Insert(node);
    }
    return ret;
}

template<class Printer>
bool StreamStructuredData<Printer>::MoveToRoot() {

    //find the last node by path
    //close the nodes along the path

    ReferenceContainerFilterReferences filter(1, ReferenceContainerFilterMode::PATH, currentNode);
    ReferenceContainer path;
    treeDescriptor->Find(path, filter);

    uint32 pathSize = path.Size();
    bool ret = (pathSize > 0u);
    for (uint32 i = 0u; (i < pathSize) && (ret); i++) {
        ReferenceT<StreamStructuredDataNodeDes> ref = path.Get(pathSize - i - 1u);
        ret = ref.IsValid();
        if (ret) {
            ref->isClosed = 1u;
            ret = stream->Printf("%s", "\n\r");
            if (ret) {
                ret = printer.PrintCloseBlock(ref->GetName());
                blockCloseState = true;
            }
        }
    }

    if (ret) {
        currentPath = "";
        currentNode = treeDescriptor;
    }
    if (ret) {
        stream->Flush();
    }

    return ret;
}

template<class Printer>
bool StreamStructuredData<Printer>::MoveToAncestor(uint32 generations) {

    bool ret = true;
    if (generations > 0u) {

        ReferenceContainerFilterReferences filter(1, ReferenceContainerFilterMode::PATH, currentNode);
        ReferenceContainer path;
        treeDescriptor->Find(path, filter);

        uint32 pathSize = path.Size();
        ret = (pathSize >= generations);

        uint32 goodOnes = (pathSize - generations);
        StreamString currentPathTmp = "";
        currentPath.Seek(0u);
        for (uint32 i = 0u; (i < pathSize) && (ret); i++) {
            if (i < goodOnes) {
                StreamString token;
                char8 terminator;
                ret = currentPath.GetToken(token, ".", terminator);
                if (ret) {
                    ret = (terminator == '.');
                }
                if (ret) {
                    currentPathTmp += token;
                    currentPathTmp += ".";
                }
            }
            else {
                ReferenceT<StreamStructuredDataNodeDes> ref = path.Get(pathSize - i + goodOnes - 1u);
                ret = (ref.IsValid());
                if (ret) {
                    ref->isClosed = 1u;
                    ret = stream->Printf("%s", "\n\r");
                }
                if (ret) {
                    ret = printer.PrintCloseBlock(ref->GetName());
                    blockCloseState = true;
                }
            }
        }
        if (ret) {
            if (goodOnes > 0u) {
                currentNode = path.Get(goodOnes - 1u);
            }
            else {
                currentNode = treeDescriptor;
            }
            currentPath = currentPathTmp;
            if (currentPath.Size() > 0u) {
                if (currentPath[currentPath.Size() - 1u] == '.') {
                    currentPath.SetSize(currentPath.Size() - 1u);
                }
            }
        }
        if (ret) {
            stream->Flush();
        }
    }
    return ret;
}

template<class Printer>
bool StreamStructuredData<Printer>::MoveAbsolute(const char8 * const path) {

    ReferenceContainerFilterObjectName filterDest(1, ReferenceContainerFilterMode::SHALLOW, path);
    ReferenceContainer resultDest;
    treeDescriptor->Find(resultDest, filterDest);

    uint32 pathDestSize = resultDest.Size();
    bool ret = (pathDestSize > 0u);
    if (ret) {
        ReferenceT<StreamStructuredDataNodeDes> ref = resultDest.Get(pathDestSize - 1u);
        ret = (ref.IsValid());
        if (ret) {
            ret = (ref->isClosed == 0u);
        }
        if (ret) {

            ReferenceContainerFilterReferences filter(1, ReferenceContainerFilterMode::PATH, currentNode);
            ReferenceContainer result;
            treeDescriptor->Find(result, filter);

            uint32 pathSize = result.Size();

            StreamString curPathTemp = currentPath;
            curPathTemp.Seek(0u);
            char8 terminator = '.';
            uint32 i = 0u;

            for (i = 0u; (i < pathDestSize) && (terminator == '.') && (ret); i++) {

                StreamString token;
                curPathTemp.GetToken(token, ".", terminator);
                ReferenceT<StreamStructuredDataNodeDes> ref = resultDest.Get(i);
                ret = (ref.IsValid());
                if (ret) {
                    StreamString nodeName = ref->GetName();
                    if (token != nodeName) {
                        break;
                    }
                }
            }

            //close nodes and braces
            uint32 exitIndex = i;
            bool blocksClosed = (exitIndex < pathSize);
            if (pathSize > 0u) {
                for (uint32 j = (pathSize - 1u); (j >= exitIndex) && (ret); j--) {
                    ReferenceT<StreamStructuredDataNodeDes> ref = result.Get(j);
                    ret = (ref.IsValid());
                    if (ret) {
                        ref->isClosed = 1u;
                        ret = stream->Printf("%s", "\n\r");
                        if (ret) {
                            ret = printer.PrintCloseBlock(ref->GetName());
                            blockCloseState = true;
                            currentNode->needsSeparatorBeforeNextBlock = false;
                        }
                    }
                }
            }

            //open braces
            for (uint32 j = exitIndex; (j < pathDestSize) && (ret); j++) {
                ReferenceT<StreamStructuredDataNodeDes> ref = resultDest.Get(j);
                ret = (ref.IsValid());
                if (ret) {
                    if (j == exitIndex) {
                        if (blockCloseState) {
                            ret = printer.PrintBlockSeparator();
                        }
                    }
                    if (ret) {
                        ret = stream->Printf("%s", "\n\r");
                    }
                    if (ret) {
                        if (currentNode->needsSeparatorBeforeNextBlock) {
                            ret = printer.PrintBlockSeparator();
                            currentNode->needsSeparatorBeforeNextBlock = false;
                        }
                    }
                    if (ret) {
                        ret = printer.PrintOpenBlock(ref->GetName());
                        currentNode->needsSeparatorBeforeNextWrite = true;
                        blockCloseState = false;
                    }
                }
            }

            if (ret) {
                if (blocksClosed) {
                    //close deeply also the children
                    uint32 curSize = currentNode->Size();
                    for (uint32 j = 0u; (j < curSize) && (ret); j++) {
                        ReferenceT<StreamStructuredDataNodeDes> ref = currentNode->Get(j);
                        if (ref.IsValid()) {
                            ret = currentNode->Delete(ref);
                        }
                    }
                }
                if (ret) {
                    currentPath = path;
                    currentNode = ref;
                }
            }
            if (ret) {
                if (ret) {
                    stream->Flush();
                }
            }
        }
    }

    return ret;
}

template<class Printer>
bool StreamStructuredData<Printer>::MoveRelative(const char8 * const path) {
    StreamString totalPath = currentPath;
    if (currentPath.Size() > 0u) {
        totalPath += ".";
    }
    totalPath += path;
    return MoveAbsolute(totalPath.Buffer());

}

template<class Printer>
bool StreamStructuredData<Printer>::MoveToChild(const uint32 childIdx) {

    ReferenceT<StreamStructuredDataNodeDes> child = currentNode->Get(childIdx);
    bool ret = child.IsValid();
    if (ret) {
        ret = (child->isClosed == 0u);
    }
    if (ret) {
        ret = stream->Printf("%s", "\n\r");
    }
    if (ret) {
        ret = printer.PrintOpenBlock(child->GetName());
        currentNode->needsSeparatorBeforeNextWrite = true;
        blockCloseState = false;
    }
    if (ret) {
        currentPath += child->GetName();
        currentNode = child;
        stream->Flush();
    }

    return ret;
}

template<class Printer>
bool StreamStructuredData<Printer>::CreateAbsolute(const char8 * const path) {
    StreamString pathStr = path;
    pathStr.Seek(0u);
    ReferenceT<StreamStructuredDataNodeDes> node = treeDescriptor;

    //tokenize the path
    char8 terminator;
    StreamString token;
    bool ret = true;
    while ((pathStr.GetToken(token, ".", terminator)) && (ret)) {
        uint32 nodeSize = node->Size();
        bool found = false;
        for (uint32 i = 0u; (i < nodeSize) && (ret) && (!found); i++) {
            ReferenceT<StreamStructuredDataNodeDes> child = node->Get(i);
            ret = child.IsValid();
            if (ret) {
                found = (token == child->GetName());
                if (found) {
                    node = child;
                }
            }
        }
        if ((ret) && (!found)) {
            //create the node
            ReferenceT<StreamStructuredDataNodeDes> newNode(GlobalObjectsDatabase::Instance()->GetStandardHeap());
            ret = newNode.IsValid();
            if (ret) {
                newNode->SetName(token.Buffer());
                ret = node->Insert(newNode);
                node = newNode;
            }
        }
        token.SetSize(0);
    }

    if (ret) {
        ret = MoveAbsolute(path);
    }

    return ret;
}

template<class Printer>
bool StreamStructuredData<Printer>::CreateRelative(const char8 * const path) {
    StreamString totalPath = (currentPath);
    if (currentPath.Size() > 0u) {
        totalPath += ".";
    }
    totalPath += path;
    return CreateAbsolute(totalPath.Buffer());
}

template<class Printer>
bool StreamStructuredData<Printer>::Delete(const char8 * const name) {
    return false;
}

template<class Printer>
const char8 *StreamStructuredData<Printer>::GetName() {
    return currentNode->GetName();
}

template<class Printer>
const char8 *StreamStructuredData<Printer>::GetChildName(const uint32 index) {
    uint32 size = currentNode->Size();

    const char8 * ret = NULL;
    if (index < size) {
        ReferenceT<StreamStructuredDataNodeDes> child = currentNode->Get(index);
        if (child.IsValid()) {
            ret = child->GetName();
        }
    }
    return ret;
}

template<class Printer>
uint32 StreamStructuredData<Printer>::GetNumberOfChildren() {
    return currentNode->Size();
}

template<class Printer>
Printer *StreamStructuredData<Printer>::GetPrinter() {
    return &printer;
}

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* SOURCE_CORE_BAREMETAL_L3STREAMS_STREAMSTRUCTUREDDATA_H_ */

