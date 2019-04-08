.. date: 02/04/2019
   author: Andre' Neto
   copyright: Copyright 2017 F4E | European Joint Undertaking for ITER and
   the Development of Fusion Energy ('Fusion for Energy').
   Licensed under the EUPL, Version 1.1 or - as soon they will be approved
   by the European Commission - subsequent versions of the EUPL (the "Licence")
   You may not use this work except in compliance with the Licence.
   You may obtain a copy of the Licence at: http://ec.europa.eu/idabc/eupl
   warning: Unless required by applicable law or agreed to in writing, 
   software distributed under the Licence is distributed on an "AS IS"
   basis, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
   or implied. See the Licence permissions and limitations under the Licence.

HTTP
====

The HTTP service (see :vcisdoxygencl:`HttpService`) enables the interaction with MARTe2 based applications using the HTTP protocol. In particular it allows to query live information (using JSON) about any Object through the ``ExportData`` method.

HttpDataExportI
---------------

In order to be able to interact directly with an HttpService instance, a class must implement the :vcisdoxygencl:`HttpDataExportI` interface. Depending on the type of request, the HttpService will either call the ``GetAsStructuredData`` or the ``GetAsText`` function. The former is expected to return data structured using a :vcisdoxygencl:`StructuredDataI` interface, while the latter is aimed at writing free text (e.g. HTML). 

The type of request can be set by including in the URL query string the parameter ``TextMode=0`` or ``TextMode=1``.

.. code-block:: c++   

    virtual bool GetAsStructuredData(MARTe::StreamStructuredDataI &data, MARTe::HttpProtocol &protocol) {
        bool ok = HttpDataExportI::GetAsStructuredData(data, protocol);
        ...
        ok = data.Write("Counter", counter);
        ...

    virtual bool GetAsText(MARTe::StreamI &stream, MARTe::HttpProtocol &protocol) {
        bool ok = HttpDataExportI::GetAsText(stream, protocol);
        MARTe::StreamString txt;
        ...
        ok = txt.Printf("<html><body><h1>Counter = %d</h1></body></html>\n", counter);
        ...
        ok = stream.Write(txt.Buffer(), nBytes);
        ...
     
.. warning::

   The data model and the data presentation layers should be as decoupled as possible. As a consequence, for the majority of use-cases, objects should not implement the HTTPDataExportI interface. Instead, objects should expose data using the ExportData method. It is assumed that all the data visualisation features are delegated to the HTTP client. 

HttpObjectBrowser
-----------------

The :vcisdoxygencl:`HttpObjectBrowser` enables the HTTP browsing of any ReferenceContainer. Using such feature one can query information about any component in a given MARTe application instance.

The HttpObjectBrowser can be configured to point at its own childs ``Root=.``, at the root of the application ``Root=/`` or at any given MARTe object ``Root=Path.To.Obj``. By setting the Realm option (see :vcisdoxygencl:`HttpRealmI`), the access to resources will be constrained to users that have valid credentials.

When accessing to an HttpObjectBrowser with text mode, the instance will check if there is an :vcisdoxygencl:`HttpDirectoryResource` child instance capable of handling the request and if so forwards the request to it. This mechanism can be used, e.g. to return an index.html page.

.. code-block:: c++

    +WebRoot = {
        Class = HttpObjectBrowser
        Root = "." //Navigation w.r.t. to this object
        +ObjectBrowse = {
            Class = HttpObjectBrowser
            Root = "/" //Navigation w.r.t. to the ObjectRegistryDatabase root
        }
        +ResourcesHtml = {
            Class = HttpDirectoryResource //If TextMode=1 will try to see if the component can resolve the request
            BaseDir = "../../../../../Resources/HTTP/"
        }    


HttpMessageInterface
--------------------

Using the :vcisdoxygencl:`HttpMessageInterface` it is possible to send command to a MARTe application using the URL query string. These commands are interpreted and forward as MARTe messages.

.. code-block:: c++

    ...
    +HttpMessageInterface = {
        Class = HttpMessageInterface
        +GOTOIDLE = { //Trigger the message GOTOIDLE if the URL contains msg=GOTOIDLE
            Class = Message
            Destination = StateMachine
            Function = GOTOIDLE
            Mode = ExpectsReply
        }
    ...

JavaScript client
-----------------

MARTe2 ships with a simple JavaScript based client framework that aims at easing the development of graphical user-interfaces.

.. note::

    For more complex client applications, users should use the JSON interface and design their own client applications.

The MARTe2 JavaScript client (see Resources/HTTP on the project folder) is based on a plug-in mechanism which retrieves data from the server (using JSON) and offers it to a given class that inherits from the :vcisjsdocs:`MARTeObject` JavaScript class.

.. code-block:: javascript

    ...
    class HttpPendulumEx1 extends MARTeObject {
    ...
        displayData(jsonData) {
            var angle = parseFloat(jsonData["angle"]);
            var width = this.canvas.clientWidth;
            var height = this.canvas.clientHeight;
    ... 


These classes can be instantiated and associated to a given JSON data stream using the :vcisjsdocs:`MARTeLoader` singleton. In particular the ``load`` method allows to allocate a given instance of the provided class to an HTML element and later associated to a JSON data source.

.. code-block:: javascript
    
    ...
    var mainTargetContainer = document.getElementById("mainTarget");
    //Associate the HttpPendulumEx1P2.js to an HTML element identified by the id "table0x1" and load with data coming from http://.../Pendulum
    MARTeLoader.instance().load("Pendulum", "HttpPendulumEx1P2", "table0x1");
    ...

The :vcisjsdocs:`HttpObjectBrowser` is a plug-in implementation that offers a navigation tree and that allows to display the selected class instance in a given display pane.

.. note::

    Objects, such as the :vcisdoxygencl:`ThreadsInformationQuery`, can be used to easily export live information about a running system.

The :vcisjsdocs:`HttpClient` allows to develop HTTP clients and is used as a core class for many of the HTTP service tests.

Examples
--------   
   
This example shows how to instantiate an HttpService and how to interface directly to an HttpDataExportI. After starting the application, point a browser at http://localhost:8084/

.. literalinclude:: /_static/examples/Core/HttpExample1.cpp
   :language: c++
   :emphasize-lines: 93,125,126,135,141,142,145,189,220,221,246,247,258,262,264
   :caption: Introspection of live objects using HTTP.
   :linenos:

For the example above, the Pendulum JavaScript class was coded as below:

.. literalinclude:: /_static/examples/Resources/HTTP/HttpPendulumEx1.js
   :language: javascript
   :emphasize-lines: 20,36,41,50,51,76
   :caption: Development of JavaScript plugins that can be integrated into client applications.
   :linenos:

Adding an HttpService to an existent MARTe application can be used to query live information about GAM signals and to change the StateMachine status. After starting the application, point a browser at http://localhost:8084 and navigate to the different GAMs. Also change the state machine state by interacting with the HttpMessageInterface component.

.. literalinclude:: /_static/examples/Configurations/RTApp-5.cfg
   :language: bash
   :emphasize-lines: 6,12,20
   :caption: Integration of an HttpService into an existent application (Run with MESSAGE=StateMachine:START and NAME_OF_THE_CONFIGURATION_FILE=RTApp-5.cfg)
   :linenos:

 
Instructions on how to compile and execute the examples can be found :doc:`here </core/examples>`.     
