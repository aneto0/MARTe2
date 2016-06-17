/**
 * @file MessageTest.cpp
 * @brief Source file for class MessageTest
 * @date 14/giu/2016
 * @author pc
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
 * the class MessageTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "MessageTest.h"
#include "ConfigurationDatabase.h"
#include "StandardParser.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

bool MessageTest::TestConstructor() {

    Message mess;
    return true;

}

bool MessageTest::TestInitialise(const MessageInitTableTest * table) {

    Message mess;

    uint32 i = 0u;

    while (table[i].conf != NULL) {
        ConfigurationDatabase cdb;
        StreamString confStr=table[i].conf;
        confStr.Seek(0ll);
        StandardParser parser(confStr, cdb);
        if(!parser.Parse()) {
            return false;
        }

        if(mess.Initialise(cdb)!=table[i].expected) {
            return false;
        }

        mess.CleanUp();
        i++;
    }

    return true;
}

bool MessageTest::TestMarkAsReply() {
    Message mess;
    mess.MarkAsReply();
    if (!mess.IsReplyMessage()) {
        return false;
    }
    mess.MarkAsReply(false);
    return (!mess.IsReplyMessage());
}

bool MessageTest::TestIsReplyMessage() {
    return TestMarkAsReply();
}

bool MessageTest::TestMarkImmediateReplyExpected() {
    Message mess;
    mess.MarkImmediateReplyExpected();
    if ((!mess.ReplyExpected()) || (!mess.ImmediateReplyExpected())) {
        return false;
    }
    mess.MarkImmediateReplyExpected(false);
    return ((!mess.ReplyExpected()) && (!mess.ImmediateReplyExpected()));
}

bool MessageTest::TestMarkLateReplyExpected() {
    Message mess;
    mess.MarkLateReplyExpected();
    if((!mess.ReplyExpected()) || (!mess.LateReplyExpected())){
        return false;
    }
    mess.MarkLateReplyExpected(false);
    return ((!mess.ReplyExpected()) && (!mess.LateReplyExpected()));
}

bool MessageTest::TestReplyExpected() {
    Message mess;
    if (mess.ReplyExpected()) {
        return false;
    }
    mess.MarkImmediateReplyExpected();
    if (!mess.ReplyExpected()) {
        return false;
    }
    mess.MarkLateReplyExpected();

    return mess.ReplyExpected();
}

bool MessageTest::TestImmediateReplyExpected() {
    Message mess;
    if (mess.ImmediateReplyExpected()) {
        return false;
    }

    mess.MarkLateReplyExpected();
    if (mess.ImmediateReplyExpected()) {
        return false;
    }
    mess.MarkImmediateReplyExpected();
    return mess.ImmediateReplyExpected();

}

bool MessageTest::TestLateReplyExpected() {
    Message mess;
    if (mess.LateReplyExpected()) {
        return false;
    }
    mess.MarkImmediateReplyExpected();
    if (mess.LateReplyExpected()) {
        return false;
    }
    mess.MarkLateReplyExpected();
    return mess.LateReplyExpected();

}

bool MessageTest::TestGetDestination() {
    ConfigurationDatabase cdb;
    cdb.Write("Destination", "A");
    cdb.Write("Function", "f");
    Message mess;
    if (!mess.Initialise(cdb)) {
        return false;
    }

    return StringHelper::Compare(mess.GetDestination(), "A") == 0;
}

bool MessageTest::TestSetSender() {
    Message mess;
    mess.SetSender("sender");
    return StringHelper::Compare(mess.GetSender(), "sender") == 0;
}

bool MessageTest::TestGetSender() {
    return TestSetSender();
}

bool MessageTest::TestGetFunction() {
    ConfigurationDatabase cdb;
    cdb.Write("Destination", "A");
    cdb.Write("Function", "f");
    Message mess;
    if (!mess.Initialise(cdb)) {
        return false;
    }

    return StringHelper::Compare(mess.GetFunction(), "f") == 0;

}

bool MessageTest::TestSetReplyTimeout() {
    TimeoutType maxWaitIn = 1000;
    Message mess;
    mess.SetReplyTimeout(maxWaitIn);
    return true;
}

