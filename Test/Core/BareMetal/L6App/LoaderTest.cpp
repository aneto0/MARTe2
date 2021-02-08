/**
 * @file LoaderTest.cpp
 * @brief Source file for class LoaderTest
 * @date 05/04/2018
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
 * the class LoaderTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "CLASSMETHODREGISTER.h"
#include "Loader.h"
#include "LoaderTest.h"
#include "MessageI.h"
#include "ObjectRegistryDatabase.h"
#include "RegisteredMethodsMessageFilter.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
class LoaderTestMessageObject1: public MARTe::Object, public MARTe::MessageI {
public:
    CLASS_REGISTER_DECLARATION()LoaderTestMessageObject1() {
        using namespace MARTe;
        ReferenceT<RegisteredMethodsMessageFilter> registeredMethodsMessageFilter(GlobalObjectsDatabase::Instance()->GetStandardHeap());
        registeredMethodsMessageFilter->SetDestination(this);

        if (registeredMethodsMessageFilter.IsValid()) {
            InstallMessageFilter(registeredMethodsMessageFilter);
        }
        functionCalled = false;
    }

    MARTe::ErrorManagement::ErrorType Callback() {
        functionCalled = true;
        return MARTe::ErrorManagement::NoError;
    }

    bool functionCalled;
};
CLASS_REGISTER(LoaderTestMessageObject1, "1.0")
CLASS_METHOD_REGISTER(LoaderTestMessageObject1, Callback)

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
bool LoaderTest::TestConstructor() {
    using namespace MARTe;
    Loader l;
    return true;
}

bool LoaderTest::TestConfigure() {
    using namespace MARTe;
    Loader l;
    StreamString config = "+A={"
            "   Class = ReferenceContainer"
            "}";
    ConfigurationDatabase params;
    params.Write("Parser", "cdb");
    bool ok = l.Configure(params, config);
    ok = (ProcessorType::GetDefaultCPUs() == 0x1u);
    return ok;
}

bool LoaderTest::TestConfigure_CPUs() {
    using namespace MARTe;
    Loader l;
    StreamString config = "+A={"
            "   Class = ReferenceContainer"
            "}";
    ConfigurationDatabase params;
    params.Write("Parser", "cdb");
    uint32 defaultCPUs = 0xfu;
    params.Write("DefaultCPUs", defaultCPUs);
    bool ok = l.Configure(params, config);
    ok = (ProcessorType::GetDefaultCPUs() == defaultCPUs);
    return ok;
}

bool LoaderTest::TestConfigure_SchedulerGranularity() {
    using namespace MARTe;
    Loader l;
    StreamString config = "+A={"
            "   Class = ReferenceContainer"
            "}";
    ConfigurationDatabase params;
    params.Write("Parser", "cdb");
    uint32 currentValue = Sleep::GetSchedulerGranularity();
    uint32 schedulerGranularity = 123456;
    params.Write("SchedulerGranularity", schedulerGranularity);
    bool ok = l.Configure(params, config);
    ok = (Sleep::GetSchedulerGranularity() == schedulerGranularity);
    Sleep::SetSchedulerGranularity(currentValue);
    return ok;
}


bool LoaderTest::TestConfigure_Json() {
    using namespace MARTe;
    Loader l;
    StreamString config = "\"+A\" : {\n"
            "   \"Class\" : \"ReferenceContainer\"\n"
            "}\n";
    ConfigurationDatabase params;
    params.Write("Parser", "json");
    bool ok = l.Configure(params, config);
    return ok;
}

bool LoaderTest::TestConfigure_Xml() {
    using namespace MARTe;
    Loader l;
    StreamString config = "<+A>"
            "   <Class>ReferenceContainer</Class>"
            "</+A>";
    ConfigurationDatabase params;
    params.Write("Parser", "xml");
    bool ok = l.Configure(params, config);
    return ok;
}

bool LoaderTest::TestConfigure_False_NoParser() {
    using namespace MARTe;
    Loader l;
    StreamString config = "+A={"
            "   Class = ReferenceContainer"
            "}";
    ConfigurationDatabase params;
    bool ok = !l.Configure(params, config);
    return ok;
}

bool LoaderTest::TestConfigure_False_BadParser() {
    using namespace MARTe;
    Loader l;
    StreamString config = "+A={"
            "   Class = ReferenceContainer"
            "}";
    ConfigurationDatabase params;
    params.Write("Parser", "zdb");
    bool ok = !l.Configure(params, config);
    return ok;
}

bool LoaderTest::TestConfigure_False_FailedParser() {
    using namespace MARTe;
    Loader l;
    StreamString config = "+A={{"
            "   Class = ReferenceContainer"
            "}";
    ConfigurationDatabase params;
    params.Write("Parser", "cdb");
    bool ok = !l.Configure(params, config);
    return ok;
}

bool LoaderTest::TestConfigure_False_FailedConfiguration() {
    using namespace MARTe;
    Loader l;
    StreamString config = "+A={"
            "   Class = ZReferenceContainer"
            "}";
    ConfigurationDatabase params;
    params.Write("Parser", "cdb");
    bool ok = !l.Configure(params, config);
    return ok;
}

bool LoaderTest::TestConfigure_False_FailedMessageFunction() {
    using namespace MARTe;
    Loader l;
    StreamString config = "+A={"
            "   Class = ReferenceContainer"
            "}"
            "+B={"
            "   Class = LoaderTestMessageObject1"
            "}";
    ConfigurationDatabase params;
    params.Write("Parser", "cdb");
    params.Write("MessageDestination", "B");
    bool ok = !l.Configure(params, config);
    ObjectRegistryDatabase::Instance()->Purge();
    return ok;
}

bool LoaderTest::TestConfigure_BuildTokens() {
    using namespace MARTe;
    Loader l;
    StreamString config = "+A={"
            "   Class = ReferenceContainer"
            "}";
    ConfigurationDatabase params;
    params.Write("Parser", "cdb");
    params.Write("BuildTokens", "_=");
    bool ok = l.Configure(params, config);
    ok = ReferenceContainer::IsBuildToken('_');
    ok &= ReferenceContainer::IsBuildToken('=');
    ok &= ReferenceContainer::IsBuildToken('+');
    ReferenceContainer::RemoveBuildToken('_');
    ReferenceContainer::RemoveBuildToken('=');
    return ok;
}

bool LoaderTest::TestConfigure_BuildTokens_False() {
    using namespace MARTe;
    Loader l;
    StreamString config = "+A={"
            "   Class = ReferenceContainer"
            "}";
    ConfigurationDatabase params;
    params.Write("Parser", "cdb");
    params.Write("BuildTokens", "_=!@#$%^&*()");
    bool ok = !l.Configure(params, config);
    ReferenceContainer::RemoveBuildToken('_');
    ReferenceContainer::RemoveBuildToken('=');
    ReferenceContainer::RemoveBuildToken('!');
    ReferenceContainer::RemoveBuildToken('@');
    ReferenceContainer::RemoveBuildToken('#');
    ReferenceContainer::RemoveBuildToken('$');
    ReferenceContainer::RemoveBuildToken('%');
    ReferenceContainer::RemoveBuildToken('^');
    ReferenceContainer::RemoveBuildToken('&');
    ReferenceContainer::RemoveBuildToken('*');
    ReferenceContainer::RemoveBuildToken('(');
    ReferenceContainer::RemoveBuildToken(')');
    return ok;
}

bool LoaderTest::TestConfigure_DomainTokens() {
    using namespace MARTe;
    Loader l;
    StreamString config = "+A={"
            "   Class = ReferenceContainer"
            "}";
    ConfigurationDatabase params;
    params.Write("Parser", "cdb");
    params.Write("DomainTokens", "_=");
    bool ok = l.Configure(params, config);
    ok = ReferenceContainer::IsDomainToken('_');
    ok &= ReferenceContainer::IsDomainToken('=');
    ok &= ReferenceContainer::IsDomainToken('$');
    ReferenceContainer::RemoveDomainToken('_');
    ReferenceContainer::RemoveDomainToken('=');
    return ok;
}

bool LoaderTest::TestConfigure_DomainTokens_False() {
    using namespace MARTe;
    Loader l;
    StreamString config = "+A={"
            "   Class = ReferenceContainer"
            "}";
    ConfigurationDatabase params;
    params.Write("Parser", "cdb");
    params.Write("DomainTokens", "_=!@#$%^&*()");
    bool ok = !l.Configure(params, config);
    ReferenceContainer::RemoveDomainToken('_');
    ReferenceContainer::RemoveDomainToken('=');
    ReferenceContainer::RemoveDomainToken('!');
    ReferenceContainer::RemoveDomainToken('@');
    ReferenceContainer::RemoveDomainToken('#');
    ReferenceContainer::RemoveDomainToken('%');
    ReferenceContainer::RemoveDomainToken('^');
    ReferenceContainer::RemoveDomainToken('&');
    ReferenceContainer::RemoveDomainToken('*');
    ReferenceContainer::RemoveDomainToken('(');
    ReferenceContainer::RemoveDomainToken(')');
    return ok;
}
bool LoaderTest::TestStart() {
    using namespace MARTe;
    Loader l;
    StreamString config = "+A={"
            "   Class = ReferenceContainer"
            "}"
            "+B={"
            "   Class = LoaderTestMessageObject1"
            "}";
    ConfigurationDatabase params;
    params.Write("Parser", "cdb");
    params.Write("MessageDestination", "B");
    params.Write("MessageFunction", "Callback");
    bool ok = l.Configure(params, config);
    if (ok) {
        ok = l.Start();
    }
    ReferenceT<LoaderTestMessageObject1> obj;
    if (ok) {
        obj = ObjectRegistryDatabase::Instance()->Find("B");
        ok = obj.IsValid();
    }
    if (ok) {
        ok = obj->functionCalled;
    }

    ObjectRegistryDatabase::Instance()->Purge();
    return ok;
}

bool LoaderTest::TestStart_WrongMessage() {
    using namespace MARTe;
    Loader l;
    StreamString config = "+A={"
            "   Class = ReferenceContainer"
            "}"
            "+B={"
            "   Class = LoaderTestMessageObject1"
            "}";
    ConfigurationDatabase params;
    params.Write("Parser", "cdb");
    params.Write("MessageDestination", "C");
    params.Write("MessageFunction", "Callback");
    bool ok = l.Configure(params, config);
    if (ok) {
        ok = !l.Start();
    }

    ObjectRegistryDatabase::Instance()->Purge();
    return ok;
}

bool LoaderTest::TestStart_NoMessage() {
    using namespace MARTe;
    Loader l;
    StreamString config = "+A={"
            "   Class = ReferenceContainer"
            "}"
            "+B={"
            "   Class = LoaderTestMessageObject1"
            "}";
    ConfigurationDatabase params;
    params.Write("Parser", "cdb");
    bool ok = l.Configure(params, config);
    if (ok) {
        ok = l.Start();
    }
    ObjectRegistryDatabase::Instance()->Purge();
    return ok;
}

bool LoaderTest::TestStop() {
    using namespace MARTe;
    using namespace MARTe;
    Loader l;
    StreamString config = "+A={"
            "   Class = ReferenceContainer"
            "}"
            "+B={"
            "   Class = LoaderTestMessageObject1"
            "}";
    ConfigurationDatabase params;
    params.Write("Parser", "cdb");
    bool ok = l.Configure(params, config);
    if (ok) {
        ok = l.Stop();
    }
    ObjectRegistryDatabase::Instance()->Purge();
    return ok;
}
