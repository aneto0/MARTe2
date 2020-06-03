/**
 * @file HttpProtocolTest.cpp
 * @brief Source file for class HttpProtocolTest
 * @date 17/09/2018
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

 * @details This source file contains the definition of all the methods for
 * the class HttpProtocolTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include <stdio.h>
/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "HttpProtocolTest.h"
#include "StringHelper.h"
#include "TCPSocket.h"
#include "Threads.h"
#include "GlobalObjectsDatabase.h"
#include "HttpDefinition.h"
#include "StreamStructuredData.h"
#include "JsonPrinter.h"
#include "HttpRealmI.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

class HttpStreamTestRealm: public Object, public HttpRealmI {
public:
    CLASS_REGISTER_DECLARATION();

    HttpStreamTestRealm();
    virtual ~HttpStreamTestRealm();

    virtual bool Validate(const char8 * key,
            int32 command,
            uint32 ipNumber);

    virtual bool DigestSecurityNeeded();

    virtual bool GetAuthenticationRequest(StreamString &message);

    void SetPassw(const char8* passwIn);

private:
    StreamString passw;
};

HttpStreamTestRealm::HttpStreamTestRealm() {

}

HttpStreamTestRealm::~HttpStreamTestRealm() {

}

void HttpStreamTestRealm::SetPassw(const char8* passwIn) {
    passw = passwIn;
}

bool HttpStreamTestRealm::Validate(const char8 * key, int32 command, uint32 ipNumber) {
    return (passw == key);
}

bool HttpStreamTestRealm::DigestSecurityNeeded() {
    return true;
}

bool HttpStreamTestRealm::GetAuthenticationRequest(StreamString &message) {
    message = passw;
    return true;
}

CLASS_REGISTER(HttpStreamTestRealm, "1.0")
/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

HttpProtocolTest::HttpProtocolTest() {
    eventSem.Create();
    retVal = true;
    command = 0;
    isKeepAlive = false;
    textMode = 0;
}

HttpProtocolTest::~HttpProtocolTest() {
    // Auto-generated destructor stub for HttpProtocolTest
    // TODO Verify if manual additions are needed
}

bool HttpProtocolTest::TestConstructor() {
    TCPSocket client;

    StreamString stream;
    HttpProtocol test(client);

    bool ret = test.KeepAlive();
    StreamString path;
    test.GetPath(path);
    ret &= StringHelper::Compare(path.Buffer(), "") == 0;
    StreamString url;
    test.GetPath(url);
    ret &= StringHelper::Compare(url.Buffer(), "") == 0;

    ret &= test.GetHttpCommand() == HttpDefinition::HSHCNone;

    return ret;
}

static void clientJobGet1(HttpProtocolTest &tt) {
    //tells to the main process that the thread begins

    InternetHost source(5554, "127.0.0.1");
    InternetHost destination(5555, "127.0.0.1");

    TCPSocket socket;

    socket.SetSource(source);
    socket.SetDestination(destination);
    socket.Open();
    tt.eventSem.Wait();
    tt.eventSem.Reset();
    bool ret = socket.Connect("127.0.0.1", 5555);
    if (ret) {
        socket.Printf("%s", "GET /docs/index.html HTTP/1.1\n");
        socket.Printf("%s", "Host: www.nowhere123.com\n");
        socket.Printf("%s", "Accept: image/gif, image/jpeg, */*\n");
        socket.Printf("%s", "Accept-Language: en-us\n");
        socket.Printf("%s", "Accept-Encoding: gzip, deflate\n");
        socket.Printf("%s", "User-Agent: Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1)\n");
        socket.Printf("%s", "\n");
        socket.Flush();
    }
    tt.eventSem.Post();
    socket.Close();
    Threads::EndThread();
}

bool HttpProtocolTest::TestReadHeader_Get1() {
    InternetHost source(5554, "127.0.0.1");
    InternetHost destination(5555, "127.0.0.1");

    TCPSocket socket;
    socket.SetSource(source);
    socket.SetDestination(destination);

    socket.Open();
    socket.Listen(5555, 255);
    //todo launch a thread with the client request
    eventSem.Reset();
    Threads::BeginThread((ThreadFunctionType) clientJobGet1, this);
    TCPSocket newSocket;

    eventSem.Post();

    socket.WaitConnection(TTInfiniteWait, &newSocket);

    StreamString stream;
    HttpProtocol test(newSocket);

    bool ret = test.ReadHeader();
    eventSem.Wait();

    ConfigurationDatabase *cdb = (ConfigurationDatabase *) (&test);

    cdb->MoveToRoot();
    StreamString output;
    output.Printf("%@", *cdb);
    StreamString peer;
    ret &= cdb->Read("Peer", peer);

    if (ret) {
        ret = cdb->MoveAbsolute("InputOptions");
        StreamString par;
        ret &= cdb->Read("Host", par);
        ret &= (par == "www.nowhere123.com");
        par.SetSize(0);
        ret &= cdb->Read("Accept", par);
        ret &= (par == "image/gif, image/jpeg, */*");
        par.SetSize(0);

        ret &= cdb->Read("Accept-Language", par);
        ret &= (par == "en-us");
        par.SetSize(0);

        ret &= cdb->Read("User-Agent", par);
        ret &= (par == "Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1)");
        par.SetSize(0);
    }

    if (ret) {
        StreamString url;
        test.GetId(url);
        ret = (url == "docs.index.html");
    }

    if (ret) {
        StreamString path;
        test.GetPath(path);
        ret = (path == "docs.index.html");
    }

    newSocket.Close();
    socket.Close();

    return ret;
}

static void clientJobGet2(HttpProtocolTest &tt) {
    //tells to the main process that the thread begins

    InternetHost source(5554, "127.0.0.1");
    InternetHost destination(5555, "127.0.0.1");

    TCPSocket socket;

    socket.SetSource(source);
    socket.SetDestination(destination);
    socket.Open();
    tt.eventSem.Wait();
    tt.eventSem.Reset();
    bool ret = socket.Connect("127.0.0.1", 5555);
    if (ret) {
        socket.Printf("%s", "GET /docs/index.html?index=2&page=3 HTTP/1.1\n");
        socket.Printf("%s", "Host: www.nowhere123.com\n");
        socket.Printf("%s", "Accept: image/gif, image/jpeg, */*\n");
        socket.Printf("%s", "Accept-Language: en-us\n");
        socket.Printf("%s", "Accept-Encoding: gzip, deflate\n");
        socket.Printf("%s", "User-Agent: Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1)\n");
        socket.Printf("%s", "\n");
        socket.Flush();
    }
    tt.eventSem.Post();
    socket.Close();
    Threads::EndThread();
}

bool HttpProtocolTest::TestReadHeader_Get2_Commands() {
    InternetHost source(5554, "127.0.0.1");
    InternetHost destination(5555, "127.0.0.1");

    TCPSocket socket;
    socket.SetSource(source);
    socket.SetDestination(destination);

    socket.Open();
    socket.Listen(5555, 255);
    //todo launch a thread with the client request
    eventSem.Reset();
    Threads::BeginThread((ThreadFunctionType) clientJobGet2, this);
    TCPSocket newSocket;

    eventSem.Post();

    socket.WaitConnection(TTInfiniteWait, &newSocket);

    StreamString stream;
    HttpProtocol test(newSocket);
    bool ret = test.ReadHeader();
    eventSem.Wait();

    ConfigurationDatabase *cdb = (ConfigurationDatabase *) (&test);

    cdb->MoveToRoot();
    StreamString output;
    output.Printf("%@", *cdb);

    StreamString peer;
    ret &= cdb->Read("Peer", peer);

    if (ret) {
        ret = cdb->MoveAbsolute("InputCommands");
        StreamString par;
        ret &= cdb->Read("index", par);
        ret &= (par == "2");
        par.SetSize(0);
        ret &= cdb->Read("page", par);
        ret &= (par == "3");
        par.SetSize(0);
    }

    if (ret) {
        ret = cdb->MoveAbsolute("InputOptions");
        StreamString par;
        ret &= cdb->Read("Host", par);
        ret &= (par == "www.nowhere123.com");
        par.SetSize(0);
        ret &= cdb->Read("Accept", par);
        ret &= (par == "image/gif, image/jpeg, */*");
        par.SetSize(0);

        ret &= cdb->Read("Accept-Language", par);
        ret &= (par == "en-us");
        par.SetSize(0);

        ret &= cdb->Read("User-Agent", par);
        ret &= (par == "Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1)");
        par.SetSize(0);
    }

    if (ret) {
        StreamString url;
        test.GetId(url);
        ret = (url == "docs.index.html");
    }

    if (ret) {
        StreamString path;
        test.GetPath(path);
        ret = (path == "docs.index.html");
    }

    newSocket.Close();
    socket.Close();

    return ret;
}

static void clientJobPut1(HttpProtocolTest &tt) {
    //tells to the main process that the thread begins

    InternetHost source(5554, "127.0.0.1");
    InternetHost destination(5555, "127.0.0.1");

    TCPSocket socket;

    socket.SetSource(source);
    socket.SetDestination(destination);
    socket.Open();
    tt.eventSem.Wait();
    tt.eventSem.Reset();
    bool ret = socket.Connect("127.0.0.1", 5555);
    if (ret) {
        socket.Printf("%s", "PUT /user/1234567890 HTTP/1.1\n");
        socket.Printf("%s", "Host: http://marte2test.com\n");
        socket.Printf("%s", "\n{\n");
        socket.Printf("%s", "\"name\": \"Someone Somebody\"\n");
        socket.Printf("%s", "\"website\": \"http://marte2test.com\"\n");
        socket.Printf("%s", "}\n");
        socket.Flush();
    }
    tt.eventSem.Post();
    socket.Close();
    Threads::EndThread();
}

bool HttpProtocolTest::TestReadHeader_Put1() {
    InternetHost source(5554, "127.0.0.1");
    InternetHost destination(5555, "127.0.0.1");

    TCPSocket socket;
    socket.SetSource(source);
    socket.SetDestination(destination);

    socket.Open();
    socket.Listen(5555, 255);
    //todo launch a thread with the client request
    eventSem.Reset();
    Threads::BeginThread((ThreadFunctionType) clientJobPut1, this);
    TCPSocket newSocket;

    eventSem.Post();

    socket.WaitConnection(TTInfiniteWait, &newSocket);

    StreamString stream;
    HttpProtocol test(newSocket);

    bool ret = test.ReadHeader();
    eventSem.Wait();

    ConfigurationDatabase *cdb = (ConfigurationDatabase *) (&test);

    cdb->MoveToRoot();
    StreamString output;
    output.Printf("%@", *cdb);
    printf("%s\n", output.Buffer());

    StreamString peer;
    ret &= cdb->Read("Peer", peer);

    if (ret) {
        ret = cdb->MoveAbsolute("InputOptions");
        StreamString par;
        ret &= cdb->Read("Host", par);
        ret &= (par == "http://marte2test.com");
        par.SetSize(0);
    }

    if (ret) {
        StreamString url;
        test.GetId(url);
        ret = (url == "user.1234567890");
        printf("url=%s\n", url.Buffer());

    }

    if (ret) {
        StreamString path;
        test.GetPath(path);
        ret = (path == "user.1234567890");
        printf("path=%s\n", path.Buffer());
    }

    newSocket.Close();
    socket.Close();

    return ret;
}

static void clientJobPost1(HttpProtocolTest &tt) {
    //tells to the main process that the thread begins

    InternetHost source(5554, "127.0.0.1");
    InternetHost destination(5555, "127.0.0.1");

    TCPSocket socket;

    socket.SetSource(source);
    socket.SetDestination(destination);
    socket.Open();
    tt.eventSem.Wait();
    tt.eventSem.Reset();
    bool ret = socket.Connect("127.0.0.1", 5555);
    if (ret) {
        socket.Printf("%s", "POST /email_verification/ HTTP/1.1\n");
        socket.Printf("%s", "Host: api.evasrv.com\n");
        socket.Printf("%s", "Cache-Control: no-cache\n");
        socket.Printf("%s", "Content-Type: application/x-www-form-urlencoded\n");
        const char8* content = "email=username%40domain.com&user_API_token=YOUR-EV-APP-API-TOKEN&free=true&disposable=true&did_you_mean=true&role=true&bad=true&ev_score=true";
        socket.Printf("%s: %d\n\n", "Content-Length", StringHelper::Length(content));
        socket.Printf("%s\n", content);
        socket.Flush();
    }

    tt.eventSem.Post();
    socket.Close();
    Threads::EndThread();
}

bool HttpProtocolTest::TestReadHeader_Post1() {
    InternetHost source(5554, "127.0.0.1");
    InternetHost destination(5555, "127.0.0.1");

    TCPSocket socket;
    socket.SetSource(source);
    socket.SetDestination(destination);

    socket.Open();
    socket.Listen(5555, 255);
    //todo launch a thread with the client request
    eventSem.Reset();
    Threads::BeginThread((ThreadFunctionType) clientJobPost1, this);
    TCPSocket newSocket;

    eventSem.Post();

    socket.WaitConnection(TTInfiniteWait, &newSocket);

    StreamString stream;
    HttpProtocol test(newSocket);
    bool ret = test.ReadHeader();
    eventSem.Wait();

    ConfigurationDatabase *cdb = (ConfigurationDatabase *) (&test);

    cdb->MoveToRoot();
    StreamString output;
    output.Printf("%@", *cdb);
    printf("%s\n", output.Buffer());

    if (ret) {
        ret = cdb->MoveAbsolute("InputOptions");
        StreamString par;
        ret &= cdb->Read("Host", par);
        ret &= (par == "api.evasrv.com");
        par.SetSize(0);
        ret &= cdb->Read("Cache-Control", par);
        ret &= (par == "no-cache");
        par.SetSize(0);
        ret &= cdb->Read("Content-Type", par);
        ret &= (par == "application/x-www-form-urlencoded");
        par.SetSize(0);
        ret &= cdb->Read("Content-Length", par);
        ret &= (par == "141");
    }

    if (ret) {
        ret = cdb->MoveAbsolute("InputCommands");
        StreamString par;
        ret &= cdb->Read("rawPost", par);
        ret &= (par == "email=username%40domain.com&user_API_token=YOUR-EV-APP-API-TOKEN&free=true&disposable=true&did_you_mean=true&role=true&bad=true&ev_score=true\n");
        par.SetSize(0);
        ret &= cdb->Read("email", par);
        ret &= (par == "username@domain.com");
        par.SetSize(0);
        ret &= cdb->Read("user_API_token", par);
        ret &= (par == "YOUR-EV-APP-API-TOKEN");
        par.SetSize(0);
        ret &= cdb->Read("free", par);
        ret &= (par == "true");
        par.SetSize(0);
        ret &= cdb->Read("disposable", par);
        ret &= (par == "true");
        par.SetSize(0);
        ret &= cdb->Read("did_you_mean", par);
        ret &= (par == "true");
        par.SetSize(0);
        ret &= cdb->Read("role", par);
        ret &= (par == "true");
        par.SetSize(0);
        ret &= cdb->Read("bad", par);
        ret &= (par == "true");
        par.SetSize(0);
        ret &= cdb->Read("ev_score", par);
        ret &= (par == "true");
        par.SetSize(0);
    }

    if (ret) {
        StreamString url;
        test.GetId(url);
        ret = (url == "email_verification.");
    }

    if (ret) {
        StreamString path;
        test.GetPath(path);
        ret = (path == "email_verification.");
    }
    newSocket.Close();
    socket.Close();
    return ret;
}

static void clientJobPost2(HttpProtocolTest &tt) {
    //tells to the main process that the thread begins

    InternetHost source(5554, "127.0.0.1");
    InternetHost destination(5555, "127.0.0.1");

    TCPSocket socket;

    socket.SetSource(source);
    socket.SetDestination(destination);
    socket.Open();
    tt.eventSem.Wait();
    tt.eventSem.Reset();
    bool ret = socket.Connect("127.0.0.1", 5555);
    if (ret) {
        socket.Printf("%s", "POST /submit.cgi HTTP/1.1\n");
        socket.Printf("%s", "Host: example.com\n");
        socket.Printf("%s", "User-Agent: curl/7.46.0\n");
        socket.Printf("%s", "Accept: */*\n");
        socket.Printf("%s", "Content-Length: 298\n");
        socket.Printf("%s", "Expect: 100-continue\n");
        socket.Printf("%s", "Content-Type: multipart/form-data; boundary=------------------------d74496d66958873e\n\n");
        socket.Printf("%s", "--------------------------d74496d66958873e\n");
        socket.Printf("%s", "Content-Disposition: form-data; name=person\n\n");
        socket.Printf("%s", "anonymous\n");
        socket.Printf("%s", "--------------------------d74496d66958873e\n");
        socket.Printf("%s", "Content-Disposition: form-data; name=\"secret\"; filename=\"file.txt\"\n");
        socket.Printf("%s", "Content-Type: text/plain\n\n");
        socket.Printf("%s", "contents of the file\n\n");
        socket.Printf("%s", "--------------------------d74496d66958873e--\n\n");
        socket.Flush();
    }

    StreamString resp;
    char8 term;
    socket.GetToken(resp, "\n", term);
    tt.eventSem.Post();
    socket.Close();
    Threads::EndThread();
}

bool HttpProtocolTest::TestReadHeader_Post2_Multiform() {
    InternetHost source(5554, "127.0.0.1");
    InternetHost destination(5555, "127.0.0.1");

    TCPSocket socket;
    socket.SetSource(source);
    socket.SetDestination(destination);

    socket.Open();
    socket.Listen(5555, 255);
    //todo launch a thread with the client request
    eventSem.Reset();
    Threads::BeginThread((ThreadFunctionType) clientJobPost2, this);
    TCPSocket newSocket;

    eventSem.Post();

    socket.WaitConnection(TTInfiniteWait, &newSocket);

    StreamString stream;
    HttpProtocol test(newSocket);
    bool ret = test.ReadHeader();
    eventSem.Wait();

    ConfigurationDatabase *cdb = (ConfigurationDatabase *) (&test);

    cdb->MoveToRoot();
    StreamString output;
    output.Printf("%@", *cdb);

    if (ret) {
        ret = cdb->MoveAbsolute("InputOptions");
        StreamString par;
        ret &= cdb->Read("Host", par);
        ret &= (par == "example.com");
        par.SetSize(0);
        ret &= cdb->Read("User-Agent", par);
        ret &= (par == "curl/7.46.0");
        par.SetSize(0);
        ret &= cdb->Read("Accept", par);
        ret &= (par == "*/*");
        par.SetSize(0);
        ret &= cdb->Read("Content-Length", par);
        ret &= (par == "298");
        par.SetSize(0);
        ret &= cdb->Read("Expect", par);
        ret &= (par == "100-continue");
        par.SetSize(0);
        ret &= cdb->Read("Content-Type", par);
        ret &= (par == "multipart/form-data; boundary=------------------------d74496d66958873e");
    }

    if (ret) {
        ret = cdb->MoveAbsolute("InputCommands");
        StreamString par;
        ret &= cdb->Read("rawPost", par);
        par.SetSize(0);
        ret &= cdb->Read("person", par);
        ret &= (par == "anonymous");
        par.SetSize(0);
        ret &= cdb->Read("secret:filename", par);
        ret &= (par == "file.txt");
        par.SetSize(0);
        ret &= cdb->Read("secret:Content-Type", par);
        ret &= (par == "text/plain");
        par.SetSize(0);
        ret &= cdb->Read("secret", par);
        ret &= (par == "contents of the file");
        par.SetSize(0);
    }
    if (ret) {
        StreamString url;
        test.GetId(url);
        ret = (url == "submit.cgi");
        printf("url=%s\n", url.Buffer());

    }

    if (ret) {
        StreamString path;
        test.GetPath(path);
        ret = (path == "submit.cgi");
    }

    newSocket.Close();
    socket.Close();
    return ret;
}

static void clientJobPost3(HttpProtocolTest &tt) {
    //tells to the main process that the thread begins

    InternetHost source(5554, "127.0.0.1");
    InternetHost destination(5555, "127.0.0.1");

    TCPSocket socket;

    socket.SetSource(source);
    socket.SetDestination(destination);
    socket.Open();
    tt.eventSem.Wait();
    tt.eventSem.Reset();
    bool ret = socket.Connect("127.0.0.1", 5555);
    if (ret) {
        socket.Printf("%s", "POST /submit.cgi HTTP/1.1\n");
        socket.Printf("%s", "Host: example.com\n");
        socket.Printf("%s", "User-Agent: curl/7.46.0\n");
        socket.Printf("%s", "Accept: */*\n");
        socket.Printf("%s", "Content-Length: 298\n");
        socket.Printf("%s", "Expect: 100-continue\n");
        socket.Printf("%s", "Content-Type: multipart/form-data; boundary=\"------------------------d74496d66958873e\"\n\n");
        socket.Printf("%s", "--------------------------d74496d66958873e\n");
        socket.Printf("%s", "Content-Disposition: form-data; name=\"person\"\n\n");
        socket.Printf("%s", "anonymous\n");
        socket.Printf("%s", "--------------------------d74496d66958873e\n");
        socket.Printf("%s", "Content-Disposition: form-data; name=\"secret\"; filename=\"file.txt\"\n");
        socket.Printf("%s", "Content-Type: text/plain\n\n");
        socket.Printf("%s", "contents of the file\n\n");
        socket.Printf("%s", "--------------------------d74496d66958873e--\n\n");
        socket.Flush();
    }

    StreamString resp;
    char8 term;
    socket.GetToken(resp, "\n", term);
    tt.eventSem.Post();
    socket.Close();
    Threads::EndThread();
}

bool HttpProtocolTest::TestReadHeader_Post2_Multiform_WrappedBoundary() {
    InternetHost source(5554, "127.0.0.1");
    InternetHost destination(5555, "127.0.0.1");

    TCPSocket socket;
    socket.SetSource(source);
    socket.SetDestination(destination);

    socket.Open();
    socket.Listen(5555, 255);
    //todo launch a thread with the client request
    eventSem.Reset();
    Threads::BeginThread((ThreadFunctionType) clientJobPost3, this);
    TCPSocket newSocket;

    eventSem.Post();

    socket.WaitConnection(TTInfiniteWait, &newSocket);

    StreamString stream;
    HttpProtocol test(newSocket);
    bool ret = test.ReadHeader();
    eventSem.Wait();

    ConfigurationDatabase *cdb = (ConfigurationDatabase *) (&test);

    cdb->MoveToRoot();
    StreamString output;
    output.Printf("%@", *cdb);
    printf("%s\n", output.Buffer());

    if (ret) {
        ret = cdb->MoveAbsolute("InputOptions");
        StreamString par;
        ret &= cdb->Read("Host", par);
        ret &= (par == "example.com");
        par.SetSize(0);
        ret &= cdb->Read("User-Agent", par);
        ret &= (par == "curl/7.46.0");
        par.SetSize(0);
        ret &= cdb->Read("Accept", par);
        ret &= (par == "*/*");
        par.SetSize(0);
        ret &= cdb->Read("Content-Length", par);
        ret &= (par == "298");
        par.SetSize(0);
        ret &= cdb->Read("Expect", par);
        ret &= (par == "100-continue");
        par.SetSize(0);
        ret &= cdb->Read("Content-Type", par);
        ret &= (par == "multipart/form-data; boundary=\"------------------------d74496d66958873e\"");
    }

    if (ret) {
        ret = cdb->MoveAbsolute("InputCommands");
        StreamString par;
        ret &= cdb->Read("rawPost", par);
        par.SetSize(0);
        ret &= cdb->Read("person", par);
        ret &= (par == "anonymous");
        par.SetSize(0);
        ret &= cdb->Read("secret:filename", par);
        ret &= (par == "file.txt");
        par.SetSize(0);
        ret &= cdb->Read("secret:Content-Type", par);
        ret &= (par == "text/plain");
        par.SetSize(0);
        ret &= cdb->Read("secret", par);
        ret &= (par == "contents of the file");
        par.SetSize(0);
    }
    if (ret) {
        StreamString url;
        test.GetId(url);
        ret = (url == "submit.cgi");

    }

    if (ret) {
        StreamString path;
        test.GetPath(path);
        ret = (path == "submit.cgi");
    }

    newSocket.Close();
    socket.Close();
    return ret;
}

static void clientJobPostEmpty(HttpProtocolTest &tt) {

    InternetHost source(5554, "127.0.0.1");
    InternetHost destination(5555, "127.0.0.1");

    TCPSocket socket;

    socket.SetSource(source);
    socket.SetDestination(destination);
    socket.Open();
    tt.eventSem.Wait();
    tt.eventSem.Reset();
    bool ret = socket.Connect("127.0.0.1", 5555);
    if (ret) {
        socket.Printf("%s", "POST /submit.cgi HTTP/1.1\n");
        socket.Printf("%s", "Host: example.com\n");
        socket.Printf("%s", "User-Agent: curl/7.46.0\n");
        socket.Printf("%s", "Accept: */*\n");
        socket.Printf("%s", "Content-Length: 315\n");
        socket.Printf("%s", "Expect: 100-continue\n");
        socket.Printf("%s", "Content-Type: multipart/form-data; boundary=------------------------d74496d66958873e\n\n");
        socket.Printf("%s", "--------------------------d74496d66958873e\n");
        socket.Printf("%s", "Content-Disposition: form-data; name=\"person\"\n\n");
        socket.Printf("%s", "anonymous\n");
        socket.Printf("%s", "anonymous2\n");
        socket.Printf("%s", "--------------------------d74496d66958873e\n");
        socket.Printf("%s", "Content-Disposition: form-data; name=\"secret\"; filename=\"file.txt\"\n");
        socket.Printf("%s", "Content-Type: text/plain\n\n");
        socket.Printf("%s", "contents of the file\n\n");
        socket.Printf("%s", "--------------------------d74496d66958873e--\n\n");

        socket.Flush();
    }

    StreamString resp;
    char8 term;
    socket.GetToken(resp, "\n", term);
    tt.eventSem.Post();
    socket.Close();
    Threads::EndThread();
}

bool HttpProtocolTest::TestReadHeader_Post2_MultiformConsecutiveData() {
    InternetHost source(5554, "127.0.0.1");
    InternetHost destination(5555, "127.0.0.1");

    TCPSocket socket;
    socket.SetSource(source);
    socket.SetDestination(destination);

    socket.Open();
    socket.Listen(5555, 255);
    //todo launch a thread with the client request
    eventSem.Reset();
    Threads::BeginThread((ThreadFunctionType) clientJobPostEmpty, this);
    TCPSocket newSocket;

    eventSem.Post();

    socket.WaitConnection(TTInfiniteWait, &newSocket);

    StreamString stream;
    HttpProtocol test(newSocket);
    bool ret = test.ReadHeader();
    eventSem.Wait();

    ConfigurationDatabase *cdb = (ConfigurationDatabase *) (&test);

    cdb->MoveToRoot();
    StreamString output;
    output.Printf("%@", *cdb);

    if (ret) {
        ret = cdb->MoveAbsolute("InputOptions");
        StreamString par;
        ret &= cdb->Read("Host", par);
        ret &= (par == "example.com");
        par.SetSize(0);
        ret &= cdb->Read("User-Agent", par);
        ret &= (par == "curl/7.46.0");
        par.SetSize(0);
        ret &= cdb->Read("Accept", par);
        ret &= (par == "*/*");
        par.SetSize(0);
        ret &= cdb->Read("Content-Length", par);
        ret &= (par == "315");
        par.SetSize(0);
        ret &= cdb->Read("Expect", par);
        ret &= (par == "100-continue");
        par.SetSize(0);
        ret &= cdb->Read("Content-Type", par);
        ret &= (par == "multipart/form-data; boundary=------------------------d74496d66958873e");
    }

    if (ret) {
        ret = cdb->MoveAbsolute("InputCommands");
        StreamString par;
        ret &= cdb->Read("rawPost", par);
        par.SetSize(0);
        ret &= cdb->Read("person", par);
        ret &= (par == "anonymous\nanonymous2");
        par.SetSize(0);
        ret &= cdb->Read("secret:filename", par);
        ret &= (par == "file.txt");
        par.SetSize(0);
        ret &= cdb->Read("secret:Content-Type", par);
        ret &= (par == "text/plain");
        par.SetSize(0);
        ret &= cdb->Read("secret", par);
        ret &= (par == "contents of the file");
        par.SetSize(0);
    }
    if (ret) {
        StreamString url;
        test.GetId(url);
        ret = (url == "submit.cgi");
    }

    if (ret) {
        StreamString path;
        test.GetPath(path);
        ret = (path == "submit.cgi");
    }

    newSocket.Close();
    socket.Close();
    return ret;
}

static void clientJobPostNoBound(HttpProtocolTest &tt) {

    InternetHost source(5554, "127.0.0.1");
    InternetHost destination(5555, "127.0.0.1");

    TCPSocket socket;

    socket.SetSource(source);
    socket.SetDestination(destination);
    socket.Open();
    tt.eventSem.Wait();
    tt.eventSem.Reset();
    bool ret = socket.Connect("127.0.0.1", 5555);
    if (ret) {
        socket.Printf("%s", "POST /submit.cgi HTTP/1.1\n");
        socket.Printf("%s", "Host: example.com\n");
        socket.Printf("%s", "User-Agent: curl/7.46.0\n");
        socket.Printf("%s", "Accept: */*\n");
        socket.Printf("%s", "Content-Length: 315\n");
        socket.Printf("%s", "Expect: 100-continue\n");
        socket.Printf("%s", "Content-Type: multipart/form-data; \n\n");
        socket.Printf("%s", "--------------------------d74496d66958873e\n");
        socket.Printf("%s", "Content-Disposition: form-data; name=\"person\"\n\n");
        socket.Printf("%s", "anonymous\n");
        socket.Printf("%s", "anonymous2\n");
        socket.Printf("%s", "--------------------------d74496d66958873e\n");
        socket.Printf("%s", "Content-Disposition: form-data; name=\"secret\"; filename=\"file.txt\"\n");
        socket.Printf("%s", "Content-Type: text/plain\n\n");
        socket.Printf("%s", "contents of the file\n\n");
        socket.Printf("%s", "--------------------------d74496d66958873e--\n\n");

        socket.Flush();
    }

    StreamString resp;
    char8 term;
    socket.GetToken(resp, "\n", term);
    tt.eventSem.Post();
    socket.Close();
    Threads::EndThread();
}

bool HttpProtocolTest::TestReadHeader_Post2_FalseMultiformNoParsedBoundary() {
    InternetHost source(5554, "127.0.0.1");
    InternetHost destination(5555, "127.0.0.1");

    TCPSocket socket;
    socket.SetSource(source);
    socket.SetDestination(destination);

    socket.Open();
    socket.Listen(5555, 255);
    //todo launch a thread with the client request
    eventSem.Reset();
    Threads::BeginThread((ThreadFunctionType) clientJobPostNoBound, this);
    TCPSocket newSocket;

    eventSem.Post();

    socket.WaitConnection(TTInfiniteWait, &newSocket);

    StreamString stream;
    HttpProtocol test(newSocket);
    bool ret = !test.ReadHeader();
    eventSem.Wait();

    newSocket.Close();
    socket.Close();
    return ret;
}

static void clientJobHead(HttpProtocolTest &tt) {
    //tells to the main process that the thread begins

    InternetHost source(5554, "127.0.0.1");
    InternetHost destination(5555, "127.0.0.1");

    TCPSocket socket;

    socket.SetSource(source);
    socket.SetDestination(destination);
    socket.Open();
    tt.eventSem.Wait();
    tt.eventSem.Reset();
    bool ret = socket.Connect("127.0.0.1", 5555);
    if (ret) {
        socket.Printf("%s", "HEAD /bit/tests-vol-1.zip.tor HTTP/1.1\n");
        socket.Printf("%s", "Host: www.marte2tests.com\n\n");
        socket.Flush();
    }
    tt.eventSem.Post();
    socket.Close();
    Threads::EndThread();
}

bool HttpProtocolTest::TestReadHeader_Head() {
    InternetHost source(5554, "127.0.0.1");
    InternetHost destination(5555, "127.0.0.1");

    TCPSocket socket;
    socket.SetSource(source);
    socket.SetDestination(destination);

    socket.Open();
    socket.Listen(5555, 255);
    //todo launch a thread with the client request
    eventSem.Reset();
    Threads::BeginThread((ThreadFunctionType) clientJobHead, this);
    TCPSocket newSocket;

    eventSem.Post();

    socket.WaitConnection(TTInfiniteWait, &newSocket);

    StreamString stream;
    HttpProtocol test(newSocket);
    bool ret = test.ReadHeader();
    eventSem.Wait();

    ConfigurationDatabase *cdb = (ConfigurationDatabase *) (&test);

    cdb->MoveToRoot();
    StreamString output;
    output.Printf("%@", *cdb);

    if (ret) {
        ret = cdb->MoveAbsolute("InputOptions");
        StreamString par;
        ret &= cdb->Read("Host", par);
        ret &= (par == "www.marte2tests.com");
    }
    if (ret) {
        StreamString url;
        test.GetId(url);
        ret = (url == "bit.tests-vol-1.zip.tor");
    }

    if (ret) {
        StreamString path;
        test.GetPath(path);
        ret = (path == "bit.tests-vol-1.zip.tor");
    }

    newSocket.Close();
    socket.Close();
    return ret;
}

static void clientJobReply(HttpProtocolTest &tt) {
    //tells to the main process that the thread begins

    InternetHost source(5554, "127.0.0.1");
    InternetHost destination(5555, "127.0.0.1");

    TCPSocket socket;

    socket.SetSource(source);
    socket.SetDestination(destination);
    socket.Open();
    tt.eventSem.Wait();
    tt.eventSem.Reset();
    bool ret = socket.Connect("127.0.0.1", 5555);
    if (ret) {
        socket.Printf("%s", "HTTP/1.1 200 OK\n");
        socket.Printf("%s", "Date: Mon, 27 Jul 2009 12:28:53 GMT\n");
        socket.Printf("%s", "Server: Apache/2.2.14 (Win32)\n");
        socket.Printf("%s", "Last-Modified: Wed, 22 Jul 2009 19:15:56 GMT\n");
        socket.Printf("%s", "Content-Length: 53\n");
        socket.Printf("%s", "Content-Type: text/html\n");
        socket.Printf("%s", "Connection: Closed\n\n");
        socket.Printf("%s", "<html>\n");
        socket.Printf("%s", "<body>\n");
        socket.Printf("%s", "<h1>Hello, World!</h1>\n");
        socket.Printf("%s", "</body>\n");
        socket.Printf("%s", "</html>\n");
        socket.Flush();
    }

    tt.eventSem.Post();
    socket.Close();
    Threads::EndThread();
}

bool HttpProtocolTest::TestReadHeader_Reply() {
    InternetHost source(5554, "127.0.0.1");
    InternetHost destination(5555, "127.0.0.1");

    TCPSocket socket;
    socket.SetSource(source);
    socket.SetDestination(destination);

    socket.Open();
    socket.Listen(5555, 255);
    //todo launch a thread with the client request
    eventSem.Reset();
    Threads::BeginThread((ThreadFunctionType) clientJobReply, this);
    TCPSocket newSocket;

    eventSem.Post();

    socket.WaitConnection(TTInfiniteWait, &newSocket);

    StreamString stream;
    HttpProtocol test(newSocket);
    bool ret = test.ReadHeader();
    eventSem.Wait();

    ConfigurationDatabase *cdb = (ConfigurationDatabase *) (&test);

    cdb->MoveToRoot();
    StreamString output;
    output.Printf("%@", *cdb);

    if (ret) {
        ret = cdb->MoveAbsolute("InputOptions");
        StreamString par;
        ret &= cdb->Read("Date", par);
        ret &= (par == "Mon, 27 Jul 2009 12:28:53 GMT");
        par.SetSize(0);
        ret &= cdb->Read("Server", par);
        ret &= (par == "Apache/2.2.14 (Win32)");
        par.SetSize(0);
        ret &= cdb->Read("Last-Modified", par);
        ret &= (par == "Wed, 22 Jul 2009 19:15:56 GMT");
        par.SetSize(0);
        ret &= cdb->Read("Content-Length", par);
        ret &= (par == "53");
        par.SetSize(0);
        ret &= cdb->Read("Content-Type", par);
        ret &= (par == "text/html");
        par.SetSize(0);
        ret &= cdb->Read("Connection", par);
        ret &= (par == "Closed");
        par.SetSize(0);
    }
    if (ret) {
        StreamString url;
        test.GetId(url);
        ret = (url == "");
    }

    if (ret) {
        StreamString path;
        test.GetPath(path);
        ret = (path == "");
    }

    newSocket.Close();
    socket.Close();
    return ret;

}

static void clientJobReplyInvalidHttpVersion(HttpProtocolTest &tt) {
    //tells to the main process that the thread begins

    InternetHost source(5554, "127.0.0.1");
    InternetHost destination(5555, "127.0.0.1");

    TCPSocket socket;

    socket.SetSource(source);
    socket.SetDestination(destination);
    socket.Open();
    tt.eventSem.Wait();
    tt.eventSem.Reset();
    bool ret = socket.Connect("127.0.0.1", 5555);
    if (ret) {
        socket.Printf("%s", "HTTP/ciaone 200 OK\n");
        socket.Printf("%s", "Date: Mon, 27 Jul 2009 12:28:53 GMT\n");
        socket.Printf("%s", "Server: Apache/2.2.14 (Win32)\n");
        socket.Printf("%s", "Last-Modified: Wed, 22 Jul 2009 19:15:56 GMT\n");
        socket.Printf("%s", "Content-Length: 53\n");
        socket.Printf("%s", "Content-Type: text/html\n\n");
        socket.Printf("%s", "<html>\n");
        socket.Printf("%s", "<body>\n");
        socket.Printf("%s", "<h1>Hello, World!</h1>\n");
        socket.Printf("%s", "</body>\n");
        socket.Printf("%s", "</html>\n");
        socket.Flush();
    }

    tt.eventSem.Post();
    socket.Close();
    Threads::EndThread();
}

bool HttpProtocolTest::TestReadHeader_IncompatibleHTTPVersion() {
    InternetHost source(5554, "127.0.0.1");
    InternetHost destination(5555, "127.0.0.1");

    TCPSocket socket;
    socket.SetSource(source);
    socket.SetDestination(destination);

    socket.Open();
    socket.Listen(5555, 255);
    //todo launch a thread with the client request
    eventSem.Reset();
    Threads::BeginThread((ThreadFunctionType) clientJobReplyInvalidHttpVersion, this);
    TCPSocket newSocket;

    eventSem.Post();

    socket.WaitConnection(TTInfiniteWait, &newSocket);

    StreamString stream;
    HttpProtocol test(newSocket);
    bool ret = test.ReadHeader();
    eventSem.Wait();

    ConfigurationDatabase *cdb = (ConfigurationDatabase *) (&test);

    cdb->MoveToRoot();
    StreamString output;
    output.Printf("%@", *cdb);

    if (ret) {
        ret = cdb->MoveAbsolute("InputOptions");
        StreamString par;
        ret &= cdb->Read("Date", par);
        ret &= (par == "Mon, 27 Jul 2009 12:28:53 GMT");
        par.SetSize(0);
        ret &= cdb->Read("Server", par);
        ret &= (par == "Apache/2.2.14 (Win32)");
        par.SetSize(0);
        ret &= cdb->Read("Last-Modified", par);
        ret &= (par == "Wed, 22 Jul 2009 19:15:56 GMT");
        par.SetSize(0);
        ret &= cdb->Read("Content-Length", par);
        ret &= (par == "53");
        par.SetSize(0);
        ret &= cdb->Read("Content-Type", par);
        ret &= (par == "text/html");
        par.SetSize(0);
    }
    if (ret) {
        StreamString url;
        test.GetId(url);
        ret = (url == "");
    }

    if (ret) {
        StreamString path;
        test.GetPath(path);
        ret = (path == "");
    }

    newSocket.Close();
    socket.Close();
    return ret;
}

static void clientJobReplyInvalidHttpVersionNoReply(HttpProtocolTest &tt) {

//tells to the main process that the thread begins

    InternetHost source(5554, "127.0.0.1");
    InternetHost destination(5555, "127.0.0.1");

    TCPSocket socket;

    socket.SetSource(source);
    socket.SetDestination(destination);
    socket.Open();
    tt.eventSem.Wait();
    tt.eventSem.Reset();
    bool ret = socket.Connect("127.0.0.1", 5555);
    if (ret) {
        socket.Printf("%s", "GET /docs/index.html HTTP/ciaone\n");
        socket.Printf("%s", "Host: www.nowhere123.com\n");
        socket.Printf("%s", "Accept: image/gif, image/jpeg, */*\n");
        socket.Printf("%s", "Accept-Language: en-us\n");
        socket.Printf("%s", "Accept-Encoding: gzip, deflate\n");
        socket.Printf("%s", "User-Agent: Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1)\n");
        socket.Printf("%s", "\n");
        socket.Flush();
    }
    tt.eventSem.Post();
    socket.Close();
    Threads::EndThread();

}

bool HttpProtocolTest::TestReadHeader_IncompatibleHTTPVersionNoReply() {
    InternetHost source(5554, "127.0.0.1");
    InternetHost destination(5555, "127.0.0.1");

    TCPSocket socket;
    socket.SetSource(source);
    socket.SetDestination(destination);

    socket.Open();
    socket.Listen(5555, 255);
    //todo launch a thread with the client request
    eventSem.Reset();
    Threads::BeginThread((ThreadFunctionType) clientJobReplyInvalidHttpVersionNoReply, this);
    TCPSocket newSocket;

    eventSem.Post();

    socket.WaitConnection(TTInfiniteWait, &newSocket);

    StreamString stream;
    HttpProtocol test(newSocket);
    bool ret = test.ReadHeader();
    eventSem.Wait();

    newSocket.Close();
    socket.Close();
    return ret;
}

static void clientJobReplyInvalidReplyCode(HttpProtocolTest &tt) {
    //tells to the main process that the thread begins

    InternetHost source(5554, "127.0.0.1");
    InternetHost destination(5555, "127.0.0.1");

    TCPSocket socket;

    socket.SetSource(source);
    socket.SetDestination(destination);
    socket.Open();
    tt.eventSem.Wait();
    tt.eventSem.Reset();
    bool ret = socket.Connect("127.0.0.1", 5555);
    if (ret) {
        socket.Printf("%s", "HTTP/1.1 ciaone OK\n");
        socket.Printf("%s", "Date: Mon, 27 Jul 2009 12:28:53 GMT\n");
        socket.Printf("%s", "Server: Apache/2.2.14 (Win32)\n");
        socket.Printf("%s", "Last-Modified: Wed, 22 Jul 2009 19:15:56 GMT\n");
        socket.Printf("%s", "Content-Length: 53\n");
        socket.Printf("%s", "Content-Type: text/html\n\n");
        socket.Printf("%s", "<html>\n");
        socket.Printf("%s", "<body>\n");
        socket.Printf("%s", "<h1>Hello, World!</h1>\n");
        socket.Printf("%s", "</body>\n");
        socket.Printf("%s", "</html>\n");
        socket.Flush();
    }

    tt.eventSem.Post();
    socket.Close();
    Threads::EndThread();
}

bool HttpProtocolTest::TestReadHeader_FalseInvalidReplyCode() {
    InternetHost source(5554, "127.0.0.1");
    InternetHost destination(5555, "127.0.0.1");

    TCPSocket socket;
    socket.SetSource(source);
    socket.SetDestination(destination);

    socket.Open();
    socket.Listen(5555, 255);
    //todo launch a thread with the client request
    eventSem.Reset();
    Threads::BeginThread((ThreadFunctionType) clientJobReplyInvalidReplyCode, this);
    TCPSocket newSocket;

    eventSem.Post();

    socket.WaitConnection(TTInfiniteWait, &newSocket);

    StreamString stream;
    HttpProtocol test(newSocket);
    bool ret = !test.ReadHeader();
    eventSem.Wait();

    newSocket.Close();
    socket.Close();
    return ret;
}

static void clientJobReplyInvalidCommand(HttpProtocolTest &tt) {
    //tells to the main process that the thread begins

    InternetHost source(5554, "127.0.0.1");
    InternetHost destination(5555, "127.0.0.1");

    TCPSocket socket;

    socket.SetSource(source);
    socket.SetDestination(destination);
    socket.Open();
    tt.eventSem.Wait();
    tt.eventSem.Reset();
    bool ret = socket.Connect("127.0.0.1", 5555);
    if (ret) {
        socket.Printf("%s", "HTTX/1.1 200 OK\n");
        socket.Printf("%s", "Date: Mon, 27 Jul 2009 12:28:53 GMT\n");
        socket.Printf("%s", "Server: Apache/2.2.14 (Win32)\n");
        socket.Printf("%s", "Last-Modified: Wed, 22 Jul 2009 19:15:56 GMT\n");
        socket.Printf("%s", "Content-Length: 53\n");
        socket.Printf("%s", "Content-Type: text/html\n\n");
        socket.Printf("%s", "<html>\n");
        socket.Printf("%s", "<body>\n");
        socket.Printf("%s", "<h1>Hello, World!</h1>\n");
        socket.Printf("%s", "</body>\n");
        socket.Printf("%s", "</html>\n");
        socket.Flush();
    }

    tt.eventSem.Post();
    socket.Close();
    Threads::EndThread();
}

bool HttpProtocolTest::TestReadHeader_FalseInvalidCommand() {
    InternetHost source(5554, "127.0.0.1");
    InternetHost destination(5555, "127.0.0.1");

    TCPSocket socket;
    socket.SetSource(source);
    socket.SetDestination(destination);

    socket.Open();
    socket.Listen(5555, 255);
    //todo launch a thread with the client request
    eventSem.Reset();
    Threads::BeginThread((ThreadFunctionType) clientJobReplyInvalidCommand, this);
    TCPSocket newSocket;

    eventSem.Post();

    socket.WaitConnection(TTInfiniteWait, &newSocket);

    StreamString stream;
    HttpProtocol test(newSocket);
    bool ret = !test.ReadHeader();
    eventSem.Wait();

    newSocket.Close();
    socket.Close();
    return ret;
}

bool HttpProtocolTest::TestCompleteReadOperation() {
    InternetHost source(5554, "127.0.0.1");
    InternetHost destination(5555, "127.0.0.1");

    TCPSocket socket;
    socket.SetSource(source);
    socket.SetDestination(destination);

    socket.Open();
    socket.Listen(5555, 255);
    //todo launch a thread with the client request
    eventSem.Reset();
    Threads::BeginThread((ThreadFunctionType) clientJobReply, this);
    TCPSocket newSocket;

    eventSem.Post();

    socket.WaitConnection(TTInfiniteWait, &newSocket);

    StreamString stream;
    HttpProtocol test(newSocket);

    bool ret = test.ReadHeader();
    StreamString remained;
    if (ret) {
        ret = test.CompleteReadOperation(&remained);
    }

    eventSem.Wait();

    ConfigurationDatabase *cdb = (ConfigurationDatabase *) (&test);

    cdb->MoveToRoot();
    StreamString output;
    output.Printf("%@", *cdb);
    printf("%s\n", output.Buffer());

    printf("remained %s\n", remained.Buffer());
    if (ret) {
        ret = (remained == "<html>\n<body>\n<h1>Hello, World!</h1>\n</body>\n</html>\n");
    }
    newSocket.Close();
    socket.Close();
    return ret;
}

static void clientJobReply_BigBody(HttpProtocolTest &tt) {
    //tells to the main process that the thread begins

    InternetHost source(5554, "127.0.0.1");
    InternetHost destination(5555, "127.0.0.1");

    TCPSocket socket;

    socket.SetSource(source);
    socket.SetDestination(destination);
    socket.Open();
    tt.eventSem.Wait();
    tt.eventSem.Reset();
    bool ret = socket.Connect("127.0.0.1", 5555);
    if (ret) {
        socket.Printf("%s", "HTTP/1.1 200 OK\n");
        socket.Printf("%s", "Date: Mon, 27 Jul 2009 12:28:53 GMT\n");
        socket.Printf("%s", "Server: Apache/2.2.14 (Win32)\n");
        socket.Printf("%s", "Last-Modified: Wed, 22 Jul 2009 19:15:56 GMT\n");
        socket.Printf("%s", "Content-Length: 3072\n");
        socket.Printf("%s", "Content-Type: text/html\n");
        socket.Printf("%s", "Connection: Closed\n\n");
        char8 buffer[3072];
        MemoryOperationsHelper::Set(buffer, 'x', 3072);
        buffer[3071] = 0;
        socket.Printf("%s", (const char8*) buffer);
        socket.Printf("%s", "\n");
        socket.Flush();
    }

    tt.eventSem.Post();
    socket.Close();
    Threads::EndThread();
}

bool HttpProtocolTest::TestCompleteReadOperation_ClipSize() {
    InternetHost source(5554, "127.0.0.1");
    InternetHost destination(5555, "127.0.0.1");

    TCPSocket socket;
    socket.SetSource(source);
    socket.SetDestination(destination);

    socket.Open();
    socket.Listen(5555, 255);
    //todo launch a thread with the client request
    eventSem.Reset();
    Threads::BeginThread((ThreadFunctionType) clientJobReply_BigBody, this);
    TCPSocket newSocket;

    eventSem.Post();

    socket.WaitConnection(TTInfiniteWait, &newSocket);

    StreamString stream;
    HttpProtocol test(newSocket);

    bool ret = test.ReadHeader();
    StreamString remained;
    if (ret) {
        ret = test.CompleteReadOperation(&remained);
    }

    eventSem.Wait();

    ret &= remained.Size() == 3072;
    for (uint32 i = 0u; (i < 3071) && (ret); i++) {
        ret = remained[i] == 'x';
    }
    ret &= remained[3072] == 0;
    //printf("remained %s %lld\n", remained.Buffer(), remained.Size());

    newSocket.Close();
    socket.Close();
    return ret;
}

static void clientJobWrite(HttpProtocolTest &tt) {
    //tells to the main process that the thread begins

    InternetHost source(5554, "127.0.0.1");
    InternetHost destination(5555, "127.0.0.1");

    TCPSocket socket;

    socket.SetSource(source);
    socket.SetDestination(destination);
    socket.Open();
    tt.eventSem.Wait();
    tt.eventSem.Reset();
    tt.retVal = socket.Connect("127.0.0.1", 5555);
    if (tt.retVal) {
        StreamString stream;
        HttpProtocol test(socket);
        test.CreateAbsolute("OutputOptions");
        test.Write("Content-Type", "application/x-www-form-urlencoded");
        test.MoveToRoot();

        stream.SetSize(0);
        stream.Printf("%s", "ciaobellooo\n");
        stream.Seek(0);
        tt.retVal = test.WriteHeader(true, tt.command, &stream, "localhost");

    }

    tt.eventSem.Post();
    socket.Close();
    Threads::EndThread();
}

static void clientJobWrite2(HttpProtocolTest &tt) {
    //tells to the main process that the thread begins

    InternetHost source(5555, "127.0.0.1");
    InternetHost destination(5555, "127.0.0.1");

    TCPSocket socket;

    socket.SetSource(source);
    socket.SetDestination(destination);
    socket.Open();
    tt.eventSem.Wait();
    tt.eventSem.Reset();
    tt.retVal = socket.Connect("127.0.0.1", 5555);
    if (tt.retVal) {

        StreamString stream;
        HttpProtocol test(socket);

        test.CreateAbsolute("OutputOptions");
        test.Write("Transfer-Encoding", "chunked");

        test.Write("Content-Type", "application/x-www-form-urlencoded");
        stream.SetSize(0);
        stream.Printf("%s", "ciao");
        stream.Seek(0);
        tt.retVal = test.WriteHeader(false, tt.command, &stream, "localhost");
        tt.eventSem.Wait();
        tt.eventSem.Reset();
        test.MoveAbsolute("OutputOptions");

        test.Delete("Connection");

        test.Write("Connection", "close");

        test.Delete("Transfer-Encoding");
        test.MoveToRoot();

        stream.SetSize(0);
        stream.Printf("%s", "bellooo\n");
        stream.Seek(0);
        tt.retVal &= test.WriteHeader(true, tt.command, &stream, NULL);
    }

    tt.eventSem.Post();
    socket.Close();
    Threads::EndThread();
}

bool HttpProtocolTest::TestWriteHeader() {

    InternetHost source(5554, "127.0.0.1");
    InternetHost destination(5555, "127.0.0.1");

    TCPSocket socket;
    socket.SetSource(source);
    socket.SetDestination(destination);

    socket.Open();
    socket.Listen(5555, 255);
    //todo launch a thread with the client request
    eventSem.Reset();
    Threads::BeginThread((ThreadFunctionType) clientJobWrite, this);
    TCPSocket newSocket;

    eventSem.Post();

    socket.WaitConnection(TTInfiniteWait, &newSocket);

    StreamString stream;
    HttpProtocol test(newSocket);

    bool ret = test.ReadHeader();
    StreamString remained;
    if (ret) {
        ret = test.CompleteReadOperation(&remained);
    }

    eventSem.Wait();

    ConfigurationDatabase *cdb = (ConfigurationDatabase *) (&test);

    cdb->MoveToRoot();
    StreamString output;
    output.Printf("%@", *cdb);

    if (command != HttpDefinition::HSHCPost) {
        if (ret) {
            ret = (remained == "ciaobellooo\n");
        }
    }
    if (ret) {
        ret = cdb->MoveAbsolute("InputOptions");
        StreamString par;
        ret &= cdb->Read("Content-Length", par);
        ret &= (par == "12");
        par.SetSize(0);

        ret &= cdb->Read("Content-Type", par);
        ret &= (par == "application/x-www-form-urlencoded");
        par.SetSize(0);

    }
    newSocket.Close();
    socket.Close();
    return (ret && retVal);

}

bool HttpProtocolTest::TestWriteHeader2() {
    InternetHost source(5555, "127.0.0.1");
    InternetHost destination(5555, "127.0.0.1");

    TCPSocket socket;
    socket.SetSource(source);
    socket.SetDestination(destination);

    socket.Open();
    socket.Listen(5555, 255);
    //todo launch a thread with the client request
    Threads::BeginThread((ThreadFunctionType) clientJobWrite2, this);
    TCPSocket newSocket;

    eventSem.Post();

    socket.WaitConnection(TTInfiniteWait, &newSocket);

    StreamString stream;
    HttpProtocol test(newSocket);

    bool ret = test.ReadHeader();

    StreamString remained;
    if (ret) {
        ret = test.CompleteReadOperation(&remained, 10);
    }

    ConfigurationDatabase *cdb = (ConfigurationDatabase *) (&test);

    cdb->MoveToRoot();
    StreamString output;
    output.Printf("%@", *cdb);
    printf("%s\n", output.Buffer());
    if (ret) {
        ret = cdb->MoveAbsolute("InputOptions");
        StreamString par;
        ret &= cdb->Read("Transfer-Encoding", par);
        ret &= (par == "chunked");
        par.SetSize(0);
    }

    eventSem.Post();

    StreamString remained2;
    if (ret) {
        ret = test.ReadHeader();
        if (ret) {
            ret = test.CompleteReadOperation(&remained2);
        }
    }

    eventSem.Wait();

    cdb->MoveToRoot();
    StreamString output2;
    output2.Printf("%@", *cdb);
    if (ret) {
        ret = cdb->MoveAbsolute("InputOptions");
        StreamString par;
        ret &= cdb->Read("Content-Length", par);
        ret &= (par == "8");
        par.SetSize(0);

        ret &= cdb->Read("Connection", par);
        ret &= (par == "close");
        par.SetSize(0);
    }
    if (command != HttpDefinition::HSHCPost) {

        if (ret) {
            ret = (remained == "ciao");
        }
        if (ret) {
            ret = (remained2 == "bellooo\n");
        }
    }

    newSocket.Close();
    socket.Close();
    return (ret && retVal);

}

bool HttpProtocolTest::TestWriteHeader_Get1() {
    retVal = true;
    eventSem.Reset();
    command = HttpDefinition::HSHCGet;
    return TestWriteHeader();
}

bool HttpProtocolTest::TestWriteHeader_Get2() {
    retVal = true;
    eventSem.Reset();
    command = HttpDefinition::HSHCGet;
    return TestWriteHeader2();
}

bool HttpProtocolTest::TestWriteHeader_Put1() {
    retVal = true;
    eventSem.Reset();
    command = HttpDefinition::HSHCPut;
    return TestWriteHeader();

}

bool HttpProtocolTest::TestWriteHeader_Put2() {
    retVal = true;
    eventSem.Reset();
    command = HttpDefinition::HSHCPut;
    return TestWriteHeader2();

}

bool HttpProtocolTest::TestWriteHeader_Head1() {
    retVal = true;
    eventSem.Reset();
    command = HttpDefinition::HSHCHead;
    return TestWriteHeader();

}

bool HttpProtocolTest::TestWriteHeader_Head2() {
    retVal = true;
    eventSem.Reset();
    command = HttpDefinition::HSHCHead;
    return TestWriteHeader2();

}

bool HttpProtocolTest::TestWriteHeader_Post() {
    retVal = true;
    eventSem.Reset();
    command = HttpDefinition::HSHCPost;
    return TestWriteHeader();

}

bool HttpProtocolTest::TestWriteHeader_Reply() {
    retVal = true;
    eventSem.Reset();
    command = HttpDefinition::HSHCReplyOK;
    return TestWriteHeader();
}

bool HttpProtocolTest::TestWriteHeader_Reply2() {
    retVal = true;
    eventSem.Reset();
    command = HttpDefinition::HSHCReplyOK;
    return TestWriteHeader2();
}

static void clientJobWriteStructuredStored(HttpProtocolTest &tt) {
    //tells to the main process that the thread begins

    InternetHost source(5555, "127.0.0.1");
    InternetHost destination(5555, "127.0.0.1");

    TCPSocket socket;

    socket.SetSource(source);
    socket.SetDestination(destination);
    socket.Open();
    tt.eventSem.Wait();
    tt.eventSem.Reset();
    tt.retVal = socket.Connect("127.0.0.1", 5555);
    if (tt.retVal) {
        StreamStructuredData<JsonPrinter> stream;
        StreamString string;
        stream.SetStream(string);
        HttpProtocol test(socket);

        test.CreateAbsolute("OutputOptions");
        test.Write("Transfer-Encoding", "chunked");
        test.Write("Content-Type", "application/x-www-form-urlencoded");
        string.SetSize(0);
        string.Printf("%s", "ciao");
        string.Seek(0);
        tt.retVal = test.WriteHeader(false, tt.command, &string, "localhost");
        tt.eventSem.Wait();
        tt.eventSem.Reset();
        test.Delete("Transfer-Encoding");
        test.MoveToRoot();

        string.SetSize(0);
        string.Printf("%s", "bellooo");
        stream.CreateAbsolute("A.B.C");
        uint32 var1 = 1u;
        stream.Write("var1", var1);
        stream.CreateAbsolute("A.D");
        uint32 var2 = 2u;
        stream.Write("var2", var2);
        stream.CreateAbsolute("A.E.F");
        uint32 var3 = 3u;
        stream.Write("var3", var3);
        stream.MoveToRoot();

        string.Seek(0);
        //test.WriteStructuredDataOnSocket();
        tt.retVal &= test.WriteHeader(true, tt.command, &string, "localhost");

    }

    tt.eventSem.Post();
    socket.Close();
    Threads::EndThread();
}

bool HttpProtocolTest::TestWriteHeader_StrucuredDataStored() {
    retVal = true;
    eventSem.Reset();
    command = HttpDefinition::HSHCReplyOK;
    InternetHost source(5555, "127.0.0.1");
    InternetHost destination(5555, "127.0.0.1");

    TCPSocket socket;
    socket.SetSource(source);
    socket.SetDestination(destination);

    socket.Open();
    socket.Listen(5555, 255);
    Threads::BeginThread((ThreadFunctionType) clientJobWriteStructuredStored, this);
    TCPSocket newSocket;

    eventSem.Post();

    socket.WaitConnection(TTInfiniteWait, &newSocket);

    StreamString stream;
    HttpProtocol test(newSocket);

    bool ret = test.ReadHeader();

    StreamString remained;
    if (ret) {
        ret = test.CompleteReadOperation(&remained, 10);
    }

    ConfigurationDatabase *cdb = (ConfigurationDatabase *) (&test);

    cdb->MoveToRoot();
    StreamString output;
    output.Printf("%@", *cdb);

    eventSem.Post();

    StreamString remained2;
    if (ret) {
        ret = test.ReadHeader();
        if (ret) {
            ret = test.CompleteReadOperation(&remained2);
        }
    }

    eventSem.Wait();

    cdb->MoveToRoot();
    StreamString output2;
    output2.Printf("%@", *cdb);

    if (ret) {
        ret = cdb->MoveAbsolute("InputOptions");
        StreamString par;
        ret &= cdb->Read("Content-Length", par);
        ret &= (par == "108");
        par.SetSize(0);
    }
    if (ret) {
        ret = (remained == "ciao");
    }
    if (ret) {
        ret = (remained2 == "bellooo\n\r"
                "\"A\": {\n\r"
                "\"B\": {\n\r"
                "\"C\": {\n\r"
                "\"var1\": 1\n\r"
                "}\n\r"
                "},\n\r"
                "\"D\": {\n\r"
                "\"var2\": 2\n\r"
                "},\n\r"
                "\"E\": {\n\r"
                "\"F\": {\n\r"
                "\"var3\": 3\n\r"
                "}\n\r"
                "}\n\r"
                "}");
    }

    newSocket.Close();
    socket.Close();
    return (ret && retVal);
}

static void clientJobWriteStructuredOnline(HttpProtocolTest &tt) {
    //tells to the main process that the thread begins

    InternetHost source(5555, "127.0.0.1");
    InternetHost destination(5555, "127.0.0.1");

    TCPSocket socket;

    socket.SetSource(source);
    socket.SetDestination(destination);
    socket.Open();
    tt.eventSem.Wait();
    tt.eventSem.Reset();
    tt.retVal = socket.Connect("127.0.0.1", 5555);
    if (tt.retVal) {
        StreamStructuredData<JsonPrinter> stream;
        HttpProtocol test(socket);

        stream.SetStream(socket);
        test.CreateAbsolute("OutputOptions");
        test.Write("Transfer-Encoding", "chunked");
        test.Write("Content-Type", "application/x-www-form-urlencoded");

        StreamString string;
        string.SetSize(0);
        string.Printf("%s", "ciao");
        string.Seek(0);
        tt.retVal = test.WriteHeader(false, tt.command, &string, "localhost");
        tt.eventSem.Wait();
        tt.eventSem.Reset();
        test.Delete("Transfer-Encoding");
        test.MoveToRoot();

        string.SetSize(0);
        string.Printf("%s", "bellooo");
        string.Seek(0);

        tt.retVal &= test.WriteHeader(false, tt.command, &string, "localhost");
        stream.CreateAbsolute("A.B.C");
        uint32 var1 = 1u;
        stream.Write("var1", var1);
        stream.CreateAbsolute("A.D");
        uint32 var2 = 2u;
        stream.Write("var2", var2);
        stream.CreateAbsolute("A.E.F");
        uint32 var3 = 3u;
        stream.Write("var3", var3);
        stream.MoveToRoot();

    }

    tt.eventSem.Post();
    socket.Close();
    Threads::EndThread();
}

bool HttpProtocolTest::TestWriteHeader_StrucuredDataOnline() {
    retVal = true;
    eventSem.Reset();
    command = HttpDefinition::HSHCReplyOK;
    InternetHost source(5555, "127.0.0.1");
    InternetHost destination(5555, "127.0.0.1");

    TCPSocket socket;
    socket.SetSource(source);
    socket.SetDestination(destination);

    socket.Open();
    socket.Listen(5555, 255);
    //todo launch a thread with the client request
    Threads::BeginThread((ThreadFunctionType) clientJobWriteStructuredOnline, this);
    TCPSocket newSocket;

    eventSem.Post();

    socket.WaitConnection(TTInfiniteWait, &newSocket);

    StreamString stream;
    HttpProtocol test(newSocket);

    bool ret = test.ReadHeader();

    StreamString remained;
    if (ret) {
        ret = test.CompleteReadOperation(&remained, 10);
    }

    ConfigurationDatabase *cdb = (ConfigurationDatabase *) (&test);

    cdb->MoveToRoot();
    StreamString output;
    output.Printf("%@", *cdb);
    eventSem.Post();

    StreamString remained2;
    if (ret) {
        ret = test.ReadHeader();
        if (ret) {
            ret = test.CompleteReadOperation(&remained2);
        }
    }

    eventSem.Wait();

    cdb->MoveToRoot();
    StreamString output2;
    output2.Printf("%@", *cdb);

    if (ret) {
        ret = (remained == "ciao");
    }
    if (ret) {
        ret = (remained2 == "bellooo\n\r"
                "\"A\": {\n\r"
                "\"B\": {\n\r"
                "\"C\": {\n\r"
                "\"var1\": 1\n\r"
                "}\n\r"
                "},\n\r"
                "\"D\": {\n\r"
                "\"var2\": 2\n\r"
                "},\n\r"
                "\"E\": {\n\r"
                "\"F\": {\n\r"
                "\"var3\": 3\n\r"
                "}\n\r"
                "}\n\r"
                "}");

    }

    newSocket.Close();
    socket.Close();
    return (ret && retVal);
}

static void clientJobWriteInvalid(HttpProtocolTest &tt) {
    //tells to the main process that the thread begins

    InternetHost source(5554, "127.0.0.1");
    InternetHost destination(5555, "127.0.0.1");

    TCPSocket socket;

    socket.SetSource(source);
    socket.SetDestination(destination);
    socket.Open();
    tt.eventSem.Wait();
    tt.eventSem.Reset();
    tt.retVal = socket.Connect("127.0.0.1", 5555);
    if (tt.retVal) {
        StreamString stream;
        HttpProtocol test(socket);
        test.CreateAbsolute("OutputOptions");
        test.Write("Content-Type", "application/x-www-form-urlencoded");
        test.MoveToRoot();

        stream.SetSize(0);
        stream.Printf("%s", "ciaobellooo\n");
        stream.Seek(0);
        tt.retVal = test.WriteHeader(true, -10, &stream, "localhost");

    }

    tt.eventSem.Post();
    socket.Close();
    Threads::EndThread();
}

bool HttpProtocolTest::TestWriteHeader_False_InvalidCommand() {

    InternetHost source(5554, "127.0.0.1");
    InternetHost destination(5555, "127.0.0.1");

    TCPSocket socket;
    socket.SetSource(source);
    socket.SetDestination(destination);

    socket.Open();
    socket.Listen(5555, 255);
    //todo launch a thread with the client request
    eventSem.Reset();
    Threads::BeginThread((ThreadFunctionType) clientJobWriteInvalid, this);
    TCPSocket newSocket;

    eventSem.Post();

    socket.WaitConnection(TTInfiniteWait, &newSocket);

    StreamString stream;
    HttpProtocol test(newSocket);

    bool ret = !test.ReadHeader();

    newSocket.Close();
    socket.Close();
    return (ret && !retVal);

}

static void clientJobAuth(HttpProtocolTest &tt) {
    //tells to the main process that the thread begins

    InternetHost source(5554, "127.0.0.1");
    InternetHost destination(5555, "127.0.0.1");

    TCPSocket socket;

    socket.SetSource(source);
    socket.SetDestination(destination);
    socket.Open();
    tt.eventSem.Wait();
    tt.eventSem.Reset();
    bool ret = socket.Connect("127.0.0.1", 5555);
    if (ret) {
        socket.Printf("%s", "GET /docs/index.html HTTP/1.1\n");
        socket.Printf("%s", "Host: www.nowhere123.com\n");
        socket.Printf("%s", "Authorization: admin1234\n");
        socket.Printf("%s", "\n");
        socket.Flush();
    }
    tt.eventSem.Post();
    socket.Close();
    Threads::EndThread();
}

bool HttpProtocolTest::TestSecurityCheck() {
    InternetHost source(5554, "127.0.0.1");
    InternetHost destination(5555, "127.0.0.1");

    ReferenceT<HttpStreamTestRealm> realm(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    realm->SetPassw("admin1234");

    TCPSocket socket;
    socket.SetSource(source);
    socket.SetDestination(destination);

    socket.Open();
    socket.Listen(5555, 255);
    //todo launch a thread with the client request
    eventSem.Reset();
    Threads::BeginThread((ThreadFunctionType) clientJobAuth, this);
    TCPSocket newSocket;

    eventSem.Post();

    socket.WaitConnection(TTInfiniteWait, &newSocket);

    StreamString stream;
    HttpProtocol test(newSocket);

    bool ret = test.ReadHeader();
    if (ret) {
        ret = test.SecurityCheck(realm);
    }
    if (ret) {
        realm->SetPassw("invalid");
        ret = !test.SecurityCheck(realm);
    }

    eventSem.Wait();

    ConfigurationDatabase *cdb = (ConfigurationDatabase *) (&test);

    cdb->MoveToRoot();
    StreamString output;
    output.Printf("%@", *cdb);
    printf("%s\n", output.Buffer());

    newSocket.Close();
    socket.Close();

    return ret;
}

bool HttpProtocolTest::TestKeepAliveDefault() {
    InternetHost source(5554, "127.0.0.1");
    InternetHost destination(5555, "127.0.0.1");

    TCPSocket socket;
    socket.SetSource(source);
    socket.SetDestination(destination);

    socket.Open();
    socket.Listen(5555, 255);
    //todo launch a thread with the client request
    eventSem.Reset();
    Threads::BeginThread((ThreadFunctionType) clientJobGet1, this);
    TCPSocket newSocket;

    eventSem.Post();

    socket.WaitConnection(TTInfiniteWait, &newSocket);

    StreamString stream;
    HttpProtocol test(newSocket);

    bool ret = test.ReadHeader();
    eventSem.Wait();

    ConfigurationDatabase *cdb = (ConfigurationDatabase *) (&test);

    cdb->MoveToRoot();
    StreamString output;
    output.Printf("%@", *cdb);
    printf("%s\n", output.Buffer());

    if (ret) {
        ret = test.KeepAlive();
    }

    newSocket.Close();
    socket.Close();

    return ret;
}

static void clientJobKeepAlive(HttpProtocolTest &tt) {
    //tells to the main process that the thread begins

    InternetHost source(5554, "127.0.0.1");
    InternetHost destination(5555, "127.0.0.1");

    TCPSocket socket;

    socket.SetSource(source);
    socket.SetDestination(destination);
    socket.Open();
    tt.eventSem.Wait();
    tt.eventSem.Reset();
    bool ret = socket.Connect("127.0.0.1", 5555);
    if (ret) {
        socket.Printf("%s", "GET /docs/index.html HTTP/1.1\n");
        socket.Printf("%s", "Host: www.nowhere123.com\n");
        if (tt.isKeepAlive) {
            socket.Printf("%s", "Connection: keep-alive\n");
        }
        else {
            socket.Printf("%s", "Connection: closed\n");
        }
        socket.Printf("%s", "\n");
        socket.Flush();
    }
    tt.eventSem.Post();
    socket.Close();
    Threads::EndThread();
}

bool HttpProtocolTest::TestKeepAlive(bool isKeepAliveIn) {
    isKeepAlive = isKeepAliveIn;

    InternetHost source(5554, "127.0.0.1");
    InternetHost destination(5555, "127.0.0.1");

    TCPSocket socket;
    socket.SetSource(source);
    socket.SetDestination(destination);

    socket.Open();
    socket.Listen(5555, 255);
    //todo launch a thread with the client request
    eventSem.Reset();
    Threads::BeginThread((ThreadFunctionType) clientJobKeepAlive, this);
    TCPSocket newSocket;

    eventSem.Post();

    socket.WaitConnection(TTInfiniteWait, &newSocket);

    StreamString stream;
    HttpProtocol test(newSocket);
    bool ret = test.ReadHeader();
    eventSem.Wait();

    ConfigurationDatabase *cdb = (ConfigurationDatabase *) (&test);

    cdb->MoveToRoot();
    StreamString output;
    output.Printf("%@", *cdb);
    printf("%s\n", output.Buffer());

    if (ret) {
        ret = (test.KeepAlive() == isKeepAlive);
    }

    newSocket.Close();
    socket.Close();

    return ret;
}

bool HttpProtocolTest::TestSetKeepAlive(bool isKeepAlive) {
    TCPSocket newSocket;

    StreamString stream;
    HttpProtocol test(newSocket);
    test.SetKeepAlive(isKeepAlive);
    return test.KeepAlive() == isKeepAlive;
}

bool HttpProtocolTest::TestGetHttpCommand(int32 commandIn) {

    InternetHost source(5554, "127.0.0.1");
    InternetHost destination(5555, "127.0.0.1");

    TCPSocket socket;
    socket.SetSource(source);
    socket.SetDestination(destination);

    socket.Open();
    socket.Listen(5555, 255);
    //todo launch a thread with the client request
    eventSem.Reset();
    if (commandIn == HttpDefinition::HSHCGet) {
        Threads::BeginThread((ThreadFunctionType) clientJobGet1, this);
    }
    else if (commandIn == HttpDefinition::HSHCPut) {
        Threads::BeginThread((ThreadFunctionType) clientJobPut1, this);
    }
    else if (commandIn == HttpDefinition::HSHCHead) {
        Threads::BeginThread((ThreadFunctionType) clientJobHead, this);
    }
    else if (commandIn == HttpDefinition::HSHCPost) {
        Threads::BeginThread((ThreadFunctionType) clientJobPost1, this);
    }
    else if (commandIn == HttpDefinition::HSHCReply) {
        Threads::BeginThread((ThreadFunctionType) clientJobReply, this);
    }
    TCPSocket newSocket;

    eventSem.Post();

    socket.WaitConnection(TTInfiniteWait, &newSocket);

    StreamString stream;
    HttpProtocol test(newSocket);
    bool ret = test.ReadHeader();
    eventSem.Wait();

    ConfigurationDatabase *cdb = (ConfigurationDatabase *) (&test);

    cdb->MoveToRoot();
    StreamString output;
    output.Printf("%@", *cdb);
    printf("%s\n", output.Buffer());

    if (ret) {
        if (commandIn == HttpDefinition::HSHCReply) {
            ret = test.GetHttpCommand() == (commandIn + 200);
        }
        else {
            ret = test.GetHttpCommand() == commandIn;
        }
    }
    newSocket.Close();
    socket.Close();

    return ret;

}

bool HttpProtocolTest::TestSetUnmatchedId() {
    TCPSocket newSocket;
    StreamString stream;
    HttpProtocol test(newSocket);
    test.SetUnmatchedId("pippo");
    StreamString ret;
    test.GetUnmatchedId(ret);
    return ret == "pippo";
}

bool HttpProtocolTest::TestGetUnmatchedId() {
    //normally it is the url

    InternetHost source(5554, "127.0.0.1");
    InternetHost destination(5555, "127.0.0.1");

    TCPSocket socket;
    socket.SetSource(source);
    socket.SetDestination(destination);

    socket.Open();
    socket.Listen(5555, 255);
    //todo launch a thread with the client request
    eventSem.Reset();
    Threads::BeginThread((ThreadFunctionType) clientJobGet1, this);
    TCPSocket newSocket;

    eventSem.Post();

    socket.WaitConnection(TTInfiniteWait, &newSocket);

    StreamString stream;
    HttpProtocol test(newSocket);
    bool ret = test.ReadHeader();
    eventSem.Wait();

    ConfigurationDatabase *cdb = (ConfigurationDatabase *) (&test);

    cdb->MoveToRoot();
    StreamString output;
    output.Printf("%@", *cdb);
    printf("%s\n", output.Buffer());

    if (ret) {
        StreamString unmatchedUrl;
        test.GetUnmatchedId(unmatchedUrl);
        StreamString url;
        test.GetUnmatchedId(url);
        ret = (unmatchedUrl == url);
    }

    newSocket.Close();
    socket.Close();

    return (ret && TestSetUnmatchedId());
}

bool HttpProtocolTest::TestGetPath() {
    return TestReadHeader_Get1();
}

bool HttpProtocolTest::TestGetId() {
    return TestReadHeader_Get1();
}

static void clientJobGetTextMode(HttpProtocolTest &tt) {
    //tells to the main process that the thread begins

    InternetHost source(5554, "127.0.0.1");
    InternetHost destination(5555, "127.0.0.1");

    TCPSocket socket;

    socket.SetSource(source);
    socket.SetDestination(destination);
    socket.Open();
    tt.eventSem.Wait();
    tt.eventSem.Reset();
    bool ret = socket.Connect("127.0.0.1", 5555);
    if (ret) {
        socket.Printf("GET /docs/index.html?TextMode=%d", tt.textMode);
        socket.Printf("%s", " HTTP/1.1\n");
        socket.Printf("%s", "Host: www.nowhere123.com\n");
        socket.Printf("%s", "Accept: image/gif, image/jpeg, */*\n");
        socket.Printf("%s", "Accept-Language: en-us\n");
        socket.Printf("%s", "Accept-Encoding: gzip, deflate\n");
        socket.Printf("%s", "User-Agent: Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1)\n");
        socket.Printf("%s", "\n");
        socket.Flush();
    }
    tt.eventSem.Post();
    socket.Close();
    Threads::EndThread();
}

bool HttpProtocolTest::TestTextMode(int8 textMode) {
    this->textMode = textMode;
    InternetHost source(5554, "127.0.0.1");
    InternetHost destination(5555, "127.0.0.1");

    TCPSocket socket;
    socket.SetSource(source);
    socket.SetDestination(destination);

    socket.Open();
    socket.Listen(5555, 255);
    //todo launch a thread with the client request
    eventSem.Reset();
    Threads::BeginThread((ThreadFunctionType) clientJobGetTextMode, this);
    TCPSocket newSocket;

    eventSem.Post();

    socket.WaitConnection(TTInfiniteWait, &newSocket);

    StreamString stream;
    HttpProtocol test(newSocket);

    bool ret = test.TextMode() == -1;

    ret &= test.ReadHeader();
    eventSem.Wait();

    ret &= test.TextMode() == textMode;

    newSocket.Close();
    socket.Close();

    return ret;
}

bool HttpProtocolTest::TestReadHeader_False_FailGetLine() {

    InternetHost source(5554, "127.0.0.1");
    InternetHost destination(5555, "127.0.0.1");

    TCPSocket socket;
    socket.SetSource(source);
    socket.SetDestination(destination);

    StreamString stream;
    HttpProtocol test(socket);

    bool ret = !test.ReadHeader();

    return ret;
}

static void clientJobWritePostNoContentType(HttpProtocolTest &tt) {
    //tells to the main process that the thread begins

    InternetHost source(5555, "127.0.0.1");
    InternetHost destination(5555, "127.0.0.1");

    TCPSocket socket;

    socket.SetSource(source);
    socket.SetDestination(destination);
    socket.Open();
    tt.eventSem.Wait();
    tt.eventSem.Reset();
    tt.retVal = socket.Connect("127.0.0.1", 5555);
    if (tt.retVal) {

        StreamString stream;
        HttpProtocol test(socket);

        test.CreateAbsolute("OutputOptions");
        test.Write("Transfer-Encoding", "chunked");

        stream.SetSize(0);
        stream.Printf("%s", "ciao");
        stream.Seek(0);

        tt.retVal = test.WriteHeader(true, HttpDefinition::HSHCPost, &stream, "localhost");

    }

    tt.eventSem.Post();
    socket.Close();
    Threads::EndThread();
}

bool HttpProtocolTest::TestReadHeader_False_PostNoContentType() {
    retVal = true;
    eventSem.Reset();

    InternetHost source(5554, "127.0.0.1");
    InternetHost destination(5555, "127.0.0.1");

    TCPSocket socket;
    socket.SetSource(source);
    socket.SetDestination(destination);

    socket.Open();
    socket.Listen(5555, 255);
    //todo launch a thread with the client request
    eventSem.Reset();
    Threads::BeginThread((ThreadFunctionType) clientJobWritePostNoContentType, this);
    TCPSocket newSocket;

    eventSem.Post();

    socket.WaitConnection(TTInfiniteWait, &newSocket);

    StreamString stream;
    HttpProtocol test(newSocket);

    bool ret = !test.ReadHeader();
    StreamString remained;
    if (ret) {
        ret = test.CompleteReadOperation(&remained);
    }

    eventSem.Wait();

    //ret&=!retVal;
    newSocket.Close();
    socket.Close();
    return (ret && retVal);
}

static void clientJobWritePostNoContentLength(HttpProtocolTest &tt) {
    //tells to the main process that the thread begins

    InternetHost source(5554, "127.0.0.1");
    InternetHost destination(5555, "127.0.0.1");

    TCPSocket socket;

    socket.SetSource(source);
    socket.SetDestination(destination);
    socket.Open();
    tt.eventSem.Wait();
    tt.eventSem.Reset();
    tt.retVal = socket.Connect("127.0.0.1", 5555);
    if (tt.retVal) {
        StreamString stream;
        HttpProtocol test(socket);
        test.CreateAbsolute("OutputOptions");
        test.Write("Content-Type", "application/x-www-form-urlencoded");
        test.MoveToRoot();

        stream.SetSize(0);
        stream.Printf("%s", "ciaobellooo\n");
        stream.Seek(0);
        tt.retVal = test.WriteHeader(false, HttpDefinition::HSHCPost, &stream, "localhost");

    }

    tt.eventSem.Post();
    socket.Close();
    Threads::EndThread();
}

bool HttpProtocolTest::TestReadHeader_False_PostNoContentLength() {
    retVal = true;
    eventSem.Reset();

    InternetHost source(5554, "127.0.0.1");
    InternetHost destination(5555, "127.0.0.1");

    TCPSocket socket;
    socket.SetSource(source);
    socket.SetDestination(destination);

    socket.Open();
    socket.Listen(5555, 255);
    //todo launch a thread with the client request
    eventSem.Reset();
    Threads::BeginThread((ThreadFunctionType) clientJobWritePostNoContentLength, this);
    TCPSocket newSocket;

    eventSem.Post();

    socket.WaitConnection(TTInfiniteWait, &newSocket);

    StreamString stream;
    HttpProtocol test(newSocket);

    bool ret = !test.ReadHeader();
    StreamString remained;
    if (ret) {
        ret = test.CompleteReadOperation(&remained);
    }

    eventSem.Wait();

    //ret&=!retVal;
    newSocket.Close();
    socket.Close();
    return (ret && retVal);
}

bool HttpProtocolTest::TestGetInputCommand() {

    TCPSocket newSocket;

    HttpProtocol test(newSocket);

    bool ret = test.CreateAbsolute("InputCommands");
    uint32 var1 = 10u;

    ret &= test.Write("var1", var1);

    uint32 var1Read = 0u;
    test.MoveToRoot();
    ret &= test.GetInputCommand("var1", var1Read);

    ret &= (var1Read == var1);

    ret &= !test.GetInputCommand("var2", var1Read);
    return ret;

}

