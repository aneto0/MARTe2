.. date: 24/03/2026
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

FileReader
==========

The :vcisdoxygenmccl:`FileReader` DataSource can be used to read data from a file and inject it into the application. This can be useful to test the application with pre-recorded data or to replay a specific scenario.

In this section, the ``ReferencePosition`` will be read from a file using the ``FileReader`` DataSource. As with the ``FileWriter``, this DataSource can be configured to store the data in a binary or text CSV format. 

.. literalinclude:: /_static/tutorial/Configurations/MassSpring/RTApp-MassSpring-21.cfg
    :language: c++
    :lines: 1243-1253
    :caption: FileReader configuration.
    :linenos:
    :emphasize-lines: 8

The ``FileReader`` DataSource can be configured to react in different ways to the end of file event. In this case it is configured to rewind and reloop forever.

The DataSource can also be configured to interpolate the data between the samples in the file. This can be useful to test the application with a specific sampling time that is different from the one used to record the data.


Running the application
-----------------------

Start the application with:

.. code-block:: bash

    ./MARTeApp.sh -f ../Configurations/MassSpring/RTApp-MassSpring-21.cfg -l RealTimeLoader -s State1

Once the application is running, inspect the ``screen`` output and verify that the application is running without any issues. The log should show entries similar to the following:

.. code-block:: console

    $ [Warning - Threads.cpp:181]: Failed to change the thread priority (likely due to insufficient permissions)
    $ [Information - RealTimeLoader.cpp:111]: Started application in state State1
    $ [Information - MARTeApp.cpp:135]: Application starting
    $ [Information - LoggerBroker.cpp:152]: Time [0:0]:1000000
    ...

Also check that the output file ``../Test/Integrated/MassSpring-21.csv`` is being created and that the ``ReferencePosition`` is set to the constant value ``2.0``.

Exercices
---------

Ex. 1: Read a ramp from the file 
--------------------------------

Modify the file ``../Test/Integrated/MassSpring-22-reference.csv`` to include a ramp for the ``ReferencePosition`` signal as shown in the following figure.

.. figure:: ../gams/images/mass_spring_refpos_ramp.svg
   :align: center
   :alt: Mass-spring-damper reference position ramp.
   :width: 400px


1. Edit the file ``../Configurations/MassSpring/RTApp-MassSpring-22.cfg`` and modity the FileReader to read from the ``../Test/Integrated/MassSpring-22-Reference.csv`` file.
2. Make sure that the ``FileReader`` interpolates the data between the samples in the file and rewinds and reloops forever.
3. Check that the output file ``../Test/Integrated/MassSpring-22.csv`` is being created and that the ``ReferencePosition`` is set to the expected ramp values.

.. code-block:: bash

    ./MARTeApp.sh -f ../Configurations/MassSpring/RTApp-MassSpring-22.cfg -l RealTimeLoader -s State1

.. dropdown:: Solution
   :icon: key

   The solution is to add the relevant points to the ``../Test/Integrated/MassSpring-22-Reference.csv``.

   .. literalinclude:: /_static/tutorial/Test/Integrated/MassSpring-22-Reference-solution.csv
      :language: console
      :caption: CSV file with ``ReferencePosition`` ramp values.
      :linenos:

   Modify the ``FileReader`` DataSource to read from the new file and to interpolate the data between the samples.

   .. literalinclude:: /_static/tutorial/Configurations/MassSpring/RTApp-MassSpring-22-solution.cfg
      :language: c++
      :lines: 1243-1253
      :caption: Updated FileReader configuration to read the ramp values.
      :linenos:

   Verify that the output file ``../Test/Integrated/MassSpring-22.csv`` is being created and that the ``ReferencePosition`` is set to the expected ramp values.
