.. date: 19/03/2026
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

Linux (generic)
===============

This section provides setup instructions for generic Linux distributions.

.. note::

   The installation of compilers and general development tools is outside the scope of this tutorial.  
   It is assumed that a working C++ compiler and a Make-based build system are already available.

MARTe2 has been successfully deployed on both Red Hat–based and Debian-based distributions (e.g. Ubuntu).

Dependencies
------------

Some DataSources and components require additional external dependencies, such as:

- `EPICS Base <https://epics-controls.org/resources-and-support/downloads/>`__
- `open62541 <https://open62541.org/>`__

Installation of these dependencies is not covered here. Please refer to their respective documentation.

Some training exercises may also require:

- `Python 3 <https://www.python.org/downloads/>`__ (≥ 3.8)
- `Matplotlib <https://matplotlib.org/stable/users/installing.html>`__
- `flask <https://flask.palletsprojects.com/>`__

Core
----

Clone and build the MARTe2 core:

.. code-block:: bash

   git clone https://vcis-gitlab.f4e.europa.eu/aneto/MARTe2.git
   cd MARTe2
   export MARTe2_DIR=$(pwd)
   export TARGET=x86-linux
   make -f Makefile.gcc core

Components
----------

Clone and build the MARTe2 components:

.. code-block:: bash

   git clone https://vcis-gitlab.f4e.europa.eu/aneto/MARTe2-components.git
   cd MARTe2-components
   export MARTe2_Components_DIR=$(pwd)
   export TARGET=x86-linux
   make -f Makefile.gcc core

.. note::

   It is recommended to add the environment variables ``MARTe2_DIR``,  
   ``MARTe2_Components_DIR``, and ``TARGET`` to your shell profile (e.g. ``~/.bashrc``).

Training material
-----------------

Install the training material:

.. code-block:: bash

   export MARTE_TRAINING_PARENT_DIR=$HOME  # Adjust if needed
   cd $MARTe2_DIR/Docs/User/source/_static/tutorial
   ./InstallTraining.sh

Verification
------------

Verify that the training material has been correctly installed:

.. code-block:: bash

    ls $MARTE_TRAINING_PARENT_DIR/MARTe2-training-proj/

The output should include:

- ``Startup``
- ``Configurations``
- ``Documentation``
- ``Resources``
- ``Source``
- ``Test``
- ``Tools``