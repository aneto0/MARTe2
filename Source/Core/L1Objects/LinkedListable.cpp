/**
 * @file LinkedListable.cpp
 * @brief Source file for class LinkedListable
 * @date 05/08/2015
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
 * the class LinkedListable (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

#include "LinkedListable.h"

LinkedListable::LinkedListable() {
    next = static_cast<LinkedListable *>(NULL);
}

LinkedListable::~LinkedListable() {
    next = static_cast<LinkedListable *>(NULL);
}

LinkedListable *LinkedListable::Next() {
    return next;
}

void LinkedListable::SetNext(LinkedListable * const p) {
    next = p;
}

uint32 LinkedListable::Size() {
    LinkedListable *p = this;
    uint32 count = 0u;
    while (p != NULL) {
        p = p->next;
        count++;
    }
    return count;
}

void LinkedListable::BSort(SortFilter * const sorter) {

    if ((sorter != NULL) && (next != NULL)) {
        uint32 count = 1u;
        LinkedListable *p = this;
        while (p->next->next != NULL) {
            count++;
            LinkedListable *a1 = p->next;
            LinkedListable *a2 = p->next->next;
            if (sorter->Compare(a1, a2) > 0) {
                a1->next = a2->next;
                a2->next = a1;
                p->next = a2;
                p = a2;
            }
            else {
                p = a1;
            }
        }
        while (count > 2u) {
            p = this;
            uint32 index = count;
            while (index > 2u) {
                index--;
                LinkedListable *a1 = p->next;
                LinkedListable *a2 = p->next->next;
                if (sorter->Compare(a1, a2) > 0) {
                    a1->next = a2->next;
                    a2->next = a1;
                    p->next = a2;
                    p = a2;
                }
                else {
                    p = a1;
                }
            }
            count--;
        }
    }
}

void LinkedListable::BSort(SortFilterFn * const sorter) {

    if ((sorter != NULL) && (next != NULL)) {
        uint32 count = 1u;
        LinkedListable *p = this;
        while (p->next->next != NULL) {
            count++;
            LinkedListable *a1 = p->next;
            LinkedListable *a2 = p->next->next;
            if (sorter(a1, a2) > 0) {
                a1->next = a2->next;
                a2->next = a1;
                p->next = a2;
                p = a2;
            }
            else {
                p = a1;
            }
        }
        while (count > 2u) {
            p = this;
            uint32 index = count;
            while (index > 2u) {
                index--;
                LinkedListable *a1 = p->next;
                LinkedListable *a2 = p->next->next;
                if (sorter(a1, a2) > 0) {
                    a1->next = a2->next;
                    a2->next = a1;
                    p->next = a2;
                    p = a2;
                }
                else {
                    p = a1;
                }
            }
            count--;
        }
    }
}

void LinkedListable::Insert(LinkedListable * p) {

    if (p != NULL) {
        LinkedListable *q = next;
        next = p;
        while (p->next != NULL) {
            p = p->next;
        }
        p->next = q;
    }
}

void LinkedListable::Insert(LinkedListable * p,
                            SortFilter * const sorter) {

    if (p != NULL) {
        if (sorter == NULL) {
            Insert(p);
        }
        else {
            if (p->next != NULL) {
                LinkedListable root;
                root.next = p;
                root.BSort(sorter);
                p = root.next;
            }
            LinkedListable *list = this;
            while ((p != NULL) && (list->next != NULL)) {
                if (sorter->Compare(list->next, p) > 0) {
                    LinkedListable *item = p;
                    p = p->next;
                    item->next = list->next;
                    list->next = item;
                }
                else {
                    list = list->next;
                }
            }
            if (p != NULL) {
                list->next = p;
            }
        }
    }
}

void LinkedListable::Insert(LinkedListable * p,
                            SortFilterFn * const sorter) {
    if (p != NULL) {
        if (sorter == NULL) {
            Insert(p);
        }
        else {
            if (p->next != NULL) {
                LinkedListable root;
                root.next = p;
                root.BSort(sorter);
                p = root.next;
            }
            LinkedListable *list = this;
            while ((p != NULL) && (list->next != NULL)) {
                if (sorter(list->next, p) > 0) {
                    LinkedListable *item = p;
                    p = p->next;
                    item->next = list->next;
                    list->next = item;
                }
                else {
                    list = list->next;
                }
            }
            if (p != NULL) {
                list->next = p;
            }
        }
    }
}

void LinkedListable::Add(LinkedListable * const p) {
    if (p != NULL) {
        LinkedListable *q = this;
        while (q->next != NULL) {
            q = q->next;
        }
        q->next = p;
        p->next = static_cast<LinkedListable *>(NULL);
    }
}

void LinkedListable::AddL(LinkedListable * const p) {
    if (p != NULL) {
        LinkedListable *q = this;
        while (q->next != NULL) {
            q = q->next;
        }
        q->next = p;
    }
}

bool LinkedListable::Search(const LinkedListable * const p) {
    bool ret = false;
    LinkedListable *q = this;
    while (q != NULL) {
        if (q == p) {
            ret = true;
            break;
        }
        q = q->next;
    }
    return ret;
}

LinkedListable *LinkedListable::Search(SearchFilter * const filter) {

    LinkedListable *ret = static_cast<LinkedListable *>(NULL);
    if (filter != NULL) {

        LinkedListable *q = this;
        while (q != NULL) {
            if (filter->Test(q)) {
                ret = q;
                break;
            }
            q = q->next;
        }
    }
    return ret;
}

LinkedListable *LinkedListable::Search(SearchFilterFn * const filter) {

    LinkedListable *ret = static_cast<LinkedListable *>(NULL);
    if (filter != NULL) {

        LinkedListable *q = this;
        while (q != NULL) {
            if (filter(q)) {
                ret = q;
                break;
            }
            q = q->next;
        }
    }
    return ret;
}

bool LinkedListable::Extract(LinkedListable * const p) {

    bool ret = false;
    if (p != NULL) {
        ret = true;
        LinkedListable *q = this;

        while ((q->next != p) && (q->next != NULL)) {
            q = q->next;
        }
        if (q->next == NULL) {
            ret = false;
        }

        if (ret) {
            q->next = q->next->next;
            p->next = static_cast<LinkedListable *>(NULL);
        }
    }
    return ret;
}

LinkedListable *LinkedListable::Extract(SearchFilter * const filter) {

    LinkedListable *ret = static_cast<LinkedListable *>(NULL);

    if (filter != NULL) {
        LinkedListable *q = this;
        while (q->next != NULL) {
            if (filter->Test(q->next)) {
                LinkedListable *p = q->next;
                q->next = q->next->next;
                p->next = static_cast<LinkedListable *>(NULL);
                ret = p;
                break;
            }
            else {
                q = q->next;
            }
        }
    }
    return ret;
}

LinkedListable *LinkedListable::Extract(SearchFilterFn * const filter) {
    LinkedListable *ret = static_cast<LinkedListable *>(NULL);
    if (filter != NULL) {

        LinkedListable *q = this;
        while (q->next != NULL) {
            if (filter(q->next)) {
                LinkedListable *p = q->next;
                q->next = q->next->next;
                p->next = static_cast<LinkedListable *>(NULL);
                ret = p;
                break;
            }
            else {
                q = q->next;
            }
        }
    }
    return ret;
}

bool LinkedListable::Delete(LinkedListable * const p) {
    bool ret = Extract(p);
    if (ret) {
        delete p;
    }
    return ret;
}

uint32 LinkedListable::Delete(SearchFilter * const filter) {
    uint32 deleted = 0u;
    if (filter != NULL) {

        LinkedListable *q = this;
        while (q->next != NULL) {
            if (filter->Test(q->next)) {
                LinkedListable *p = q->next;
                q->next = q->next->next;
                delete p;
                deleted++;
            }
            else {
                q = q->next;
            }
        }
    }
    return deleted;
}

uint32 LinkedListable::Delete(SearchFilterFn * const filter) {
    uint32 deleted = 0u;
    if (filter != NULL) {

        LinkedListable *q = this;
        while (q->next != NULL) {
            if (filter(q->next)) {
                LinkedListable *p = q->next;
                q->next = q->next->next;
                delete p;
                deleted++;
            }
            else {
                q = q->next;
            }
        }
    }
    return deleted;
}

LinkedListable *LinkedListable::Peek(uint32 index) {
    LinkedListable *p = this;
    while ((p != NULL) && (index > 0u)) {
        p = p->next;
        index--;
    }
    return p;
}

void LinkedListable::Iterate(Iterator * const it) {
    LinkedListable *p = this;
    while (p != NULL) {
        it->Do(p);
        p = p->next;
    }
}

void LinkedListable::Iterate(IteratorFn * const it) {
    LinkedListable *p = this;
    while (p != NULL) {
        it(p);
        p = p->next;
    }
}

