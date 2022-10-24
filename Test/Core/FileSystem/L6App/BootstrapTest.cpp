/**
 * @file BootstrapTest.cpp
 * @brief Source file for class BootstrapTest
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
 * the class BootstrapTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "BasicFile.h"
#include "Bootstrap.h"
#include "BootstrapTest.h"
#include "Directory.h"
#include "ConfigurationDatabase.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
bool BootstrapTest::TestConstructor() {
    using namespace MARTe;
    Bootstrap bp;
    return true;
}

bool BootstrapTest::TestReadParameters() {
    using namespace MARTe;
    uint32 argc = 15;
    const char8 *argv[] =
            { "Test", "-l", "LOADERCLASS", "-f", "FILENAME", "-p", "xml", "-s", "FIRST_STATE", "-m", "MSG_DESTINATION:MSG_FUNCTION", "-c", "0xff", "-k", "STOP_MSG_DESTINATION:STOP_MSG_FUNCTION"};
    Bootstrap bp;
    ConfigurationDatabase loaderParameters;
    bp.ReadParameters(argc, const_cast<char8 **>(argv), loaderParameters);
    StreamString param;
    bool ok = loaderParameters.Read("Loader", param);
    if (ok) {
        ok = (param == "LOADERCLASS");
        param = "";
    }
    if (ok) {
        ok = loaderParameters.Read("Filename", param);
    }
    if (ok) {
        ok = (param == "FILENAME");
        param = "";
    }
    if (ok) {
        ok = loaderParameters.Read("Parser", param);
    }
    if (ok) {
        ok = (param == "xml");
        param = "";
    }
    if (ok) {
        ok = loaderParameters.Read("FirstState", param);
    }
    if (ok) {
        ok = (param == "FIRST_STATE");
        param = "";
    }
    if (ok) {
        ok = loaderParameters.Read("MessageDestination", param);
    }
    if (ok) {
        ok = (param == "MSG_DESTINATION");
        param = "";
    }
    if (ok) {
        ok = loaderParameters.Read("MessageFunction", param);
    }
    if (ok) {
        ok = (param == "MSG_FUNCTION");
        param = "";
    }
    if (ok) {
        ok = loaderParameters.Read("StopMessageDestination", param);
    }
    if (ok) {
        ok = (param == "STOP_MSG_DESTINATION");
        param = "";
    }
    if (ok) {
        ok = loaderParameters.Read("StopMessageFunction", param);
    }
    if (ok) {
        ok = (param == "STOP_MSG_FUNCTION");
        param = "";
    }
    uint32 cpus = 0;
    if (ok) {
        ok = loaderParameters.Read("DefaultCPUs", cpus);
    }
    if (ok) {
        ok = (cpus == 0xff);
    }

    return ok;
}

bool BootstrapTest::TestReadParameters_FalseNoLoader() {
    using namespace MARTe;
    uint32 argc = 11;
    const char8 *argv[] = { "Test", "-f", "FILENAME", "-p", "xml", "-s", "FIRST_STATE", "-m", "MSG_DESTINATION:MSG_FUNCTION", "-c", "0xff" };
    Bootstrap bp;
    ConfigurationDatabase loaderParameters;
    bool ok = !bp.ReadParameters(argc, const_cast<char8 **>(argv), loaderParameters);
    return ok;
}

bool BootstrapTest::TestReadParameters_FalseNoFilename() {
    using namespace MARTe;
    uint32 argc = 11;
    const char8 *argv[] = { "Test", "-l", "LOADER", "-p", "xml", "-s", "FIRST_STATE", "-m", "MSG_DESTINATION:MSG_FUNCTION", "-c", "0xff" };
    Bootstrap bp;
    ConfigurationDatabase loaderParameters;
    bool ok = !bp.ReadParameters(argc, const_cast<char8 **>(argv), loaderParameters);
    return ok;
}

bool BootstrapTest::TestGetConfigurationStream() {
    using namespace MARTe;
    uint32 argc = 5;
    const char8 * const filename = "TestFile";
    const char8 *argv[] = { "Test", "-l", "LOADER", "-f", filename};
    BasicFile bf;
    bool ok = bf.Open(filename, BasicFile::ACCESS_MODE_W | BasicFile::FLAG_CREAT);
    if (ok) {
        uint32 size = 3u;
        bf.Write("A=1", size);
        bf.Close();
    }

    Bootstrap bp;
    ConfigurationDatabase loaderParameters;
    if (ok) {
        ok = bp.ReadParameters(argc, const_cast<char8 **>(argv), loaderParameters);
    }
    if (ok) {
        StreamI *stream = NULL;
        ok = bp.GetConfigurationStream(loaderParameters, stream);
        if (ok) {
            ok = (stream != NULL);
        }
    }

    Directory d(filename);
    d.Delete();
    return ok;
}

bool BootstrapTest::TestGetConfigurationStream_False() {
    using namespace MARTe;
    uint32 argc = 5;
    const char8 * const filename = "TestFileBad";
    const char8 *argv[] = { "Test", "-l", "LOADER", "-f", filename};

    Bootstrap bp;
    bp.CloseConfigurationStream(); 
    ConfigurationDatabase loaderParameters;
    bool ok = bp.ReadParameters(argc, const_cast<char8 **>(argv), loaderParameters);
    if (ok) {
        StreamI *stream = NULL;
        if (ok) {
            ErrorManagement::ErrorType err = bp.CloseConfigurationStream();
            ok = (err == ErrorManagement::NoError);
        }
        if (ok) {
            ok = !bp.GetConfigurationStream(loaderParameters, stream);
            if (ok) {
                ok = (stream == NULL);
            }
        }
    }

    Directory d(filename);
    d.Delete();
    return ok;
}

bool BootstrapTest::TestCloseConfigurationStream() {
    using namespace MARTe;
    uint32 argc = 5;
    const char8 * const filename = "TestFile";
    const char8 *argv[] = { "Test", "-l", "LOADER", "-f", filename};
    BasicFile bf;
    bool ok = bf.Open(filename, BasicFile::ACCESS_MODE_W | BasicFile::FLAG_CREAT);
    if (ok) {
        uint32 size = 3u;
        bf.Write("A=1", size);
        bf.Close();
    }

    Bootstrap bp;
    ConfigurationDatabase loaderParameters;
    if (ok) {
        ok = bp.ReadParameters(argc, const_cast<char8 **>(argv), loaderParameters);
    }
    if (ok) {
        StreamI *stream = NULL;
        ok = bp.GetConfigurationStream(loaderParameters, stream);
        if (ok) {
            ok = (stream != NULL);
        }
    }
    if (ok) {
        ErrorManagement::ErrorType err = bp.CloseConfigurationStream();
        ok = (err == ErrorManagement::NoError);
    }

    Directory d(filename);
    d.Delete();
    return ok;
}

bool BootstrapTest::TestCloseConfigurationStream_False() {
    using namespace MARTe;
    uint32 argc = 5;
    const char8 * const filename = "TestFileBad";
    const char8 *argv[] = { "Test", "-l", "LOADER", "-f", filename};

    Bootstrap bp;
    ConfigurationDatabase loaderParameters;
    bool ok = bp.ReadParameters(argc, const_cast<char8 **>(argv), loaderParameters);
    if (ok) {
        StreamI *stream = NULL;
        if (ok) {
            ErrorManagement::ErrorType err = bp.CloseConfigurationStream();
            ok = (err == ErrorManagement::NoError);
        }
        if (ok) {
            ok = !bp.GetConfigurationStream(loaderParameters, stream);
            if (ok) {
                ok = (stream == NULL);
            }
        }
        if (ok) {
            ErrorManagement::ErrorType err = bp.CloseConfigurationStream();
            ok = (err == ErrorManagement::NoError);
        }
    }

    Directory d(filename);
    d.Delete();
    return ok;
}

bool BootstrapTest::TestPrintf() {
    using namespace MARTe;
    Bootstrap bp;
    bp.Printf("A");
    return true;
}
