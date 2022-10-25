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
#include "ConfigurationLoaderHashCRC.h"
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
    ReferenceT<Loader> l = Reference("Loader", GlobalObjectsDatabase::Instance()->GetStandardHeap());
    return true;
}

bool LoaderTest::TestConfigure() {
    using namespace MARTe;
    ReferenceT<Loader> l = Reference("Loader", GlobalObjectsDatabase::Instance()->GetStandardHeap());
    StreamString config = "+A={"
            "   Class = ReferenceContainer"
            "}";
    ConfigurationDatabase params;
    params.Write("Parser", "cdb");
    bool ok = l->Configure(params, config);
    ok = (ProcessorType::GetDefaultCPUs() == 0x1u);
    ObjectRegistryDatabase::Instance()->Purge();
    return ok;
}

bool LoaderTest::TestConfigure_CPUs() {
    using namespace MARTe;
    ReferenceT<Loader> l = Reference("Loader", GlobalObjectsDatabase::Instance()->GetStandardHeap());
    StreamString config = "+A={"
            "   Class = ReferenceContainer"
            "}";
    ConfigurationDatabase params;
    params.Write("Parser", "cdb");
    uint32 defaultCPUs = 0xfu;
    params.Write("DefaultCPUs", defaultCPUs);
    bool ok = l->Configure(params, config);
    ok = (ProcessorType::GetDefaultCPUs() == defaultCPUs);
    ObjectRegistryDatabase::Instance()->Purge();
    return ok;
}

bool LoaderTest::TestConfigure_SchedulerGranularity() {
    using namespace MARTe;
    ReferenceT<Loader> l = Reference("Loader", GlobalObjectsDatabase::Instance()->GetStandardHeap());
    StreamString config = "+A={"
            "   Class = ReferenceContainer"
            "}";
    ConfigurationDatabase params;
    params.Write("Parser", "cdb");
    uint32 currentValue = Sleep::GetSchedulerGranularity();
    uint32 schedulerGranularity = 123456;
    params.Write("SchedulerGranularity", schedulerGranularity);
    bool ok = l->Configure(params, config);
    ok = (Sleep::GetSchedulerGranularity() == schedulerGranularity);
    Sleep::SetSchedulerGranularity(currentValue);
    ObjectRegistryDatabase::Instance()->Purge();
    return ok;
}


bool LoaderTest::TestConfigure_Json() {
    using namespace MARTe;
    ReferenceT<Loader> l = Reference("Loader", GlobalObjectsDatabase::Instance()->GetStandardHeap());
    StreamString config = "\"+A\" : {\n"
            "   \"Class\" : \"ReferenceContainer\"\n"
            "}\n";
    ConfigurationDatabase params;
    params.Write("Parser", "json");
    bool ok = l->Configure(params, config);
    ObjectRegistryDatabase::Instance()->Purge();
    return ok;
}

bool LoaderTest::TestConfigure_Xml() {
    using namespace MARTe;
    ReferenceT<Loader> l = Reference("Loader", GlobalObjectsDatabase::Instance()->GetStandardHeap());
    StreamString config = "<+A>"
            "   <Class>ReferenceContainer</Class>"
            "</+A>";
    ConfigurationDatabase params;
    params.Write("Parser", "xml");
    bool ok = l->Configure(params, config);
    ObjectRegistryDatabase::Instance()->Purge();
    return ok;
}

bool LoaderTest::TestConfigure_False_NoParser() {
    using namespace MARTe;
    ReferenceT<Loader> l = Reference("Loader", GlobalObjectsDatabase::Instance()->GetStandardHeap());
    StreamString config = "+A={"
            "   Class = ReferenceContainer"
            "}";
    ConfigurationDatabase params;
    bool ok = !l->Configure(params, config);
    ObjectRegistryDatabase::Instance()->Purge();
    return ok;
}

bool LoaderTest::TestConfigure_False_BadParser() {
    using namespace MARTe;
    ReferenceT<Loader> l = Reference("Loader", GlobalObjectsDatabase::Instance()->GetStandardHeap());
    StreamString config = "+A={"
            "   Class = ReferenceContainer"
            "}";
    ConfigurationDatabase params;
    params.Write("Parser", "zdb");
    bool ok = !l->Configure(params, config);
    ObjectRegistryDatabase::Instance()->Purge();
    return ok;
}

bool LoaderTest::TestConfigure_False_FailedParser() {
    using namespace MARTe;
    ReferenceT<Loader> l = Reference("Loader", GlobalObjectsDatabase::Instance()->GetStandardHeap());
    StreamString config = "+A={{"
            "   Class = ReferenceContainer"
            "}";
    ConfigurationDatabase params;
    params.Write("Parser", "cdb");
    bool ok = !l->Configure(params, config);
    ObjectRegistryDatabase::Instance()->Purge();
    return ok;
}

bool LoaderTest::TestConfigure_False_FailedConfiguration() {
    using namespace MARTe;
    ReferenceT<Loader> l = Reference("Loader", GlobalObjectsDatabase::Instance()->GetStandardHeap());
    StreamString config = "+A={"
            "   Class = ZReferenceContainer"
            "}";
    ConfigurationDatabase params;
    params.Write("Parser", "cdb");
    bool ok = !l->Configure(params, config);
    ObjectRegistryDatabase::Instance()->Purge();
    return ok;
}

bool LoaderTest::TestConfigure_False_FailedMessageFunction() {
    using namespace MARTe;
    ReferenceT<Loader> l = Reference("Loader", GlobalObjectsDatabase::Instance()->GetStandardHeap());
    StreamString config = "+A={"
            "   Class = ReferenceContainer"
            "}"
            "+B={"
            "   Class = LoaderTestMessageObject1"
            "}";
    ConfigurationDatabase params;
    params.Write("Parser", "cdb");
    params.Write("MessageDestination", "B");
    bool ok = !l->Configure(params, config);
    ObjectRegistryDatabase::Instance()->Purge();
    return ok;
}

bool LoaderTest::TestConfigure_BuildTokens() {
    using namespace MARTe;
    ReferenceT<Loader> l = Reference("Loader", GlobalObjectsDatabase::Instance()->GetStandardHeap());
    StreamString config = "+A={"
            "   Class = ReferenceContainer"
            "}";
    ConfigurationDatabase params;
    params.Write("Parser", "cdb");
    params.Write("BuildTokens", "_=");
    bool ok = l->Configure(params, config);
    ok = ReferenceContainer::IsBuildToken('_');
    ok &= ReferenceContainer::IsBuildToken('=');
    ok &= ReferenceContainer::IsBuildToken('+');
    ReferenceContainer::RemoveBuildToken('_');
    ReferenceContainer::RemoveBuildToken('=');
    ObjectRegistryDatabase::Instance()->Purge();
    return ok;
}

bool LoaderTest::TestConfigure_BuildTokens_False() {
    using namespace MARTe;
    ReferenceT<Loader> l = Reference("Loader", GlobalObjectsDatabase::Instance()->GetStandardHeap());
    StreamString config = "+A={"
            "   Class = ReferenceContainer"
            "}";
    ConfigurationDatabase params;
    params.Write("Parser", "cdb");
    params.Write("BuildTokens", "_=!@#$%^&*()");
    bool ok = !l->Configure(params, config);
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
    ObjectRegistryDatabase::Instance()->Purge();
    return ok;
}

bool LoaderTest::TestConfigure_DomainTokens() {
    using namespace MARTe;
    ReferenceT<Loader> l = Reference("Loader", GlobalObjectsDatabase::Instance()->GetStandardHeap());
    StreamString config = "+A={"
            "   Class = ReferenceContainer"
            "}";
    ConfigurationDatabase params;
    params.Write("Parser", "cdb");
    params.Write("DomainTokens", "_=");
    bool ok = l->Configure(params, config);
    ok = ReferenceContainer::IsDomainToken('_');
    ok &= ReferenceContainer::IsDomainToken('=');
    ok &= ReferenceContainer::IsDomainToken('$');
    ReferenceContainer::RemoveDomainToken('_');
    ReferenceContainer::RemoveDomainToken('=');
    ObjectRegistryDatabase::Instance()->Purge();
    return ok;
}

bool LoaderTest::TestConfigure_DomainTokens_False() {
    using namespace MARTe;
    ReferenceT<Loader> l = Reference("Loader", GlobalObjectsDatabase::Instance()->GetStandardHeap());
    StreamString config = "+A={"
            "   Class = ReferenceContainer"
            "}";
    ConfigurationDatabase params;
    params.Write("Parser", "cdb");
    params.Write("DomainTokens", "_=!@#$%^&*()");
    bool ok = !l->Configure(params, config);
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
    ObjectRegistryDatabase::Instance()->Purge();
    return ok;
}
bool LoaderTest::TestStart() {
    using namespace MARTe;
    ReferenceT<Loader> l = Reference("Loader", GlobalObjectsDatabase::Instance()->GetStandardHeap());
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
    bool ok = l->Configure(params, config);
    if (ok) {
        ok = l->Start();
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
    ReferenceT<Loader> l = Reference("Loader", GlobalObjectsDatabase::Instance()->GetStandardHeap());
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
    bool ok = l->Configure(params, config);
    if (ok) {
        ok = !l->Start();
    }

    ObjectRegistryDatabase::Instance()->Purge();
    return ok;
}

bool LoaderTest::TestStart_NoMessage() {
    using namespace MARTe;
    ReferenceT<Loader> l = Reference("Loader", GlobalObjectsDatabase::Instance()->GetStandardHeap());
    StreamString config = "+A={"
            "   Class = ReferenceContainer"
            "}"
            "+B={"
            "   Class = LoaderTestMessageObject1"
            "}";
    ConfigurationDatabase params;
    params.Write("Parser", "cdb");
    bool ok = l->Configure(params, config);
    if (ok) {
        ok = l->Start();
    }
    ObjectRegistryDatabase::Instance()->Purge();
    return ok;
}

bool LoaderTest::TestStop() {
    using namespace MARTe;
    ReferenceT<Loader> l = Reference("Loader", GlobalObjectsDatabase::Instance()->GetStandardHeap());
    StreamString config = "+A={"
            "   Class = ReferenceContainer"
            "}"
            "+B={"
            "   Class = LoaderTestMessageObject1"
            "}";
    ConfigurationDatabase params;
    params.Write("Parser", "cdb");
    bool ok = l->Configure(params, config);
    if (ok) {
        ok = l->Stop();
    }
    ObjectRegistryDatabase::Instance()->Purge();
    return ok;
}

bool LoaderTest::TestReconfigure() {
    using namespace MARTe;
    ReferenceT<Loader> l = Reference("Loader", GlobalObjectsDatabase::Instance()->GetStandardHeap());
    StreamString config = "+A={"
            "   Class = ReferenceContainer"
            "}"
            "+B={"
            "   Class = LoaderTestMessageObject1"
            "}";
    ConfigurationDatabase params;
    params.Write("Parser", "cdb");
    bool ok = l->Configure(params, config);
    if (ok) {
        Reference ref = ObjectRegistryDatabase::Instance()->Find("A");
        ok = ref.IsValid();
    }
    if (ok) {
        Reference ref = ObjectRegistryDatabase::Instance()->Find("C");
        ok = !ref.IsValid();
    }
    config = "+C={"
            "   Class = ReferenceContainer"
            "}"
            "+D={"
            "   Class = LoaderTestMessageObject1"
            "}";
    (void)config.Seek(0LLU);
    if  (ok) {
        StreamString ignored;
        ok = l->Reconfigure(config, ignored);
    }
    if (ok) {
        Reference ref = ObjectRegistryDatabase::Instance()->Find("A");
        ok = !ref.IsValid();
    }
    if (ok) {
        Reference ref = ObjectRegistryDatabase::Instance()->Find("C");
        ok = ref.IsValid();
    }
    if (ok) {
        ok = l->Stop();
    }

    ObjectRegistryDatabase::Instance()->Purge();
    return ok;
}

bool LoaderTest::TestReconfigure_StructuredDataI() {
    using namespace MARTe;
    ReferenceT<Loader> l = Reference("Loader", GlobalObjectsDatabase::Instance()->GetStandardHeap());
    StreamString config = "+A={"
            "   Class = ReferenceContainer"
            "}"
            "+B={"
            "   Class = LoaderTestMessageObject1"
            "}";
    ConfigurationDatabase params;
    params.Write("Parser", "cdb");
    bool ok = l->Configure(params, config);
    if (ok) {
        Reference ref = ObjectRegistryDatabase::Instance()->Find("A");
        ok = ref.IsValid();
    }
    if (ok) {
        Reference ref = ObjectRegistryDatabase::Instance()->Find("C");
        ok = !ref.IsValid();
    }
    ConfigurationDatabase cdb;
    cdb.CreateAbsolute("+C");
    cdb.MoveAbsolute("+C");
    cdb.Write("Class", "ReferenceContainer");
    cdb.CreateAbsolute("+D");
    cdb.Write("Class", "ReferenceContainer");
    cdb.MoveAbsolute("+D");
    if  (ok) {
        StreamString ignored;
        ok = l->Reconfigure(cdb, ignored);
    }
    if (ok) {
        Reference ref = ObjectRegistryDatabase::Instance()->Find("A");
        ok = !ref.IsValid();
    }
    if (ok) {
        Reference ref = ObjectRegistryDatabase::Instance()->Find("C");
        ok = ref.IsValid();
    }
    if (ok) {
        ok = l->Stop();
    }

    ObjectRegistryDatabase::Instance()->Purge();
    return ok;
}

bool LoaderTest::TestReconfigure_Hash() {
    using namespace MARTe;
    ReferenceT<Loader> l = Reference("Loader", GlobalObjectsDatabase::Instance()->GetStandardHeap());
    StreamString config = ""
            "+LoaderPostInit={"
            "   Class = ReferenceContainer"
            "   +Hash = {"
            "       Class = ConfigurationLoaderHashCRC"
            "       Key = 0x1"
            "   }"
            "}"
            "+H={"
            "   Class = ConfigurationLoaderHashCRC"
            "   Key = 0x1"
            "}";
    ConfigurationDatabase params;
    params.Write("Parser", "cdb");
    bool ok = l->Configure(params, config);
    config = "+C={"
            "   Class = ReferenceContainer"
            "}"
            "+D={"
            "   Class = LoaderTestMessageObject1"
            "}";

    ReferenceT<ConfigurationLoaderHashCRC> hasher = ObjectRegistryDatabase::Instance()->Find("H");
    if (ok) {
        ok = hasher.IsValid();
    }
    uint32 expectedHash = 0u;
    if (ok) {
        expectedHash = hasher->ComputeHash(config.Buffer(), config.Size());
    }
    if (ok) {
        StreamString ignored;
        ok = l->Reconfigure(config, ignored, expectedHash);
    }
    if (ok) {
        Reference ref = ObjectRegistryDatabase::Instance()->Find("H");
        ok = !ref.IsValid();
    }
    if (ok) {
        Reference ref = ObjectRegistryDatabase::Instance()->Find("C");
        ok = ref.IsValid();
    }
    if (ok) {
        ok = l->Stop();
    }

    ObjectRegistryDatabase::Instance()->Purge();
    return ok;
}


bool LoaderTest::TestGetLastValidConfiguration() {
    using namespace MARTe;
    ReferenceT<Loader> l = Reference("Loader", GlobalObjectsDatabase::Instance()->GetStandardHeap());
    StreamString config = "+A={"
            "   Class = ReferenceContainer"
            "}"
            "+B={"
            "   Class = LoaderTestMessageObject1"
            "}";
    ConfigurationDatabase params;
    params.Write("Parser", "cdb");
    bool ok = l->Configure(params, config);
    ConfigurationDatabase output1;
    if (ok) {
        ok = l->GetLastValidConfiguration(output1);
    }
    if (ok) {
        ok = output1.MoveAbsolute("+A");
    }
    ObjectRegistryDatabase::Instance()->Purge();
    config = "+C={"
            "   Class = ReferenceContainer"
            "}"
            "+D={"
            "   Class = LoaderTestMessageObject1"
            "}";
    (void)config.Seek(0LLU);
    if  (ok) {
        StreamString ignored;
        ok = l->Reconfigure(config, ignored);
    }
    ConfigurationDatabase output2;
    if (ok) {
        ok = l->GetLastValidConfiguration(output2);
    }
    if (ok) {
        ok = !output2.MoveAbsolute("+A");
    }
    if (ok) {
        ok = output2.MoveAbsolute("+D");
    }

    if (ok) {
        ok = l->Stop();
    }

    ObjectRegistryDatabase::Instance()->Purge();
    return ok;
}

bool LoaderTest::TestGetLastValidConfiguration_AfterFailure() {
    using namespace MARTe;
    ReferenceT<Loader> l = Reference("Loader", GlobalObjectsDatabase::Instance()->GetStandardHeap());
    StreamString config = "+A={"
            "   Class = ReferenceContainer"
            "}"
            "+B={"
            "   Class = LoaderTestMessageObject1"
            "}";
    ConfigurationDatabase params;
    params.Write("Parser", "cdb");
    bool ok = l->Configure(params, config);
    ConfigurationDatabase output1;
    if (ok) {
        ok = l->GetLastValidConfiguration(output1);
    }
    if (ok) {
        ok = output1.MoveAbsolute("+A");
    }
    ObjectRegistryDatabase::Instance()->Purge();
    config = "+C={"
            "   Class = ReferenceContainerzz"
            "}"
            "+D={"
            "   Class = LoaderTestMessageObject1"
            "}";
    (void)config.Seek(0LLU);
    if  (ok) {
        StreamString ignored;
        ok = !l->Reconfigure(config, ignored);
    }
    ConfigurationDatabase output2;
    if (ok) {
        ok = l->GetLastValidConfiguration(output2);
    }
    if (ok) {
        ok = output2.MoveAbsolute("+A");
    }
    if (ok) {
        ok = !output2.MoveAbsolute("+D");
    }

    if (ok) {
        ok = l->Stop();
    }

    ObjectRegistryDatabase::Instance()->Purge();
    return ok;

}

bool LoaderTest::TestReloadLastValidConfiguration() {
    using namespace MARTe;
    ReferenceT<Loader> l = Reference("Loader", GlobalObjectsDatabase::Instance()->GetStandardHeap());
    StreamString config = "+A={"
            "   Class = ReferenceContainer"
            "}"
            "+B={"
            "   Class = LoaderTestMessageObject1"
            "}";
    ConfigurationDatabase params;
    params.Write("Parser", "cdb");
    bool ok = l->Configure(params, config);
    ConfigurationDatabase output1;
    if (ok) {
        ok = l->GetLastValidConfiguration(output1);
    }
    if (ok) {
        ok = output1.MoveAbsolute("+A");
    }
    config = "+C={"
            "   Class = ReferenceContainerzz"
            "}"
            "+D={"
            "   Class = LoaderTestMessageObject1"
            "}";
    (void)config.Seek(0LLU);
    if  (ok) {
        StreamString ignored;
        ok = !l->Reconfigure(config, ignored);
    }
    if (ok) {
        Reference ref = ObjectRegistryDatabase::Instance()->Find("A");
        ok = !ref.IsValid();
    }
    if (ok) {
        ok = l->ReloadLastValidConfiguration();
    }
    if (ok) {
        Reference ref = ObjectRegistryDatabase::Instance()->Find("A");
        ok = ref.IsValid();
    }
    if (ok) {
        ok = l->Stop();
    }

    ObjectRegistryDatabase::Instance()->Purge();
    return ok;
}

bool LoaderTest::TestLoaderPostInit() {
    using namespace MARTe;
    ReferenceT<Loader> l = Reference("Loader", GlobalObjectsDatabase::Instance()->GetStandardHeap());
    StreamString config = 
        "+LoaderPostInit={"
        "   Class = ReferenceContainer"
        "}"
        "+A={"
        "   Class = LoaderTestMessageObject1"
        "}";
    ConfigurationDatabase params;
    params.Write("Parser", "cdb");
    bool ok = l->Configure(params, config);
    ObjectRegistryDatabase::Instance()->Purge();
    return ok; 
}

bool LoaderTest::TestLoaderPostInit_ReloadLast_true() {
    using namespace MARTe;
    ReferenceT<Loader> l = Reference("Loader", GlobalObjectsDatabase::Instance()->GetStandardHeap());
    StreamString config = 
        "+LoaderPostInit={"
        "   Class = ReferenceContainer"
        "   +Parameters = {"
        "       Class = ConfigurationDatabase"
        "       ReloadLast = \"true\""
        "   }"
        "}"
        "+A={"
        "   Class = LoaderTestMessageObject1"
        "}";
    ConfigurationDatabase params;
    params.Write("Parser", "cdb");
    bool ok = l->Configure(params, config);
    config = "+C={"
            "   Class = ReferenceContainerzz"
            "}"
            "+D={"
            "   Class = LoaderTestMessageObject1"
            "}";
    (void)config.Seek(0LLU);
    if  (ok) {
        StreamString ignored;
        ok = !l->Reconfigure(config, ignored);
    }
    if (ok) {
        Reference ref = ObjectRegistryDatabase::Instance()->Find("A");
        ok = ref.IsValid();
    }
    ObjectRegistryDatabase::Instance()->Purge();
    return ok; 
}

bool LoaderTest::TestLoaderPostInit_ReloadLast_false() {
    using namespace MARTe;
    ReferenceT<Loader> l = Reference("Loader", GlobalObjectsDatabase::Instance()->GetStandardHeap());
    StreamString config = 
        "+LoaderPostInit={"
        "   Class = ReferenceContainer"
        "   +Parameters = {"
        "       Class = ConfigurationDatabase"
        "       ReloadLast = \"false\""
        "   }"
        "}"
        "+A={"
        "   Class = LoaderTestMessageObject1"
        "}";
    ConfigurationDatabase params;
    params.Write("Parser", "cdb");
    bool ok = l->Configure(params, config);
    config = "+C={"
            "   Class = ReferenceContainerzz"
            "}"
            "+D={"
            "   Class = LoaderTestMessageObject1"
            "}";
    (void)config.Seek(0LLU);
    if  (ok) {
        StreamString ignored;
        ok = !l->Reconfigure(config, ignored);
    }
    if (ok) {
        Reference ref = ObjectRegistryDatabase::Instance()->Find("A");
        ok = !ref.IsValid();
    }
    ObjectRegistryDatabase::Instance()->Purge();
    return ok; 
}

bool LoaderTest::TestLoaderPostInit_ReloadLast_invalid() {
    using namespace MARTe;
    ReferenceT<Loader> l = Reference("Loader", GlobalObjectsDatabase::Instance()->GetStandardHeap());
    StreamString config = 
        "+LoaderPostInit={"
        "   Class = ReferenceContainer"
        "   +Parameters = {"
        "       Class = ConfigurationDatabase"
        "       ReloadLast = \"invalid\""
        "   }"
        "}"
        "+A={"
        "   Class = LoaderTestMessageObject1"
        "}";
    ConfigurationDatabase params;
    params.Write("Parser", "cdb");
    bool ok = l->Configure(params, config);
    ObjectRegistryDatabase::Instance()->Purge();
    return !ok; 
}

bool LoaderTest::TestMessage_PreConfiguration() {
    using namespace MARTe;
    ReferenceT<Loader> l = Reference("Loader", GlobalObjectsDatabase::Instance()->GetStandardHeap());
    StreamString config = 
        "+LoaderPostInit={"
        "   Class = ReferenceContainer"
        "   +Messages = {"
        "       Class = ReferenceContainer"
        "       +PreConfiguration = {"
        "           Class = Message"
        "           Destination = TestObj"
        "           Function = Callback"
        "           Mode = ExpectsReply"
        "       }"
        "   }"
        "}"
        "+TestObj = {"
        "   Class = LoaderTestMessageObject1"
        "}";
    ConfigurationDatabase params;
    params.Write("Parser", "cdb");
    bool ok = l->Configure(params, config);
    ReferenceT<LoaderTestMessageObject1> testObj = ObjectRegistryDatabase::Instance()->Find("TestObj");
    if (ok) {
        ok = testObj.IsValid();
    }

    config = "+C={"
            "   Class = ReferenceContainer"
            "}"
            "+D={"
            "   Class = LoaderTestMessageObject1"
            "}";
    (void)config.Seek(0LLU);
    if  (ok) {
        StreamString ignored;
        ok = l->Reconfigure(config, ignored);
    }
    if (ok) {
        ok = testObj->functionCalled;
    }
    ObjectRegistryDatabase::Instance()->Purge();
    return ok; 
}

bool LoaderTest::TestMessage_PostConfiguration() {
    using namespace MARTe;
    ReferenceT<Loader> l = Reference("Loader", GlobalObjectsDatabase::Instance()->GetStandardHeap());
    StreamString config = 
        "+LoaderPostInit={"
        "   Class = ReferenceContainer"
        "   +Messages = {"
        "       Class = ReferenceContainer"
        "       +PostConfiguration = {"
        "           Class = Message"
        "           Destination = TestObj"
        "           Function = Callback"
        "           Mode = ExpectsReply"
        "       }"
        "   }"
        "}"
        "+TestObj = {"
        "   Class = LoaderTestMessageObject1"
        "}";
    ConfigurationDatabase params;
    params.Write("Parser", "cdb");
    bool ok = l->Configure(params, config);

    config = "+C={"
            "   Class = ReferenceContainer"
            "}"
            "+D={"
            "   Class = LoaderTestMessageObject1"
            "}"
            "+TestObj = {"
            "   Class = LoaderTestMessageObject1"
            "}";
    (void)config.Seek(0LLU);
    if  (ok) {
        StreamString ignored;
        ok = l->Reconfigure(config, ignored);
    }
    ReferenceT<LoaderTestMessageObject1> testObj = ObjectRegistryDatabase::Instance()->Find("TestObj");
    if (ok) {
        ok = testObj.IsValid();
    }
    if (ok) {
        ok = testObj->functionCalled;
    }
    ObjectRegistryDatabase::Instance()->Purge();
    return ok; 
}

bool LoaderTest::TestLoaderPostInit_KeepAlive() {
    using namespace MARTe;
    ReferenceT<Loader> l = Reference("Loader", GlobalObjectsDatabase::Instance()->GetStandardHeap());
    StreamString config = 
        "+LoaderPostInit={"
        "   Class = ReferenceContainer"
        "   +Parameters = {"
        "       Class = ConfigurationDatabase"
        "       ReloadLast = \"false\""
        "       KeepAlive = {B C}"
        "   }"
        "}"
        "+A={"
        "   Class = LoaderTestMessageObject1"
        "}"
        "+B={"
        "   Class = LoaderTestMessageObject1"
        "}"
        "+C={"
        "   Class = LoaderTestMessageObject1"
        "}";
    ConfigurationDatabase params;
    params.Write("Parser", "cdb");
    bool ok = l->Configure(params, config);
    config = "+C={"
            "   Class = ReferenceContainerzz"
            "}"
            "+D={"
            "   Class = LoaderTestMessageObject1"
            "}";
    (void)config.Seek(0LLU);
    if  (ok) {
        StreamString ignored;
        ok = !l->Reconfigure(config, ignored);
    }
    if (ok) {
        Reference ref = ObjectRegistryDatabase::Instance()->Find("A");
        ok = !ref.IsValid();
    }
    if (ok) {
        Reference ref = ObjectRegistryDatabase::Instance()->Find("B");
        ok = ref.IsValid();
    }
    if (ok) {
        Reference ref = ObjectRegistryDatabase::Instance()->Find("C");
        ok = ref.IsValid();
    }

    ObjectRegistryDatabase::Instance()->Purge();
    return ok; 
}

bool LoaderTest::TestMessage_FailedConfiguration() {
    using namespace MARTe;
    ReferenceT<Loader> l = Reference("Loader", GlobalObjectsDatabase::Instance()->GetStandardHeap());
    StreamString config = 
        "+LoaderPostInit={"
        "   Class = ReferenceContainer"
        "   +Parameters = {"
        "       Class = ConfigurationDatabase"
        "       ReloadLast = \"false\""
        "       KeepAlive = {TestObj}"
        "   }"
        "   +Messages = {"
        "       Class = ReferenceContainer"
        "       +FailedConfiguration = {"
        "           Class = Message"
        "           Destination = TestObj"
        "           Function = Callback"
        "           Mode = ExpectsReply"
        "       }"
        "   }"
        "}"
        "+TestObj = {"
        "   Class = LoaderTestMessageObject1"
        "}";
    ConfigurationDatabase params;
    params.Write("Parser", "cdb");
    bool ok = l->Configure(params, config);
    ReferenceT<LoaderTestMessageObject1> testObj = ObjectRegistryDatabase::Instance()->Find("TestObj");
    if (ok) {
        ok = testObj.IsValid();
    }
    config = "+C={"
            "   Class = ReferenceContainerzz"
            "}"
            "+D={"
            "   Class = LoaderTestMessageObject1"
            "}";
    (void)config.Seek(0LLU);
    if  (ok) {
        StreamString ignored;
        ok = !l->Reconfigure(config, ignored);
    }
    if (ok) {
        ok = testObj->functionCalled;
    }

    ObjectRegistryDatabase::Instance()->Purge();
    return ok; 
}

bool LoaderTest::TestMessage_ReloadedConfiguration() {
    using namespace MARTe;
    ReferenceT<Loader> l = Reference("Loader", GlobalObjectsDatabase::Instance()->GetStandardHeap());
    StreamString config = 
        "+LoaderPostInit={"
        "   Class = ReferenceContainer"
        "   +Parameters = {"
        "       Class = ConfigurationDatabase"
        "       ReloadLast = \"true\""
        "       KeepAlive = {TestObj}"
        "   }"
        "   +Messages = {"
        "       Class = ReferenceContainer"
        "       +ReloadedConfiguration = {"
        "           Class = Message"
        "           Destination = TestObj"
        "           Function = Callback"
        "           Mode = ExpectsReply"
        "       }"
        "   }"
        "}"
        "+TestObj = {"
        "   Class = LoaderTestMessageObject1"
        "}";
    ConfigurationDatabase params;
    params.Write("Parser", "cdb");
    bool ok = l->Configure(params, config);
    ReferenceT<LoaderTestMessageObject1> testObj = ObjectRegistryDatabase::Instance()->Find("TestObj");
    if (ok) {
        ok = testObj.IsValid();
    }
    config = "+C={"
            "   Class = ReferenceContainerzz"
            "}"
            "+D={"
            "   Class = LoaderTestMessageObject1"
            "}";
    (void)config.Seek(0LLU);
    if  (ok) {
        StreamString ignored;
        ok = !l->Reconfigure(config, ignored);
    }
    if (ok) {
        ok = testObj->functionCalled;
    }

    ObjectRegistryDatabase::Instance()->Purge();
    return ok; 
}

bool LoaderTest::TestGetSeed() {
    using namespace MARTe;
    ReferenceT<Loader> l = Reference("Loader", GlobalObjectsDatabase::Instance()->GetStandardHeap());
    bool ok = (l->GetSeed() == 0);
    if (ok) {
        ok = TestReconfigure_Hash();
    }
    ObjectRegistryDatabase::Instance()->Purge();
    return ok;
}


