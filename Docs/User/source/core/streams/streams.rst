.. date: 30/04/2018
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

Streams
=======

The framework :vcisdoxygencl:`StreamI` interface offers a portable stream definition. The :vcisdoxygencl:`BufferedStreamI` complements the ``StreamI`` interface by offering an interface to buffered operations (e.g. ``Printf`` and ``GetToken``).

.. warning::

   Some operations (e.g. Read, Printf, ...) might change the stream cursor position. Always remember to ``Seek`` to the appropriate position (e.g. ``Seek(0)``).

StreamString
------------

The :vcisdoxygencl:`StreamString` is the framework string implementation and allows to read and write to an underlying unbounded string that can grow dynamically.

.. code-block:: c++

   StreamString s1 = "Hello";
   //The cursor of s1 will be pointing at the end. As a consequence an empty string will be printed.
   REPORT_ERROR_STATIC(ErrorManagement::Information, "s1 = %s", s1);
   s1.Seek(0);
   REPORT_ERROR_STATIC(ErrorManagement::Information, "{After seek} s1 = %s", s1);
   //The buffer will return the pointer to the start of the string.
   REPORT_ERROR_STATIC(ErrorManagement::Information, "{Buffer()} s1 = %s", s1.Buffer());

   StreamString s3 = s1;
   s3.Seek(0);
   s3.Printf("%s", "Hi");
   //This will print Hillo....
   REPORT_ERROR_STATIC(ErrorManagement::Information, "{After Printf} s3 = %s", s3.Buffer());
   //Reset the string
   s3 = "";
   s3.Printf("%s", "Hi");
   REPORT_ERROR_STATIC(ErrorManagement::Information, "{After =\"\"} s3 = %s", s3.Buffer());

Basic streams
-------------

The Basic streams implementation allow to access the underlying media (e.g. file, socket, ...) using the :vcisdoxygencl:`StreamI` interface, but do not allow more advanced operations (e.g. Printf, GetToken, ...) which require buffered support.

The following basic streams are available: :vcisdoxygencl:`BasicConsole`, :vcisdoxygencl:`BasicFile`, :vcisdoxygencl:`BasicTCPSocket` and :vcisdoxygencl:`BasicUDPSocket`. 

.. code-block:: c++

   BasicUDPSocket sock;
   bool ok = sock.Open();
   ...
   ok = sock.Listen(port);
   ...
   BasicConsole bc;
   ...
   bc.Open(BasicConsoleMode::EnablePaging);
   bc.SetSceneSize(ncols, nrows);
   ...
   BasicFile f;
   ok = f.Open(TEST_FILENAME, BasicFile::FLAG_CREAT | BasicFile::ACCESS_MODE_W);
   ...
   MARTe::uint32 writeSize = str.Size();
   stream.Write(str.Buffer(), writeSize);
   
Buffered streams
----------------

The buffered streams complement the basic streams with buffered operations.

The following buffered streams are available: :vcisdoxygencl:`StreamString`, :vcisdoxygencl:`File`, :vcisdoxygencl:`TCPSocket` and :vcisdoxygencl:`UDPSocket`.

.. code-block:: c++

   UDPSocket sock;
   bool ok = sock.Open();
   ...
   ok = sock.Listen(port);
   ...
   File f;
   ok = f.Open(TEST_FILENAME, BasicFile::FLAG_CREAT | BasicFile::ACCESS_MODE_W);
   ...
   stream.GetLine(line);
   while (line.GetToken(token, ":", term)) {
      ...
      token = "";
   }
   line = "";
   
.. warning::

   When using the GetLine and GetToken methods, always remember to reset the output parameters (e.g. ``line = ""; token = "";``).

Examples
--------

StreamString
~~~~~~~~~~~~

The following example shows some typical use-cases of the strings.

.. literalinclude:: /_static/examples/Core/StreamStringExample1.cpp
   :language: c++
   :emphasize-lines: 50,53,55,61,62
   :caption: StreamString example (StreamStringExample1)
   :linenos:

Basic streams
~~~~~~~~~~~~~
The following is an example which highlights the usage of the basic streams.

.. literalinclude:: /_static/examples/Core/BasicStreamsExample1.cpp
   :language: c++
   :emphasize-lines: 47,58,63,74,89,92,93,103,104,117,118,126,127
   :caption: Basic streams example (BasicStreamsExample1)
   :linenos:

Start the application and, in another console, type ``echo -e "HELLO" | nc 127.0.0.1 24680 -w0``.

Basic streams
~~~~~~~~~~~~~
The following is an example that shows the usage of the buffered streams (including the ``GetLine``, ``GetToken`` and ``Printf`` methods).

.. literalinclude:: /_static/examples/Core/BufferedStreamsExample1.cpp
   :language: c++
   :emphasize-lines: 50,53,55,58,60,72,84,90-91,93,94,100,102
   :caption: Buffered streams example (BufferedStreamsExample1)
   :linenos:

Start the application and, in another console, type ``echo -e "SOCKET\nA1:B1:C1\nD2:E2:F2\nEND" | nc -u 127.0.0.1 24680 -w0``.

Instructions on how to compile and execute the example can be found :doc:`here </core/examples>`.
