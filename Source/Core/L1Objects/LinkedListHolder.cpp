/**
 * @file LinkedListHolder.cpp
 * @brief Source file for class LinkedListHolder
 * @date 06/08/2015
 * @author Giuseppe Ferrò
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
 * the class LinkedListHolder (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "LinkedListHolder.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

void LinkedListHolder::CleanUp() {
    LinkedListable *p = llhRoot.Next();
    llhRoot.SetNext(static_cast<LinkedListable *>(NULL));
    while (p != NULL) {
        LinkedListable *q = p;
        p = p->Next();
        delete q;
    }
    llhSize = 0u;
}

void LinkedListHolder::Reset() {
    llhRoot.SetNext(static_cast<LinkedListable *>(NULL));
    llhSize = 0u;
}

LinkedListHolder::LinkedListHolder() {
    llhSize = 0u;
    llhRoot.SetNext(static_cast<LinkedListable *>(NULL));
}

LinkedListHolder::~LinkedListHolder() {
    CleanUp();
}

LinkedListable *LinkedListHolder::List() {
    return llhRoot.Next();
}

uint32 LinkedListHolder::ListSize() const {
    return llhSize;
}

void LinkedListHolder::ListInsert(LinkedListable * const p) {
    if (p != NULL) {

        llhSize += p->Size();
        llhRoot.Insert(p);
    }
}

void LinkedListHolder::ListInsert(LinkedListable * const p,
                                  SortFilter * const sorter) {
    if (p != NULL) {

        llhSize += p->Size();
        llhRoot.Insert(p, sorter);
    }
}

void LinkedListHolder::ListInsert(LinkedListable * const p,
                                  SortFilterFn * const sorter) {
    if (p != NULL) {

        llhSize += p->Size();
        llhRoot.Insert(p, sorter);
    }
}

void LinkedListHolder::ListInsert(LinkedListable * const q,
                                  uint32 index) {
    if (q != NULL) {

        LinkedListable *p = &llhRoot;
        while ((p->Next() != NULL) && (index > 0u)) {
            p = p->Next();
            index--;
        }
        llhSize += q->Size();
        p->Insert(q);
    }
}

void LinkedListHolder::ListAdd(LinkedListable * const p) {
    if (p != NULL) {

        llhSize++;
        llhRoot.Add(p);
    }
}

void LinkedListHolder::ListAddL(LinkedListable * const p) {
    if (p != NULL) {

        llhSize += p->Size();
        llhRoot.AddL(p);
    }
}

bool LinkedListHolder::ListSearch(const LinkedListable * const p) {

    return (llhRoot.Next() == NULL) ? false : llhRoot.Next()->Search(p);
}

LinkedListable *LinkedListHolder::ListSearch(SearchFilter * const filter) {

    return (llhRoot.Next() == NULL) ? static_cast<LinkedListable*>(NULL) : llhRoot.Next()->Search(filter);
}

LinkedListable *LinkedListHolder::ListSearch(SearchFilterFn * const filter) {
    return (llhRoot.Next() == NULL) ? static_cast<LinkedListable*>(NULL) : llhRoot.Next()->Search(filter);
}

bool LinkedListHolder::ListExtract(LinkedListable * const p) {

    bool ret = false;

    if (llhRoot.Extract(p)) {
        llhSize--;
        ret = true;
    }

    return ret;
}

LinkedListable *LinkedListHolder::ListExtract(SearchFilter * const filter) {
    LinkedListable *p = llhRoot.Extract(filter);
    if (p != NULL) {
        llhSize--;
    }
    return p;
}

LinkedListable *LinkedListHolder::ListExtract(SearchFilterFn * const filter) {
    LinkedListable *p = llhRoot.Extract(filter);
    if (p != NULL) {
        llhSize--;
    }
    return p;
}

bool LinkedListHolder::ListDelete(LinkedListable * const p) {
    bool ret = false;

    if (llhRoot.Delete(p)) {
        llhSize--;
        ret = true;
    }
    return ret;
}

bool LinkedListHolder::ListDelete(SearchFilter * const filter) {
    uint32 deleted = llhRoot.Delete(filter);
    llhSize -= deleted;
    return (deleted > 0u);
}

bool LinkedListHolder::ListSafeDelete(SearchFilter * const filter) {
    uint32 deleted = 0u;
    if (filter != NULL) {

        LinkedListable *p = List();
        while (p != NULL) {
            if (filter->Test(p)) {
                if (ListExtract(p)) {
                    delete p;
                    // p->next may not be a safe pointer so assume the worst and start over again
                    p = List();
                    deleted++;
                }
                else {
                    p = p->Next();
                }
            }
            else {
                p = p->Next();
            }
        }
    }
    return (deleted > 0u);
}

bool LinkedListHolder::ListDelete(SearchFilterFn * const filter) {
    uint32 deleted = llhRoot.Delete(filter);
    llhSize -= deleted;
    return (deleted > 0u);
}

void LinkedListHolder::ListBSort(SortFilter * const sorter) {
    llhRoot.BSort(sorter);
}

void LinkedListHolder::ListBSort(SortFilterFn * const sorter) {
    llhRoot.BSort(sorter);
}

LinkedListable *LinkedListHolder::ListPeek(const uint32 index) {
    return llhRoot.Next()->Peek(index);
}

LinkedListable *LinkedListHolder::ListExtract(uint32 index) {

    LinkedListable *ret = static_cast<LinkedListable *>(NULL);
    LinkedListable *p = &llhRoot;
    while ((p != NULL) && (index > 0u)) {
        p = p->Next();
        index--;
    }

    if (p != NULL) {

        LinkedListable *q = p->Next();
        if (q != NULL) {
            llhSize--;
            p->SetNext(q->Next());
        }
        if (q != NULL) {
            q->SetNext(static_cast<LinkedListable *>(NULL));
        }
        ret = q;

    }

    return ret;
}

void LinkedListHolder::ListIterate(Iterator * const it) {
    if (llhRoot.Next() != NULL) {
        llhRoot.Next()->Iterate(it);
    }
}

void LinkedListHolder::ListIterate(IteratorFn * const it) {
    if (llhRoot.Next() != NULL) {
        llhRoot.Next()->Iterate(it);
    }
}

