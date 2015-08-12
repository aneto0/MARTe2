/**
 * @file ReferenceContainer.cpp
 * @brief Source file for class ReferenceContainer
 * @date 12/08/2015
 * @author Andre Neto
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
 * the class ReferenceContainer (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "ReferenceContainer.h"
#include "ReferenceContainerItem.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
ReferenceContainer::ReferenceContainer() :
        Object() {
    if (mux.Create()) {
        msecTimeout = TTInfiniteWait;
    }
}

/*lint -e{1551} no exception should be thrown given that ReferenceContainer is
 * the sole owner of the list (LinkedListHolder)*/
ReferenceContainer::~ReferenceContainer() {
    LinkedListable *p = list.List();
    list.Reset();
    while (p != NULL) {
        LinkedListable *q = p;
        p = p->Next();
        delete q;
    }
}

bool ReferenceContainer::Insert(Reference ref,
                                const int32 &position) {
    bool ok = true;
    ReferenceContainerItem *newItem = new ReferenceContainerItem();
    if (newItem->Load(ref)) {
        if (position == -1) {
            list.ListAdd(newItem);
        }
        else {
            list.ListInsert(newItem, static_cast<uint32>(position));
        }
    }
    else {
        delete newItem;
        ok = false;
    }

    return ok;
}

bool GCRCFind(GCReference &reference,
              uint32 index,
              bool remove,
              bool recurse) {

    LinkedListable *ll = NULL;

    // if the request is to get a specific element then just do it
    if ((name == NULL) && (selector == NULL)) {

        // unlocks automatically on exit from function
        // just lock for this code block
        MuxLock muxLock;
        if (!muxLock.Lock(gcrc.mux, gcrc.msecTimeout)) {
            gcrc.AssertErrorCondition(Timeout, "GCRCFind: timeout on resource sharing ");
            return False;
        }

        // get the LinkedListable;
        if (remove) {
            ll = gcrc.list.ListExtract(index);
        }
        else {
            ll = gcrc.list.ListPeek(index);
        }
        if (ll == NULL) {
            gcrc.AssertErrorCondition(FatalError, "GCRCFind(%i): no element in the list fits criteria", index);
            return False;
        }

        // check type
        GCRCItem *gcri = dynamic_cast<GCRCItem *>(ll);
        if (gcri == NULL) {
            gcrc.AssertErrorCondition(FatalError, "GCRCFind(): non GCRCItem derived element in the list");

            if (remove)
                delete ll;
            return False;
        }

        if (recurse && (gcri->Link() != NULL)) {
            GCRTemplate < GCNOExtender<BString> > gcrtgcnobs(GCFT_Create);
            BString *bs = gcrtgcnobs.operator->();
            (*bs) = gcri->Link();
            gc = gcrtgcnobs;
        }
        else {
            // retrieve reference
            gc = gcri->Reference();
        }
        if (remove)
            delete ll;
        return True;
    }
}

CLASS_REGISTER(ReferenceContainer, "1.0")
