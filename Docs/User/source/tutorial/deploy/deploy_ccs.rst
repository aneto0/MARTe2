.. date: 18/03/2026
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

CODAC Core System
=================

This section provides setup instructions for systems based on the 
`CODAC Core System <https://www.iter.org/machine/supporting-systems/codac/codac-core-system>`__ distributions.

Packages
--------

Ensure that the following packages are installed:

**MARTe2 components**

* $CODAC_RPM_PREFIX-marte2-components-dan-devel
* $CODAC_RPM_PREFIX-marte2-components-dan
* $CODAC_RPM_PREFIX-marte2-components-epics-devel
* $CODAC_RPM_PREFIX-marte2-components-epics
* $CODAC_RPM_PREFIX-marte2-components-helpers-devel
* $CODAC_RPM_PREFIX-marte2-components-helpers
* $CODAC_RPM_PREFIX-marte2-components-matlab-devel
* $CODAC_RPM_PREFIX-marte2-components-matlab
* $CODAC_RPM_PREFIX-marte2-components-opcua-devel
* $CODAC_RPM_PREFIX-marte2-components-opcua
* $CODAC_RPM_PREFIX-marte2-components-pxi-devel
* $CODAC_RPM_PREFIX-marte2-components-pxi
* $CODAC_RPM_PREFIX-marte2-components-rio-devel
* $CODAC_RPM_PREFIX-marte2-components-rio
* $CODAC_RPM_PREFIX-marte2-components-sdn-devel
* $CODAC_RPM_PREFIX-marte2-components-sdn
* $CODAC_RPM_PREFIX-marte2-components-tcn-devel
* $CODAC_RPM_PREFIX-marte2-components-tcn

**Core packages**

* marte2-components-devel
* marte2-components
* marte2-core-devel
* marte2-core

**Python and tools**

* python38
* python3-flask
* $CODAC_RPM_PREFIX-python38-matplotlib.x86_64
* $CODAC_RPM_PREFIX-python38-matplotlib.x86_64
* $CODAC_RPM_PREFIX-dan-archiver-reader-python38-plotters
* $CODAC_RPM_PREFIX-python36-opcua

Python
------

Any Python version ≥ 3.8 is compatible with the training material.

.. important::

   To avoid confusion between Python versions, it is recommended to alias ``python`` to ``python3``.  
   Add the following line to your ``.bashrc``:

   .. code-block:: bash

       alias python=python3.8

Training material
-----------------

Retrieve the training material using:

.. code-block:: bash

   svn co https://svnpub.iter.org/codac/iter/codac/dev/units/m-training-marte/

.. important::

   Some exercises assume the training directory is defined via the environment variable ``MARTE_TRAINING_PARENT_DIR``.  
   Add the following to your ``.bashrc``:

   .. code-block:: bash

      export MARTE_TRAINING_PARENT_DIR=/path/to/m-training-marte

.. note::

   The training material is also included in the MARTe2 core repository under:

   ::

      Docs/User/source/_static/tutorial

Verification
------------

Verify that the training material is correctly installed:

.. code-block:: bash

    ls $MARTE_TRAINING_PARENT_DIR/MARTe2-training-proj/

The output should include directories such as:

- ``Startup``
- ``Configurations``
- ``Documentation``
- ``Resources``
- ``Source``
- ``Test``
- ``Tools``