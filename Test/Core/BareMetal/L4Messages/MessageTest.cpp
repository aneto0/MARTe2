/**
 * @file MessageTest.cpp
 * @brief Source file for class MessageTest
 * @date 14/06/2016
 * @author Giuseppe Ferrò
 * @author Ivan Herrero
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
#include "StreamString.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

MessageTest::MessageTest() {
}

MessageTest::~MessageTest() {
}

bool MessageTest::TestConstructor() {
    using namespace MARTe;
    Message mess;
    return true;

}

bool MessageTest::TestInitialise() {
    using namespace MARTe;
    bool result = false;
    const MessageTest::MessageInitTableTest table[] = { { "dummyDestination", "dummyFunction", 1000, "ExpectsReply",
            "Destination=dummyDestination\n Function=dummyFunction\n MaxWait=1000\n Mode=ExpectsReply\n", true }, { "dummyDestination", "dummyFunction", 1000,
            "", "Destination=dummyDestination\n Function=dummyFunction\n MaxWait=1000\n ", true }, { "dummyDestination", "dummyFunction", TTInfiniteWait,
            "ExpectsImmediateReply", "Destination=dummyDestination\n Function=dummyFunction\n Mode=ExpectsImmediateReply\n", true }, { "dummyDestination",
            "dummyFunction", TTInfiniteWait, "", "Destination=dummyDestination\n Function=dummyFunction\n ", true }, { "dummyDestination", "", TTInfiniteWait,
            "", "Destination=dummyDestination\n ", false }, { "", "dummyFunction", TTInfiniteWait, "", "Function=dummyFunction\n ", false }, { NULL, NULL, 0,
            NULL, NULL, false } };
    result = TestInitialise(table);
    return result;
}

bool MessageTest::TestInitialise(const MessageInitTableTest * table) {
    using namespace MARTe;

    bool result = true;

    uint32 i = 0u;

    while (table[i].configuration != NULL) {

        Message mess;
        bool initOK = false;

        //Initialise data:
        {
            bool status;
            ConfigurationDatabase cdb;
            StreamString confStr = table[i].configuration;
            confStr.Seek(0ll);
            StandardParser parser(confStr, cdb);
            status = parser.Parse();
            if (status) {
                status = mess.Initialise(cdb);
                initOK = status;
                result &= (status == table[i].expected);
            }
            else {
                initOK = false;
                result = false;
            }
        }

        //Verify data
        if (initOK) {
            result &= (StringHelper::Compare(mess.GetDestination(), table[i].destination) == 0);
            result &= (StringHelper::Compare(mess.GetFunction(), table[i].function) == 0);
            result &= (StringHelper::Compare(mess.GetSender(), "") == 0);
            result &= (mess.GetReplyTimeout() == table[i].maxwait);
            if (StringHelper::Compare(table[i].mode, "ExpectsReply") == 0) {
                result &= mess.ExpectsReply();
            }
            if (StringHelper::Compare(table[i].mode, "ExpectsImmediateReply") == 0) {
                result &= !mess.ExpectsIndirectReply();
            }
            result &= !mess.IsReply();
        }

        mess.CleanUp(); //??
        i++;
    }

    return result;
}

bool MessageTest::TestSetAsReply() {
    using namespace MARTe;
    bool result = true;
    Message mess;
    mess.SetAsReply();
    result &= mess.IsReply();
    mess.SetAsReply(true);
    result &= mess.IsReply();
    mess.SetAsReply(false);
    result &= !mess.IsReply();
    return (result);
}

bool MessageTest::TestIsReply() {
    bool result;
    result = TestSetAsReply();
    return result;
}

bool MessageTest::TestSetExpectsReply() {
    using namespace MARTe;
    bool result = true;
    Message mess;
    result &= !mess.ExpectsReply();
    result &= !mess.ExpectsIndirectReply();
    mess.SetExpectsReply(false);
    result &= !mess.ExpectsReply();
    result &= !mess.ExpectsIndirectReply();
    mess.SetExpectsReply(true);
    result &= mess.ExpectsReply();
    result &= !mess.ExpectsIndirectReply();
    return result;
}

bool MessageTest::TestSetExpectsIndirectReply() {
    using namespace MARTe;
    bool result = true;
    Message mess;
    result &= !mess.ExpectsReply();
    result &= !mess.ExpectsIndirectReply();
    mess.SetExpectsIndirectReply(false);
    result &= !mess.ExpectsReply();
    result &= !mess.ExpectsIndirectReply();
    mess.SetExpectsIndirectReply(true);
    result &= !mess.ExpectsReply();
    result &= mess.ExpectsIndirectReply();
    mess.SetExpectsIndirectReply(false);
    result &= !mess.ExpectsReply();
    result &= !mess.ExpectsIndirectReply();
    return result;
}

bool MessageTest::TestExpectsReply() {
    using namespace MARTe;
    bool result = true;
    Message mess;
    result &= !mess.ExpectsReply();
    result &= !mess.ExpectsIndirectReply();
    mess.SetExpectsReply(false);
    result &= !mess.ExpectsReply();
    result &= !mess.ExpectsIndirectReply();
    mess.SetExpectsReply(true);
    result &= mess.ExpectsReply();
    result &= !mess.ExpectsIndirectReply();
    mess.SetExpectsReply(false);
    result &= !mess.ExpectsReply();
    result &= !mess.ExpectsIndirectReply();
    return result;
}

bool MessageTest::TestExpectsIndirectReply() {
    using namespace MARTe;
    bool result = true;
    Message mess;
    result &= !mess.ExpectsReply();
    result &= !mess.ExpectsIndirectReply();
    mess.SetExpectsIndirectReply(false);
    result &= !mess.ExpectsReply();
    result &= !mess.ExpectsIndirectReply();
    mess.SetExpectsIndirectReply(true);
    result &= !mess.ExpectsReply();
    result &= mess.ExpectsIndirectReply();
    mess.SetExpectsIndirectReply(false);
    result &= !mess.ExpectsReply();
    result &= !mess.ExpectsIndirectReply();
    return result;
}

bool MessageTest::TestGetDestination() {
    using namespace MARTe;
    bool result = true;
    ConfigurationDatabase cdb;
    result &= cdb.Write("Destination", "dummyDestination");
    result &= cdb.Write("Function", "dummyFunction");
    Message mess;
    result &= mess.Initialise(cdb);
    result &= (StringHelper::Compare(mess.GetDestination(), "dummyDestination") == 0);
    return result;
}

bool MessageTest::TestSetSender() {
    using namespace MARTe;
    bool result = true;
    Message mess;
    mess.SetSender("sender");
    result &= (StringHelper::Compare(mess.GetSender(), "sender") == 0);
    return result;
}

bool MessageTest::TestGetSender() {
    bool result;
    result = TestSetSender();
    return result;
}

bool MessageTest::TestGetFunction() {
    using namespace MARTe;
    bool result = true;
    ConfigurationDatabase cdb;
    result &= cdb.Write("Destination", "dummyDestination");
    result &= cdb.Write("Function", "dummyFunction");
    Message mess;
    result &= mess.Initialise(cdb);
    result &= (StringHelper::Compare(mess.GetFunction(), "dummyFunction") == 0);
    return result;
}

bool MessageTest::TestSetReplyTimeout() {
    using namespace MARTe;
    TimeoutType maxWaitIn = 1000;
    Message mess;
    mess.SetReplyTimeout(maxWaitIn);
    return (mess.GetReplyTimeout() == maxWaitIn);
}

